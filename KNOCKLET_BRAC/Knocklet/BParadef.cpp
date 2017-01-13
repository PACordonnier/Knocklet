/**
  ******************************************************************************
  * @file    BParadef.cpp
  * @author  Nicolas LAW
  * @version V1.0.0
  * @date    05-January-2017
  * @brief   Définition des paramètres relatifs au BLE
  ******************************************************************************/
	
#include "BEvendef.h"				// Définition des évènements relatifs au BLE
#include "BLueproc.h"				// Gestionnaire des procédures BLE
#include "BParadef.h"				// Définition des paramètres relatifs au BLE
#include "main.h"					// Include du CORE

#include "stm32_bluenrg_ble.h"		// Initialisation BLE
#include "bluenrg_gatt_aci.h"		// Header file with GATT commands for BlueNRG FW6.3.s
#include "bluenrg_gap.h"			// Header file for BlueNRG's GAP layer. 
#include "bluenrg_gap_aci.h"		// Header file with GAP commands for BlueNRG FW6.3.
#include "mbed.h"					// Librairie Mbed

BPX_PARA para;						// Liste des paramètres BLE

/*----------------------------------------------------------------------------
* BPX_initdata() : Initialisations des services
*-----------------------------------------------------------------------------
* Input  : - Liste des paramètres BLE
* Output : -
* Return : - Success or not
*-----------------------------------------------------------------------------
*
*---------------------------------------------------------------------------*/
bool BPX_initdata(BPX_PARA *para)
{
	// Initialisation des flags des procédures BLE
	if (BLX_initproc() == false)
	{
		Printf("BPX_initdata: Init procedures fail");
		return false;
	}
	
	// Initialisation du service generic
	if (BPX_initgene(para) == false)
	{
		Printf("BPX_initdata: Init service generic fail");
		return false;
	}

	// Initialisation du service device information
	if (BPX_initdevi(para) == false)
	{
		Printf("BPX_initdata: Init service device information fail");
		return false;
	}

	// Initialisation du service custom
	if (BPX_initcust(para) == false)
	{
		Printf("BPX_initdata: Init service custom fail");
		return false;
	}

	return true;
}

/*----------------------------------------------------------------------------
* BPX_initserv() : Initialisation d'un service
*-----------------------------------------------------------------------------
* Input  : - Liste des paramètres BLE
*          - Service
*          - Type de service
*          - Nombre d'attribut
* Output : -
* Return : - Success or not
*-----------------------------------------------------------------------------
*
*---------------------------------------------------------------------------*/
bool BPX_initserv(BPX_SERV_BASE *serv, uint8_t type, uint8_t numb)
{
	uint8_t uuid[BPX_UUID__128];	// UUID
	uint8_t size = serv->size;		// Type d'UUID

	// Sauvegarde de l'uuid avant mise à zero de la mémoire
	if (size == UUID_TYPE_16)
		memcpy(uuid, serv->uuid, BPX_UUID___16);
	else
		memcpy(uuid, serv->uuid, BPX_UUID__128);

	// Initialisation du service
	serv->size = size;
	serv->type = type;
	serv->numb = numb;

	// Recopie de l'uuid sauvegardé
	if (size == UUID_TYPE_16)
		memcpy(serv->uuid, uuid, BPX_UUID___16);
	else
		memcpy(serv->uuid, uuid, BPX_UUID__128);

	return true;
}

