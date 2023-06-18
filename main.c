#include <stdio.h>
#include "pico/stdlib.h"
#include "dht22.h"
#include "hardware/gpio.h"


#define SENSOR 28

int main() {
    stdio_init_all();
    printf("DHT test\n");
    float temp, humidity;
    dht_t dht;
    setup_DHT(&dht, SENSOR);
    while(true) {
        dht_result result = read_DHT(&dht);
        if (result == DHT_RESULT_OK) {
            printf("result ok\n");
        } else if (result == DHT_RESULT_TIMEOUT) {
            printf("timeout\n");
        } else if (result == DHT_RESULT_BAD_CHECKSUM) {
            printf("bad checksum\n");
        }
        readTemp(&dht, &temp);
        readHumidity(&dht, &humidity);

        printf("Temperature: %.1f\n", temp);
        printf("Humidity: %.1f\n", humidity);
        //printf("%d", dht.data[0]);

        sleep_ms(2000);
    }

    
}