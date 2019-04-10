/*************************************************************************************//*!@addtogroup file Files*//*@{*/
/*!
 ***********************************************************************************************************************
* File identification:      $Id: RelayControl.h 38 2017-12-13 14:47:28Z cypax $
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

#ifndef RELAYCONTROL_H
#define RELAYCONTROL_H

/**********************************************************************************************************************/
/* Library includes                                                                                                   */
/**********************************************************************************************************************/
#include <QObject>
#include <QStringList>
#include <QSettings>
#include <QDateTime>

/**********************************************************************************************************************/
/* Own includes                                                                                                       */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* Constants (#define)                                                                                                */
/*******************************************************************************//*!@addtogroup define Constants*//*@{*/

#define RELAYCONTROL_NUMBER_OF_RELAYS ((int)8)

/***** End of: define Constants *********************************************************************************//*@}*/

/**********************************************************************************************************************/
/* Class                                                                                                              */
/**********************************************************************************//*!@addtogroup class Classes*//*@{*/
/**
* @brief
**/
class RelayControl : public QObject
{
  Q_OBJECT

/**********************************************************************************************************************/
/* Types (typedef)                                                                                                    */
/**********************************************************************************//*!@addtogroup typedef Types*//*@{*/
public:

  typedef enum
  {
    E_STATE_OFF       = 0,  // relay on
    E_STATE_ON        = 1,  // relay off
  } RELAYCONTROL_STATES_ENUM;

  typedef enum
  {
    E_MODE_KEEP_OFF  = 0,  // always on
    E_MODE_KEEP_ON   = 1,  // always off
    E_MODE_SCHEDULE  = 2   // controlled by schedule
    //TODO future feature: E_MODE_AUTO      = 3   // controlled by external control
  } RELAYCONTROL_OPERATION_MODE_ENUM;

  typedef struct
  {
    RELAYCONTROL_OPERATION_MODE_ENUM  eOperationMode;       // operation mode of relay
    RELAYCONTROL_STATES_ENUM          eOverwriteState;      // overwrite state of relay
    QDateTime                         overwriteModeEndTime; // end time of overwrite mode
    bool                              relayIsOpener;        // relay operates as a switch (false) or as a opener (true)
    QStringList                       weeklySchedule;       // weekly schedule; "1" means ON for one hour
    RELAYCONTROL_STATES_ENUM          eRelayState;          // current state of relay (on / off)
    int                               iGPIO;                // assigned GPIO pin number
  } RELAYCONTROL_RELAY_STRUCT;



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
  RelayControl(void);

/**********************************************************************************************************************/
/* Destructor                                                                                                         */
/**********************************************************************************************************************/
public:
  ~RelayControl(void);

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
  bool setRelayOverwrite(int relay, int delaySeconds, RELAYCONTROL_STATES_ENUM eOverwriteState);
  bool isRelayInOverwriteMode(const int relay);
  int getRelayOverwriteDelay(const int relay);

  bool setRelayMode(const int relay, const RELAYCONTROL_OPERATION_MODE_ENUM eMode);
  RELAYCONTROL_OPERATION_MODE_ENUM getRelayMode(const int relay) const;

  RELAYCONTROL_STATES_ENUM getRelayState(const int relay) const;

  bool setRelaySchedule(const int relay, const QString schedule);
  QString getRelaySchedule(const int relay) const;

  void saveSettings(void);
  void loadSettings(void);


/**********************************************************************************************************************/
/* Protected slots                                                                                                    */
/**********************************************************************************************************************/
protected slots:
  void updateRelays(void);
  void setRelay(int relay, RELAYCONTROL_STATES_ENUM eState);
  void setGPIO(int relay, RELAYCONTROL_STATES_ENUM eState) const;

/**********************************************************************************************************************/
/* Private slots                                                                                                      */
/**********************************************************************************************************************/
private slots:

/**********************************************************************************************************************/
/* Protected attributes                                                                                               */
/**********************************************************************************************************************/
protected:

RELAYCONTROL_RELAY_STRUCT m_Relays[RELAYCONTROL_NUMBER_OF_RELAYS];
QSettings                 m_Settings;

/**********************************************************************************************************************/
/* Private attributes                                                                                                 */
/**********************************************************************************************************************/
private:
};

/***** End of: Classes ******************************************************************************************//*@}*/

#endif // RELAYCONTROL_H
/**** Last line of source code ****************************************************************************************/