/*----------------------------------------------------------------------------
* BPX_initchar() : Initialisation d'une characteristic
*-----------------------------------------------------------------------------
* Input  : - Characteristic
*          - Security permissions
*          - Characteristic Properties
*          - Event
*          - Encryption Key
*          - Is Variable
*          - Taille des données
*          - Valeur par défaut
* Output : -
* Return : - Success or not
*-----------------------------------------------------------------------------
*
*---------------------------------------------------------------------------*/
bool BPX_initchar(BPX_CHAR_BASE *crac, uint8_t secu, uint8_t prop, uint8_t even, uint8_t xkey, uint8_t isva, uint8_t leng, uint8_t *defo)
{
	uint8_t uuid[BPX_UUID__128];	// UUID
	uint8_t size = crac->size;		// Type d'UUID

	// Sauvegarde UUID
	if (size == UUID_TYPE_16)
		memcpy(uuid, crac->uuid, BPX_UUID___16);
	else
		memcpy(uuid, crac->uuid, BPX_UUID__128);

	// Initialisation de la characteristic
	crac->secu = secu;
	crac->prop = prop;
	crac->even = even;
	crac->xkey = xkey;
	crac->isva = isva;
	crac->leng = leng;

	// Initialisation de la valeur de la characteristic
	crac->data.hand = 0;
	crac->data.vale = (uint8_t*)malloc(crac->leng);
	crac->data.updt = false;
	memset(crac->data.vale, 0, crac->leng);
	
	// Set de la valeur par défaut
	if (defo != NULL)
		memcpy(crac->data.vale, defo, crac->leng);

	// Set de l'UUID
	if (size == UUID_TYPE_16)
		memcpy(crac->uuid, uuid, BPX_UUID___16);
	else
		memcpy(crac->uuid, uuid, BPX_UUID__128);
	
	return true;
}

/*----------------------------------------------------------------------------
* BPX_initdesc() : Initialisation d'un descriptor
*-----------------------------------------------------------------------------
* Input  : - Descriptor
*          - Security permissions
*          - Access permissions
*          - Event
*          - Encryption Key
*          - Is Variable
*          - Taille des données
*          - Valeurs
* Output : -
* Return : - Success or not
*-----------------------------------------------------------------------------
*
*---------------------------------------------------------------------------*/
bool BPX_initdesc(BPX_DESC_BASE *desc, uint8_t secu, uint8_t acce, uint8_t even, uint8_t xkey, uint8_t isva, uint8_t leng, uint8_t *vale)
{
	uint8_t uuid[BPX_UUID__128];	// UUID
	uint8_t size = desc->size;		// Type d'UUID

	// Sauvegarde de l'uuid avant mise à zero de la mémoire
	if (size == UUID_TYPE_16)
		memcpy(uuid, desc->uuid, BPX_UUID___16);
	else
		memcpy(uuid, desc->uuid, BPX_UUID__128);

	// Initialisation du descriptor
	desc->secu = secu;
	desc->acce = acce;
	desc->even = even;
	desc->xkey = xkey;
	desc->isva = isva;
	desc->leng = leng;
	
	// Initialisation de la valeur de la characteristic
	desc->hand = 0;
	desc->vale = (uint8_t*)malloc(desc->leng);
	memset(desc->vale, 0, desc->leng);
	
	// Recopie de l'uuid sauvegardé
	if (size == UUID_TYPE_16)
		memcpy(desc->uuid, uuid, BPX_UUID___16);
	else
		memcpy(desc->uuid, uuid, BPX_UUID__128);

	return true;
}

/*----------------------------------------------------------------------------
* BPX_initgene() : Initialisation du service generic
*-----------------------------------------------------------------------------
* Input  : - Liste des paramètres BLE
* Output : -
* Return : - Success or not
*-----------------------------------------------------------------------------
*
*---------------------------------------------------------------------------*/
bool BPX_initgene(BPX_PARA *para)
{
	// Initialisation du service generic
	SERV_GENERIC_SERV(para->BPX_gene.serv.uuid, para->BPX_gene.serv.size);
	BPX_initserv(&para->BPX_gene.serv, PRIMARY_SERVICE, 0);

	// Initialisation de la characteristic de nom
	ATTR_DEVICE_NAME(para->BPX_gene.name.uuid, para->BPX_gene.name.size);
	BPX_initchar(&para->BPX_gene.name,
		ATTR_PERMISSION_NONE,
		CHAR_PROP_READ,
		GATT_DONT_NOTIFY_EVENTS,
		0,
		false,
		BPX_VALE_TEXT,
		(uint8_t*)BPX_GENE_NAME);

	// Initialisation de la characteristic de appararence
	ATTR_APPEARANCE(para->BPX_gene.apar.uuid, para->BPX_gene.apar.size);
	BPX_initchar(&para->BPX_gene.apar,
		ATTR_PERMISSION_NONE,
		CHAR_PROP_READ,
		GATT_DONT_NOTIFY_EVENTS,
		0,
		false,
		BPX_VALE_UI16,
		(uint8_t*)BPX_GENE_APAR);

	return true;
}

