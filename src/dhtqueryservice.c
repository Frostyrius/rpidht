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

// DHT type check
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

    struct sched_param param;
    param.sched_priority = 1;
    sched_setscheduler( 0, SCHED_RR, &param );
    
    if( readDHT(type, dhtpin, &temp_data, MAX_RETRIES) > 0 ) {
	printf("INSERT INTO %s VALUES (datetime(\"now\", \"localtime\"), %.1f, %.1f);\n" , argv[3], temp_data.degrees_c, temp_data.humidity);
    }

    return 0;
}
