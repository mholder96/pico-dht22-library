#include "dht22.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <stdio.h>


int setup_DHT(dht_t *dht, uint pin)
{   
    //assert(dht != NULL);
    dht->_data_pin = pin;
    gpio_init(dht->_data_pin);
    //set gpio to input with pull up, and high impedance until a reading is taken
    gpio_set_dir(dht->_data_pin, GPIO_IN);
     gpio_pull_up(dht->_data_pin);
    
}


/*
*/
dht_result read_DHT(dht_t *dht)
{
     dht->data[0] = dht->data[1] = dht->data[2] = dht->data[3] = dht->data[4] = 0;
     uint32_t cycles[80];
     //set direction GPIO direction to output
     gpio_set_dir(dht->_data_pin, GPIO_OUT);
     gpio_put(dht->_data_pin, 0);
     //stay low for 1-10ms, then high for 20-40us
     sleep_ms(2);
     gpio_put(dht->_data_pin, 1);
     sleep_us(40);
     //change direction to input to read incoming message
     gpio_set_dir(dht->_data_pin, GPIO_IN);
     gpio_pull_up(dht->_data_pin);
     //wait for sensor to pull down for 80us
     if (waitForPulse(dht, 0) == 0)
     {
        //sensor timeout, return error
        return DHT_RESULT_TIMEOUT;
     }
     //sensor has gone low and now goes high for 80us
     if (waitForPulse(dht, 1) == 0)
     {
        //sensor timeout, return error
        return DHT_RESULT_TIMEOUT;
     }
     
     for (int i = 0; i < 80; i+=2) {
        cycles[i] = waitForPulse(dht, 0);
        cycles[i+1] = waitForPulse(dht, 1);
     }

     for (int i = 0; i < 40; i++) {
        uint32_t lowCycles = cycles[2*i];
        uint32_t highCycles = cycles[2*i+1];

        //shift the bit to the left, because masking with 1 
        //will occur in bit 0 of each byte
        //so I'm putting bits in then shift to the left for each byte
        //
        dht->data[i/8] <<= 1;
        if (highCycles > lowCycles) {
            dht->data[i/8] |=1;
        }
     }
     //check that checksum matches
     uint32_t checksum = (dht->data[0]+dht->data[1]+dht->data[2]+dht->data[3]) & 0xFF;
     if (dht->data[4] == checksum)
     {
        return DHT_RESULT_OK;
     }
     else
     {
        return DHT_RESULT_BAD_CHECKSUM;
     }

}



uint32_t waitForPulse(dht_t *dht, bool level)
{
    uint32_t count = 0;
    while (gpio_get(dht->_data_pin) == level)
    {
        //1ms timeout for waiting for DHT
        if (count++ >= MAXCYCLES)
        {
            return 0;
        }
    }
    return count;


}

int countCycles (dht_t *dht)
{
    uint32_t count = 0;
    while (gpio_get(dht->_data_pin) == 1)
    {
        //printf("you get here\n");
        //if cycles is greater than 30us, send back a 1
        if (count++ > LOW_CYCLES)
        {
            printf("you get here\n");
            return 1;
        }
    }
    //if count doesn't equal 30us, send back a zero
    printf("getting zeros");
    return 0;    
}

void readTemp(dht_t *dht, float *temp)
{
    *temp = ((dht->data[2] << 8) + dht->data[3])*0.1f;
    
}

void readHumidity(dht_t *dht, float *humidity)
{
    *humidity = ((dht->data[0] << 8) + dht->data[1])*0.1f;
}