/*----------------------------------------------------------------------------
* BPX_initdevi() : Initialisation du service Device Information
*-----------------------------------------------------------------------------
* Input  : - Liste des paramètres BLE
* Output : -
* Return : - Success or not
*-----------------------------------------------------------------------------
*
*---------------------------------------------------------------------------*/
bool BPX_initdevi(BPX_PARA *para)
{
	// Initialisation du service du device
	SERV_DEVICE(para->BPX_devi.serv.uuid, para->BPX_devi.serv.size);
	BPX_initserv(&para->BPX_devi.serv, PRIMARY_SERVICE, 1 + BPX_DEVI_SIZE * 2);

	// Initialisation de la characteristic du numéro de modèle
	ATTR_MODEL_NUMBER(para->BPX_devi.modl.uuid, para->BPX_devi.modl.size);
	BPX_initchar(&para->BPX_devi.modl,
		ATTR_PERMISSION_NONE,
		CHAR_PROP_READ,
		GATT_DONT_NOTIFY_EVENTS,
		BPX_CHAR_XKEY,
		false,
		BPX_VALE_TEXT,
		(uint8_t*)BPX_MODL_NUMB);

	// Initialisation de la characteristic du numéro de série
	ATTR_SERIAL_NUMBER(para->BPX_devi.seri.uuid, para->BPX_devi.seri.size);
	BPX_initchar(&para->BPX_devi.seri,
		ATTR_PERMISSION_NONE,
		CHAR_PROP_READ,
		GATT_DONT_NOTIFY_EVENTS,
		BPX_CHAR_XKEY,
		false,
		BPX_VALE_TEXT,
		(uint8_t*)BPX_SERI_NUMB);

	return true;
}

/*----------------------------------------------------------------------------
* BPX_initcust() : Initialisation du service custom
*-----------------------------------------------------------------------------
* Input  : - Liste des paramètres BLE
* Output : -
* Return : - Success or not
*-----------------------------------------------------------------------------
*
*---------------------------------------------------------------------------*/
bool BPX_initcust(BPX_PARA *para)
{
	// Initialisation de l'uuid du service custom
	SERV_CUST(para->BPX_data.serv.uuid, para->BPX_data.serv.size);
	BPX_initserv(&para->BPX_data.serv, PRIMARY_SERVICE, BPS_DATA_SIZE);
	
	// Initialisation de la characteristic A
	ATTR_CARA(para->BPX_data.carA.uuid, para->BPX_data.carA.size);
	BPX_initchar(&para->BPX_data.carA,
	ATTR_PERMISSION_NONE,
	CHAR_PROP_READ,
	GATT_DONT_NOTIFY_EVENTS,
	BPX_CHAR_XKEY,
	false,
	BPX_VALE_UI08,
	(uint8_t*)BPX_CHAR_DATA);
		
	// Initialisation du descriptor CPFM de la characteristic A
	CPFM_CARC(para->BPX_data.carA.desc[0].uuid, para->BPX_data.carA.desc[0].size);
	BPX_initdesc(&para->BPX_data.carA.desc[0],
	ATTR_PERMISSION_NONE,
	ATTR_ACCESS_READ_ONLY,
	GATT_DONT_NOTIFY_EVENTS,
	BPX_DESC_XKEY,
	false,
	BPX_DESC_CPFM,
	0);

	return true;
}

/*----------------------------------------------------------------------------
* BPX_confdata() : Configuration des données BLE
*-----------------------------------------------------------------------------
* Input  : - Liste des paramètres BLE
* Output : -
* Return : - Success or not
*-----------------------------------------------------------------------------
*
*---------------------------------------------------------------------------*/
bool BPX_confdata(BPX_PARA *para)
{
	// Configuration du service generic
	if (BPX_confgene(para) == false)
	{
		Printf("BPX_confdata: Configure service generic fail");
		return false;
	}

	// Configuration du service device information
	if (BPX_confdevi(para) == false)
	{
		Printf("BPX_confdata: Configure service device information fail");
		return false;
	}

	// Configuration du service custom
	if (BPX_confcust(para) == false)
	{
		Printf("BPX_confdata: Configure service custom fail");
		return false;
	}

	return true;
}

