/**
  ******************************************************************************
  * @file    BEvendef.cpp
  * @author  Nicolas LAW
  * @version V1.0.0
  * @date    05-January-2017
  * @brief   Définition des évènements relatifs au BLE
  ******************************************************************************/

#include "BEvendef.h"				// Définition des évènements relatifs au BLE
#include "BLueproc.h"				// Gestionnaire des procédures BLE
#include "BParadef.h"				// Définition des paramètres relatifs au BLE

#include "bluenrg_gatt_aci.h"		// Header file with GATT commands for BlueNRG FW6.3.
#include "bluenrg_aci_const.h"		// Header file with ACI definitions for BlueNRG FW6.3.
#include "bluenrg_gap.h"			// Header file for BlueNRG's GAP layer. 
#include "bluenrg_gap_aci.h"		// Header file with GAP commands for BlueNRG FW6.3.
#include "sm.h"						// Macros Security Manager
#include "mbed.h"					// Librairie Mbed

/*----------------------------------------------------------------------------
* ALLOCATION DE VARIABLES
*---------------------------------------------------------------------------*/
BEX_CONN BEX_clie;	// Handle du client
BEX_CONN BEX_slav;	// Handle du slave

/*----------------------------------------------------------------------------
* HCI_Event_CB() : Receive packet
*-----------------------------------------------------------------------------
* Input  : - Packet
* Output : -
* Return : -
*-----------------------------------------------------------------------------
*
*---------------------------------------------------------------------------*/
void HCI_Event_CB(void *pckt)
{
	hci_uart_pckt     *hciu;	// Obtain uart packet
	hci_event_pckt    *pack;	// Obtain event packet

	// Recuperation du packet uart
	hciu = (hci_uart_pckt*)pckt;

	// Vérification du type de packet événement
	if (hciu->type != HCI_EVENT_PKT)
		return;

	// Recuperation de l'événement
	pack = (hci_event_pckt*)hciu->data;

	// Sélection de l'événement géneral
	switch (pack->evt)
	{
	// Evénement vendor
	case EVT_VENDOR:
		{
			// Traitement de l'événement vendor
			if (BEX_vendeven(pack->data) == false)
			{
				
			}
		}
		break;

	// Evénement meta
	case EVT_LE_META_EVENT:
		{
			// Trateiment de l'événement meta
			if (BEX_metaeven(pack->data) == false)
			{
				
			}
		}
		break;

	// Déconnexion d'un client
	//Page 1138 La bible BLE v4.1
	case EVT_DISCONN_COMPLETE:
		{
			evt_disconn_complete *deco = (evt_disconn_complete *)pack;
			// If the disconnection was not successful, the value of the reason parameter can be ignored by the Host
			BEX_decoclie(deco->handle, deco->reason, deco->status);
		}
		break;

	// Erreur dans le hardware
	case EVT_HARDWARE_ERROR:
		{
			
		}
		break;

	// Command Complete
	// Page 1150 La bible BLE v4.1
	case EVT_CMD_COMPLETE:
		{
			
		}
		break;

	// Evénement non traité
	default:
		{

		}
		break;

	}
}

/*----------------------------------------------------------------------------
* BEX_metaeven() : Evénement Meta
*-----------------------------------------------------------------------------
* Input  : - Packet
* Output : -
* Return : - Success or not
*-----------------------------------------------------------------------------
*
*---------------------------------------------------------------------------*/
bool BEX_metaeven(void *pack)
{
	// Récuperation de l'événement meta
	evt_le_meta_event *meta = (evt_le_meta_event *)pack;

	// Sélection du sub événement meta
	switch (meta->subevent)
	{
	// A device has been discovered (only for BlueNRG_MS)
	case EVT_LE_ADVERTISING_REPORT:
		{
			le_advertising_info *even = (le_advertising_info *)(meta->data + 1); // evt->data[0] isnumber of reports (On BlueNRG-MS is always 1)
			BEX_devfound(even->evt_type, even->bdaddr, even->data_length, even->data_RSSI[even->data_length]);

		}
		break;

	// Connexion d'un client
	// Page 1135 La bible BLE v4.1
	case EVT_LE_CONN_COMPLETE:
		{
			evt_le_connection_complete *even = (evt_le_connection_complete *)meta->data;
    		BEX_connclie(even->handle, even->peer_bdaddr, even->status);
		}
		break;

    // Mise à jour du status de connexion
	case EVT_LE_CONN_UPDATE_COMPLETE:
		{
			
		}
		break;

	// Sub-Evenement non traité
	default:
		{
			
		}
		break;
	}

	return true;
}

