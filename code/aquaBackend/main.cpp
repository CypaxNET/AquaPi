/*************************************************************************************//*!@addtogroup file Files*//*@{*/
/*!
 ***********************************************************************************************************************
* File identification:      $Id: main.cpp 38 2017-12-13 14:47:28Z cypax $
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
 * @brief       Contains main() function of the software. Creates a BackendServer object.
 */
/****************************************************************************************************************//*@}*/

/**********************************************************************************************************************/
/* Library Includes                                                                                                   */
/**********************************************************************************************************************/
#include <QCoreApplication>
#include <QObject>
#include <QtGlobal>
#include <QDebug>

/**********************************************************************************************************************/
/* Own Includes                                                                                                       */
/**********************************************************************************************************************/
#include "GuiBackendServer.h"
#include "SunMoonBackendServer.h"

/**********************************************************************************************************************/
/* Defines                                                                                                            */
/**********************************************************************************************************************/
#define DEBUG_MAIN 1
#if DEBUG_MAIN
#define QDEBUG qDebug()<<__func__<<"()\t"
#else
#define QDEBUG if (0) qDebug()
#endif

#define MIN_ARGC_VALUE    1
#define ARG_GUI_PORT      "-gp"
#define ARG_MOON_PORT     "-mp"
#define ARG_USAGE         "-?"
#define DEFAULT_GUI_PORT  (long)58236
#define DEFAULT_MOON_PORT (long)58237
#define MIN_PORT_VALUE    (long)1024
#define MAX_PORT_VALUE    (long)65535

enum iProgramExitCodes
{
  iProgramExitSuccess =          (int)0,
  iProgramInvalidArgument =      (int)1,
  iProgramArgPortWithoutNumber = (int)2,
  iProgramArgPortWithInvalidNo = (int)3,
  iProgramArgHostWithoutName   = (int)4,
  iProgramAddToQtReturnValue   = (int)5
};

/**********************************************************************************************************************/
/* Global Variables Definitions                                                                                       */
/**********************************************************************************************************************/


/**********************************************************************************************************************/
/* Private Variables                                                                                                  */
/**********************************************************************************************************************/


/**********************************************************************************************************************/
/* Private Functions-Prototypes                                                                                       */
/**********************************************************************************************************************/



/**********************************************************************************************************************/
/* Private Functions                                                                                                  */
/**********************************************************************************************************************/
void printUsage(void)
{
  QDEBUG << "USAGE:\n"
         "[-gp <port>] [-mp <port>] [-h <host>] [-v] [-?]\n"
         "  -gp <port>    Defines the GUI listening port of the backend. (default: 58236)\n"
         "  -mp <port>    Defines the moon info listening port of the backend. (default: 58237)\n"
         "  -?            This help text.\n"
         ;
}

/**********************************************************************************************************************/
/* Global Functions                                                                                                   */
/**********************************************************************************************************************/
/**
 * @brief main
 * @param argc: number of arguments passed to the programm + 1
 * @param argv: array of pointers to the arguments
 * @return int: Integer value passed to the operating system after program termination
 */
int main(int argc, char *argv[])
{
  // Declaration of main Qt Application
  QCoreApplication coreApp(argc, argv);

  // Variables to store arguments passed to program
  long lGuiPort = DEFAULT_GUI_PORT;
  long lMoonPort = DEFAULT_MOON_PORT;

  // Read arguments and use them to configure the program
  if (1 < argc)
  {
    for (quint16 i = 1; i < argc; i++)
    {
      if (0 == strcmp(ARG_USAGE, argv[i]))
      {
        printUsage();
        return iProgramExitSuccess;
      }
      else if (0 == strcmp(ARG_GUI_PORT, argv[i]))
      {
        // If port configuration argument was passed, there must be a port no. following
        if (i < (argc -1))
        {
          i++;
          lGuiPort = strtol(argv[i], 0, 10);
          if (lGuiPort < MIN_PORT_VALUE || lGuiPort > MAX_PORT_VALUE)
          {
            printUsage();
            return iProgramArgPortWithInvalidNo;
          }
        }
        else
        {
          printUsage();
          return iProgramArgPortWithoutNumber;
        }
      }
      else if (0 == strcmp(ARG_MOON_PORT, argv[i]))
      {
        // If port configuration argument was passed, there must be a port no. following
        if (i < (argc -1))
        {
          i++;
          lMoonPort = strtol(argv[i], 0, 10);
          if (lMoonPort < MIN_PORT_VALUE || lMoonPort > MAX_PORT_VALUE)
          {
            printUsage();
            return iProgramArgPortWithInvalidNo;
          }
        }
        else
        {
          printUsage();
          return iProgramArgPortWithoutNumber;
        }
      }
      else
      {
        printUsage();
        return iProgramInvalidArgument;
      }
    }
  }

  GuiBackendServer guiBackendServer(lGuiPort);
  SunMoonBackendServer sunmoonBackendServer(lMoonPort);

  // exit application when GUI server receives terminate request
  coreApp.connect(&guiBackendServer, SIGNAL(signalTerminate()), &coreApp, SLOT(quit()));

  int retVal = coreApp.exec();
  QDEBUG << "Exit with code" << retVal;
  return retVal;
}
/**********************************************************************************************************************/


/************************* End of File ********************************************************************************/
