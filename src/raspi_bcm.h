#ifndef __RASPI_BCM_H
#define __RASPI_BCM_H

#include <bcm2835.h>

// define hardware used change to fit your need
// Uncomment the board you have, if not listed
// uncommment custom board and set wiring tin custom section

// LoRasPi board
// see https://github.com/hallard/LoRasPI
//#define BOARD_LORASPI

// iC880A and LinkLab Lora Gateway Shield (if RF module plugged into)
// see https://github.com/ch2i/iC880A-Raspberry-PI
//#define BOARD_IC880A_PLATE

// Raspberri PI Lora Gateway for multiple modules
// see https://github.com/hallard/RPI-Lora-Gateway
//#define BOARD_PI_LORA_GATEWAY

// Dragino Raspberry PI hat
// see https://github.com/dragino/Lora
//#define BOARD_DRAGINO_PIHAT

// Now we include RasPi_Boards.h so this will expose defined
// constants with CS/IRQ/RESET/on board LED pins definition
#include "RasPiBoards.h"

int setup_bcm()
{
    if (!bcm2835_init())
    {
        fprintf(stderr, "%s bcm2835_init() Failed\n\n", __BASEFILE__);
        return 1;
    }

    printf("RF95 CS=GPIO%d", RF_CS_PIN);

#ifdef RF_LED_PIN
    pinMode(RF_LED_PIN, OUTPUT);
    digitalWrite(RF_LED_PIN, HIGH);
#endif

#ifdef RF_IRQ_PIN
    printf(", IRQ=GPIO%d", RF_IRQ_PIN);
    // IRQ Pin input/pull down
    pinMode(RF_IRQ_PIN, INPUT);
    bcm2835_gpio_set_pud(RF_IRQ_PIN, BCM2835_GPIO_PUD_DOWN);
    // Now we can enable Rising edge detection
    bcm2835_gpio_ren(RF_IRQ_PIN);
#endif

#ifdef RF_RST_PIN
    printf(", RST=GPIO%d", RF_RST_PIN);
    // Pulse a reset on module
    pinMode(RF_RST_PIN, OUTPUT);
    digitalWrite(RF_RST_PIN, LOW);
    bcm2835_delay(150);
    digitalWrite(RF_RST_PIN, HIGH);
    bcm2835_delay(100);
#endif

#ifdef RF_LED_PIN
    printf(", LED=GPIO%d", RF_LED_PIN);
    digitalWrite(RF_LED_PIN, LOW);
#endif

    printf("\n");
    return 0;
}
#endif