/*----------------------------------------------------------------------------
* BPX_confgene() : Configuration du service generic
*-----------------------------------------------------------------------------
* Input  : - Liste des paramètres BLE
* Output : -
* Return : - Success or not
*-----------------------------------------------------------------------------
*
*---------------------------------------------------------------------------*/
bool BPX_confgene(BPX_PARA *para)
{
	// Set le nom du device
	if (BPX_set_vale(para->BPX_gene.serv.hand, &para->BPX_gene.name) == false)
	{
		Printf("BPX_confgene: Set device name fail");
		return false;
	}

	// Set le nom de l'apparence (3138	Wrist Worn)
	para->BPX_gene.apar.data.vale[1] = 0x0C;
	para->BPX_gene.apar.data.vale[0] = 0x42;
	if (BPX_set_vale(para->BPX_gene.serv.hand, &para->BPX_gene.apar) == false)
	{
		Printf("BPX_confgene: Set apparence fail");
		return false;
	}
	
	return true;
}

/*----------------------------------------------------------------------------
* BPX_confdevi() : Configuration du service device information
*-----------------------------------------------------------------------------
* Input  : - Liste des paramètres BLE
* Output : -
* Return : - Success or not
*-----------------------------------------------------------------------------
*
*---------------------------------------------------------------------------*/
bool BPX_confdevi(BPX_PARA *para)
{
	// Configuration du service device information
	if (BPX_add_serv(&para->BPX_devi.serv) == false)
	{
		Printf("BPX_confdevi: Add service device information fail");
		return false;
	}

	// Configuration de la characteristic du model number
	if (BPX_add_char(para->BPX_devi.serv.hand, &para->BPX_devi.modl) == false)
	{
		Printf("BPX_confdevi: Add characteristic model number fail");
		return false;
	}

	// Set le nom du model number
	if (BPX_set_vale(para->BPX_devi.serv.hand, &para->BPX_devi.modl) == false)
	{
		Printf("BPX_confdevi: Set characteristic model number fail");
		return false;
	}

	// Configuration de la characteristic du numéro de série
	if (BPX_add_char(para->BPX_devi.serv.hand, &para->BPX_devi.seri) == false)
	{
		Printf("BPX_confdevi: Add characteristic serial number fail");
		return false;
	}

	// Set le nom du numéro de série
	if (BPX_set_vale(para->BPX_devi.serv.hand, &para->BPX_devi.seri) == false)
	{
		Printf("BPX_confdevi: Set characteristic serial number fail");
		return false;
	}

	return true;
}

/*----------------------------------------------------------------------------
* BPX_confcust() : Configuration du service Custom
*-----------------------------------------------------------------------------
* Input  : - Liste des paramètres BLE
* Output : -
* Return : - Success or not
*-----------------------------------------------------------------------------
*
*---------------------------------------------------------------------------*/
bool BPX_confcust(BPX_PARA *para)
{	
	// Configuration du service Custom
	if (BPX_add_serv(&para->BPX_data.serv) == false)
	{
		Printf("BPX_confcust: Add service custom fail");
		return false;
	}
	
	// Configuration de la characteristic A
	if (BPX_add_char(para->BPX_data.serv.hand, &para->BPX_data.carA) == false)
	{
		Printf("BPX_confcust: Add characteristic A fail");
		return false;
	}
	
	// Set des descriptor de la characteristic A					  
	// Initialisation du descriptor CPFM de la characteristic A (unitless)
	para->BPX_data.carA.desc[0].vale[0] = 0x04;
	para->BPX_data.carA.desc[0].vale[1] = 0xFF;
	para->BPX_data.carA.desc[0].vale[2] = 0x00;
	para->BPX_data.carA.desc[0].vale[3] = 0x27;
	para->BPX_data.carA.desc[0].vale[4] = 0x00;
	para->BPX_data.carA.desc[0].vale[5] = 0x00;
	para->BPX_data.carA.desc[0].vale[6] = 0x00;
	para->BPX_data.carA.desc[0].vale[7] = 0x00;	
	
	  // Ajout du descriptor CPFM 
	if (BPX_add_desc(para->BPX_data.serv.hand, para->BPX_data.carA.hand, &para->BPX_data.carA.desc[0]) == false) 
	{ 
		Printf("BPX_confcust: Add descriptor CPFM fail");
		return false; 
	}
	
	return true;
}

