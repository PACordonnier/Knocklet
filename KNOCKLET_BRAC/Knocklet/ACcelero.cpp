/******************************************************************************
* @file	   ACcelero.cpp
* @author  Nicolas LAW
* @version V1.0.0
* @date    11-January-2017
* @brief   Gestionnaire de l'acc�lerom�tre
*******************************************************************************/

#include "MMA8452.h"	// Librairie de l'acc�lerom�tre
#include "ACcelero.h"	// Gestionnaire de l'acc�lerom�tre
#include "BParadef.h"	// D�finition des param�tres relatifs au BLE
#include "main.h"		// Include du CORE

/*----------------------------------------------------------------------------
* ALLOCATION DE VARIABLES
*---------------------------------------------------------------------------*/
MMA8452			ACX_acce(PB_9, PB_8, 100000);	// Connection de l'acc�lerom�tre (I2C: sda, scl, frequency)
InterruptIn		ACX_taps(PB_2);					// Interrupt d�tectant un tap
Timer			ACX_time;						// Timer lorsqu'un tap est d�tect�
uint8_t			ACX_ntap;						// Nombres de tap
bool			ACX_stop;						// D�sactivation de l'interrupt pendant l'�change Bluetooth
/*----------------------------------------------------------------------------
* ACX_intefonc() : Fonction lorsque une interruption est d�tect�
*-----------------------------------------------------------------------------
* Input  : - 
* Output : -
* Return : - Success or not
*-----------------------------------------------------------------------------
*
*---------------------------------------------------------------------------*/
void ACX_intefonc(void) 
{ 
	// Si le timer est inactif
	if (ACX_time.read_ms() == 0) 
	{ 
		// D�marrage du timer
		ACX_time.start(); 
	}
	// Timer d�j� en marche, reset du temps timer
	else 
	{ 
		ACX_time.reset(); 
	} 
	
	// Incr�mentation du nombre de tap
	ACX_ntap++; 
}  

/*----------------------------------------------------------------------------
* ACX_acceinit() : Initialisation des param�tres de l'acc�lerom�tre
*-----------------------------------------------------------------------------
* Input  : - 
* Output : -
* Return : - Success or not
*-----------------------------------------------------------------------------
*
*---------------------------------------------------------------------------*/
bool ACX_acceinit(void)
{
	// Initialisation de la variable d'arr�t de l'interrupt
	ACX_stop = false;
	
	// Initialisation de l'interrupt sur le Core
	ACX_taps.rise(&ACX_intefonc);
	
	return true;
}

/*----------------------------------------------------------------------------
* ACX_acceconf() : Configuration de l'acc�lerom�tre
*-----------------------------------------------------------------------------
* Input  : - 
* Output : -
* Return : - Success or not
*-----------------------------------------------------------------------------
*
*---------------------------------------------------------------------------*/
bool ACX_acceconf(void)
{
	// Standby mode TEST
	// pc.printf("Value registre standby: %d\n",acc.standby());
	
	// Standby Mode
	if (ACX_acce.standby() != ACX_ACCE_GOOD)
	{
		return false;
	}

	// Enable LPF
	//acc.writeRegister(0x0F, 0x10);
	//
	// ODR : 400 Hz, Standby Mode
	if (ACX_acce.writeRegister(MMA8452_CTRL_REG_1, 0x08) != ACX_ACCE_GOOD)
	{
		return false;
	}
	// Pulse Configuration : Single tap
	if (ACX_acce.writeRegister(MMA8452_PULSE_CFG, 0x15) != ACX_ACCE_GOOD)
	{
		return false;
	}
	// Pulse threshold for X,Y,Z
	// Threshold X: 2g
	if (ACX_acce.writeRegister(MMA8452_PULSE_THSX, 0x15) != ACX_ACCE_GOOD)
	{
		return false;
	}
	// Threshold Y: 2g
	if (ACX_acce.writeRegister(MMA8452_PULSE_THSY, 0x15) != ACX_ACCE_GOOD)
	{
		return false;
	}
	// Threshold Z: 2g
	if (ACX_acce.writeRegister(MMA8452_PULSE_THSZ, 0x15) != ACX_ACCE_GOOD)
	{
		return false;
	}
	// Pulse Time Window 1: 20 ms, Normal Mode, No LPF
	if (ACX_acce.writeRegister(MMA8452_PULSE_TMLT, 0x20) != ACX_ACCE_GOOD)
	{
		return false;
	}
	// Pulse Latency Timer: 250 ms 
	if (ACX_acce.writeRegister(MMA8452_PULSE_LTCY, 0x64) != ACX_ACCE_GOOD)
	{
		return false;
	}
	// Interrupt Routing
	// Enable Interrupt Register
	if (ACX_acce.writeRegister(MMA8452_CTRL_REG_4, 0x08) != ACX_ACCE_GOOD)
	{
		return false;
	}
	// Route INT1 to System Interrupt
	if (ACX_acce.writeRegister(MMA8452_CTRL_REG_5, 0x08) != ACX_ACCE_GOOD)
	{
		return false;
	}
	// Active Mode
	if (ACX_acce.writeRegister(MMA8452_CTRL_REG_1, 0x01) != ACX_ACCE_GOOD)
	{
		return false;
	}
	
	// Configuration OK
	return true;
}

/*----------------------------------------------------------------------------
* ACX__process() : Process de l'acc�lerom�tre
*-----------------------------------------------------------------------------
* Input  : - 
* Output : -
* Return : - Success or not
*-----------------------------------------------------------------------------
*
*---------------------------------------------------------------------------*/
bool ACX__process(void)
{
	// V�rification de r�activation de l'interrupt
	if (ACX_stop == true)
	{
		// R�activation de l'interrupt
		ACX_taps.enable_irq();
		
		// Remise � z�ro du flag
		ACX_stop = false;
	}
	
	// Dur�e sans tap d�passe le d�lai maximum
	if (ACX_time.read_ms() > ACX_ACCE_TOUT) 
	{    
		// Arr�t et reset du timer
		ACX_time.stop();
		ACX_time.reset();
		
		Printf("ACX__process: Nombre de taps: %d", ACX_ntap);
		
		// Blindage de la valeur
		if (ACX_ntap > ACX_ACCE_NMAX)
		{
			// Trop de tap d�tect�
			Printf("ACX__process: Taps ignore");
		}
		else
		{
			// Tap valide
			Printf("ACX__process: Taps OK");
			// D�sactivation de l'interrupt
			ACX_taps.disable_irq();
#if BRAC_ACTI_XBLE
			// R�cup�ration du nombre de tap pour les donn�es Bluetooth
			para.BPX_data.carA.data.vale[0] = ACX_ntap;
			// Activation du flag pour mise � jour Bluetooth
			para.BPX_data.carA.data.updt = true;
#else 
			// Activation du flag pour r�activer l'interrupt
			ACX_stop = true;
#endif // BRAC_ACTI_XBLE	
		}

		// Remise � z�ro du nombre de tap
		ACX_ntap = 0;     
	}
	
	return true;
} 