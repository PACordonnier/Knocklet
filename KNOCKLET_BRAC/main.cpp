/**
  ******************************************************************************
  * @file    main.c
  * @author  Nicolas LAW
  * @version V1.0.0
  * @date    05-January-2017
  * @brief   main du programme
  ******************************************************************************/

#include "main.h"	// Include du CORE
#include "mbed.h"	// Librairie mbed

/*----------------------------------------------------------------------------
* ALLOCATION DE VARIABLES                                                     
*---------------------------------------------------------------------------*/
Serial pc(PA_2, PA_3);	// Port S�rie pour Debug

int main(void)
{
	bool cret = true; // Code retour

#if BRAC_ACTI_XBLE
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
#endif // BRAC_ACTI_XBLE
		
	/* Configure the system clock */
	SystemClock_Config();

	// Reconfiguration du baud rate apr�s SystemClock_Config()
	pc.baud(9600);
	
	// NICOLAW TEST
	//NVIC_SetPriority(EXTI4_15_IRQn, 0);
	//NVIC_SetPriority(EXTI2_3_IRQn, 1);
	//NVIC_SetPriority(EXTI0_1_IRQn, 2);
	
	// Initialisation du syst�me
	if (coreinit() == false)
	{
		Printf("main: Initialisation Core fail");
		cret = false;
	}
	
	// Configuration du syst�me
	if (coreconf() == false)
	{ 
		Printf("main: Configuration Core fail");
		cret = false;
	}
	
	if (cret != false) Printf("main: Init et Config OK");

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
* coreinit() : Initialisation du syst�me
*-----------------------------------------------------------------------------
* Input  : -
* Output : -
* Return : -
*-----------------------------------------------------------------------------
*---------------------------------------------------------------------------*/
bool coreinit(void)
{	
#if BRAC_ACTI_ACCE
	// Initialisation des param�tres de l'acc�lerom�tre
	if (ACX_acceinit() == false)
	{
		Printf("coreinit: Initialisation accelerometre fail");
		return false;
	}		  
#endif // BRAC_ACTI_ACCE

#if BRAC_ACTI_XBLE
	// Initialisation du gestionnaire BLE
	if (BIX_init_ble() == false)
	{
		Printf("coreinit: Initialisation BLE fail");
		return false;
	}			  
#endif // BRAC_ACTI_XBLE
	
	return true;
}

/*----------------------------------------------------------------------------
* coreconf() : Configuration du syst�me
*-----------------------------------------------------------------------------
* Input  : -
* Output : -
* Return : - Succes or not
*-----------------------------------------------------------------------------
*---------------------------------------------------------------------------*/
bool coreconf(void)
{	
#if BRAC_ACTI_ACCE
	// Configuration de l'acc�lerom�tre
	if (ACX_acceconf() == false)
	{
		Printf("coreconf: Configuration accelerometre fail");
		return false;
	}
#endif // BRAC_ACTI_ACCE

#if BRAC_ACTI_XBLE
	// Configuration du BLE
	if (BIX_configur() == false)
	{
		Printf("coreconf: Configuration BLE fail");
		return false;
	}		  
#endif // BRAC_ACTI_XBLE

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
#if BRAC_ACTI_ACCE
	// Process de l'acc�lerom�tre
	if (ACX__process() == false)
	{
		Printf("coreloop: Process accelerometre fail");
		return false;
	}
#endif // BRAC_ACTI_ACCE

#if BRAC_ACTI_XBLE
	// Process du Bluetooth
	if (BLX__process() == false)
	{
		Printf("coreloop: Process BLE fail");
		return false;
	}		  
#endif // BRAC_ACTI_XBLE
    
	return true;
}

/*----------------------------------------------------------------------------
* Printf() : Printf avec delay pour le Serial et retour ligne
*-----------------------------------------------------------------------------
* Input  : -
* Output : -
* Return : - Succes or not
*-----------------------------------------------------------------------------
*---------------------------------------------------------------------------*/
void Printf(const char *format, ...)
{
#if BRAC_ACTI_LOGS
	char	dest[256];
	va_list argptr;
	if (pc.writeable())
	{
		va_start(argptr, format);
		vsprintf(dest, format, argptr);
		va_end(argptr);
		pc.printf(dest);
		pc.printf("\r\n");
		wait_ms(5);
	}
#endif // BRAC_ACTI_LOGS
}
