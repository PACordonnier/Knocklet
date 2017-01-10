/**
  ******************************************************************************
  * @file    BParadef.h
  * @author  Nicolas LAW
  * @version V1.0.0
  * @date    05-January-2017
  * @brief   Définition des paramètres relatifs au BLE
  ******************************************************************************/
#ifndef _BPARADEF_H_
#define _BPARADEF_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h" // Définition des types

//****************************************************************************
// LISTE DES PIECES :
//****************************************************************************
// BPX : Traitements généraux
// BPS : Traitements sur les services
//****************************************************************************

#define BPX_UUID__128 16 // Taille en octets de l'uuid 128 bits
// Copy d'un UUID 128 bits
#define BPX_COPY_UUID__128(uuid, size, uuid_15, uuid_14, uuid_13, uuid_12, uuid_11, uuid_10, uuid_9, uuid_8, uuid_7, uuid_6, uuid_5, uuid_4, uuid_3, uuid_2, uuid_1, uuid_0) \
     size = UUID_TYPE_128;\
do {\
	uuid[0]  = uuid_0;  uuid[1]  = uuid_1;  uuid[2]  = uuid_2;  uuid[3]  = uuid_3;  \
	uuid[4]  = uuid_4;  uuid[5]  = uuid_5;  uuid[6]  = uuid_6;  uuid[7]  = uuid_7;  \
	uuid[8]  = uuid_8;  uuid[9]  = uuid_9;  uuid[10] = uuid_10; uuid[11] = uuid_11; \
	uuid[12] = uuid_12; uuid[13] = uuid_13; uuid[14] = uuid_14; uuid[15] = uuid_15; \
}while(0)

#define BPX_UUID___16 2 // Taille en octets de l'uuid 16 bits
// Copy d'un UUID 16 bits
#define BPX_COPY_UUID___16(uuid, size, uuid_1, uuid_0) \
    size = UUID_TYPE_16;\
do {\
	uuid[0]  = uuid_0;  \
	uuid[1]  = uuid_1;  \
}while(0)

/*----------------------------------------------------------------------------
* STRUCTURE DE BASE
*---------------------------------------------------------------------------*/
typedef struct // Définition d'une characteristic value de base
{
#define BPX_VALE_UI08 1	// Unsigned octet
#define BPX_VALE_UI16 2	// Unsigned 2 octet
#define BPX_VALE_TEXT 8	// Taille de la valeur texte

	uint16_t  hand;				// Attribut Handler Characteristic
	uint8_t  *vale;				// Value
	bool      updt;				// Value update
}BPX_VALE_BASE;

// Définition d'un characteristic descriptor de base
// https://www.bluetooth.com/specifications/gatt/descriptors
typedef struct
{
#define BPX_DESC_SIZE 2	// Taille en octet de la description handle
#define BPX_DESC_DATA 2	// Taille en octet de la value du descriptor
#define BPX_DESC_XKEY 16 // Encryption Key
	
	uint16_t	hand;												// Characteristic Descriptor Handler
	uint8_t		size;												// Size of UUID
	uint8_t		uuid[BPX_UUID__128];								// UUID
	uint8_t		leng;												// Value Length
	uint8_t		*vale;												// Value
	uint8_t		secu;												// Security Permissions
	uint8_t		acce;												// Access Permissions
	uint8_t		even;												// GATT Event Mask
	uint8_t		xkey;												// Encryption Key Size
	bool		isva;												// IsVariable
}BPX_DESC_BASE;

// http://www.st.com/content/ccc/resource/technical/document/user_manual/6d/a1/5b/6c/dc/ab/48/76/DM00162667.pdf/files/DM00162667.pdf/jcr:content/translations/en.DM00162667.pdf
typedef struct // Définition d'un service de base
{
	uint16_t	hand;									// Attribut Handler Service
	uint8_t		size;									// Size of UUID (16 bit ou 128 bit)
	uint8_t		uuid[BPX_UUID__128];	// UUID
	uint8_t		type;									// Type de service (0x01 : Primary, 0x02 : Secondary)
	uint8_t		numb;									// Nombre de characteristic dans le service
}BPX_SERV_BASE;

typedef struct // Définition d'une characteristic de base
{
#define BPX_CHAR_XKEY 16 // Encryption Key

	uint16_t			hand;									// Attribut Handler Characteristic
	uint8_t				size;									// Size of UUID
	uint8_t				uuid[BPX_UUID__128];					// UUID
	uint8_t				leng;									// Value Length
	uint8_t				secu;									// Security Permissions
	uint8_t				prop;									// Characteristic Properties
	uint8_t				even;									// GATT Event Mask
	uint8_t				xkey;									// Encryption Key Size
	bool				isva;									// IsVariable
	BPX_VALE_BASE		data;									// Characteristic value
	BPX_DESC_BASE		desc[2];								// Characteristic descriptor (2 descriptor max)
}BPX_CHAR_BASE;

/*----------------------------------------------------------------------------
* STRUCTURE DE DEFINITION
*---------------------------------------------------------------------------*/
// https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.service.generic_access.xml
typedef struct						// Service generic access
{
#define SERV_GENERIC_SERV(uuid, size) BPX_COPY_UUID___16(uuid, size, 0x18, 0x00)
#define ATTR_DEVICE_NAME(uuid, size) BPX_COPY_UUID___16(uuid, size, 0x2A, 0x00)
#define ATTR_APPEARANCE(uuid, size) BPX_COPY_UUID___16(uuid, size, 0x2A, 0x01)

#define BPX_GENE_NAME "KNOCKLET"
// https://devzone.nordicsemi.com/documentation/nrf51/4.4.0/html/group___b_l_e___a_p_p_e_a_r_a_n_c_e_s.html
//#define BPX_GENE_APAR 576	// Appearance KEYRING
#define BPX_GENE_APAR 3138 // Apparence Wrist Worn
#define BPX_GENE_SIZE -1	// Nombre d'attribut du service generic

	BPX_SERV_BASE serv;			// Service generic
	BPX_CHAR_BASE name;			// Nom du device
	BPX_CHAR_BASE apar;			// Appearance value
}BPS_GENE;