/*----------------------------------------------------------------------------
* BEX_vendeven() : Evénement Vendor
*-----------------------------------------------------------------------------
* Input  : - Packet
* Output : -
* Return : - Success or not
*-----------------------------------------------------------------------------
*
*---------------------------------------------------------------------------*/
bool BEX_vendeven(void *pack)
{
	// Récuperation de l'événement vendor
	evt_blue_aci *blue = (evt_blue_aci*)pack;

	// Séléction du type de vendor événement
	switch (blue->ecode & BEX_MASK_EVEN)
	{
	// Evénemenent en provenance du serveur GATT
	case BEX_EVEN_GATT:
		{
			// Traitement de l'événement GATT
			if (BEX_gatteven(blue) == false)
			{
				
			}
		}
		break;

	// Evénemenent en provenance du serveur GAP
	case BEX_EVEN__GAP:
		{
			// Traitement de l'événement GAP
			if (BEX_gap_even(blue) == false)
			{
				
			}
		}
		break;

	// Evénement non traité
	default:
		{
			
		}
		break;
	}

	return true;
}

/*----------------------------------------------------------------------------
* BEX_gatteven() : Evénement GATT
*-----------------------------------------------------------------------------
* Input  : - Packet
* Output : -
* Return : - Success or not
*-----------------------------------------------------------------------------
*
*---------------------------------------------------------------------------*/
bool BEX_gatteven(void *pack)
{
	evt_blue_aci *blue = (evt_blue_aci*)pack;	// Vendor event

	// Sélection de l'événement GATT
	switch (blue->ecode)
	{
	/******************** Event GATT Server ********************/

	// Modification d'un attribut du BLE
	case EVT_BLUE_GATT_ATTRIBUTE_MODIFIED:
		{
#if BLUENRG_MS
			evt_gatt_attr_modified_IDB05A1 *even = (evt_gatt_attr_modified_IDB05A1*)blue->data;
			BEX_var_attr(even->attr_handle, even->data_length, even->att_data);
#else
			evt_gatt_attr_modified_IDB04A1 *even = (evt_gatt_attr_modified_IDB04A1*)blue->data;
			BEX_var_attr(even->attr_handle, even->data_length, even->att_data);
#endif
		}
		break;

	// Requête d'écriture sur une characteristic
	case EVT_BLUE_GATT_WRITE_PERMIT_REQ:
		{
			evt_gatt_write_permit_req *even = (evt_gatt_write_permit_req*)blue->data;
			BEX_requwrit(even->attr_handle, even->conn_handle, even->data_length, even->data);
		}
		break;

	// Requête de lecture sur une characteristic
	case EVT_BLUE_GATT_READ_PERMIT_REQ:
		{
			evt_gatt_read_permit_req *even = (evt_gatt_read_permit_req*)blue->data;
			BEX_requread(even->attr_handle, even->conn_handle);
		}
		break;

	/******************** Event GATT Client ********************/

	// Notification d'un attribut du BLE par le serveur
	case EVT_BLUE_GATT_NOTIFICATION:
		{
		}
		break;

	// Indication d'un attribut du BLE par le serveur
	case EVT_BLUE_GATT_INDICATION:
		{
			
		}
		break;

    // Fin de procédure GATT
	case EVT_BLUE_GATT_PROCEDURE_COMPLETE:
    {
      evt_gatt_procedure_complete *even = (evt_gatt_procedure_complete*)blue->data;
      if (BEX_gattdone(even->error_code) == false)
      {
        return false;
      }
    }
    break;

	// Error Response is received from the server
	case EVT_BLUE_GATT_ERROR_RESP:
		{
			
		}
		break;

	/******************** Event GATT Server/Client ********************/

	// GATT timeout
	case EVT_BLUE_GATT_PROCEDURE_TIMEOUT:
		{
			
		}
		break;

	// Evénement non traité
	default:
		{
			
		}
		break;
	}

	return true;
}

