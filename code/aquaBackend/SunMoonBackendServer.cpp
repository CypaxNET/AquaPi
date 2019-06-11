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
#include <QMap>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QTimer>
#include <QDateTime>

/**********************************************************************************************************************/
/* Own includes                                                                                                       */
/**********************************************************************************************************************/
#include "SunMoonBackendServer.h"

/**********************************************************************************************************************/
/* Namespaces                                                                                                         */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* Constants (#define)                                                                                                */
/*******************************************************************************//*!@addtogroup define Constants*//*@{*/
#define DEBUG_SUNMOONBACKENDSERVER 1
#if DEBUG_SUNMOONBACKENDSERVER
#define QDEBUG qDebug()<<__func__<<"()\t"
#else
#define QDEBUG if (0) qDebug()
#endif

const char* MOONINFOCOMMAND = "python mooninfo.py";

/***** End of: define Constants *********************************************************************************//*@}*/

/**********************************************************************************************************************/
/* Constructors                                                                                                       */
/**************************************************************************//*!@addtogroup constrc Constructors *//*@{*/
SunMoonBackendServer::SunMoonBackendServer(long lPort) : BackendServer(lPort)
{
  QDEBUG << "Starting sun/moon backend server";
  m_Timer.setInterval(1000);
  m_Timer.setSingleShot(false);
  QObject::connect(&m_Timer, SIGNAL(timeout()), this, SLOT(requestMoonInfo()));
  m_Timer.start();
}

/***** End of: constrc Constructors *****************************************************************************//*@}*/

/**********************************************************************************************************************/
/* Deconstructor                                                                                                      */
/**********************************************************************//*!@addtogroup deconstrc Deconstructors *//*@{*/
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
void SunMoonBackendServer::handleReceive(void)
{
  QTcpSocket* pSocket = qobject_cast<QTcpSocket*>(sender());
  if (Q_NULLPTR != pSocket)
  {
    QString socketString = pSocket->readAll();
    //QDEBUG << socketString;
    socketString = socketString.replace('\'', '"');
    QJsonDocument jsondoc = QJsonDocument::fromJson(socketString.toUtf8());
    QJsonObject jsonobj;

    // check validity of the document
    if(!jsondoc.isNull())
    {
        if(jsondoc.isObject())
        {
            bool convResult = false;
            double doubleValue;

            double fraction = 1.0;
            double altitude = 1.0;
            QDateTime riseTime;
            QDateTime setTime;
            bool isShining = false;

            jsonobj = jsondoc.object();
            //QDEBUG << "keys:" << jsonobj.keys();

            // obtain moon fractional brightness
            if (jsonobj.contains("fraction"))
            {
              QJsonValue jvFraction = jsonobj.value("fraction");
              QString strFraction = jvFraction.toString();
              doubleValue = strFraction.toDouble(&convResult);
              if (false != convResult) fraction = doubleValue;
            }

            // obtain moon alitude
            if (jsonobj.contains("altitude"))
            {
              QJsonValue jvAltitude = jsonobj.value("altitude");
              QString strAltitude = jvAltitude.toString();
              doubleValue = strAltitude.toDouble(&convResult);
              if (false != convResult) altitude = doubleValue;
            }

            // obtain moon rise time
            if (jsonobj.contains("rise"))
            {
              QJsonValue jvRiseTime = jsonobj.value("rise");
              QString strRiseTime = jvRiseTime.toString();
              riseTime = QDateTime::fromString(strRiseTime, "yyyy-MM-dd HH:mm:ss");
              riseTime.setTimeSpec(Qt::UTC);
            }

            // obtain moon set time
            if (jsonobj.contains("set"))
            {
              QJsonValue jvSetTime = jsonobj.value("set");
              QString strSetTime = jvSetTime.toString();
              setTime = QDateTime::fromString(strSetTime, "yyyy-MM-dd HH:mm:ss");
              setTime.setTimeSpec(Qt::UTC);
            }

            // obtain moon is shining
            if (jsonobj.contains("shining"))
            {
              QJsonValue jvIsShining = jsonobj.value("shining");
              QString strIsShining = jvIsShining.toString();
              if("True" == strIsShining)
              {
                isShining = true;
              }
            }

            double moonBrightness = 0.0;
            if (false != isShining)
            {
              moonBrightness = fraction * altitude;
            }
            QString strCommand = "python blinktLED.py -v ";
            strCommand.append(QString::number(moonBrightness));
            QDEBUG << "Moon brightness:" << strCommand;
            system(strCommand.toLatin1());
        }
        else
        {
            QDEBUG << "ERROR: received JSON data is not an object";
        }
    }
    else
    {
        QDEBUG << "ERROR: received data is invalid JSON";
    }
  }
  else
  {
    QDEBUG << "ERROR: receive from null pointer";
  }
}

void SunMoonBackendServer::requestMoonInfo(void)
{
  int retCode = system(MOONINFOCOMMAND);
  if (retCode != 0)
  {
    QDEBUG << "ERROR" << MOONINFOCOMMAND << "returned with code" << retCode;
  }
}

/***** End of: protslots Protected slots ************************************************************************//*@}*/

/**********************************************************************************************************************/
/* Private slots                                                                                                      */
/*****************************************************************//*!@addtogroup privslots Private slots       *//*@{*/
/***** End of:privslots Private slots ***************************************************************************//*@}*/

/**** Last line of source code                                                                                     ****/