// https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.service.device_information.xml
typedef struct						// Service de device information
{
#define SERV_DEVICE(uuid, size)			BPX_COPY_UUID___16(uuid, size, 0x18, 0x0A)
#define ATTR_MODEL_NUMBER(uuid, size) 	BPX_COPY_UUID___16(uuid, size, 0x2A, 0x24)
#define ATTR_SERIAL_NUMBER(uuid, size) 	BPX_COPY_UUID___16(uuid, size, 0x2A, 0x25)

#define BPX_DEVI_SIZE 2		// Taille du service de device information

#define BPX_MODL_NUMB "06101994"
#define BPX_MODL_SIZE 2		// Taille du model number
#define BPX_SERI_NUMB "12345678"
#define BPX_SERI_SIZE 2		// Taille du serial number

	BPX_SERV_BASE serv;			// Service device informations
	BPX_CHAR_BASE modl;			// Handler de l'attribut model number
	BPX_CHAR_BASE seri;			// Handler de l'attribut serial number

}BPS_DEVI;

typedef struct						// Service custom
{
// HEXA TO ASCII = MAGIE 62 6f 69 72 65 64 65 6c 61 6c 63 6f 6f 6c 21
#define SERV_CUST(uuid, size) BPX_COPY_UUID__128			(uuid, size, 0x62,0x6f,0x69,0x72, 0x65,0x64, 0x65,0x6c, 0x61,0x6c, 0x63,0x6f,0x6f,0x6c,0x21,0x00)
#define ATTR_CARA(uuid, size) BPX_COPY_UUID__128			(uuid, size, 0x62,0x6f,0x69,0x72, 0x65,0x64, 0x65,0x6c, 0x61,0x6c, 0x63,0x6f,0x6f,0x6c,0x21,0x01)
#define CPFM_CARC(uuid, size) BPX_COPY_UUID___16			(uuid, size, 0x29,0x04); // Characteristic Presentation Format descriptor
#define CCCD_CARC(uuid, size) BPX_COPY_UUID___16			(uuid, size, 0x29,0x02); // Client Characteristic Configuration descriptor
#define BPS_DATA_SIZE	8	// Taille du service custom

	
#define BPX_DESC_CCCD 2 // Taille des données de la CCCD (0x0100 pour notify only)
#define	BPX_DESC_CPFM 7 // Taille des données de la CPFM (0x04FF0027000000 pour unitless)
#define BPX_CHAR_DATA 2 // Taille des données des characteristic
	// Taille du service : (nb Charac *2) + (nb Desc *2) + 1
	
	BPX_SERV_BASE serv;			// Service Data Custom
	BPX_CHAR_BASE carA;			// Characteristic en Read/Notify (nombre de tap)
}BPS_DATA;

typedef struct					// Paramètres du serveur GATT
{
	//	Variables
	BPS_GENE BPX_gene;			// Service generic access
	BPS_DEVI BPX_devi;			// Service du device informations
	BPS_DATA BPX_data;			// Service data custom
}BPX_PARA;

/*----------------------------------------------------------------------------
* VARIABLES EXTERNALISEES
*---------------------------------------------------------------------------*/
extern BPX_PARA para;			// Structure qui contient les paramètres du serveur GATT

// INITIALISATION
bool BPX_initdata(BPX_PARA *para);
bool BPX_initserv(BPX_SERV_BASE *serv, uint8_t type, uint8_t numb);																																													// Allocation d'un service
bool BPX_initchar(BPX_CHAR_BASE *crac, uint8_t secu, uint8_t prop, uint8_t even, uint8_t xkey, uint8_t isva, uint8_t leng, uint8_t *defo);									// Allocation d'une characteristic
bool BPX_initdesc(BPX_DESC_BASE *desc, uint8_t secu, uint8_t acce, uint8_t even, uint8_t xkey, uint8_t isva, uint8_t leng, uint8_t *vale);									// Initialisation d'un descriptor											
bool BPX_initgene(BPX_PARA *para);																																																													// Initialisation du service generic
bool BPX_initdevi(BPX_PARA *para);																																																													// Initialisation du service Device Information
bool BPX_initcust(BPX_PARA *para);																																																													// Initialisation du service Custom
// CONFIGURATION
bool BPX_confdata(BPX_PARA *para);																																																													// Configuration des services
bool BPX_confgene(BPX_PARA *para);																																																													// Configuration du service generic
bool BPX_confdevi(BPX_PARA *para);																																																													// Configuration du service device informations                                                                                                                   // Configuration du service Debug
bool BPX_confcust(BPX_PARA *para);																																																													// Configuration du service Custom
bool BPX_add_serv(BPX_SERV_BASE *crac);																																																											// Ajout d'un service
bool BPX_add_char(uint16_t serv, BPX_CHAR_BASE *crac);																																																			// Ajout d'une characteristic au service
bool BPX_add_desc(uint16_t serv, uint16_t crac, BPX_DESC_BASE *desc);																																												// Ajout d'un descriptor à la characteristic du service
// UPDATE
bool BPX_set_vale(uint16_t serv, BPX_CHAR_BASE *crac);																																																			// Set la valeur d'une characteristic																																																	// Set la valeur d'un descriptor
bool BPX_updtdata(BPX_PARA *para);																																																													// Mise à jour des données si besoin
#ifdef __cplusplus
}
#endif

#endif /* _BPARADEF_H_ */
