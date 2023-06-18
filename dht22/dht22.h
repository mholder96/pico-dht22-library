#ifndef _DHT22_H_
#define _DHT22_H_


#include <stdio.h>
#include "pico/stdlib.h"

#ifdef __cplusplus
extern "C" {
#endif


#define MAXCYCLES 125000 //clock cycles for 1ms
#define LOW_CYCLES 5000 //clock cycles for 30us 


typedef struct dht_t{ 
    uint _data_pin;
    uint8_t data[5];

} dht_t;

typedef enum {
    DHT_RESULT_OK,
    DHT_RESULT_TIMEOUT,
    DHT_RESULT_BAD_CHECKSUM,
} dht_result;

int setup_DHT(dht_t *dht, uint pin);

dht_result read_DHT(dht_t *dht);

uint32_t waitForPulse(dht_t *dht, bool level);

int countCycles (dht_t *dht);

void readTemp(dht_t *dht, float *temp);

void readHumidity(dht_t *dht, float *humidity);

#ifdef __cplusplus
}
#endif

#endif
