/*************************************************************************************//*!@addtogroup file Files*//*@{*/
/*!
 ***********************************************************************************************************************
* File identification:      $Id: BackendServer.h 38 2017-12-13 14:47:28Z cypax $
* Revision of last commit:  $Rev: 38 $
* Author of last commit:    $Author: cypax $
* Date of last commit:      $Date: 2017-12-13 15:47:28 +0100 (Mi, 13 Dez 2017) $
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

#ifndef BACKENDSERVER_H
#define BACKENDSERVER_H

/**********************************************************************************************************************/
/* Library includes                                                                                                   */
/**********************************************************************************************************************/
#include <QObject>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>
#include <QList>

/**********************************************************************************************************************/
/* Own includes                                                                                                       */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* Constants (#define)                                                                                                */
/*******************************************************************************//*!@addtogroup define Constants*//*@{*/
#define U16_CONNECTION_TIMEOUT    (quint16)2000
#define U8_RECONNECT_COUNTER      (quint8)5
#define U8_NUMBER_OF_SENT_STORED  (quint8)10
#define MIN_TELEGRAM_BYTE_SIZE    1
/***** End of: define Constants *********************************************************************************//*@}*/

/**********************************************************************************************************************/
/* Class                                                                                                              */
/**********************************************************************************//*!@addtogroup class Classes*//*@{*/
/**
* @brief
**/
class BackendServer : public QObject
{
  Q_OBJECT

/**********************************************************************************************************************/
/* Types (typedef)                                                                                                    */
/**********************************************************************************//*!@addtogroup typedef Types*//*@{*/
public:

/***** End of: typedef Types ************************************************************************************//*@}*/

/**********************************************************************************************************************/
/* Properties (Q_PROPERTY)                                                                                            */
/**************************************************************************//*!@addtogroup Q_PROPERTY Properties*//*@{*/
public:

/***** End of: Q_PROPERTY Properties ****************************************************************************//*@}*/

/**********************************************************************************************************************/
/* Constructors                                                                                                       */
/**********************************************************************************************************************/
public:
  BackendServer(long lPort);

/**********************************************************************************************************************/
/* Destructor                                                                                                         */
/**********************************************************************************************************************/
public:
  ~BackendServer();
/**********************************************************************************************************************/
/* Public function prototypes                                                                                         */
/**********************************************************************************************************************/
public:

/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* Protected function prototypes                                                                                      */
/**********************************************************************************************************************/
protected:

/**********************************************************************************************************************/
/* Private function prototypes                                                                                        */
/**********************************************************************************************************************/
private:

/**********************************************************************************************************************/
/* Signals                                                                                                            */
/**********************************************************************************************************************/
signals:

/**********************************************************************************************************************/
/* Public slots                                                                                                       */
/**********************************************************************************************************************/
public slots:

/**********************************************************************************************************************/
/* Protected slots                                                                                                    */
/**********************************************************************************************************************/
protected slots:
  void handleNewConnection(void);
  void handleDisconnectSocket(void);
  virtual void handleReceive(void) = 0;

/**********************************************************************************************************************/
/* Private slots                                                                                                      */
/**********************************************************************************************************************/
private slots:

/**********************************************************************************************************************/
/* Protected attributes                                                                                               */
/**********************************************************************************************************************/
protected:
  QTcpServer*         m_pServer;
  quint16             m_u16Port;
  QList<QTcpSocket*>  m_ListOfSockets;

/**********************************************************************************************************************/
/* Private attributes                                                                                                 */
/**********************************************************************************************************************/
private:

};

/***** End of: Classes ******************************************************************************************//*@}*/

#endif // BACKENDSERVER_H
/**** Last line of source code ****************************************************************************************/
