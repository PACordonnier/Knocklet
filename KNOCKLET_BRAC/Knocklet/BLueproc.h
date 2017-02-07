/**
  ******************************************************************************
  * @file    BLueproc.h
  * @author  Nicolas LAW
  * @version V1.0.0
  * @date    05-January-2017
  * @brief   Gestionnaire des procédures BLE
  ******************************************************************************/
#ifndef _BLUEPROC_H_
#define _BLUEPROC_H_

#ifdef __cplusplus
extern "C" {
#endif
	
#include "link_layer.h"						// Couche de liaison
#include "stdbool.h"

//****************************************************************************
// LISTE DES PIECES :
//****************************************************************************
// BLX : Traitements généraux
// BLP : Traitements sur les procédures
//****************************************************************************

/*----------------------------------------------------------------------------
* VARIABLES EXTERNALISEES
*---------------------------------------------------------------------------*/
extern bool			BLX_bond;					// Device bonded

extern uint8_t		BLP_proc;					// Procedure en cours
extern bool			BLP_broa;					// Demande de procedure broadcast mode
	
/*----------------------------------------------------------------------------
* CONSTANTES
*---------------------------------------------------------------------------*/
#define BLX_SIZE_ADDR 6					// Taille de l'adresse du serveur

#define BLX_SCAN_INTR 0x0800			// Scan interval when LE scan start Time Range: 2.5 msec to 10240 msec (0x0004 to 0x4000)
#define BLX_SCAN_WIND 0x1000			// Duration of LE scan start Time Range: 2.5 msec to 10240 msec (0x0004 to 0x4000)

#define BLX_CONN_MINI 0x0006			// Minimum value for the connection event interval Time Range: 7.5 msec to 4 seconds (0x0006 to 0x0C80)
#define BLX_CONN_MAXI 0x0078			// Maximum value for the connection event interval Time Range: 7.5 msec to 4 seconds (0x0006 to 0x0C80)

#define BLX_SLAV_LATE 0x0000			// Slave latency Range: 0x0000 to 0x01F4
#define BLX_SLAV_TOUT 0x0064			// Slave timeout Time Range: 100 msec to 32 seconds (0x000A to 0x0C80)

#define BLX_SUPE_TOUT 0x01F4			// Supervision timeout Range : 0x000A to 0x0C80\n

#define BLX_MINI_NEED 0x0050			// Minimum length of connection need 0x0000 - 0xFFFF
#define BLX_MAXI_NEED 0x0050			// Minimum length of connection need 0x0000 - 0xFFFF

#define BLP_PROC_NONE 0					// Aucune procedure en cours
	
#define BLP_TYPE_NAME 0x0A				// Size du type AD_TYPE_COMPLETE_LOCAL_NAME
#define	BLP_TYPE_DATA 0x12				// Size du type AD_TYPE_SERVICE_DATA_128_BIT_UUID
#define BLP_BROA_TIME 1500				// Temps d'activation du broadcast mode

/*----------------------------------------------------------------------------
* BLX : Traitements généraux
*---------------------------------------------------------------------------*/
bool BLX_initproc(void);					// Initialisation des flags des procédures ble
bool BLX__process(void);					// Processing de la communication BLE
bool BLP_setbroad(void);					// Puts the device in broadcast mode
	
#ifdef __cplusplus
}
#endif

#endif /* _BLUEPROC_H_ */
