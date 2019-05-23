/*************************************************************************************//*!@addtogroup file Files*//*@{*/
/*!
 ***********************************************************************************************************************
* File identification:      $Id: main.cpp 45 2017-12-13 15:37:51Z cypax $
* Revision of last commit:  $Rev: 45 $
* Author of last commit:    $Author: cypax $
* Date of last commit:      $Date: 2017-12-13 16:37:51 +0100 (Mi, 13 Dez 2017) $
 ***********************************************************************************************************************
 *
 * @project     Raspberry Pi Aquarium Control
 *
 * @author      Cypax / cypax.net
 * @copyright
 *
 * @brief       Contains main() function of the software. Sends a TCP request.
 */
/****************************************************************************************************************//*@}*/

/**********************************************************************************************************************/
/* Library Includes                                                                                                   */
/**********************************************************************************************************************/
#include <QCoreApplication>
#include <QObject>
#include <QtGlobal>
#include <QDebug>

#include <iostream>
#include <stdlib.h>


using namespace std;

/**********************************************************************************************************************/
/* Own Includes                                                                                                       */
/**********************************************************************************************************************/
#include "ClientRequest.h"

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
#define ARG_PORT          "-p"
#define ARG_USAGE         "-?"
#define DEFAULT_TCP_PORT  (long)58236
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
         "[-p <port>] [-h <host>] [-v] [-?]\n"
         "  -p <port>     Defines the listening port of the backend. (default: 58236)\n"
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
  int retVal = iProgramExitSuccess;

  // Declaration of main Qt Application
  QCoreApplication coreApp(argc, argv);

  // Variables to store arguments passed to program
  long lPort = DEFAULT_TCP_PORT;

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
      else if (0 == strcmp(ARG_PORT, argv[i]))
      {
        // If port configuration argument was passed, there must be a port no. following
        if (i < (argc -1))
        {
          i++;
          lPort = strtol(argv[i], 0, 10);
          if (lPort < MIN_PORT_VALUE || lPort > MAX_PORT_VALUE)
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


  cout << "Content-type:text/html\r\n\r\n";

  // attempt to retrieve value of query string
  char* cpQuery = getenv("QUERY_STRING");

#ifdef Q_OS_WIN
  //#warning "WARNING: FIT is active!"
  //cpQuery = (char*)"request=getStatus";
  //cpQuery = (char*)"request=setRelayOverwrite6|30,OFF";
  //cpQuery = (char*)"request=getRelayState0";
  //cpQuery = (char*)"request=setRelayMode2|SCHEDULE";
  //cpQuery = (char*)"request=getRelaySchedule0";
  //cpQuery = (char*)"request=setRelaySchedule7|111111111111111111111111,111111111111111111111111,111111111111111111111111,111111111111111111111111,111111111111111111111111,111111111111111111111111,111111111111111111111111";
#endif


  if ( cpQuery != NULL )
  {
    ClientRequest clientRequest(lPort, QString(cpQuery).trimmed());

    // exit application when server receives terminate request
    coreApp.connect(&clientRequest, SIGNAL(signalTerminate()), &coreApp, SLOT(quit()));

    retVal = coreApp.exec();
  }
  else
  {
    //Environment variable does not exist.
  }


  QDEBUG << "Exit with code" << retVal;
  return retVal;
}
/**********************************************************************************************************************/


/************************* End of File ********************************************************************************/