/*----------------------------------------------------------------------------
* BEX_gap_even() : Evénement GAP
*-----------------------------------------------------------------------------
* Input  : - Packet
* Output : -
* Return : - Success or not
*-----------------------------------------------------------------------------
*
*---------------------------------------------------------------------------*/
bool BEX_gap_even(void *pack)
{
	evt_blue_aci *blue = (evt_blue_aci*)pack;	// Vendor event

	// Sélection de l'événement GAP
	switch (blue->ecode)
	{
    /******************** Event GAP Central ********************/

		// A device has been discovered (only for BlueNRG device)
		case EVT_BLUE_GAP_DEVICE_FOUND:
			{
				
			}
			break;

    /******************** Event GAP Peripheral ********************/

	// Limited discoverable mode timeout (180 secondes)
		case EVT_BLUE_GAP_LIMITED_DISCOVERABLE:
			{
				
			}
			break;

    // Impossible de résoudre l'adresse privé
		case EVT_BLUE_GAP_ADDR_NOT_RESOLVED_IDB05A1:
			{
				
			}
			break;

    // Niveau de sécurité du slave reçu par le master
		case EVT_BLUE_GAP_SLAVE_SECURITY_INITIATED:
			{
				
			}
			break;

	// Demande de re-bond du master
		case EVT_BLUE_GAP_BOND_LOST:
			{
				
	#if BLUENRG_MS
				aci_gap_allow_rebond_IDB05A1(BEX_clie.hand);
	#else
				aci_gap_allow_rebond_IDB04A1();
	#endif
			}
			break;

    /******************** Event GAP Central/Peripheral ********************/

    // Procédure terminé
		case EVT_BLUE_GAP_PROCEDURE_COMPLETE:
			{
				evt_gap_procedure_complete *even = (evt_gap_procedure_complete*)blue->data;
				if (BEX_gap_done(even->procedure_code, even->status, even->data) == false)
				{
					return false;
				}
			}
			break;

		// Requête de mot de passe pour pairing
		case EVT_BLUE_GAP_PASS_KEY_REQUEST:
			{
				evt_gap_pass_key_req *even = (evt_gap_pass_key_req*)blue->data;
				aci_gap_pass_key_response(even->conn_handle, 0x00123456);
			}
			break;

	// Authorization request
		case EVT_BLUE_GAP_AUTHORIZATION_REQUEST:
			{
				aci_gap_authorization_response(BEX_clie.hand, CONNECTION_AUTHORIZED);
			}
			break;

	// Evénement concernant le pairing
		case EVT_BLUE_GAP_PAIRING_CMPLT:
			{
				evt_gap_pairing_cmplt *even = (evt_gap_pairing_cmplt*)blue->data;
				switch (even->status)
				{
	
					// Pairing successful
					case (SM_PAIRING_SUCCESS):
						{					
							// Device bonded
							BLX_bond = true;
						}
						break;
	
					// Pairing timeout
					case (SM_PAIRING_TIMEOUT):
						{
							
						}
						break;
	
					// Pairing failed
					case (SM_PAIRING_FAILED):
						{
							
						}
						break;
				}
			}
			break;

	// Evénement non traité
	default:
		{
			
		}
		break;
	}

	return true;
}

