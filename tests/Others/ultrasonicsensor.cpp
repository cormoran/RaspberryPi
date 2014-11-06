#include<iostream>
#include<cstdio>
#include<cmath>
#include <bcm2835.h>
#include<sys/time.h>

#define TRIG RPI_BPLUS_GPIO_J8_03
#define ECHO RPI_BPLUS_GPIO_J8_05

int main(){
  if (!bcm2835_init()){
    std::cout<<"bcm2835 init error";
    return 1;
  }

  //pin initialize
  bcm2835_gpio_fsel(TRIG,BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_fsel(ECHO,BCM2835_GPIO_FSEL_INPT);
  bcm2835_gpio_set_pud(ECHO,BCM2835_GPIO_PUD_DOWN);
  bcm2835_gpio_write(TRIG, LOW);
  delay(500);




  while(1){
    struct timeval stime,etime;
    //sensor start
    bcm2835_gpio_write(TRIG, LOW);
    delay(1);
    bcm2835_gpio_write(TRIG, HIGH);
    delay(1);
    bcm2835_gpio_write(TRIG, LOW);    
    //measure the pulse width
    std::cout<<"lowwait";
    while(bcm2835_gpio_lev(ECHO)==LOW);
    gettimeofday(&stime, NULL);
    std::cout<<" highwait"<<std::endl;
    while(bcm2835_gpio_lev(ECHO)==HIGH);
    gettimeofday(&etime, NULL);
    printf("dist=%d",std::abs(etime.tv_usec-stime.tv_usec)/2*340);
    delay(100);
  }
  bcm2835_close();
  return 0;
}
