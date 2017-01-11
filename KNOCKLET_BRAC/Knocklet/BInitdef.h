/**
  ******************************************************************************
  * @file    BInitdef.h
  * @author  Nicolas LAW
  * @version V1.0.0
  * @date    05-January-2017
  * @brief   Initialisation de la board
  *******************************************************************************/

#ifndef _BINITDEF_H_
#define _BINITDEF_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "link_layer.h" 		// Couche de liaison

//****************************************************************************
// LISTE DES PIECES :
//****************************************************************************
// BIX : Traitements généraux
//****************************************************************************

/*----------------------------------------------------------------------------
* VARIABLES EXTERNALISEES
*---------------------------------------------------------------------------*/
extern tBDAddr	BIX_corp;										// Adresse corporate BLE
extern uint32_t BIX_pass;										// Pin d'authentification
/*----------------------------------------------------------------------------
* CONSTANTES
*---------------------------------------------------------------------------*/
#define BIX_ADR1_CORP 0x4b									// Addresse corporate(1 byte de l'adresse du device BLE)
#define BIX_ADR2_CORP 0x4E									// Addresse corporate(2 byte de l'adresse du device BLE)
#define BIX_ADR3_CORP 0x4F									// Addresse corporate(3 byte de l'adresse du device BLE)
#define BIX_SIZE_CORP 3										// Taille de l'adresse corporate
#define BIX_DEVI_SIZE 7										// Taille d'une adresse stockée dans la security database (EEPROM BLE)
#define BIX_MAXI_DEVI 12									// Maximum number of devices bonded saved in the security database (EEPROM BLE)

/*----------------------------------------------------------------------------
* BIX : Traitements généraux
*---------------------------------------------------------------------------*/
bool BIX_init_ble(void);									// Initialisation du device BLE
bool BIX_configur(void);									// Configuration du device BLE
bool BIX_get_hard(void);									// Recuperation de la configuration hardware du device BLE
bool BIX_conf_srv(void);									// Configuration des serveurs GAP et GATT
bool BIX_conf_sec(void);									// Configuration de la sécurité du device BLE
bool BIX_conf_wlt(void);									// Configuration de la white list du device BLE
#ifdef __cplusplus
}
#endif

#endif /* _BINITDEF_H_ */
