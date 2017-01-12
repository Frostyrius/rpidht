//  How to access GPIO registers from C-code on the Raspberry-Pi
//  Example program
//  15-January-2012
//  Dom and Gert
//

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <bcm2835.h>
#include <stdio.h>
#include <sched.h>
#include <time.h>

#include <dhtxx.h>

// Access from ARM Running Linux

#define BCM2708_PERI_BASE        0x20000000
#define GPIO_BASE                (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */
#define MAX_RETRIES		 5
#define MAXTIMINGS 40

int main(int argc, char **argv)
{
    DHTData temp_data;

    if (argc != 3) {
	printf("usage: %s [11|22|2302] GPIOpin#\n", argv[0]);
	printf("example: %s 2302 4 - Read from an AM2302 connected to GPIO #4\n", argv[0]);
	return 2;
    }
    int type = 0;
    if (strcmp(argv[1], "11") == 0) type = DHT11;
    if (strcmp(argv[1], "22") == 0) type = DHT22;
    if (strcmp(argv[1], "2302") == 0) type = AM2302;
    if (type == 0) {
	printf("Select 11, 22, 2302 as type!\n");
	return 3;
    }  
  
    int dhtpin = atoi(argv[2]);

    if (dhtpin <= 0) {
	printf("Please select a valid GPIO pin #\n");
	return 3;
    } 

#ifdef VERBOSE 
    printf("Using pin #%d\n", dhtpin);
#endif

    struct sched_param param;
    param.sched_priority = 1;
    sched_setscheduler( 0, SCHED_RR, &param );
    
    if( readDHT(type, dhtpin, &temp_data, MAX_RETRIES) > 0 ) {
	printf("INSERT INTO temps VALUES (datetime(\"now\"), %.1f, %.1f);\n" , temp_data.degrees_c, temp_data.humidity);
    } else {
	printf( "Could not retrieve temperatures\n" );
    }

    return 0;
}
