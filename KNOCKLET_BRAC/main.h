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

#include "stdint.h"	// Définition des types
#include <stdlib.h>
#include <stdbool.h>
#include "cube_hal.h"
#include "hal_types.h"
#include "bluenrg_gatt_server.h"
#include "bluenrg_gap.h"
#include "string.h"
#include "bluenrg_gap_aci.h"
#include "bluenrg_gatt_aci.h"
#include "hci_const.h"
#include "gp_timer.h"
#include "bluenrg_hal_aci.h"
#include "bluenrg_aci_const.h"   
#include "hci.h"
#include "hal.h"
#include "sm.h"
#include "debug.h"
#include "osal.h"
#include "stm32_bluenrg_ble.h"
#include "bluenrg_utils.h"

#include "BInitdef.h"	// Initialisation de la board
#include "BLueproc.h"	// Gestionnaire des procédures BLE

int	main(void);							// Main	
bool coreinit(void);				// Initialisation du système
bool coreconf(void);				// Configuration du système
bool coreloop(void);				// Loop du CORE
#ifdef __cplusplus
}
#endif
	
#endif
