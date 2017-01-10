 /******************************************************************************
  * @file	 BInitdef.cpp
  * @author  Nicolas LAW
  * @version V1.0.0
  * @date    05-January-2017
  * @brief	 Initialisation de la board
  *******************************************************************************/

#include "BEvendef.h"				// Définition des évènements relatifs au BLE
#include "BInitdef.h"				// Initialisation de la board
#include "BLueproc.h"				// Gestionnaire des procédures BLE
#include "BParadef.h"				// Définition des paramètres relatifs au BLE

#include "stm32_bluenrg_ble.h"		// Initialisation BLE
#include "bluenrg_gatt_aci.h"		// Header file with GATT commands for BlueNRG FW6.3.
#include "bluenrg_aci_const.h"		// Header file with ACI definitions for BlueNRG FW6.3.
#include "bluenrg_gap.h"			// Header file for BlueNRG's GAP layer. 
#include "bluenrg_gap_aci.h"		// Header file with GAP commands for BlueNRG FW6.3.
#include "bluenrg_hal_aci.h"		// Header file with HCI commands for BlueNRG FW6.3.
#include "bluenrg_utils.h"			// Header file for BlueNRG utility functions 
#include "sm.h"						// Macros Security Manager
#include "mbed.h"					// Librairie Mbed

/*----------------------------------------------------------------------------
* ALLOCATION DE VARIABLES													  
*---------------------------------------------------------------------------*/		
tBDAddr		BIX_corp;			// Adresse corporate BLE
uint8_t		BIX_role;			// Role du device BLE
tBDAddr		BIX_addr;			// Adresse du serveur
tBDAddr		BIX_conn;			// Adresse du client
BPX_PARA	BIX_para;			// Liste des paramètres BLE
bool		BIX_blms;			// Bluetooth-ms device
  
/*----------------------------------------------------------------------------
* BIX_init_ble() : Initialisation du device BLE
*-----------------------------------------------------------------------------
* Input	 : - 
* Output : -
* Return : - 
*-----------------------------------------------------------------------------
* 
*---------------------------------------------------------------------------*/
bool BIX_init_ble(void)
{
	/* Initialize the BlueNRG SPI driver */
	BNRG_SPI_Init();
	
	/* Initialize the BlueNRG HCI */
	HCI_Init();
	
	// Initialisation du gestionnaire de données BLE;
	if (BPX_initdata(&para) == false)
	{
		return false;
	}
	
	return true;
}

/*----------------------------------------------------------------------------
* BIX_configur() : Configuration du device BLE
*-----------------------------------------------------------------------------
* Input	 : - 
* Output : -
* Return : - Success or not
*-----------------------------------------------------------------------------
* 
*---------------------------------------------------------------------------*/
bool BIX_configur(void)
{
	tBleStatus cret; // Code retour
	
	//Mode configuration disable
	BIX_blms = false;
	
	// Mise à zero des informations BLE
	BEX_clie.hand = BEX_DECO_CLIE;
	BEX_clie.stat = 0;
	memset(BEX_clie.addr, 0, sizeof(BEX_clie.addr));
		
	memset(BIX_corp, 0, sizeof(BIX_corp));
	
	// Reset BlueNRG hardware
	BlueNRG_RST();
	
	// Récupération de la configuration hardware
	if (BIX_get_hard() == false)
	{
		return false;
	}
	
	// Configuration du serveur
	if (BIX_conf_srv() == false)
	{
		return false;
	}
	
	// Configuration des données BLE
	if (BPX_confdata(&para) == false)
	{
		return false;
	}
	
	/* Set output power level */
  cret = aci_hal_set_tx_power_level(1,4);
	if (cret != BLE_STATUS_SUCCESS) 
	{
		return false;
	}

	return true;
}

/*----------------------------------------------------------------------------
* BIX_get_hard() : Recuperation de la configuration hardware du device BLE
*-----------------------------------------------------------------------------
* Input	 : - 
* Output : -
* Return : - Success or not
*-----------------------------------------------------------------------------
* 
*---------------------------------------------------------------------------*/
bool BIX_get_hard(void)
{
	tBleStatus cret; // Code retour
	uint8_t	   hard; // Version du hardware
	uint16_t   firm; // Version du firmware
		
	// get the BlueNRG HW and FW versions
	cret = getBlueNRGVersion(&hard, &firm);
	if (cret != BLE_STATUS_SUCCESS) 
	{
		return false;
	}
	
	// X-NUCLEO-IDB04A1 expansion board is used it's not a Master-Slave BLE device
	if (hard <= 0x30) 
	{ 
		// Test de la compatibilité
		return false;
	}
	// Bluetooth-ms Device
	else
		BIX_blms = true;
	
	/* 
	* Reset BlueNRG again otherwise we won't be able to change its MAC address. 
	* aci_hal_write_config_data() must be the first command after reset otherwise it will fail.
	*/
	BlueNRG_RST();
	
	return true;
}

