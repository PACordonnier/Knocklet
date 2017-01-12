/**
  ******************************************************************************
  * @file    BEvendef.cpp
  * @author  Nicolas LAW
  * @version V1.0.0
  * @date    05-January-2017
  * @brief   D�finition des �v�nements relatifs au BLE
  ******************************************************************************/

#include "ACcelero.h"				// Gestionnaire de l'acc�lerom�tre
#include "BEvendef.h"				// D�finition des �v�nements relatifs au BLE
#include "BLueproc.h"				// Gestionnaire des proc�dures BLE
#include "BParadef.h"				// D�finition des param�tres relatifs au BLE
#include "main.h"					// Include du CORE

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

	// V�rification du type de packet �v�nement
	if (hciu->type != HCI_EVENT_PKT)
		return;

	// Recuperation de l'�v�nement
	pack = (hci_event_pckt*)hciu->data;

	// S�lection de l'�v�nement g�neral
	switch (pack->evt)
	{
	// Ev�nement vendor
	case EVT_VENDOR:
		{
			// Traitement de l'�v�nement vendor
			if (BEX_vendeven(pack->data) == false)
			{
				Printf("HCI_Event_CB: Probleme dans le traitement de l'evenement Vendor");
			}
		}
		break;

	// Ev�nement meta
	case EVT_LE_META_EVENT:
		{
			// Trateiment de l'�v�nement meta
			if (BEX_metaeven(pack->data) == false)
			{
				Printf("HCI_Event_CB: Probleme dans le traitement de l'evenement Meta");
			}
		}
		break;

	// D�connexion d'un client
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
			Printf("HCI_Event_CB: HARDWARE ERROR");
		}
		break;

	// Command Complete
	// Page 1150 La bible BLE v4.1
	case EVT_CMD_COMPLETE:
		{
			Printf("HCI_Event_CB: EVT_CMD_COMPLETE");
		}
		break;

	// Ev�nement non trait�
	default:
		{

		}
		break;

	}
}

/*----------------------------------------------------------------------------
* BEX_metaeven() : Ev�nement Meta
*-----------------------------------------------------------------------------
* Input  : - Packet
* Output : -
* Return : - Success or not
*-----------------------------------------------------------------------------
*
*---------------------------------------------------------------------------*/
bool BEX_metaeven(void *pack)
{
	// R�cuperation de l'�v�nement meta
	evt_le_meta_event *meta = (evt_le_meta_event *)pack;

	// S�lection du sub �v�nement meta
	switch (meta->subevent)
	{
	// A device has been discovered (only for BlueNRG_MS)
	case EVT_LE_ADVERTISING_REPORT:
		{
			le_advertising_info *even = (le_advertising_info *)(meta->data + 1); // evt->data[0] isnumber of reports (On BlueNRG-MS is always 1)
		}
		break;

	// Connexion d'un client
	// Page 1135 La bible BLE v4.1
	case EVT_LE_CONN_COMPLETE:
		{
			evt_le_connection_complete *even = (evt_le_connection_complete *)meta->data;
    		BEX_connclie(even->handle, even->peer_bdaddr, even->status);
			Printf("BEX_metaeven: EVT_LE_CONN_COMPLETE: interval: %04x, latency: %04x, supervision_timeout: %04x, master_clock_accuracy: %02x",
				even->interval,
				even->latency,
				even->supervision_timeout,
				even->master_clock_accuracy);
		}
		break;

    // Mise � jour du status de connexion
	case EVT_LE_CONN_UPDATE_COMPLETE:
		{
			Printf("BEX_metaeven: EVT_LE_CONN_UPDATE_COMPLETE");
		}
		break;

	// Sub-Evenement non trait�
	default:
		{
			
		}
		break;
	}

	return true;
}

/*----------------------------------------------------------------------------
* BEX_vendeven() : Ev�nement Vendor
*-----------------------------------------------------------------------------
* Input  : - Packet
* Output : -
* Return : - Success or not
*-----------------------------------------------------------------------------
*
*---------------------------------------------------------------------------*/
bool BEX_vendeven(void *pack)
{
	// R�cuperation de l'�v�nement vendor
	evt_blue_aci *blue = (evt_blue_aci*)pack;

	// S�l�ction du type de vendor �v�nement
	switch (blue->ecode & BEX_MASK_EVEN)
	{
	// Ev�nemenent en provenance du serveur GATT
	case BEX_EVEN_GATT:
		{
			// Traitement de l'�v�nement GATT
			if (BEX_gatteven(blue) == false)
			{
				Printf("BEX_vendeven: Probleme dans le traitement de l'evenement du GATT server");
			}
		}
		break;

	// Ev�nemenent en provenance du serveur GAP
	case BEX_EVEN__GAP:
		{
			// Traitement de l'�v�nement GAP
			if (BEX_gap_even(blue) == false)
			{
				Printf("BEX_vendeven: Probleme dans le traitement de l'evenement du GAP server");
			}
		}
		break;

	// Ev�nement non trait�
	default:
		{
			
		}
		break;
	}

	return true;
}

