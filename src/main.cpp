// Uses the bcm2835 library to access the GPIO pins to drive the RFM95 module
// Requires bcm2835 library to be already installed
// http://www.airspayce.com/mikem/bcm2835/

#include <Arduino.h>
#include <bcm2835.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <RH_RF95.h>
#include <RH_RF69.h>
#include "raspi_bcm.h"
#include "RasPiBoards.h"

#include "modem.h"
#include "gps.h"

volatile sig_atomic_t force_exit = false;

extern RH_RF95 rf95;

void sig_handler(int sig)
{
  printf("\n%s Break received, exiting!\n", __BASEFILE__);
  force_exit = true;
}

fd_set readfds;
struct timeval timeout;
unsigned long led_blink = 0;

void setup()
{
  signal(SIGINT, sig_handler);

  if (setup_bcm() != 0)
  {
    fprintf(stderr, "\nBCM init failed, Please verify wiring/module\n");
    return;
  }

  init_RF95();
  out_println("LoRa radio init OK!");

  rf95.setModeRx();

#ifdef USE_GPS
  init_gps();
#endif

  FD_ZERO(&readfds);
  timeout.tv_sec = 0;
  timeout.tv_usec = 0;

  out_println("+READY");
}
void loop()
{
  char message[512];
  FD_SET(STDIN_FILENO, &readfds);

  if (select(1, &readfds, NULL, NULL, &timeout) > 0)
  {
    if (fgets(message, 512, stdin) != NULL)
    {
      String cmd = String(message);
      cmd.trim();
      cmd.toUpperCase();
      handle_command(cmd);
    }
  }
#ifdef RF_IRQ_PIN
  // Rising edge fired ?
  if (bcm2835_gpio_eds(RF_IRQ_PIN))
  {
    // Now clear the eds flag by setting it to 1
    bcm2835_gpio_set_eds(RF_IRQ_PIN);
    //printf("Packet Received, Rising event detect for pin GPIO%d\n", RF_IRQ_PIN);
#endif

    modem_receive();

#ifdef RF_LED_PIN
    led_blink = millis();
    digitalWrite(RF_LED_PIN, HIGH);
#endif

#ifdef RF_IRQ_PIN
  }
#endif

#ifdef RF_LED_PIN
  // Led blink timer expiration ?
  if (led_blink && millis() - led_blink > 200)
  {
    led_blink = 0;
    digitalWrite(RF_LED_PIN, LOW);
  }
#endif
#ifdef USE_GPS
  gps_loop_tick();
#endif
  // Let OS doing other tasks
  // For timed critical appliation you can reduce or delete
  // this delay, but this will charge CPU usage, take care and monitor
  bcm2835_delay(5);
}

int main(int argc, const char *argv[])
{
  float freq = 0.0;
  if(argc==2) {
    freq = atof(argv[1]);
  }
  setup();
  if (freq > 0) {
    char cmd_str[1024];
    snprintf(cmd_str, 1024, "AT+FREQ=%.2f", freq);
    printf("Setting default frequency to %.2f Mhz\n", freq);
    handle_command(cmd_str);
  }
  while (!force_exit)
  {
    loop();
  }
#ifdef RF_LED_PIN
  digitalWrite(RF_LED_PIN, LOW);
#endif
  printf("\n%s Ending\n", __BASEFILE__);
  bcm2835_close();
  return 0;
}
