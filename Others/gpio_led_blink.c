//LED点滅テスト
#include <stdio.h>
#include <stdlib.h>
#include <bcm2835.h>
#include <signal.h>

#define PIN RPI_V2_GPIO_P1_07

void signal_callback_handler(int signum)
{
  printf("\ndetect key interrupt\n",signum);
  bcm2835_close();
  printf("Program exit\n");
  exit(0);
}

int main(int argc, char **argv)
{
  if(!bcm2835_init())
    return 1;
  
  bcm2835_gpio_fsel(PIN,BCM2835_GPIO_FSEL_OUTP);
  
  signal(SIGINT, signal_callback_handler);
  printf("press ^C to exit program ...\n");
  
  while (1)
    {
      bcm2835_gpio_write(PIN, HIGH);
      delay(500);
      bcm2835_gpio_write(PIN, LOW);
      delay(500);
    }
}
