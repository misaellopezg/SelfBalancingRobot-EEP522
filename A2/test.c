//Created by: Misa Lopez
//01/28/23
//Testing bcm2835 library to blink led

#include <bcm2835.h>
#include <stdio.h>
 
// Blink GPIO 17 (or header pin 11)
#define PIN RPI_GPIO_P1_11
 
int main()
{
	if( !bcm2835_init())
	{
		return 1;
	}
	//set  pin to be an output
	bcm2835_gpio_fsel(PIN, BCM2835_GPIO_FSEL_OUTP);
    	// blink
    	while (1)
    	{
		//turn on
        	bcm2835_gpio_write(PIN, HIGH);
        	//wait 1 second
        	bcm2835_delay(1000);
		//turn off
        	bcm2835_gpio_write(PIN, LOW);
		//wait 1 second
        	bcm2835_delay(1000);
    	}
	//exit
    	bcm2835_close();
    	return 0;
}