/*----------------------------------------------------------------------------
* BPX_add_serv() : Ajout d'un service
*-----------------------------------------------------------------------------
* Input  : - Handle du service
* Output : -
* Return : - Success or not
*-----------------------------------------------------------------------------
*
*---------------------------------------------------------------------------*/
bool BPX_add_serv(BPX_SERV_BASE *serv)
{
	tBleStatus cret;	// Code retour

	// Ajout d'un service au serveur GATT
	cret = aci_gatt_add_serv(serv->size, serv->uuid, serv->type, serv->numb, &serv->hand);
	if (cret != BLE_STATUS_SUCCESS)
	{
		return false;
	}

	return true;
}

/*----------------------------------------------------------------------------
* BPX_add_char() : Ajout d'une characteristic au service
*-----------------------------------------------------------------------------
* Input  : - Handle du service
*          - Characteristic
* Output : -
* Return : - Success or not
*-----------------------------------------------------------------------------
*
*---------------------------------------------------------------------------*/
bool BPX_add_char(uint16_t serv, BPX_CHAR_BASE *crac)
{
	tBleStatus cret;	// Code retour

	// Ajout d'une characteristic au serveur GATT
	cret = aci_gatt_add_char(serv,
		crac->size,
		crac->uuid,
		crac->leng,
		crac->prop,
		crac->secu,
		crac->even,
		crac->xkey,
		crac->isva,
		&crac->hand);

	if (cret != BLE_STATUS_SUCCESS)
	{
		return false;
	}
	else
	{
		// Récupération de l'Attribut Handler Characteristic 
		crac->data.hand = crac->hand + 1;
	}
	
	return true;
}

/*----------------------------------------------------------------------------
* BPX_add_desc() : Ajout d'un descriptor à la characteristic du service
*-----------------------------------------------------------------------------
* Input  : - Handle du service
*          - Handle de la characteristic
*					 - Descriptor
* Output : -
* Return : - Success or not
*-----------------------------------------------------------------------------
*
*---------------------------------------------------------------------------*/
bool BPX_add_desc(uint16_t serv, uint16_t crac, BPX_DESC_BASE *desc)
{
	tBleStatus cret;	// Code retour

	// Ajout d'un descriptor à la characteristic au serveur GATT
	cret = aci_gatt_add_char_desc(serv,
		crac,
		desc->size,
		desc->uuid,
		7,
		desc->leng,
		desc->vale,
		desc->secu,
		desc->acce,
		desc->even,
		desc->xkey,
		desc->isva,
		&desc->hand);

	if (cret != BLE_STATUS_SUCCESS)
	{
		return false;
	}

	return true;
}

/*----------------------------------------------------------------------------
* BPX_set_vale() : Set la valeur d'une characteristic
*-----------------------------------------------------------------------------
* Input  : - Service handler
*          - Characteristic
* Output : -
* Return : - Success or not
*-----------------------------------------------------------------------------
*
*---------------------------------------------------------------------------*/
bool BPX_set_vale(uint16_t serv, BPX_CHAR_BASE *crac)
{
	tBleStatus cret;	// Code retour

	// Mise à jour de la valeur de la characteristic
	cret = aci_gatt_update_char_value(serv, crac->hand, 0, crac->leng, crac->data.vale);
	if (cret != BLE_STATUS_SUCCESS)
	{
		return false;
	}

	// Mise à jour effectuée
	crac->data.updt = false;

	return true;
}

/*----------------------------------------------------------------------------
* BPX_updtdata() : Mise à jour des données si besoin
*-----------------------------------------------------------------------------
* Input  : - Liste des paramètres BLE
* Output : -
* Return : - Success or not
*-----------------------------------------------------------------------------
* 
*---------------------------------------------------------------------------*/
bool BPX_updtdata(BPX_PARA *para)
{
	tBleStatus cret;	// Code retour
  
	// Si demande de mise à jour sur la characteristic A
	if (para->BPX_data.carA.data.updt == true)
	{
		// Valeur de la characteristic A = nombre de tap
		cret = BPX_set_vale(para->BPX_data.serv.hand, &para->BPX_data.carA);
		// IF SET_VALE OK, cret == 1 (wtf ??)
		if (cret == false)
		{
			return false;
		}
		else
		// Update réussi, on active le mode broadcast
		{
			BLP_broa = true;
		}
	}
	
	return true;
}
