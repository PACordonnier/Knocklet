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
#ifdef __cplusplus
}
#endif
	
#endif
