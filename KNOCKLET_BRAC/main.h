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
#include "BLueproc.h"				// Gestionnaire des procédures BLE
#include "ACcelero.h"				// Gestionnaire de l'accéleromètre
	
int	 main(void);						// Main	
bool coreinit(void);					// Initialisation du système
bool coreconf(void);					// Configuration du système
bool coreloop(void);					// Loop du CORE
void Printf(const char *format, ...);	// Printf avec delay pour le Serial et retour ligne
	
/*----------------------------------------------------------------------------
* CONFIGURATION                                              
*---------------------------------------------------------------------------*/
#define BRAC_ACTI_NUM1 1								// Mettre à 1 pour bracelet numéro 1
#define BRAC_ACTI_LOGS 0								// Activation ou non des logs
#define BRAC_ACTI_ACCE 1								// Activation ou non de l'accéleromètre
#define BRAC_ACTI_XBLE 0								// Activation ou non du Bluetooth
	
#ifdef __cplusplus
}
#endif
	
#endif
