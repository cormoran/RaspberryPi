//液晶制御ライブラリ動作確認テスト

#include<bcm2835.h>
#include<unistd.h>
#include"M014C9163SPI.h"

int main()
{
  M014C9163SPI lcd;
  unsigned char rgb[3]={200,200,200};
  int i;
      for(i=0;i<100;i++)
	{
	  rgb[0]=i*2;
	  rgb[1]=255-i;
	  rgb[2]=(23*i)%256;
	  lcd.Draw_rectangle(10+i%20,10-i%10,50+i%20,50-i%10,rgb);
	  usleep(200000);
	}
  return 0;
}
