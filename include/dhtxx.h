#ifndef __DHTXX__

#define __DHTXX__
#define DHT11 11
#define DHT22 22
#define AM2302 22

typedef struct {
    float degrees_c;
    float degrees_f;
    float humidity;
} DHTData;

int readDHT(int type, int pin, DHTData *temp_data, int numRetriesAllowed);

#endif
