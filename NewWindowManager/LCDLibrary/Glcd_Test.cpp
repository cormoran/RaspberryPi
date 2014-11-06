#include<bcm2835.h>
#include"Glcd.h"
#include<unistd.h>
#include<iostream>
#include<cstdio>
int main()
{
  puts("1 made");
  Glcd *lcd1=new Glcd;
  puts("2 make");
  Glcd *lcd2=new Glcd;

  unsigned char rgb[3]={200,200,200};
  int i;
  for(i=0;i<50;i++)
    {
      rgb[0]=i*2;
      rgb[1]=255-i;
      rgb[2]=(23*i)%256;
      lcd1->Draw_rectangle(10+i%20,10-i%10,50+i%20,50-i%10,rgb);

      lcd2->Draw_rectangle(60-i%20,60-i%20,50+i%20,50-i%10,rgb);
      usleep(100000);
    }
  delete lcd2;
  std::cout<<"lcd2 end"<<std::endl;
  lcd1->Draw_rectangle(0,0,128,128,rgb);
  for(i=0;i<50;i++)
    {
      rgb[0]=i*2;
      rgb[1]=255-i;
      rgb[2]=(23*i)%256;
      lcd1->Draw_rectangle(10+i%20,10-i%10,50+i%20,50-i%10,rgb);
      usleep(100000);
    }
  delete lcd1;
  puts("Last Message");

  return 0;


}
