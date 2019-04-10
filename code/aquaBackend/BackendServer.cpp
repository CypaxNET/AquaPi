/*************************************************************************************//*!@addtogroup file Files*//*@{*/
/*!
 ***********************************************************************************************************************
* File identification:      $Id: BackendServer.cpp 49 2017-12-13 16:31:12Z cypax $
* Revision of last commit:  $Rev: 49 $
* Author of last commit:    $Author: cypax $
* Date of last commit:      $Date: 2017-12-13 17:31:12 +0100 (Mi, 13 Dez 2017) $
 ***********************************************************************************************************************
 *
 * @project     Raspberry Pi Aquarium Control
 *
 * @author      Cypax / cypax.net
 * @copyright
 *
 * @brief       Sets up a QTcpServer. Passes client requests to a RelayControl object.
 */
/****************************************************************************************************************//*@}*/

/**********************************************************************************************************************/
/* Library includes                                                                                                   */
/**********************************************************************************************************************/
#include <QtDebug>
#include <QDataStream>
#include <QRegExp>

/**********************************************************************************************************************/
/* Own includes                                                                                                       */
/**********************************************************************************************************************/
#include "BackendServer.h"

/**********************************************************************************************************************/
/* Namespaces                                                                                                         */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* Constants (#define)                                                                                                */
/*******************************************************************************//*!@addtogroup define Constants*//*@{*/
#define DEBUG_BACKENDSERVER 1
#if DEBUG_BACKENDSERVER
#define QDEBUG qDebug()<<__func__<<"()\t"
#else
#define QDEBUG if (0) qDebug()
#endif
/***** End of: define Constants *********************************************************************************//*@}*/

/**********************************************************************************************************************/
/* Constructors                                                                                                       */
/**************************************************************************//*!@addtogroup constrc Constructors *//*@{*/
BackendServer::BackendServer(long lPort)
{
  QDEBUG << "Starting backend server";

  this->m_pServer = new QTcpServer(this);

  if (Q_NULLPTR != m_pServer)
  {
    connect(m_pServer, SIGNAL(newConnection()), SLOT(handleNewConnection()));

    m_pServer->listen(QHostAddress::Any, (quint16)lPort);
  }
  else
  {
    QDEBUG << "Failed to set up TCP server.";
  }
}

/***** End of: constrc Constructors *****************************************************************************//*@}*/

/**********************************************************************************************************************/
/* Deconstructor                                                                                                      */
/**********************************************************************//*!@addtogroup deconstrc Deconstructors *//*@{*/
BackendServer::~BackendServer()
{
  if (Q_NULLPTR != m_pServer)
  {
    // close all sockets
    foreach (QTcpSocket* pListSocket, m_ListOfSockets)
    {
      QString s = (pListSocket->peerAddress().toString() + ":" + QString::number(pListSocket->peerPort()));
      QDEBUG << "disconnecting from" << s;
      pListSocket->close();
      m_ListOfSockets.removeOne(pListSocket);
    }

    m_pServer->close();
    delete m_pServer;
  }
}
/***** End of: deconstrc Constructors ***************************************************************************//*@}*/

/**********************************************************************************************************************/
/* Public functions                                                                                                   */
/*********************************************************************//*!@addtogroup pubfunct Public functions *//*@{*/
/***** End of: pubfunct Public functions ************************************************************************//*@}*/

/**********************************************************************************************************************/
/* Protected functions                                                                                                */
/*****************************************************************//*!@addtogroup protfunct Protected functions *//*@{*/
/***** End of: protfunct Protected functions ********************************************************************//*@}*/

/**********************************************************************************************************************/
/* Private functions                                                                                                  */
/*****************************************************************//*!@addtogroup privfunct Private functions   *//*@{*/
/***** End of: privfunct Private functions **********************************************************************//*@}*/

/**********************************************************************************************************************/
/* Public slots                                                                                                       */
/*****************************************************************//*!@addtogroup pubslots Public slots         *//*@{*/
/***** End of: pubslots Public slots ****************************************************************************//*@}*/

/**********************************************************************************************************************/
/* Protected slots                                                                                                    */
/*************************************************************//*!@addtogroup protslots Protected slots         *//*@{*/
void BackendServer::handleNewConnection(void)
{
  while (m_pServer->hasPendingConnections())
  {
    QTcpSocket* pSocket = m_pServer->nextPendingConnection();
    connect(pSocket, SIGNAL(readyRead()),    this, SLOT(handleReceive()));
    connect(pSocket, SIGNAL(disconnected()), this, SLOT(handleDisconnectSocket()));

    m_ListOfSockets.append(pSocket);

    QString s = (m_ListOfSockets.last()->peerAddress().toString() + ":" + QString::number(m_ListOfSockets.last()->peerPort()));
    //QDEBUG << "new connection with" << s;
  }
}

void BackendServer::handleDisconnectSocket()
{
  QTcpSocket* pSocket = qobject_cast<QTcpSocket*>(sender());
  if (Q_NULLPTR != pSocket)
  {
    // search in list for this socket
    foreach (QTcpSocket* pListSocket, m_ListOfSockets)
    {
      if (pListSocket == pSocket)
      {
        QString s = (pSocket->peerAddress().toString() + ":" + QString::number(pSocket->peerPort()));
        //QDEBUG << "disconnecting from" << s;
        pSocket->close();
        m_ListOfSockets.removeOne(pSocket);
        break;
      }
    }
  }
  else
  {
    // nothing to do here, cannot disconnect from null pointer
  }
}

void BackendServer::handleReceive(void)
{
  QTcpSocket* pSocket = qobject_cast<QTcpSocket*>(sender());
  if (Q_NULLPTR != pSocket)
  {
    quint16 blockSize = 0;
    QDataStream in(pSocket);
    in.setVersion(QDataStream::Qt_4_0);

    // read bock size
    if (blockSize == 0)
    {
        if (pSocket->bytesAvailable() < (int)sizeof(quint16))
        {
          QDEBUG << "Message too short";
          return;
        }
        in >> blockSize;
    }

    if (pSocket->bytesAvailable() < blockSize)
    {
      QDEBUG << "Message payload too short";
      return;
    }

    QString clientRequest;

    in >> clientRequest;
    clientRequest = clientRequest.trimmed();

    QString s = (pSocket->peerAddress().toString() + ":" + QString::number(pSocket->peerPort())) + "  " + clientRequest;
    QDEBUG << "received request" << s;

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint16)0;

    if (false == clientRequest.startsWith("request=", Qt::CaseInsensitive))
    {
      out << QString("ERROR: invalid syntax\n");
    }
    else
    {
      clientRequest = clientRequest.remove(0, QString("request=").length());

      if (clientRequest.startsWith("terminate", Qt::CaseInsensitive))
      {
        out << QString("Bye\n");
        emit signalTerminate();
      }
      else if (clientRequest.startsWith("getStatus", Qt::CaseInsensitive))
      {
        clientRequest = clientRequest.remove(0, QString("getRelayOverwriteDelay").length());
        QString response = QDate::currentDate().toString("yyyy-MM-dd") + " " + QTime::currentTime().toString("hh:mm:ss") + "|";

        response.append(m_TemperatureSensor.getTemperature() + "|");

        for (int i=0; i<RELAYCONTROL_NUMBER_OF_RELAYS; i++)
        {
          if (RelayControl::E_STATE_ON == m_RelayControl.getRelayState(i))
          {
            response.append("ON,");
          }
          else
          {
            response.append("OFF,");
          }

          switch (m_RelayControl.getRelayMode(i))
          {
            case RelayControl::E_MODE_KEEP_ON:
            {
              response.append("KEEP_ON,");
            }
            break;
            case RelayControl::E_MODE_KEEP_OFF:
            {
              response.append("KEEP_OFF,");
            }
            break;
            case RelayControl::E_MODE_SCHEDULE:
            {
              response.append("SCHEDULE,");
            }
            break;
            default:
            {
              response.append("???,");
            }
            break;
          }
          response.append(QString::number(m_RelayControl.getRelayOverwriteDelay(i), 10));
          if (i < (RELAYCONTROL_NUMBER_OF_RELAYS-1)) response.append("|");
        }
        out << response;
      }
      else if (clientRequest.startsWith("isRelayInOverwriteMode", Qt::CaseInsensitive))
      {
        clientRequest = clientRequest.remove(0, QString("isRelayInOverwriteMode").length());
        bool conversionResult = false;
        int relay = clientRequest.toInt(&conversionResult, 10);
        if (false != conversionResult)
        {
          if ((relay >= 0) && (relay < RELAYCONTROL_NUMBER_OF_RELAYS))
          {
            if (m_RelayControl.isRelayInOverwriteMode(relay))
            {
              out << QString("1\n");
            }
            else
            {
              out << QString("0\n");
            }
          }
          else
          {
            out << QString("Invalid relay index\n");
          }
        }
        else
        {
          out << QString("Invalid syntax\n");
        }
      }
      else if (clientRequest.startsWith("getRelayOverwriteDelay", Qt::CaseInsensitive))
      {
        clientRequest = clientRequest.remove(0, QString("getRelayOverwriteDelay").length());
        bool conversionResult = false;
        int relay = clientRequest.toInt(&conversionResult, 10);
        if (false != conversionResult)
        {
          if ((relay >= 0) && (relay < RELAYCONTROL_NUMBER_OF_RELAYS))
          {
            out << QString(QString::number(m_RelayControl.getRelayOverwriteDelay(relay)) + "\n");
          }
          else
          {
            out << QString("Invalid relay index\n");
          }
        }
        else
        {
          out << QString("Invalid syntax\n");
        }
      }
      else if (clientRequest.startsWith("setRelayMode", Qt::CaseInsensitive))
      {
        clientRequest = clientRequest.remove(0, QString("setRelayMode").length());
        QStringList keyvalue = clientRequest.split("|");
        if (keyvalue.length() != 2)
        {
          out << QString("ERROR: invalid syntax");
        }
        else
        {
          bool conversionResult = false;
          int relay = keyvalue.at(0).toInt(&conversionResult, 10);
          if (false != conversionResult)
          {
            if ((relay >= 0) && (relay < RELAYCONTROL_NUMBER_OF_RELAYS))
            {
              if (keyvalue.at(1) == "KEEP_ON")
              {
                m_RelayControl.setRelayMode(relay, RelayControl::E_MODE_KEEP_ON);
              }
              else if (keyvalue.at(1) == "KEEP_OFF")
              {
                m_RelayControl.setRelayMode(relay, RelayControl::E_MODE_KEEP_OFF);
              }
              else if (keyvalue.at(1) == "SCHEDULE")
              {
                m_RelayControl.setRelayMode(relay, RelayControl::E_MODE_SCHEDULE);
              }
              else
              {
                out << QString("Invalid syntax\n");
              }
            }
            else
            {
              out << QString("Invalid relay index\n");
            }
          }
          else
          {
            out << QString("Invalid syntax\n");
          }
        }
      }
      else if (clientRequest.startsWith("setRelayOverwrite", Qt::CaseInsensitive))
      {
        clientRequest = clientRequest.remove(0, QString("setRelayOverwrite").length());
        QStringList keyvalue = clientRequest.split("|");
        if (keyvalue.length() != 2)
        {
          out << QString("ERROR: invalid syntax");
        }
        else
        {
          bool conversionResult = false;
          int relay = keyvalue.at(0).toInt(&conversionResult, 10);
          if (false != conversionResult)
          {
            if ((relay >= 0) && (relay < RELAYCONTROL_NUMBER_OF_RELAYS))
            {
              QString value = keyvalue.at(1);
              keyvalue = value.split(",");
              if (keyvalue.length() != 2)
              {
                out << QString("ERROR: invalid syntax");
              }
              else
              {
                conversionResult = false;
                int delay = keyvalue.at(0).toInt(&conversionResult, 10);
                if (false != conversionResult)
                {
                  if (keyvalue.at(1) == "ON")
                  {
                    m_RelayControl.setRelayOverwrite(relay, delay, RelayControl::E_STATE_ON);
                  }
                  else
                  {
                    m_RelayControl.setRelayOverwrite(relay, delay, RelayControl::E_STATE_OFF);
                  }
                }
                else
                {
                  out << QString("ERROR: invalid syntax");
                }
              }
            }
            else
            {
              out << QString("Invalid relay index\n");
            }
          }
          else
          {
            out << QString("Invalid syntax\n");
          }
        }
      }
      else if (clientRequest.startsWith("setRelaySchedule", Qt::CaseInsensitive))
      {
        clientRequest = clientRequest.remove(0, QString("setRelaySchedule").length());
        QStringList keyvalue = clientRequest.split("|");
        if (keyvalue.length() != 2)
        {
          out << QString("ERROR: invalid syntax");
        }
        else
        {
          bool conversionResult = false;
          int relay = keyvalue.at(0).toInt(&conversionResult, 10);
          if (false != conversionResult)
          {
            if ((relay >= 0) && (relay < RELAYCONTROL_NUMBER_OF_RELAYS))
            {
              if (m_RelayControl.setRelaySchedule(relay, keyvalue.at(1)))
              {
                out << QString("1\n");
              }
              else
              {
                out << QString("0\n");
              }
            }
            else
            {
              out << QString("Invalid relay index\n");
            }
          }
          else
          {
            out << QString("Invalid syntax\n");
          }
        }
      }
      else if (clientRequest.startsWith("getRelaySchedule", Qt::CaseInsensitive))
      {
        clientRequest = clientRequest.remove(0, QString("getRelaySchedule").length());
        bool conversionResult = false;
        int relay = clientRequest.toInt(&conversionResult, 10);
        if (false != conversionResult)
        {
          if ((relay >= 0) && (relay < RELAYCONTROL_NUMBER_OF_RELAYS))
          {
            out << m_RelayControl.getRelaySchedule(relay);
          }
          else
          {
            out << QString("Invalid relay index\n");
          }
        }
        else
        {
          out << QString("Invalid syntax\n");
        }
      }
      else if (clientRequest.startsWith("getAllSchedules", Qt::CaseInsensitive))
      {
        clientRequest = clientRequest.remove(0, QString("getAllSchedules").length());

        QString response = "";
        for (int i=0; i<RELAYCONTROL_NUMBER_OF_RELAYS; i++)
        {
          response.append(m_RelayControl.getRelaySchedule(i));
          if (i < (RELAYCONTROL_NUMBER_OF_RELAYS-1)) response.append("|");
        }

        out << response;
      }
      else if (clientRequest.startsWith("getRelayState", Qt::CaseInsensitive))
      {
        clientRequest = clientRequest.remove(0, QString("getRelayState").length());
        bool conversionResult = false;
        int relay = clientRequest.toInt(&conversionResult, 10);
        if (false != conversionResult)
        {
          if (RelayControl::E_STATE_ON == m_RelayControl.getRelayState(relay))
          {
            out << QString("ON\n");
          }
          else
          {
            out << QString("OFF\n");
          }
        }
        else
        {
          out << QString("Invalid syntax\n");
        }
      }
      else
      {
        out << QString("Unknown request\n");
      }
    }

    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    pSocket->write(block);

  }
  else
  {
    QDEBUG << "ERROR: receive from null pointer";
  }
}


/***** End of: protslots Protected slots ************************************************************************//*@}*/

/**********************************************************************************************************************/
/* Private slots                                                                                                      */
/*****************************************************************//*!@addtogroup privslots Private slots       *//*@{*/
/***** End of:privslots Private slots ***************************************************************************//*@}*/

/**** Last line of source code                                                                                     ****/
