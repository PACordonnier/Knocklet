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
#include "main.h"					// Include du CORE

#include "hci.h"					// Librairie Host Controller Interface
#include "bluenrg_gatt_aci.h"		// Header file with GATT commands for BlueNRG FW6.3.
#include "bluenrg_gap.h"			// Header file for BlueNRG's GAP layer. 
#include "bluenrg_gap_aci.h"		// Header file with GAP commands for BlueNRG FW6.3.
#include "mbed.h"					// Librairie Mbed

/*----------------------------------------------------------------------------
* ALLOCATION DE VARIABLES
*---------------------------------------------------------------------------*/
bool		BLX_bond; 	// Device Bonded
uint8_t		BLP_proc; 	// Procedure en cours
bool		BLP_broa;	// Demande de procedure broadcast mode

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
	BLP_broa = false;
	
	return true;
}

/*----------------------------------------------------------------------------
* BLX__process() : Traitement d'une occurence de traitement
*-----------------------------------------------------------------------------
* Input  : -
* Output : -
* Return : - Success or not
*-----------------------------------------------------------------------------
*
*---------------------------------------------------------------------------*/
bool BLX__process(void)
{
	tBleStatus cret = BLE_STATUS_SUCCESS;	// Code retour
	
	// Processing du BLE
	HCI_Process();
		
	// Aucune procédure est en cours
	if (BLP_proc == false)
	{
		// Nécessite un broadcast
		if (BLP_broa == true)
		{
			// Set du device en discoverable
			if (BLP_setbroad() == false)
			{
				Printf("BLX__process: Mode discoverable no white list fail");
				return false;
			}
		}
	
		// Mise à jour des données BLE
		if (BPX_updtdata(&para) == false)
		{
			Printf("BLX__process: Update data fail");
			return false;
		}
	}

	return true;
}

/*----------------------------------------------------------------------------
* BLP_setbroad() : Puts the device in broadcast mode
*-----------------------------------------------------------------------------
* Input  : -
* Output : -
* Return : - Success or not
*-----------------------------------------------------------------------------
* 
*---------------------------------------------------------------------------*/
bool BLP_setbroad(void)
{
	tBleStatus cret = BLE_STATUS_SUCCESS;	// Code retour
	
	uint8_t adve[] = {
#if BRAC_ACTI_NUM1
		// Nom Public
		BLP_TYPE_NAME, AD_TYPE_COMPLETE_LOCAL_NAME, 'K', 'N', 'O', 'C', 'K', 'L', 'E', 'T', '1', 
#else
		// Nom Public
		BLP_TYPE_NAME, AD_TYPE_COMPLETE_LOCAL_NAME, 'K', 'N', 'O', 'C', 'K', 'L', 'E', 'T', '2', 
#endif // BRAC_ACTI_NUM1
		// Adresse du Service
		BLP_TYPE_DATA, AD_TYPE_SERVICE_DATA_128_BIT_UUID, 0x00, 0x00, 0x6c, 0x6f, 0x6f, 0x63, 0x6c, 0x61, 0x6c, 0x65, 0x64, 0x65, 0x72, 0x69, 0x6f, 0x62, para.BPX_data.carA.data.vale[0]};

	// Set broadcast mode
	cret = aci_gap_set_broadcast_mode(0x00A0, 0x00A0, ADV_NONCONN_IND, PUBLIC_ADDR, sizeof(adve), adve, 0, NULL);
	if (cret != BLE_STATUS_SUCCESS)
	{
		Printf("BLP_setbroad: Set broadcast mode fail %02x", cret);
		return false;
	}
		
	else
	{
		// Attente que le recepteur recoit la valeur
		Printf("BLP_setbroad: Broadcast Mode ON");
		wait_ms(BLP_BROA_TIME);

		// On arrete le broadcast
		cret = aci_gap_set_non_discoverable();
		if (cret != BLE_STATUS_SUCCESS)
		{
			Printf("BLP_setbroad: Set non discoverable mode fail %02x", cret);
			return false;
		}
		else
		{
			// Process broadcast termine
			BLP_broa = false;
			Printf("BLP_setbroad: Broadcast Mode OFF");
			// Remise à zero de la valeur
			para.BPX_data.carA.data.vale[0] = 0x00;

			// On reactive l'interrupt de l'accelerometre
			Printf("BLP_setbroad: Reactivation de l'accelerometre");
			ACX_stop = true;
		}
	}

	return true;
}