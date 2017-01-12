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

#include "dhtxx.h"

// Access from ARM Running Linux

#define BCM2708_PERI_BASE        0x20000000
#define GPIO_BASE                (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */
#define MAX_RETRIES		 5

#define MAXTIMINGS 40

int readDHT(int type, int pin, DHTData *temp_data, int allowRetry);

int readBits( int pin, int bits[MAXTIMINGS+1] ) {
    int bitidx = 0;
    int i=0;
    int counter = 0;
    struct sched_param param;

    if (!bcm2835_init()) {
        return 0;
    }

    // Set GPIO pin to output
    bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_OUTP);

    // Yield to get a good block of time for reading.
    sched_yield();

    bcm2835_gpio_write(pin, LOW);
    usleep(2);
    bcm2835_gpio_write(pin, HIGH);

    bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_INPT);

    // wait for pin to drop
    while (bcm2835_gpio_lev(pin) == HIGH){
	if( counter++ > 10000 ) {
	    printf( "ERROR: Pin never dropped\n" );
            return 1;
	}
    }

    // read data!
    while (i<= MAXTIMINGS) {
        counter = 0;
        while ( bcm2835_gpio_lev(pin) == LOW) {
            if (counter++ == 1000)
	      break;
        }
        counter = 0;
        while ( bcm2835_gpio_lev(pin) == HIGH) {
            if (counter++ == 1000)
	      break;
        }
        bits[bitidx++] = counter;
        i++;
    }
    return bitidx;
}

int compileBits( int bitidx, int bits[MAXTIMINGS+1], int data[5] ) {
    int j = 0;

    data[0] = data[1] = data[2] = data[3] = data[4] = 0;

    for (int i=1; i<bitidx; i++) {
        // shove each bit into the storage bytes
        data[j/8] <<= 1;
        if (bits[i] > 200) {
            data[j/8] |= 1;
	}
        j++;
    }

#ifdef DEBUG
    for (int i=1; i<bitidx; i++) {
        printf("bit %d: %d (%d)\n", i, bits[i], bits[i] > 200);
    }
#endif

#ifdef VERBOSE
    printf("Data (%d): 0x%x 0x%x 0x%x 0x%x 0x%x\n", j, data[0], data[1], data[2], data[3], data[4]);
#endif

    return j;
}

int compileData( int type, int numBits, int data[5], DHTData *temp_data ) {
    // Check to make sure we have enough bits and a correct checksum
    if ((numBits >= 39) && (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) ) {
	if( type == DHT11 ) {
	    temp_data->degrees_c = (float)data[2];
	    temp_data->degrees_f = ((temp_data->degrees_c*9)/5)+32;
	    temp_data->humidity = data[0];
	} else {
	    float f, h;
	    h = data[0] * 256 + data[1];
	    h /= 10;

	    f = (data[2] & 0x7F)* 256 + data[3];
            f /= 10.0;
            if (data[2] & 0x80)  f *= -1;

	    temp_data->degrees_c = f;
	    temp_data->degrees_f = ((f*9)/5)+32;
	    temp_data->humidity = h;
	
	    return 1;
        }
    }

    return 0;
}

int readDHT(int type, int pin, DHTData *temp_data, int allowRetry) {
    int bits[MAXTIMINGS+1], data[5];
    int bitidx;
    int j;

    bitidx = readBits( pin, bits );
    j = compileBits( bitidx, bits, data );
    if( compileData( type, j, data, temp_data ) == 0 ) {
    	if( allowRetry > 0 ) {
    	    sleep(1);
#ifdef VERBOSE
            printf( "Error getting information. Retrying\n" );
#endif
     	    return readDHT( type, pin, temp_data, --allowRetry );
        } else {
#ifdef VERBOSE
            printf( "Error getting information. Retries exhausted.\n" );
#endif
    	    return 0;
        }
    }
    return 1;
}