/*----------------------------------------------------------------------------
* BEX_gatteven() : Ev�nement GATT
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

	// S�lection de l'�v�nement GATT
	switch (blue->ecode)
	{
	/******************** Event GATT Server ********************/

	// Modification d'un attribut du BLE
	case EVT_BLUE_GATT_ATTRIBUTE_MODIFIED:
		{
			Printf("BEX_gatteven: EVT_BLUE_GATT_ATTRIBUTE_MODIFIED: modification d'un attribut sur le GATT serveur");
#if BLUENRG_MS
			evt_gatt_attr_modified_IDB05A1 *even = (evt_gatt_attr_modified_IDB05A1*)blue->data;
			BEX_var_attr(even->attr_handle, even->data_length, even->att_data);
#else
			evt_gatt_attr_modified_IDB04A1 *even = (evt_gatt_attr_modified_IDB04A1*)blue->data;
			BEX_var_attr(even->attr_handle, even->data_length, even->att_data);
#endif
		}
		break;

	// Requ�te d'�criture sur une characteristic
	case EVT_BLUE_GATT_WRITE_PERMIT_REQ:
		{
			Printf("BEX_gatteven: EVT_BLUE_GATT_WRITE_PERMIT_REQ");
			evt_gatt_write_permit_req *even = (evt_gatt_write_permit_req*)blue->data;
			BEX_requwrit(even->attr_handle, even->conn_handle, even->data_length, even->data);
		}
		break;

	// Requ�te de lecture sur une characteristic
	case EVT_BLUE_GATT_READ_PERMIT_REQ:
		{
			Printf("BEX_gatteven: EVT_BLUE_GATT_READ_PERMIT_REQ");
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

    // Fin de proc�dure GATT
	case EVT_BLUE_GATT_PROCEDURE_COMPLETE:
    {
	    Printf("BEX_gatteven: EVT_BLUE_GATT_PROCEDURE_COMPLETE: fin de procedure GATT");
		evt_gatt_procedure_complete *even = (evt_gatt_procedure_complete*)blue->data;
		if (BEX_gattdone(even->error_code) == false)
		{
			Printf("BEX_gatteven: Impossible de recuperer le status de fin de procedure");
			return false;
		}
    }
    break;

	// Error Response is received from the server
	case EVT_BLUE_GATT_ERROR_RESP:
		{
			Printf("BEX_gatteven: EVT_BLUE_GATT_ERROR_RESP");
		}
		break;

	/******************** Event GATT Server/Client ********************/

	// GATT timeout
	case EVT_BLUE_GATT_PROCEDURE_TIMEOUT:
		{
			Printf("BEX_gatteven: EVT_BLUE_GATT_PROCEDURE_TIMEOUT: GATT timeout");
		}
		break;

	// Ev�nement non trait�
	default:
		{
			Printf("BEX_gatteven: Gatt Event Unknow %04x", blue->ecode);
		}
		break;
	}

	return true;
}

/*----------------------------------------------------------------------------
* BEX_gap_even() : Ev�nement GAP
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

	// S�lection de l'�v�nement GAP
	switch (blue->ecode)
	{
    /******************** Event GAP Central ********************/

		// A device has been discovered (only for BlueNRG device)
		case EVT_BLUE_GAP_DEVICE_FOUND:
			{
				Printf("BEX_gap_even: EVT_BLUE_GAP_DEVICE_FOUND: Device discovered");
			}
			break;

    /******************** Event GAP Peripheral ********************/

	// Limited discoverable mode timeout (180 secondes)
		case EVT_BLUE_GAP_LIMITED_DISCOVERABLE:
			{
				Printf("DBG0: BEX_gap_even: EVT_BLUE_GAP_LIMITED_DISCOVERABLE: Limited discoverable mode ends due to timeout");
			}
			break;

    // Impossible de r�soudre l'adresse priv�
		case EVT_BLUE_GAP_ADDR_NOT_RESOLVED_IDB05A1:
			{
				Printf("DBG0: BEX_gap_even: EVT_BLUE_GAP_ADDR_NOT_RESOLVED_IDB05A1: Impossible de resoudre l'addresse prive");
			}
			break;

    // Niveau de s�curit� du slave re�u par le master
		case EVT_BLUE_GAP_SLAVE_SECURITY_INITIATED:
			{
				Printf("BEX_gap_even: EVT_BLUE_GAP_SLAVE_SECURITY_INITIATED: exigence de securite envoye au master");
			}
			break;

	// Demande de re-bond du master
		case EVT_BLUE_GAP_BOND_LOST:
			{
				Printf("BEX_gap_even: EVT_BLUE_GAP_BOND_LOST: tentative de re-bond");
				#if BLUENRG_MS
				aci_gap_allow_rebond_IDB05A1(BEX_clie.hand);
				#else
				aci_gap_allow_rebond_IDB04A1();
				#endif
			}
			break;

    /******************** Event GAP Central/Peripheral ********************/

    // Proc�dure termin�
		case EVT_BLUE_GAP_PROCEDURE_COMPLETE:
			{
				evt_gap_procedure_complete *even = (evt_gap_procedure_complete*)blue->data;
				if (BEX_gap_done(even->procedure_code, even->status, even->data) == false)
				{
					return false;
				}
			}
			break;

		// Requ�te de mot de passe pour pairing
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

	// Ev�nement concernant le pairing
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

	// Ev�nement non trait�
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
	Printf("BEX_connclie: EVT_LE_CONN_COMPLETE: Connexion (adresse: %02x:%02x:%02x:%02x:%02x:%02x) hand: %02x, status %02x",
		addr[5],
		addr[4],
		addr[3],
		addr[2],
		addr[1],
		addr[0],
		hand,
		stat);
	
	// API connect�
	BEX_clie.find = true;

	// Set du master api
	BEX_clie.hand = hand;
	BEX_clie.stat = stat;
	memcpy(BEX_clie.addr, addr, sizeof(tBDAddr));
	BLP_conn = true;
			
	return true;
}

