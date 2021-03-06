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
#define DEBUG_BACKENDSERVER 0
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
    QDEBUG << "Listening on" << m_pServer->serverAddress() << "at port" << m_pServer->serverPort();
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
    QDEBUG << "new connection with" << s;
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
        QDEBUG << "disconnecting from" << s;
        pSocket->close();
        m_ListOfSockets.removeOne(pSocket);
        pSocket->deleteLater();
        //QDEBUG << "number of sockets now " << m_ListOfSockets.count();
        break;
      }
    }
  }
  else
  {
    // nothing to do here, cannot disconnect from null pointer
  }
}

/***** End of: protslots Protected slots ************************************************************************//*@}*/

/**********************************************************************************************************************/
/* Private slots                                                                                                      */
/*****************************************************************//*!@addtogroup privslots Private slots       *//*@{*/
/***** End of:privslots Private slots ***************************************************************************//*@}*/

/**** Last line of source code                                                                                     ****/
