/**
  ******************************************************************************
  * @file    BEvendef.h
  * @author  Nicolas LAW
  * @version V1.0.0
  * @date    05-January-2017
  * @brief   Définition des évènements relatifs au BLE
  ******************************************************************************/

#ifndef _BEVENDEF_H_ 
#define _BEVENDEF_H_

#ifdef __cplusplus
extern "C" {
#endif
	 
#include "link_layer.h" // Couche de liaison
#include "stdbool.h"

//****************************************************************************
// LISTE DES PIECES :
//****************************************************************************
// BEX : Traitements généraux
//****************************************************************************

/*----------------------------------------------------------------------------
* STRUCTURE
*---------------------------------------------------------------------------*/
typedef struct	   // Définition d'un client
{
    uint16_t hand; // Handle du client
    uint8_t  stat; // Status du client
    tBDAddr  addr; // Adresse du client
    bool     find; // Client trouvé ou non
}BEX_CONN;

/*----------------------------------------------------------------------------
* CONSTANTES
*---------------------------------------------------------------------------*/
#define BEX_MAXI_CLIE 2         // Nombre maximum de client
#define BEX_DECO_CLIE 0					// il n'y a pas de client connecté sur le BLE
#define BEX_MASK_EVEN 0xFF00    // Mask pour connaître le type d'evenement vendor
#define BEX_EVEN__GAP 0x0400    // Evenement pour le GATT serveur
#define BEX_EVEN_GATT 0x0C00		// Evenement pour le GAP serveur

/*----------------------------------------------------------------------------
* VARIABLES EXTERNALISEES
*---------------------------------------------------------------------------*/
extern BEX_CONN BEX_clie; 			// Handle du client connecte

/*----------------------------------------------------------------------------
* BEX : Traitements généraux
*---------------------------------------------------------------------------*/
void		HCI_Event_CB(void *pckt);																									// Receive packet
bool		BEX_metaeven(void *pack);																									// Evénement Meta
bool		BEX_vendeven(void *pack);																									// Evénement Vendor
bool		BEX_gatteven(void *pack);																									// Evénement GATT
bool		BEX_gap_even(void *pack);																									// Evénement GAP
bool		BEX_connclie(uint16_t hand, tBDAddr addr, uint8_t stat);									// Connexion d'un client
void		BEX_decoclie(uint16_t hand, uint8_t reas, uint8_t stat);									// Déconnexion d'un client
bool		BEX_gap_done(uint8_t code, uint8_t stat, uint8_t *data);									// Procédure du GAP serveur complète
bool		BEX_gattdone(uint8_t code);																								// Procédure du GATT serveur complète
void		BEX_requread(uint16_t hand, uint16_t clie);																// Requête de lecture
void		BEX_requwrit(uint16_t hand, uint16_t clie, uint8_t leng, uint8_t *data);	// Requête d'écriture
void		BEX_var_attr(uint16_t hand, uint8_t leng, uint8_t *data);									// This function is called attribute value change
#ifdef __cplusplus
}
#endif

#endif /* _BEVENDEF_H_ */
