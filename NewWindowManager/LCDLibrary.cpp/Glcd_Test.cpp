#include<bcm2835.h>
#include"Glcd.h"

int main()
{
  Glcd lcd1,lcd2;
  unsigned char rgb[3]={200,200,200};
  int i;
  for(i=0;i<100;i++)
    {
      rgb[0]=i*2;
      rgb[1]=255-i;
      rgb[2]=(23*i)%256;
      lcd1.Draw_rectangle(10+i%20,10-i%10,50+i%20,50-i%10,rgb);
      lcd2.Draw_rectangle(120-i%20,120-i%20,50+i%20,50-i%10,rgb);
      usleep(200000);
    }
  return 0;


}
