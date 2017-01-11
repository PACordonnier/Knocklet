/**
  ******************************************************************************
  * @file    BLueproc.cpp
  * @author  Nicolas LAW
  * @version V1.0.0
  * @date    05-January-2017
  * @brief   Gestionnaire des procédures BLE
  ******************************************************************************/

#include "BLueproc.h"				// Gestionnaire des procédures BLE
#include "BEvendef.h"				// Définition des évènements relatifs au BLE
#include "BParadef.h"				// Définition des paramètres relatifs au BLE

#include "hci.h"					// Librairie Host Controller Interface
#include "bluenrg_gatt_aci.h"		// Header file with GATT commands for BlueNRG FW6.3.
#include "bluenrg_gap.h"			// Header file for BlueNRG's GAP layer. 
#include "bluenrg_gap_aci.h"		// Header file with GAP commands for BlueNRG FW6.3.
#include "mbed.h"					// Librairie Mbed

DigitalIn test(PC_13);

/*----------------------------------------------------------------------------
* ALLOCATION DE VARIABLES
*---------------------------------------------------------------------------*/
bool		BLX_bond; 	// Device Bonded
uint8_t		BLP_proc; 	// Procedure en cours
bool		BLP_disc; 	// Flag de discoverable device BLE
bool		BLP_pdis; 	// Demande de procedure discoverable en cours
bool		BLP_pwdi; 	// Demande de procedure discoverable avec whitelist
bool		BLP_conn; 	// Flag d'un device connecté

/*----------------------------------------------------------------------------
* BLX_initproc() : Initialisation des flags des procédures ble
*-----------------------------------------------------------------------------
* Input  : -
* Output : -
* Return : - Success or not
*-----------------------------------------------------------------------------
*
*---------------------------------------------------------------------------*/
bool BLX_initproc(void)
{
	// Aucune procédure en cours
	BLP_proc = BLP_PROC_NONE;
	
	// Nécessite mode white list au démarrage
	BLX_bond = false;
	BLP_disc = false;
	BLP_pdis = false;
	BLP_pwdi = false;
	BLP_conn = false;
	
	return true;
}

/*----------------------------------------------------------------------------
* BLX__process() : Traitement d'une occurence de traitement
*-----------------------------------------------------------------------------
* Input  : -
* Output : -
* Return : -
*-----------------------------------------------------------------------------
*
*---------------------------------------------------------------------------*/
bool BLX__process(void)
{
	tBleStatus cret = BLE_STATUS_SUCCESS;	// Code retour
	
	// Processing du BLE
	HCI_Process();
	
	if (test.read() == false)
	{
		para.BPX_data.carA.data.vale[0] = 0x01;
		para.BPX_data.carA.data.updt = true;
	}
	
	// Aucune procédure est en cours
	else if (BLP_proc == false)
	{
		// Nécessite un discoverable all
		if (BLP_pdis == true)
		{
			// Set du device en discoverable
			if (BLP_set_disc(NO_WHITE_LIST_USE) == false)
			{
				return false;
			}
		}
		
		// Nécessite un discoverable white list
		else if (BLP_pwdi == true)
		{
			// Set du device en discoverable
			if (BLP_set_disc(WHITE_LIST_FOR_ALL) == false)
			{
				return false;
			}
		}
	
		// Mise à jour des données BLE
		if (BPX_updtdata(&para) == false)
		{
			return false;
		}
	}

	return true;
}

