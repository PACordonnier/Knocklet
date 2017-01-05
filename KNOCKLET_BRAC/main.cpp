/**
  ******************************************************************************
  * @file    main.c
  * @author  Law & Hurabielle
  * @version V1.0.0
  * @date    29-September-2016
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
	
	/* Configure the User Button in GPIO Mode */
	BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);
	
	  /* Configure LED2 */
	//BSP_LED_Init(LED2);
	
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
	// Processing du sequenceur
	if (BLX__process() == false)
	{
		return false;
	}
    
	return true;
}