/*----------------------------------------------------------------------------
* BIX_conf_srv() : Configuration des serveurs GAP et GATT
*-----------------------------------------------------------------------------
* Input	 : - 
* Output : -
* Return : - 
*-----------------------------------------------------------------------------
* 
*---------------------------------------------------------------------------*/
bool BIX_conf_srv(void)
{
	tBleStatus cret; // Code retour
	uint8_t	   mode; // Mode de configuration
	
	// Set de l'address corporate
	BIX_addr[0] = BIX_corp[0] = BIX_ADR1_CORP;
	BIX_addr[1] = BIX_corp[1] = BIX_ADR2_CORP; 
	BIX_addr[2] = BIX_corp[2] = BIX_ADR3_CORP; 
	
	// Set l'adresse du client unique
	BIX_addr[3] = 0xE1; 
	BIX_addr[4] = 0x00; 
	BIX_addr[5] = 0x05;
	   	
	// Mode 2 : slave or master, 1 connection, RAM1 and RAM2(large GATT DB)
	mode = 0x02;
	
	// Bluetooth-ms Device
	if (BIX_blms == true)
	{
		// Set du role du master
		BIX_role = GAP_PERIPHERAL_ROLE_IDB05A1;
	}
	else
	{
		// Set du role du master
		BIX_role = GAP_PERIPHERAL_ROLE_IDB05A1;
	}
	   
	// Configuration de l'adresse publique du device BLE
	cret = aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET, CONFIG_DATA_PUBADDR_LEN, BIX_addr);	  
	if (cret != BLE_STATUS_SUCCESS) 
	{
		//printf("BIX_conf_srv: Setting BD_ADDR failed.");
		return false;
	}
	
	// Configuration du role du ble
	cret = aci_hal_write_config_data(CONFIG_DATA_MODE_OFFSET, CONFIG_DATA_MODE_LEN, &mode);
	if (cret != BLE_STATUS_SUCCESS) 
	{
		return false;
	}

	// Initialisation du serveur GATT
	cret = aci_gatt_init();	   
	if (cret != BLE_STATUS_SUCCESS) 
	{
		return false;
	}

	// Bluetooth-ms Device
	if (BIX_blms == true)
	{
		// Initialisation du serveur GAP
		cret = aci_gap_init_IDB05A1(BIX_role, PRIVACY_DISABLED, BPX_VALE_TEXT, &para.BPX_gene.serv.hand, &para.BPX_gene.name.hand, &para.BPX_gene.apar.hand);
		if (cret != BLE_STATUS_SUCCESS) 
		{
			return false;
		}  
	}
	// Bluetooth Normal Device
	else
	{
		// Initialisation du serveur GAP
		cret = aci_gap_init_IDB04A1(BIX_role, &para.BPX_gene.serv.hand, &para.BPX_gene.name.hand, &para.BPX_gene.apar.hand);
		if (cret != BLE_STATUS_SUCCESS)
		{
			return false;
		}
	}
	
	// Configuration de la sécurité du device BLE
	if (BIX_conf_sec() == false)
	{
		return false;
	}
		
	// Configuration de la white list du device BLE
	if (BIX_conf_wlt() == false)
	{
		return false;
	}
	
	//// Mise à jour du nom du device
	//const char name[] = { AD_TYPE_COMPLETE_LOCAL_NAME, 'K', 'N', 'O', 'C', 'K', 'L', 'E', 'T' };
	//cret = aci_gatt_update_char_value(para.BPX_gene.serv.hand, para.BPX_gene.name.hand, NO_WHITE_LIST_USE, sizeof(name), (uint8_t *)name);
	//if (cret != BLE_STATUS_SUCCESS) 
	//{
	//	return false;
	//}
	
	return true;
}

/*----------------------------------------------------------------------------
* BIX_conf_sec() : Configuration de la sécurité du device BLE
*-----------------------------------------------------------------------------
* Input  : - 
* Output : -
* Return : - Success or not
*-----------------------------------------------------------------------------
* 
*---------------------------------------------------------------------------*/
bool BIX_conf_sec(void)
{
	tBleStatus cret; // Code retour
	
	// Configuration de l'authentification 
	uint32_t pin = 123456;
	cret = aci_gap_set_auth_requirement(MITM_PROTECTION_NOT_REQUIRED,
		OOB_AUTH_DATA_ABSENT,
		NULL,
		7,
		16,
		USE_FIXED_PIN_FOR_PAIRING,
		pin,/* Fixed pin */
		BONDING);
	if (cret != BLE_STATUS_SUCCESS) 
	{
		return false;
	}  

	return true;
}

/*----------------------------------------------------------------------------
* BIX_conf_wlt() : Configuration de la white list du device BLE
*-----------------------------------------------------------------------------
* Input	 : - 
* Output : -
* Return : - Success or not
*-----------------------------------------------------------------------------
* 
*---------------------------------------------------------------------------*/
bool BIX_conf_wlt(void)
{
	tBleStatus cret;								// Code retour
	uint8_t	   numb;								// Nombre de device
	uint8_t	   list[7]; 						// Liste des devices

	// Récupération des adresses des appareils bonded
	cret = aci_gap_get_bonded_devices(&numb, list, sizeof(list));
	if (cret != BLE_STATUS_SUCCESS)
	{
		return false;
	}
	
	// Il y a des adresses en database
	if (numb != 0)
	{
		// Device bonded, mise à jour du flag
		BLX_bond = true;
		// Set des adresses bonded dans la whitelist
		cret = aci_gap_configure_whitelist();
		if (cret != BLE_STATUS_SUCCESS)
		{
			return false;
		}
		// On demande de lancer une procedure de discoverable whitelist all
		BLP_pwdi = true;
	}
	// Pas de device dans la white list, on passe en discoverable all
	else
	{
		BLP_pdis = true;
	}
	
	return true;
}
