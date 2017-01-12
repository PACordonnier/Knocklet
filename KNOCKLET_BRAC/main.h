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
#ifdef __cplusplus
}
#endif
	
#endif
