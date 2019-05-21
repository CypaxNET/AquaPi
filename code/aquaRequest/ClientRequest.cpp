/*************************************************************************************//*!@addtogroup file Files*//*@{*/
/*!
 ***********************************************************************************************************************
* File identification:      $Id: ClientRequest.cpp 37 2017-12-13 14:41:26Z cypax $
* Revision of last commit:  $Rev: 37 $
* Author of last commit:    $Author: cypax $
* Date of last commit:      $Date: 2017-12-13 15:41:26 +0100 (Mi, 13 Dez 2017) $
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
#include <QTimer>

#include <iostream>
#include <stdlib.h>


using namespace std;

/**********************************************************************************************************************/
/* Own includes                                                                                                       */
/**********************************************************************************************************************/
#include "ClientRequest.h"

/**********************************************************************************************************************/
/* Namespaces                                                                                                         */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* Constants (#define)                                                                                                */
/*******************************************************************************//*!@addtogroup define Constants*//*@{*/
#define DEBUG_ClientRequest 1
#if DEBUG_ClientRequest
#define QDEBUG qDebug()<<__func__<<"()\t"
#else
#define QDEBUG if (0) qDebug()
#endif
/***** End of: define Constants *********************************************************************************//*@}*/

/**********************************************************************************************************************/
/* Constructors                                                                                                       */
/**************************************************************************//*!@addtogroup constrc Constructors *//*@{*/
ClientRequest::ClientRequest(long lPort, QString strRequest)
{
  m_Request = strRequest;
  QDEBUG << strRequest;

  m_pSocket = new QTcpSocket();

  connect(m_pSocket, SIGNAL(readyRead()), this, SLOT(handleReceive()));
  connect(m_pSocket, SIGNAL(disconnected()), this, SLOT(handleDisconnect()));
  connect(m_pSocket, SIGNAL(connected()), this, SLOT(sendRequest()));
  connect(m_pSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(handleDisconnect()));

#ifdef Q_OS_WIN
#warning "WARNING: FIT is active!"
  m_pSocket->connectToHost("127.0.0.1", lPort);
#else
  m_pSocket->connectToHost("localhost", lPort);
#endif


  // terminate on timeout
  QTimer::singleShot(2000, Qt::VeryCoarseTimer, this, SLOT(handleTimeout()));
}

/***** End of: constrc Constructors *****************************************************************************//*@}*/

/**********************************************************************************************************************/
/* Deconstructor                                                                                                      */
/**********************************************************************//*!@addtogroup deconstrc Deconstructors *//*@{*/
ClientRequest::~ClientRequest()
{
  if (Q_NULLPTR != m_pSocket)
  {
    QDEBUG << "disconnecting from backend";
    m_pSocket->close();
    delete m_pSocket;
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

void ClientRequest::sendRequest(void)
{
  if (Q_NULLPTR != m_pSocket)
  {
    if (m_pSocket->isOpen())
    {
      QDEBUG << "sending request";

      QByteArray block;
      QDataStream out(&block, QIODevice::WriteOnly);
      out.setVersion(QDataStream::Qt_4_0);
      out << (quint16)0;

      out << m_Request.trimmed();


      out.device()->seek(0);
      out << (quint16)(block.size() - sizeof(quint16));

      m_pSocket->write(block);

      QDEBUG << block;
    }
  }
}

void ClientRequest::handleTimeout(void)
{
  QDEBUG << "timeout";
  emit signalTerminate();
}

void ClientRequest::handleDisconnect(void)
{
  QDEBUG << "disconnect";
  emit signalTerminate();
}

void ClientRequest::handleReceive(void)
{
  quint16 blockSize = 0;
  QDataStream in(m_pSocket);
  in.setVersion(QDataStream::Qt_4_0);

  // read bock size
  if (blockSize == 0)
  {
      if (m_pSocket->bytesAvailable() < (int)sizeof(quint16))
      {
        QDEBUG << "Message too short";
        return;
      }
      in >> blockSize;
  }

  if (m_pSocket->bytesAvailable() < blockSize)
  {
    QDEBUG << "Message payload too short";
    return;
  }

  QString serverResponse;

  in >> serverResponse;
  serverResponse = serverResponse.trimmed();

  QDEBUG << serverResponse;

  const char* retStr = serverResponse.toUtf8().constData();
  cout << retStr;

  emit signalTerminate();
  //emit signalReceive(serverResponse);
}


/***** End of: protslots Protected slots ************************************************************************//*@}*/

/**********************************************************************************************************************/
/* Private slots                                                                                                      */
/*****************************************************************//*!@addtogroup privslots Private slots       *//*@{*/
/***** End of:privslots Private slots ***************************************************************************//*@}*/

/**** Last line of source code                                                                                     ****/
