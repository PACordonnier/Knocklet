#include <mbed.h>

#include "MMA8452.h"
//Using MMA8452 accelerometer. SDA on 28, SCL on 27. Writes gravities to the screen. 
//Also brightens/dims LEDs 1-3 based on whether or not they are 'level'( 0 Gs)
 
// BUT DU PROGRAMME : Si un tap est detecte; allume la LED

Serial pc(USBTX, USBRX);

//InterruptIn tap(USER_BUTTON);
// NICOLAW
InterruptIn tap(PA_0);
DigitalOut  led1(PB_3);
DigitalOut  here(PA_1);

double x, y, z;
char check;
 
MMA8452 acc(PB_9, PB_8, 100000);
 
// Si une interrupt est trigger, LED s'allume pendant 1 seconde
void interrupt()
{
	led1 = !led1;
}

int main() {
   
	// CONFIG NUCLEO
	
	// Configuration de l'interrupt
	tap.rise(&interrupt);
	
	// CONFIG ACCELEROMETRE
	
	// Standby mode TEST
	// pc.printf("Value registre standby: %d\n",acc.standby());
	
	// Standby Mode
	acc.standby();
	//
	// Enable LPF
	//acc.writeRegister(0x0F, 0x10);
	//
	// ODR : 400 Hz, Standby Mode
	acc.writeRegister(MMA8452_CTRL_REG_1,0x08);
	// Pulse Configuration : Single tap
	acc.writeRegister(MMA8452_PULSE_CFG, 0x15);
	// Pulse threshold for X,Y,Z
	// Threshold X: 2g
	acc.writeRegister(MMA8452_PULSE_THSX, 0x20);
	// Threshold Y: 2g
	acc.writeRegister(MMA8452_PULSE_THSY, 0x20);
	// Threshold Z: 2g
	acc.writeRegister(MMA8452_PULSE_THSZ, 0x20);
	// Pulse Time Window 1: 50 ms, Normal Mode, No LPF
	acc.writeRegister(MMA8452_PULSE_TMLT, 0x50);
	// Pulse Latency Timer: 300 ms 
	acc.writeRegister(MMA8452_PULSE_LTCY, 0xF0);
	// Interrupt Routing
	// Enable Interrupt Register
	acc.writeRegister(MMA8452_CTRL_REG_4, 0x08);
	// Route INT1 to System Interrupt
	acc.writeRegister(MMA8452_CTRL_REG_5, 0x08);
	// Active Mode
	acc.writeRegister(MMA8452_CTRL_REG_1, 0x01);
	
	
	while (1) {
	
		here = !here;
		wait(0.4);

	}
}