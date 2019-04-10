/*************************************************************************************//*!@addtogroup file Files*//*@{*/
/*!
 ***********************************************************************************************************************
* File identification:      $Id: TemperatureSensor.cpp 46 2017-12-13 15:48:47Z cypax $
* Revision of last commit:  $Rev: 46 $
* Author of last commit:    $Author: cypax $
* Date of last commit:      $Date: 2017-12-13 16:48:47 +0100 (Mi, 13 Dez 2017) $
 ***********************************************************************************************************************
 *
 * @project     Raspberry Pi Aquarium Control
 *
 * @author      Cypax / cypax.net
 * @copyright
 *
 * @brief       Manages weekly schedules and relay switching. Relays are continuously updated every second by a timer.
 */
/****************************************************************************************************************//*@}*/

/**********************************************************************************************************************/
/* Library includes                                                                                                   */
/**********************************************************************************************************************/
#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QTimer>

/**********************************************************************************************************************/
/* Own includes                                                                                                       */
/**********************************************************************************************************************/
#include "TemperatureSensor.h"

/**********************************************************************************************************************/
/* Namespaces                                                                                                         */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* Constants (#define)                                                                                                */
/*******************************************************************************//*!@addtogroup define Constants*//*@{*/
#define DEBUG_TEMPERATURESENSOR 1
#if DEBUG_TEMPERATURESENSOR
#define QDEBUG qDebug()<<__func__<<"()\t"
#else
#define QDEBUG if (0) qDebug()
#endif

#define INI_FILENAME  "/temperature.ini"
#define DEFAULT_RET_STRING QString("???")

/***** End of: define Constants *********************************************************************************//*@}*/

/**********************************************************************************************************************/
/* Constructors                                                                                                       */
/**************************************************************************//*!@addtogroup constrc Constructors *//*@{*/
TemperatureSensor::TemperatureSensor() : m_Settings(QCoreApplication::applicationDirPath() + INI_FILENAME, QSettings::IniFormat)
{
  m_Temperature = DEFAULT_RET_STRING;
  loadSettings();

  updateTemperature();
}

/***** End of: constrc Constructors *****************************************************************************//*@}*/

/**********************************************************************************************************************/
/* Deconstructor                                                                                                      */
/**********************************************************************//*!@addtogroup deconstrc Deconstructors *//*@{*/
TemperatureSensor::~TemperatureSensor()
{
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

QString TemperatureSensor::getTemperature(void) const
{
  return m_Temperature;
}

void TemperatureSensor::loadSettings(void)
{
  m_Filename = m_Settings.value("Sensor/File", "").toString();
  QDEBUG << "Sensor file:" << m_Filename;
}


/***** End of: pubslots Public slots ****************************************************************************//*@}*/

/**********************************************************************************************************************/
/* Protected slots                                                                                                    */
/*************************************************************//*!@addtogroup protslots Protected slots         *//*@{*/


void TemperatureSensor::updateTemperature(void)
{
  QString strRet = DEFAULT_RET_STRING;

  if ("" != m_Filename)
  {
    QFile file(m_Filename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      QTextStream inStream(&file);

      QString line = inStream.readLine();
      while (!line.isNull())
      {
        QStringList fields = line.split("t=");
        if (fields.length() == 2)
        {
          QString strTemp = fields.at(1);
          bool bConv = false;
          int tempValue = strTemp.toInt(&bConv);
          if (true == bConv)
          {
            strRet = QString::number(((double)tempValue)/1000.0, 'f', 1);
            break;
          }
        }
        line = inStream.readLine();
      }

    }
    else
    {
      QDEBUG << "ERROR: cannot open file" << m_Filename;
    }
    file.close();
  }

  m_Temperature = strRet;

  QTimer::singleShot(10000, Qt::VeryCoarseTimer, this, SLOT(updateTemperature()));
}



/***** End of: protslots Protected slots ************************************************************************//*@}*/

/**********************************************************************************************************************/
/* Private slots                                                                                                      */
/*****************************************************************//*!@addtogroup privslots Private slots       *//*@{*/
/***** End of:privslots Private slots ***************************************************************************//*@}*/

/**** Last line of source code                                                                                     ****/
