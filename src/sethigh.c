//  How to access GPIO registers from C-code on the Raspberry-Pi
//  Example program
//  15-January-2012
//  Dom and Gert
//


// Access from ARM Running Linux

#define BCM2708_PERI_BASE        0x20000000
#define GPIO_BASE                (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */
#define MAX_RETRIES		 5

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <bcm2835.h>
#include <unistd.h>
#include <sched.h>

#define MAXTIMINGS 40

//#define DEBUG
#define SILENT 1

int main(int argc, char **argv)
{
    if (!bcm2835_init())
        return 1;

    if (argc != 3) {
	printf("usage: %s [0|1] GPIOpin#\n", argv[0]);
	printf("example: %s 1 22 - Set GPIO #22 high.\n", argv[0]);
	return 2;
    }
    int type = 0;
    if (strcmp(argv[1], "0") == 0)
        type = 0;
    else
        type = 1;
  
    int dhtpin = atoi(argv[2]);

    if (dhtpin <= 0) {
	printf("Please select a valid GPIO pin #\n");
	return 3;
    } 

    if( SILENT < 1 ) {
    	printf("Using pin #%d\n", dhtpin);
    }
    if( type > 0 ) 
    	setHigh(dhtpin);
    else
    	setLow(dhtpin);

    return 0;
}

int setHigh(int pin) {
    bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_write(pin, HIGH);
}

int setLow(int pin) {
    bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_write(pin, LOW);
}