/*----------------------------------------------------------------------------
* BEX_decoclie() : D�connexion d'un client
*-----------------------------------------------------------------------------
* Input  : - Handle d�connect�
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
	
	// D�connexion du client slave
	Printf("BEX_decoclie: EVT_DISCONN_COMPLETE: Deconnexion du client (hand: %02x, reason: %02x, status: %02x)", hand, reas, stat);
	
	// D�connexion du client
	BLP_disc = false;
	BLP_conn = false;
	
	// Lib�ration du client
	BEX_clie.find = false;
	BEX_clie.hand = BEX_DECO_CLIE;
	BEX_clie.stat = 0;
	memset(BEX_clie.addr, 0, sizeof(BEX_clie.addr));
	
	// Remise � z�ro de la valeur
	para.BPX_data.carA.data.vale[0] = 0x00;

	// On r�active l'interrupt de l'acc�lerom�tre
	Printf("BEX_decoclie: Reactivation de l'accelerometre");
	ACX_stop = true;
}

/*----------------------------------------------------------------------------
* BEX_gap_done() : Proc�dure du gap serveur compl�te
*-----------------------------------------------------------------------------
* Input  : - Code de proc�dure
*          - Status
*          - Donn�es
* Output : -
* Return : - Success or not
*-----------------------------------------------------------------------------
*
*---------------------------------------------------------------------------*/
bool BEX_gap_done(uint8_t code, uint8_t stat, uint8_t *data)
{
	// Plus de proc�dure en cours
	BLP_proc = false;

	// S�lection du code proc�dure
	switch (code)
	{
		// Discovery done
		case GAP_GENERAL_DISCOVERY_PROC:
			{
				Printf("BEX_gap_done: Procedure aci_gap_terminate_gap_procedure(GAP_GENERAL_DISCOVERY_PROC) termine");
			}
			break;
		
		// Connection done
		case GAP_DIRECT_CONNECTION_ESTABLISHMENT_PROC:
			{
				Printf("BEX_gap_done: Procedure de connexion termine");
				
				// Plus besoin de se connecter
				BLP_conn = false;
				
				// Aucune proc�dure en cours
				BLP_proc = 0;
			}
			break;
		
		// Discovery in limited mode
		case GAP_LIMITED_DISCOVERY_PROC:
			{
				
			}
			break;
		
		// Proc�dure inconnu
		default:
			{
				Printf("DBG0: BEX_gap_done: Procedure code unknow %02x", code);
			}
			break;
	}
	
	return true;
}

/*----------------------------------------------------------------------------
* BEX_gattdone() : Proc�dure du gatt serveur compl�te
*-----------------------------------------------------------------------------
* Input  : - Longueur de la donn�e
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

	// Fin de proc�dure
	BLP_proc = BLP_PROC_NONE;  
	
	return true;
}

/*----------------------------------------------------------------------------
* BEX_requread() : Requ�te de lecture
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

	// Un client est connect�
	if (clie != 0)
	{
		// Accepter la lecture de la characteristic
		cret = aci_gatt_allow_read(clie);
		if (cret != BLE_STATUS_SUCCESS)
		{
			Printf("BEX_requread: Impossible d'autoriser la lecture de la characteristic %d", hand);
		}
	}
}

/*----------------------------------------------------------------------------
* BEX_requwrit() : Requ�te d'�criture
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
	uint8_t    blok = false;													// Bloquer la mise � jour ou non
	uint8_t	   eror = BLE_STATUS_INVALID_PARAMS;			// Error

	// Fonction avec v�rification de blok
	
	// Inversion du blok car la fonction aci_gatt_write_response() l'impose
	blok = !blok;
	// Un client est connect�
	if (clie != 0)
	{
		// Accepter l'�criture de la characteristic sur GATT server
		cret = aci_gatt_write_response(clie, hand, blok, eror, leng, data);
		if (cret != BLE_STATUS_SUCCESS)
		{
			Printf("DBG0: BEX_requwrit: Impossible d'autoriser l'ecriture de la characteristic %02x", hand);
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