/*----------------------------------------------------------------------------
* BEX_connclie() : Connexion d'un client
*-----------------------------------------------------------------------------
* Input  : - Handle du client
*          - Adresse du client
*          - Status du client
* Output : -
* Return : - Success or not
*-----------------------------------------------------------------------------
*
*---------------------------------------------------------------------------*/
bool BEX_connclie(uint16_t hand, tBDAddr addr, uint8_t stat)
{
	// Slave connecté, 
	BLP_slav = true;
	BLP_pcon = false;
		
	BEX_slav.find = true;
	BEX_slav.hand = hand;
	BEX_slav.stat = stat;
	memcpy(BEX_slav.addr, addr, sizeof(tBDAddr));
	
	return true;
}

/*----------------------------------------------------------------------------
* BEX_decoclie() : Déconnexion d'un client
*-----------------------------------------------------------------------------
* Input  : - Handle déconnecté
*          - Reason
*          - Status
* Output : -
* Return : -
*-----------------------------------------------------------------------------
*
*---------------------------------------------------------------------------*/
void BEX_decoclie(uint16_t hand, uint8_t reas, uint8_t stat)
{
	tBleStatus cret; // Code retour

	// Page 806 La bible ble v4.1
	cret = aci_gap_terminate(hand, reas);
	if (cret != ERR_UNKNOWN_CONN_IDENTIFIER)
	{
		
	}

	// Déconnexion du client
	BLP_disc = false;
	
	// Libération du client
	BEX_clie.find = false;
	BEX_clie.hand = BEX_DECO_CLIE;
	BEX_clie.stat = 0;
	memset(BEX_clie.addr, 0, sizeof(BEX_clie.addr));

	// On demande de réactiver une procédure de discovery
	if (BLX_bond == true && BLP_pwdi == false)
	{
		BLP_pwdi = true;
	}
	else 
	{
		BLP_pdis = true;
	}
}

/*----------------------------------------------------------------------------
* BEX_gap_done() : Procédure du gap serveur complète
*-----------------------------------------------------------------------------
* Input  : - Code de procédure
*          - Status
*          - Données
* Output : -
* Return : - Success or not
*-----------------------------------------------------------------------------
*
*---------------------------------------------------------------------------*/
bool BEX_gap_done(uint8_t code, uint8_t stat, uint8_t *data)
{
	// Plus de procédure en cours
  BLP_proc = false;

	// Sélection du code procédure
	switch (code)
	{
		// Discovery done
		case GAP_GENERAL_DISCOVERY_PROC:
			{
				
			}
			break;
		
		// Connection done
		case GAP_DIRECT_CONNECTION_ESTABLISHMENT_PROC:
			{
				// Plus besoin de se connecter
				BLP_conn = false;
				
				// Aucune procédure en cours
				BLP_proc = 0;
			}
			break;
		
		// Discovery in limited mode
		case GAP_LIMITED_DISCOVERY_PROC:
			{
				
			}
			break;
		
		// Procédure inconnu
		default:
			{
				
			}
			break;
	}
	
	return true;
}

/*----------------------------------------------------------------------------
* BEX_gattdone() : Procédure du gatt serveur complète
*-----------------------------------------------------------------------------
* Input  : - Longueur de la donnée
* Output : -
* Return : - Success or not
*-----------------------------------------------------------------------------
*
*---------------------------------------------------------------------------*/
bool BEX_gattdone(uint8_t code)
{
	// Selection du code procedure
	switch (code)
	{
	case BLE_STATUS_FAILED:
		{
			
		}
		break;
		
	case BLE_STATUS_SUCCESS:
		{
			
		}
		break;
		
	default:
		{
			
		}
		break;
	}

	// Fin de procédure
	BLP_proc = BLP_PROC_NONE;  
	
	return true;
}

