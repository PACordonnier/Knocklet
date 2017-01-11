/******************************************************************************
* @file	   ACcelero.h
* @author  Nicolas LAW
* @version V1.0.0
* @date    11-January-2017
* @brief   Gestionnaire de l'accéleromètre
*******************************************************************************/
#ifndef _ACCELERO_H_
#define _ACCELERO_H_

#ifdef __cplusplus
extern "C" {
#endif

//****************************************************************************
// LISTE DES PIECES :
//****************************************************************************
// ACX : Traitements généraux
//****************************************************************************

/*----------------------------------------------------------------------------
* VARIABLES EXTERNALISEES
*---------------------------------------------------------------------------*/
extern bool	ACX_stop;		// Désactivation de l'interrupt pendant l'échange Bluetooth
	
/*----------------------------------------------------------------------------
* CONSTANTES
*---------------------------------------------------------------------------*/
#define ACX_ACCE_GOOD 0
#define ACX_ACCE_TOUT 800

/*----------------------------------------------------------------------------
* ACX : Traitements généraux
*---------------------------------------------------------------------------*/
void ACX_intefonc(void);	// Fonction lorsque une interruption est détecté
bool ACX_acceinit(void);	// Initialisation des paramètres de l'accéleromètre
bool ACX_acceconf(void);	// Configuration de l'accéleromètre
bool ACX__process(void);	// Process de l'accéleromètre
#ifdef __cplusplus
}
#endif

#endif /* _ACCELERO_H_ */