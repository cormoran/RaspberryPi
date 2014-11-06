#include"../../../Library/easy_v1/Glcd.h"
#include"../../../Library/easy_v1/FrameBuffer/framebuffer.h"
#include<bcm2835.h>
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<unistd.h>

#define SW_g RPI_BPLUS_GPIO_J8_03//green 
#define SW_b1 RPI_BPLUS_GPIO_J8_05 //blue1
#define SW_b2 RPI_BPLUS_GPIO_J8_07 //blue2
#define SW_w1 RPI_BPLUS_GPIO_J8_11 //white1
#define SW_w2 RPI_BPLUS_GPIO_J8_12 //white2

Glcd lcd;
gl_lcd::framebuffer fb;

void Tetris_Func()
{
  system("../Tetris/tetris.out");
  usleep(500000);
  while(!bcm2835_gpio_lev(SW_g));
}


void FB_Func()
{
  while(1)
    {
      //if(!bcm2835_gpio_lev(SW_g))return ;
      lcd.SendData_RGB565(fb.GetFrameAddress(),128*128*2);
      usleep(50000);//50ms
    }
}

void shutdown()
{
  int cnt=0;
  printf("if you want to shutdown\npress more than 1s.\n");
  while(!bcm2835_gpio_lev(SW_g))
    {
      lcd.SendData_RGB565(fb.GetFrameAddress(),128*128*2);
      usleep(10000);//10ms
      cnt++;
      if(cnt>100)
	{
	  system("shutdown -h now");
	  FB_Func();
	}
    }
}

void Home_Func()
{


}

int main()
{
  bcm2835_init();
  bcm2835_gpio_fsel(SW_g, BCM2835_GPIO_FSEL_INPT);
  bcm2835_gpio_fsel(SW_w1, BCM2835_GPIO_FSEL_INPT);
  bcm2835_gpio_fsel(SW_w2, BCM2835_GPIO_FSEL_INPT);
  bcm2835_gpio_fsel(SW_b1, BCM2835_GPIO_FSEL_INPT);
  bcm2835_gpio_fsel(SW_b2, BCM2835_GPIO_FSEL_INPT);
  bcm2835_gpio_set_pud(SW_g, BCM2835_GPIO_PUD_UP);
  bcm2835_gpio_set_pud(SW_w1, BCM2835_GPIO_PUD_UP);
  bcm2835_gpio_set_pud(SW_w2, BCM2835_GPIO_PUD_UP);
  bcm2835_gpio_set_pud(SW_b1, BCM2835_GPIO_PUD_UP);
  bcm2835_gpio_set_pud(SW_b2, BCM2835_GPIO_PUD_UP);

  while(true)
    {
      lcd.SendData_RGB565(fb.GetFrameAddress(),128*128*2);
      usleep(50000);//50ms

      if(!bcm2835_gpio_lev(SW_b1))
	{
	  //FB_Func();
	  Tetris_Func();
	}
      else if(!bcm2835_gpio_lev(SW_b2))
	{
	  Tetris_Func();
	}
      else if(!bcm2835_gpio_lev(SW_w1))
	{
	  Tetris_Func();
	}
      else if(!bcm2835_gpio_lev(SW_w2))
	{
	  Tetris_Func();
	}
      else if(!bcm2835_gpio_lev(SW_g))
	{
	  shutdown();
	}
    }
}