/*----------------------------------------------------------------------------
* BEX_requread() : Requête de lecture
*-----------------------------------------------------------------------------
* Input  : - Handle of the attribute
*		   		 - Client
* Output : -
* Return : -
*-----------------------------------------------------------------------------
*
*---------------------------------------------------------------------------*/
void BEX_requread(uint16_t hand, uint16_t clie)
{
	tBleStatus cret; // Code retour

	// Un client est connecté
	if (clie != 0)
	{
		// Accepter la lecture de la characteristic
		cret = aci_gatt_allow_read(clie);
		if (cret != BLE_STATUS_SUCCESS)
		{
			//printf("BEX_requread: Impossible d'autoriser la lecture de la characteristic %d", hand);
		}
	}
}

/*----------------------------------------------------------------------------
* BEX_requwrit() : Requête d'écriture
*-----------------------------------------------------------------------------
* Input  : - Handle of the attribute
*					 - Client
*					 - Taille de l'information
*					 - Informations
* Output : -
* Return : -
*-----------------------------------------------------------------------------
*
*---------------------------------------------------------------------------*/
void BEX_requwrit(uint16_t hand, uint16_t clie, uint8_t leng, uint8_t *data)
{
	tBleStatus cret;																	// Code retour
	uint8_t    blok = false;													// Bloquer la mise à jour ou non
	uint8_t	   eror = BLE_STATUS_INVALID_PARAMS;			// Error

	// Fonction avec vérification de blok
	
	// Inversion du blok car la fonction aci_gatt_write_response() l'impose
	blok = !blok;
	// Un client est connecté
	if (clie != 0)
	{
		// Accepter l'écriture de la characteristic sur GATT server
		cret = aci_gatt_write_response(clie, hand, blok, eror, leng, data);
		if (cret != BLE_STATUS_SUCCESS)
		{

		}
	}
}

/*----------------------------------------------------------------------------
* BEX_var_attr() : This function is called attribute value change
*-----------------------------------------------------------------------------
* Input  : - Handle of the attribute
*          - Size of the modified attribute data
*          - Pointer to the modified attribute data
* Output : -
* Return : - Success or not
*-----------------------------------------------------------------------------
* 
*---------------------------------------------------------------------------*/
void BEX_var_attr(uint16_t hand, uint8_t leng, uint8_t *data)
{
	switch (hand)
	{
	default: 
		break;
	}
}

/*----------------------------------------------------------------------------
* BEX_devfound() : Device Found
*-----------------------------------------------------------------------------
* Input  : - Type of event (@ref ADV_IND, @ref ADV_DIRECT_IND, @ref ADV_SCAN_IND, @ref ADV_NONCONN_IND, @ref SCAN_RSP)
*          - Address of the peer device found during scanning.
*          - Length of advertising or scan response data.
*          - Advertising or scan response data + RSSI. RSSI is last octect (signed integer).
* Output : -
* Return : - 
*-----------------------------------------------------------------------------
* 
*---------------------------------------------------------------------------*/
void BEX_devfound(uint8_t type, tBDAddr addr, uint8_t leng, uint8_t rssi)
{   			
	tBleStatus cret; // Code retour
	
	// Vérification de l'adresse du slave
	if (addr[0] == 0xA5 && addr[1] == 0x78 && addr[2] == 0x5E && addr[3] == 0xEB && addr[4] == 0x27 && addr[5] == 0xB8)
	{
		// Le slave est connectable
		if (type == ADV_IND)
		{
			// Aucun slave trouvé précédemment
			if (BEX_slav.find == false)
			{	    		
				// Le slave a été trouvé
				BEX_slav.find = true;
				
				// On sauvegarde l'adresse du slave
				memcpy((uint8_t*)BEX_slav.addr, (uint8_t*)addr, sizeof(tBDAddr)); 
				
				// Arrêt de la procédure de discovery
				BLP_stop = true;	
			}
		}
	}
}

