#include<bcm2835.h>
#include<M014C9163SPI.h>
#include<unistd.h>

int main()
{
  M014C9163SPI LCD;
  unsigned char rgb[3]={200,200,200};
  LCD.Draw_rectangle(10,10,50,50,rgb};
  sleep(1);
  rgb[0]=0;
  LCD.Draw_rectangle(10,10,50,50,rgb};
  sleep(1);
  rgb[1]=0;
  LCD.Draw_rectangle(10,10,50,50,rgb};
  return 0;
}