/*----------------------------------------------------------------------------
* BLP_set_disc() : Puts the device in connectable mode
*-----------------------------------------------------------------------------
* Input  : - Mode discoverable
* Output : -
* Return : -
*-----------------------------------------------------------------------------
* If you want to specify a UUID list in the advertising data, those data can
* be specified as a parameter in aci_gap_set_discoverable().
* For manufacture data, aci_gap_update_adv_data must be called.
*---------------------------------------------------------------------------*/
bool BLP_set_disc(unsigned mode)
{
	tBleStatus cret = BLE_STATUS_SUCCESS;	// Code retour
	
	// LOGO Corporate
	// https://www.bluetooth.com/specifications/assigned-numbers/16-bit-uuids-for-members
	uint8_t serviceUUIDList[] = { AD_TYPE_SERVICE_DATA, 0xE4, 0xFE };

	const char name[] = { AD_TYPE_COMPLETE_LOCAL_NAME, 'K', 'N', 'O', 'C', 'K', 'L', 'E', 'T', '0', '1' };

	// Flag de vérification
	if (BLP_disc == false)
	{
		/* disable scan response */
		cret = hci_le_set_scan_resp_data(0, NULL);
		if (cret != BLE_STATUS_SUCCESS)
		{
			return false;
		}
		
		//// Set le device en discoverable général
		cret = aci_gap_set_discoverable(ADV_IND, 0, 0, PUBLIC_ADDR, mode, sizeof(name), name, 3, serviceUUIDList, 0, 0);
		if (cret != BLE_STATUS_SUCCESS)
		{
			
		}
		else
		{
			// Activation du flag de device discoverable
			BLP_disc = true;
		}
	}

	// Desactivation de la procedure en fonction du mode demandé
	if (mode == WHITE_LIST_FOR_ALL)
	{
		// Plus besoin de discoverable white list
		BLP_pwdi = false;
	}
	else if (mode == NO_WHITE_LIST_USE)
	{
		// Plus besoin de discoverable
		BLP_pdis = false;
	}

	return true;
}

/*----------------------------------------------------------------------------
* BLX_clearble() : Efface la security database du BLE
*-----------------------------------------------------------------------------
* Input  :
* Output : -
* Return : - Connecté ou non
*-----------------------------------------------------------------------------
*
*---------------------------------------------------------------------------*/
bool BLX_clearble(void)
{
	tBleStatus cret; // Code retour

	// Arrêt du BLE
	BLX_stop_ble();

	// On clean la white list
	cret = hci_le_clear_white_list();
	if (cret != BLE_STATUS_SUCCESS)
	{
		return false;
	}

	// On clean la database
	cret = aci_gap_clear_security_database();
	if (cret != BLE_STATUS_SUCCESS)
	{

	}

	// On remet nos flags et adresses stockés à zéro
	// Aucune procédure en cours
	BLP_proc = BLP_PROC_NONE;

	// On met tout les flags à zéro
	BLX_bond = false;
	BLP_disc = false;
	BLP_pdis = false;
	BLP_pwdi = false;
	BLP_conn = false;
	BLP_pcon = false;

	// Mise à zero des informations BLE
	BEX_clie.hand = false;
	BEX_clie.hand = BEX_DECO_CLIE;
	BEX_clie.stat = 0;
	memset(BEX_clie.addr, 0, sizeof(BEX_clie.addr));

	return true;
}

/*----------------------------------------------------------------------------
* BLX_stop_ble() : Arrêt du device BLE
*-----------------------------------------------------------------------------
* Input  : -
* Output : -
* Return : -
*-----------------------------------------------------------------------------
*
*---------------------------------------------------------------------------*/
void BLX_stop_ble(void)
{
	tBleStatus cret; // Code retour

	// Coupe la procédure de connexion si besoin
	if (BLP_pcon == true)
	{
		// Plus besoin de tenter de se connecter
		BLP_pcon = false;

		// On demande d'arrêter la procedure de connexion
		cret = aci_gap_terminate_gap_procedure(GAP_DIRECT_CONNECTION_ESTABLISHMENT_PROC);
		if (cret != BLE_STATUS_SUCCESS)
		{

		}
	}

	// On test si l'on est dans le mode discoverable
	if (BLP_disc == true)
	{
		// Arrêt du mode discoverable
		cret = aci_gap_set_non_discoverable();
		if (cret != BLE_STATUS_SUCCESS)
		{

		}
		else
		{
			// Le device n'est plus en mode discoverable
			BLP_disc = false;
		}
	}

	// Termine la connexion si la connection est existante
	if (BLP_conn == true)
	{
		// Page 806 Core_V4.1
		cret = aci_gap_terminate(BEX_clie.hand, 1);
		if (cret != ERR_CMD_SUCCESS)
		{
			if (cret != ERR_UNKNOWN_CONN_IDENTIFIER)
			{

			}
		}
	}
	
	HCI_Process();

	// Remise à zéro des flags
	BLP_proc = BLP_PROC_NONE;
}
