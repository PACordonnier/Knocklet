/**
  ******************************************************************************
  * @file    main.c
  * @author  Nicolas LAW
  * @version V1.0.0
  * @date    05-January-2017
  * @brief   main du programme
  ******************************************************************************/

#include "main.h"

int main(void)
{
	bool cret = true; // Code retour
	
	/* STM32Cube HAL library initialization:
   *  - Configure the Flash prefetch, Flash preread and Buffer caches
   *  - Systick timer is configured by default as source of time base, but user
   *    can eventually implement his proper time base source (a general purpose 
   *    timer for example or other time source), keeping in mind that Time base 
   *    duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
   *    handled in milliseconds basis.
   *  - Low Level Initialization
   */
	HAL_Init();
		
	/* Configure the system clock */
	SystemClock_Config();

	// Initialisation du système
	if (coreinit() == false)
	{ 
		cret = false;
	}
	
	// Configuration du système
	if (coreconf() == false)
	{ 
		cret = false;
	}
	
	// Boucle de processing
	while (cret)
	{
		cret = coreloop();
		
		if (cret == false)
		{
			
		}
	}

	return 0;
}

/*----------------------------------------------------------------------------
* coreinit() : Initialisation du système
*-----------------------------------------------------------------------------
* Input  : -
* Output : -
* Return : -
*-----------------------------------------------------------------------------
*---------------------------------------------------------------------------*/
bool coreinit(void)
{		
	// Initialisation des paramètres de l'accéleromètre
	if (ACX_acceinit() == false)
	{
		return false;
	}
	
	// Initialisation du gestionnaire ble
	if (BIX_init_ble() == false)
	{
		return false;
	}
   
	return true;
}

/*----------------------------------------------------------------------------
* coreconf() : Configuration du système
*-----------------------------------------------------------------------------
* Input  : -
* Output : -
* Return : - Succes or not
*-----------------------------------------------------------------------------
*---------------------------------------------------------------------------*/
bool coreconf(void)
{	
	// Configuration de l'accéleromètre
	if (ACX_acceconf() == false)
	{
		return false;
	}
	
	// Configuration du BLE
	if (BIX_configur() == false)
	{
		return false;
	}
	
	return true;
}

/*----------------------------------------------------------------------------
* coreloop() : Loop du CORE
*-----------------------------------------------------------------------------
* Input  : -
* Output : -
* Return : -
*-----------------------------------------------------------------------------
*---------------------------------------------------------------------------*/
bool coreloop(void)
{
	// Process de l'accéleromètre
	if (ACX__process() == false)
	{
		return false;
	}
	
	// Process du Bluetooth
	if (BLX__process() == false)
	{
		return false;
	}
    
	return true;
}
