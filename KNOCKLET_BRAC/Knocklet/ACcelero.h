/******************************************************************************
* @file	   ACcelero.h
* @author  Nicolas LAW
* @version V1.0.0
* @date    11-January-2017
* @brief   Gestionnaire de l'acc�lerom�tre
*******************************************************************************/
#ifndef _ACCELERO_H_
#define _ACCELERO_H_

#ifdef __cplusplus
extern "C" {
#endif

//****************************************************************************
// LISTE DES PIECES :
//****************************************************************************
// ACX : Traitements g�n�raux
//****************************************************************************

/*----------------------------------------------------------------------------
* VARIABLES EXTERNALISEES
*---------------------------------------------------------------------------*/
extern bool	ACX_stop;		// D�sactivation de l'interrupt pendant l'�change Bluetooth
	
/*----------------------------------------------------------------------------
* CONSTANTES
*---------------------------------------------------------------------------*/
#define ACX_ACCE_GOOD 0
#define ACX_ACCE_TOUT 800

/*----------------------------------------------------------------------------
* ACX : Traitements g�n�raux
*---------------------------------------------------------------------------*/
void ACX_intefonc(void);	// Fonction lorsque une interruption est d�tect�
bool ACX_acceinit(void);	// Initialisation des param�tres de l'acc�lerom�tre
bool ACX_acceconf(void);	// Configuration de l'acc�lerom�tre
bool ACX__process(void);	// Process de l'acc�lerom�tre
#ifdef __cplusplus
}
#endif

#endif /* _ACCELERO_H_ */