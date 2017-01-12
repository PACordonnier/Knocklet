/**
  ******************************************************************************
  * @file    main.h
  * @author  Law & Hurabielle
  * @version V1.0.0
  * @date    29-September-2016
  * @brief   Includes
  *******************************************************************************/
#ifndef _MAIN_H_
#define _MAIN_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "cube_hal.h"
#include "BInitdef.h"				// Initialisation de la board
#include "BLueproc.h"				// Gestionnaire des proc�dures BLE
#include "ACcelero.h"				// Gestionnaire de l'acc�lerom�tre
	
int	 main(void);						// Main	
bool coreinit(void);					// Initialisation du syst�me
bool coreconf(void);					// Configuration du syst�me
bool coreloop(void);					// Loop du CORE
void Printf(const char *format, ...);	// Printf avec delay pour le Serial et retour ligne
	
/*----------------------------------------------------------------------------
* CONFIGURATION                                              
*---------------------------------------------------------------------------*/
#define BRAC_ACTI_NUM1 0								// Mettre � 1 pour bracelet num�ro 1
#define BRAC_ACTI_LOGS 1								// Activation ou non des logs
#define BRAC_ACTI_ACCE 1								// Activation ou non de l'acc�lerom�tre
#define BRAC_ACTI_XBLE 1								// Activation ou non du Bluetooth
	
#ifdef __cplusplus
}
#endif
	
#endif
