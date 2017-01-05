#include <mbed.h>

#include "MMA8452.h"

//extern "C" {
#include "stm32l0xx_hal.h"			// Contains all the functions prototypes for the HAL module driver.
#include "stm32_bluenrg_ble.h"		// Initialisation BLE	
#include "bluenrg_gatt_server.h"	// Header file for BlueNRG's GATT server layer.
#include "bluenrg_gatt_aci.h"		// Header file with GATT commands for BlueNRG FW6.3.
#include "bluenrg_hal_aci.h"		// Header file with HCI commands for BlueNRG FW6.3.
#include "bluenrg_aci_const.h"		// Header file with ACI definitions for BlueNRG FW6.3.
#include "bluenrg_utils.h"			// Header file for BlueNRG utility functions 
#include "bluenrg_gap.h"			// Header file for BlueNRG's GAP layer. 
#include "bluenrg_gap_aci.h"		// Header file with GAP commands for BlueNRG FW6.3.
#include "stm32xx_it.h"				// Interface pour le STM32

#include "hci.h"					// Librairie Host Controller Interface
#include "sm.h"						// Macros Security Manager
#include "hal.h"					// Librairie Hardware Abstraction Layer
//}

//Using MMA8452 accelerometer. SDA on 28, SCL on 27. Writes gravities to the screen. 
//Also brightens/dims LEDs 1-3 based on whether or not they are 'level'( 0 Gs)
 
// BUT DU PROGRAMME : Si un tap est detecte; allume la LED

//Serial pc(USBTX, USBRX);
//
////InterruptIn tap(USER_BUTTON);
//// NICOLAW
//InterruptIn tap(PA_0);
//DigitalOut  led1(PB_3);
//DigitalOut  here(PA_1);
//
//double x, y, z;
//char check;
// 
//MMA8452 acc(PB_9, PB_8, 100000);
// 
//// Si une interrupt est trigger, LED s'allume pendant 1 seconde
//void interrupt()
//{
//	led1 = !led1;
//}

int main() {
   
	// CONFIG NUCLEO
	
	// Configuration de l'interrupt
	//tap.rise(&interrupt);
	//
	//// CONFIG ACCELEROMETRE
	//
	//// Standby mode TEST
	//// pc.printf("Value registre standby: %d\n",acc.standby());
	//
	//// Standby Mode
	//acc.standby();
	////
	//// Enable LPF
	////acc.writeRegister(0x0F, 0x10);
	////
	//// ODR : 400 Hz, Standby Mode
	//acc.writeRegister(MMA8452_CTRL_REG_1,0x08);
	//// Pulse Configuration : Single tap
	//acc.writeRegister(MMA8452_PULSE_CFG, 0x15);
	//// Pulse threshold for X,Y,Z
	//// Threshold X: 2g
	//acc.writeRegister(MMA8452_PULSE_THSX, 0x20);
	//// Threshold Y: 2g
	//acc.writeRegister(MMA8452_PULSE_THSY, 0x20);
	//// Threshold Z: 2g
	//acc.writeRegister(MMA8452_PULSE_THSZ, 0x20);
	//// Pulse Time Window 1: 50 ms, Normal Mode, No LPF
	//acc.writeRegister(MMA8452_PULSE_TMLT, 0x50);
	//// Pulse Latency Timer: 300 ms 
	//acc.writeRegister(MMA8452_PULSE_LTCY, 0xF0);
	//// Interrupt Routing
	//// Enable Interrupt Register
	//acc.writeRegister(MMA8452_CTRL_REG_4, 0x08);
	//// Route INT1 to System Interrupt
	//acc.writeRegister(MMA8452_CTRL_REG_5, 0x08);
	//// Active Mode
	//acc.writeRegister(MMA8452_CTRL_REG_1, 0x01);
	
	/* STM32Cube HAL library initialization:
   *  - Configure the Flash prefetch, Flash preread and Buffer caches
   *  - Systick timer is configured by default as source of time base, but user 
   *    can eventually implement his proper time base source (a general purpose 
   *    timer for example or other time source), keeping in mind that Time base 
   *    duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
   *    handled in milliseconds basis.
   *  - Low Level Initialization
   */
	// Utile pour le BLE
	HAL_Init();
	
	/* Configure the system clock */
	SystemClock_Config();
	
	/* Initialize the BlueNRG SPI driver */
	BNRG_SPI_Init();
	
	/* Initialize the BlueNRG HCI */
	HCI_Init();
	
	// Reset BlueNRG hardware
	BlueNRG_RST();
	
	tBleStatus cret; // Code retour
	uint8_t	   hard; // Version du hardware
	uint16_t   firm; // Version du firmware
		
	// get the BlueNRG HW and FW versions
	cret = getBlueNRGVersion(&hard, &firm);
	if (cret != BLE_STATUS_SUCCESS) 
	{
		return false;
	}
	//while (1) {
	//
	//	here = !here;
	//	wait(0.5);
	//
	//}
}	//