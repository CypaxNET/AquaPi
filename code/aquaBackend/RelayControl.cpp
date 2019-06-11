/*************************************************************************************//*!@addtogroup file Files*//*@{*/
/*!
 ***********************************************************************************************************************
* File identification:      $Id: RelayControl.cpp 38 2017-12-13 14:47:28Z cypax $
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
 * @brief       Manages weekly schedules and relay switching. Relays are continuously updated every second by a timer.
 */
/****************************************************************************************************************//*@}*/

/**********************************************************************************************************************/
/* Library includes                                                                                                   */
/**********************************************************************************************************************/
#include <QCoreApplication>
#include <QDebug>
#include <QTimer>

#ifdef Q_OS_WIN
#warning "WiringPi library not available for Windows"
#else
#include <wiringPi.h>
#endif

/**********************************************************************************************************************/
/* Own includes                                                                                                       */
/**********************************************************************************************************************/
#include "RelayControl.h"

/**********************************************************************************************************************/
/* Namespaces                                                                                                         */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* Constants (#define)                                                                                                */
/*******************************************************************************//*!@addtogroup define Constants*//*@{*/
#define DEBUG_RELAYCONTROL 1
#if DEBUG_RELAYCONTROL
#define QDEBUG qDebug()<<__func__<<"()\t"
#else
#define QDEBUG if (0) qDebug()
#endif

#define INI_FILENAME  "/relay_control.ini"
#define SCHEDULE_DELIMITER QString(",")
#define UNSET_GPIO ((int)-1)

/***** End of: define Constants *********************************************************************************//*@}*/

/**********************************************************************************************************************/
/* Constructors                                                                                                       */
/**************************************************************************//*!@addtogroup constrc Constructors *//*@{*/
RelayControl::RelayControl() : m_Settings(QCoreApplication::applicationDirPath() + INI_FILENAME, QSettings::IniFormat)
{
#ifdef Q_OS_WIN
#warning "WiringPi library not available for Windows"
#else
  int wiringPiSetupReturn = wiringPiSetup();
  QDEBUG << "Output from wiringPiSetup() is" << wiringPiSetupReturn;

  if (0 == wiringPiSetupReturn)
  {
    QDEBUG << "Initializing GPIOs";

    pinMode(22, OUTPUT);
    pinMode(23, OUTPUT);
    pinMode(24, OUTPUT);
    pinMode(25, OUTPUT);
    pinMode(26, OUTPUT);
    pinMode(27, OUTPUT);
    pinMode(28, OUTPUT);
    pinMode(29, OUTPUT);
  }
#endif


  loadSettings();
  for (int i = 0; i<RELAYCONTROL_NUMBER_OF_RELAYS; i++)
  {
    m_Relays[i].eOverwriteState = E_STATE_OFF;
    m_Relays[i].eRelayState = E_STATE_OFF;
    m_Relays[i].overwriteModeEndTime = QDateTime(); // initialize with invalid datetime
  }
  saveSettings();

  updateRelays();
}

/***** End of: constrc Constructors *****************************************************************************//*@}*/

/**********************************************************************************************************************/
/* Deconstructor                                                                                                      */
/**********************************************************************//*!@addtogroup deconstrc Deconstructors *//*@{*/
RelayControl::~RelayControl()
{
  saveSettings();
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

bool RelayControl::setRelayOverwrite(int relay, int delaySeconds, RELAYCONTROL_STATES_ENUM eOverwriteState)
{
  bool bRet = false;

  if ((relay >= 0) && (relay < RELAYCONTROL_NUMBER_OF_RELAYS))
  {
    m_Relays[relay].eOverwriteState = eOverwriteState;
    m_Relays[relay].overwriteModeEndTime = QDateTime::currentDateTime().addSecs(delaySeconds);
    bRet = isRelayInOverwriteMode(relay);
  }
  else
  {
    QDEBUG << "ERROR: invalid relay";
    bRet = false;
  }
  return bRet;
}

bool RelayControl::isRelayInOverwriteMode(const int relay)
{
  bool bRet = false;

  if ((relay >= 0) && (relay < RELAYCONTROL_NUMBER_OF_RELAYS))
  {
    if (m_Relays[relay].overwriteModeEndTime.isValid())
    {
      if(m_Relays[relay].overwriteModeEndTime > QDateTime::currentDateTime())
      {
         // is still in overwrite mode
         bRet = true;
      }
      else
      {
        // overwrite mode expired, set it to invalid
        QDEBUG << "overwrite mode expired";
        m_Relays[relay].overwriteModeEndTime = QDateTime();
        bRet = false;
      }
    }
    else
    {
      // is not in overwrite mode
      bRet = false;
    }
  }
  else
  {
    QDEBUG << "ERROR: relay index out of bounds";
  }

  return bRet;
}

int RelayControl::getRelayOverwriteDelay(const int relay)
{
  int iRet = 0;

  if ((relay >= 0) && (relay < RELAYCONTROL_NUMBER_OF_RELAYS))
  {
    if (m_Relays[relay].overwriteModeEndTime.isValid())
    {
      qint64 i64DiffTime = (m_Relays[relay].overwriteModeEndTime.toMSecsSinceEpoch() - QDateTime::currentMSecsSinceEpoch()) / ((qint64)1000);
      if (i64DiffTime <= ((qint64)0))
      {
        // overwrite mode expired, set it to invalid
        QDEBUG << "overwrite mode expired";
        m_Relays[relay].overwriteModeEndTime = QDateTime();
        iRet = 0;
      }
      else
      {
        // limit to maximum int value if necessary
        if (i64DiffTime > INT_MAX)
        {
          iRet = INT_MAX;
        }
        else
        {
          iRet = (int)i64DiffTime;
        }
      }
    }
    else
    {
      // is not in overwrite mode
      iRet = 0;
    }
  }
  else
  {
    QDEBUG << "ERROR: relay index out of bounds";
  }

  return iRet;
}


bool RelayControl::setRelayMode(const int relay, const RelayControl::RELAYCONTROL_OPERATION_MODE_ENUM eMode)
{
  bool bRet = false;

  switch (eMode)
  {
    case E_MODE_KEEP_OFF:
    case E_MODE_KEEP_ON:
    case E_MODE_SCHEDULE:
    break;
    default:
    {
      QDEBUG << "ERROR: unsupported relay mode";
      return false;
    }
  }

  if ((relay >= 0) && (relay < RELAYCONTROL_NUMBER_OF_RELAYS))
  {
    m_Relays[relay].eOperationMode = eMode;
    saveSettings();
    bRet = true;
  }
  else
  {
    QDEBUG << "ERROR: relay index out of bounds";
    bRet = false;
  }

  return bRet;
}

RelayControl::RELAYCONTROL_OPERATION_MODE_ENUM RelayControl::getRelayMode(const int relay) const
{
  RELAYCONTROL_OPERATION_MODE_ENUM eRet = E_MODE_KEEP_OFF;

  if ((relay >= 0) && (relay < RELAYCONTROL_NUMBER_OF_RELAYS))
  {
    eRet = m_Relays[relay].eOperationMode;
  }
  else
  {
    QDEBUG << "ERROR: relay index out of bounds";
  }

  return eRet;

}


RelayControl::RELAYCONTROL_STATES_ENUM RelayControl::getRelayState(const int relay) const
{
  RELAYCONTROL_STATES_ENUM eRet = E_STATE_OFF;

  if ((relay >= 0) && (relay < RELAYCONTROL_NUMBER_OF_RELAYS))
  {
    eRet = m_Relays[relay].eRelayState;
  }
  else
  {
    QDEBUG << "ERROR: relay index out of bounds";
  }

  return eRet;
}


bool RelayControl::setRelaySchedule(const int relay, const QString schedule)
{
  bool bRet = false;

  if ((relay >= 0) && (relay < RELAYCONTROL_NUMBER_OF_RELAYS))
  {
    m_Relays[relay].weeklySchedule = schedule.split(SCHEDULE_DELIMITER);

    // check if schedule is complete
    for (int day = 0; day<7; day++)
    {
      if (day >= m_Relays[relay].weeklySchedule.length())
      {
        m_Relays[relay].weeklySchedule.append("000000000000000000000000");
      }
      else
      {
        QString s = m_Relays[relay].weeklySchedule.at(day).trimmed();
        s = s.replace(QRegExp("[^01]"),"0");
        if (s.length() < 24)
        {
          // fill up with "0"
          s += QString((24-s.length()), '0');
          m_Relays[relay].weeklySchedule[day] = s;
        }
      }
    }
    bRet = true;
    saveSettings();
  }
  else
  {
    QDEBUG << "ERROR: relay index out of bounds";
    bRet = false;
  }

  return bRet;
}


QString RelayControl::getRelaySchedule(const int relay) const
{
  QString strRet = "ERROR: relay index out of bounds";

  if ((relay >= 0) && (relay < RELAYCONTROL_NUMBER_OF_RELAYS))
  {
    strRet = m_Relays[relay].weeklySchedule.join(SCHEDULE_DELIMITER);
  }

  return strRet;
}


void RelayControl::saveSettings(void)
{
  for (int i = 0; i<RELAYCONTROL_NUMBER_OF_RELAYS; i++)
  {
    m_Settings.beginGroup(QString("Relay"+QString::number(i)));
      m_Settings.setValue("OperationMode", (int)m_Relays[i].eOperationMode);
      m_Settings.setValue("IsOpener", m_Relays[i].relayIsOpener);
      m_Settings.setValue("GPIO", m_Relays[i].iGPIO);
      m_Settings.setValue("WeeklySchedule", m_Relays[i].weeklySchedule.join(SCHEDULE_DELIMITER));
    m_Settings.endGroup();
  }
}

void RelayControl::loadSettings(void)
{
  for (int i = 0; i<RELAYCONTROL_NUMBER_OF_RELAYS; i++)
  {
    m_Relays[i].eOperationMode = (RELAYCONTROL_OPERATION_MODE_ENUM)m_Settings.value(QString("Relay"+QString::number(i)+"/OperationMode"), ((int)E_MODE_KEEP_OFF)).toInt();
    m_Relays[i].relayIsOpener = m_Settings.value(QString("Relay"+QString::number(i)+"/IsOpener"), (false)).toBool();
    m_Relays[i].iGPIO = m_Settings.value(QString("Relay"+QString::number(i)+"/GPIO"), UNSET_GPIO).toInt();
    QString strSchedule = m_Settings.value(QString("Relay"+QString::number(i)+"/WeeklySchedule"), "").toString();
    m_Relays[i].weeklySchedule = strSchedule.split(SCHEDULE_DELIMITER);

    // check if schedule is complete
    for (int day = 0; day<7; day++)
    {
      if (day >= m_Relays[i].weeklySchedule.length())
      {
        m_Relays[i].weeklySchedule.append("000000000000000000000000");
      }
      else
      {
        QString s = m_Relays[i].weeklySchedule.at(day).trimmed();
        s = s.replace(QRegExp("[^01]"),"0");
        if (s.length() < 24)
        {
          // fill up with "0"
          s += QString((24-s.length()), '0');
          m_Relays[i].weeklySchedule[day] = s;
        }
      }
    }

    m_Relays[i].eOverwriteState = E_STATE_OFF;
    m_Relays[i].eRelayState = E_STATE_OFF;
    m_Relays[i].overwriteModeEndTime = QDateTime();
  }
}


/***** End of: pubslots Public slots ****************************************************************************//*@}*/

/**********************************************************************************************************************/
/* Protected slots                                                                                                    */
/*************************************************************//*!@addtogroup protslots Protected slots         *//*@{*/

void RelayControl::updateRelays(void)
{
  for (int i = 0; i<RELAYCONTROL_NUMBER_OF_RELAYS; i++)
  {
    if (false != isRelayInOverwriteMode(i))
    {
      // is in overwrite mode
      setRelay(i, m_Relays[i].eOverwriteState);
    }
    else
    {
      // in not in overwrite mode
      switch (m_Relays[i].eOperationMode)
      {
        case E_MODE_KEEP_OFF:
        {
          setRelay(i, E_STATE_OFF);
        }
        break;
        case E_MODE_KEEP_ON:
        {
          setRelay(i, E_STATE_ON);
        }
        break;
        case E_MODE_SCHEDULE:
        {
          QDateTime now = QDateTime::currentDateTime();
          int currentDayOfWeek = now.date().dayOfWeek()-1; // 0..6
          int currentHour = now.time().hour();             // 0..23

          if (
                 (currentDayOfWeek >= 0)
              && (currentDayOfWeek < 7)
              && (currentHour >= 0)
              && (currentHour < 24)
              )
          {
            QString s = m_Relays[i].weeklySchedule.at(currentDayOfWeek);

            if (currentHour > s.length())
            {
              QDEBUG << "ERROR: schedule string for day" << currentDayOfWeek << "is too short";
              setRelay(i, E_STATE_OFF);
            }
            else
            {
              if (s.at(currentHour) == QChar('1'))
              {
                setRelay(i, E_STATE_ON);
              }
              else
              {
                setRelay(i, E_STATE_OFF);
              }
            }
          }
          else
          {
            QDEBUG << "ERROR: current day of week or hour invalid";
            setRelay(i, E_STATE_OFF);
          }
        }
        break;
        default:
        {
          QDEBUG << "ERROR: unsupported relay operation mode" << m_Relays[i].eOperationMode;
          setRelay(i, E_STATE_OFF);
        }
      }
    }
  }

  QTimer::singleShot(1000, Qt::VeryCoarseTimer, this, SLOT(updateRelays()));

}




void RelayControl::setRelay(int relay, RELAYCONTROL_STATES_ENUM eState)
{
  if ((relay >= 0) && (relay < RELAYCONTROL_NUMBER_OF_RELAYS))
  {
    switch (eState)
    {
      case E_STATE_ON:
        {
          m_Relays[relay].eRelayState = E_STATE_ON;
        }
        break;
        case E_STATE_OFF:
        {
          m_Relays[relay].eRelayState = E_STATE_OFF;
        }
        break;
      default:
        {
          QDEBUG << "ERROR: invalid relay state";
        }
        break;
    }
    setGPIO(relay, m_Relays[relay].eRelayState);
  }
  else
  {
    QDEBUG << "ERROR: relay index out of bounds";
  }
}


void RelayControl::setGPIO(int relay, RELAYCONTROL_STATES_ENUM eState) const
{
  if ((relay >= 0) && (relay < RELAYCONTROL_NUMBER_OF_RELAYS))
  {
    if ((E_STATE_OFF != eState) && (E_STATE_ON != eState))
    {
      QDEBUG << "ERROR: invalid syntax";
      return;
    }

    if (UNSET_GPIO == m_Relays[relay].iGPIO)
    {
      //QDEBUG << "Warning: no GPIO assigned to relay" << relay;
      return;
    }

    // invert new output state if relay works as an opener
    if (false != m_Relays[relay].relayIsOpener)
    {
      if (E_STATE_OFF == eState)
      {
        eState = E_STATE_ON;
      }
      else
      {
        eState = E_STATE_OFF;
      }
    }

    if (E_STATE_ON == eState)
    {
#ifdef Q_OS_WIN
#warning "WiringPi library not available for Windows"
#else
      // write 0 to activate relay
      digitalWrite(m_Relays[relay].iGPIO, LOW);
#endif
    }
    else
    {
#ifdef Q_OS_WIN
#warning "WiringPi library not available for Windows"
#else
      // write 1 to deactivate relay
      digitalWrite(m_Relays[relay].iGPIO, HIGH);
#endif
    }

  }
  else
  {
    QDEBUG << "ERROR: relay index out of bounds";
  }
}

/***** End of: protslots Protected slots ************************************************************************//*@}*/

/**********************************************************************************************************************/
/* Private slots                                                                                                      */
/*****************************************************************//*!@addtogroup privslots Private slots       *//*@{*/
/***** End of:privslots Private slots ***************************************************************************//*@}*/

/**** Last line of source code                                                                                     ****/
