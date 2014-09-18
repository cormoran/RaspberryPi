typedef unsigned char uchar;

#include"gl_lcd.h"
#include<math.h>
#include<bcm2835.h>

/*SPI Initialize
  bcm2835 must be initialized before call this function
*/
void SPI_init_glcd(){
  bcm2835_spi_begin();
  bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      //transfer MSB first 
  bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);                   //
  bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_8);     //SPI Clock is 125MHz/8  NOTE:Max is 125MHx/2,Min is /65536
  bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                      //select CS0
  bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);      //CS0 active LOW  
}

/*GPIO(RST & DC pin) initialize
  bcm2835 must be initialized before call this function
 */
void GPIO_init_glcd(){
  bcm2835_gpio_fsel(RST,BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_fsel(DC,BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_write(RST,HIGH);
  bcm2835_gpio_write(DC,LOW);
}

void init_glcd(){
  GPIO_init_glcd();
  SPI_init_glcd();
  delay(120);

  bcm2835_gpio_write(RST,HIGH);
  delay(120);
  bcm2835_gpio_write(RST,LOW);
  delay(1);
  bcm2835_gpio_write(RST,HIGH);
  delay(120);
  EXIT_SLEEP_MODED_glcd;
  delay(200);
  SET_DISPLAY_ON_glcd;
  SET_ADDRESS_MODE_glcd(0b00101000);
  /* 7-5が0のとき通常のグラフ座標（左下が原点）
   * 詳細はDataSheet P127
   *
   * 7:MY :Row Address Order
   *  :1でy軸反転(上端がy=0に)
   * 6:MX :Column Address Order
   *  :1でx軸反転(右端がx=0に)
   * 5:MV :Page/Column Selection
   *  :原点反転
   * 4:ML :Vertical Order
   *  :LCD Vertical refresh direction control
   * 3:RGB:RGB/BGR Order
   *  :0でRGB,1でBGR表示
   * 2:MH :Display data latch order
   *  :1=LCD Refresh right to left
   *   0=LCD Refresh left to right
   * 1:none
   * 0:none
   */



  //  Draw_rectangle_glcd(0, 0, 129,129, colors_glcd[BLACK_glcd]);
 
}


void Sendbyte_glcd(char cmd,uchar data){
  bcm2835_gpio_write(DC, cmd&0x01);
  bcm2835_spi_transfer(data);
}
void Sendbytes_glcd(char cmd,uchar *data,unsigned int len)
{
  bcm2835_gpio_write(DC,cmd&0x01);
  bcm2835_spi_writenb(data,len);
}



void Draw_rectangle_glcd(uchar x,uchar y,uchar w,uchar h, uchar *rgb)
{
  uchar i,j;
  SET_PAGE_ADDRESS_glcd(y,y+h-1);
  SET_COLUMN_ADDRESS_glcd(x,x+w-1);
  WRITE_MEMORY_START_glcd;
  for(i = 0; i <h; i++)
    for(j=0;j<w;j++)
      {
	Sendbyte_glcd(1, rgb[0]);
	Sendbyte_glcd(1, rgb[1]);
	Sendbyte_glcd(1, rgb[2]);
      }
}

/*直線描写関数
 * (xs,ys)から(xe,ye)まで太さwで色colorの直線を引く
 */
void Draw_line_glcd(uchar xs,uchar ys,uchar xe,uchar ye, uchar w,uchar *rgb)
{
  //Bresenhamで直線描写
  uchar dx,dy;
  char sx,sy;
  int slope;
  if(xs<xe){
    sx=1;
    dx=xe-xs;
  }
  else{
    sx=-1;
    dx=xs-xe;
  }
  if(ys<ye){
    sy=1;
    dy=ye-ys;
  }
  else{
    sy=-1;
    dy=ys-ye;
  }
  slope=dx-dy;

  while(1){
    Draw_rectangle_glcd(xs,ys,w,w,rgb);
    if(xs==xe && ys==ye)break;
    if(slope*2>-dy){
      slope-=dy;
      xs+=sx;
    }
    if(slope*2<dx){
      slope+=dx;
      ys+=sy;
    }
  }

  return;
}

void Draw_cycle_glcd(uchar x0,uchar y0,uchar r,uchar w,uchar *rgb)
{
  uchar i;
  uchar x = r;
  uchar y = 0;
  int F = -2 * r + 3;
  while ( x >= y ) {
    Draw_rectangle_glcd( x0 + x, y0 + y,w,w, rgb );
    Draw_rectangle_glcd( x0 - x, y0 + y,w,w, rgb );
    Draw_rectangle_glcd( x0 + x, y0 - y,w,w, rgb );
    Draw_rectangle_glcd( x0 - x, y0 - y,w,w, rgb );
    Draw_rectangle_glcd( x0 + y, y0 + x,w,w, rgb );
    Draw_rectangle_glcd( x0 - y, y0 + x,w,w, rgb );
    Draw_rectangle_glcd( x0 + y, y0 - x,w,w, rgb );
    Draw_rectangle_glcd( x0 - y, y0 - x,w,w, rgb );
    if(F>=0){
      x--;
      F -= 4 * x;
    }
    y++;
    F += 4 * y + 2;
  }
}

void Draw_eclipse_glcd(uchar x0,uchar y0,uchar r,uchar a,uchar b,uchar w, uchar *rgb)
{
  uchar x = (int)( (double)r / sqrt( (double)a ) );
  int y = 0;
  double d = sqrt( (double)a ) * (double)r;
  int F = (int)( -2.0 * d ) +     a + 2 * b;
  int H = (int)( -4.0 * d ) + 2 * a     + b;

  while (1) {
    Draw_rectangle_glcd( x0 + x, y0 + y,w,w, rgb );
    Draw_rectangle_glcd( x0 - x, y0 + y,w,w, rgb );
    Draw_rectangle_glcd( x0 + x, y0 - y,w,w, rgb );
    Draw_rectangle_glcd( x0 - x, y0 - y,w,w, rgb );

    if(!x)break;

    if ( F >= 0 ) {
      --x;
      F -= 4 * a * x;
      H -= 4 * a * x - 2 * a;
    }
    if ( H < 0 ) {
      ++y;
      F += 4 * b * y + 2 * b;
      H += 4 * b * y;
    }
  }
}
  

void Draw_chara_glcd(uchar x,uchar y,char chara,uchar *fontrgb, uchar *backrgb)
{
  char charanum=chara-' ';
  char i,j;
  SET_PAGE_ADDRESS_glcd(y,y+5-1);
  SET_COLUMN_ADDRESS_glcd(x,x+8-1);
  WRITE_MEMORY_START_glcd;
  for(i=0;i<5;i++)
    {
      unsigned char fline=Font[charanum][i];
      for(j=0;j<8;j++)
        {
	  if(fline & 0x01){
	    Sendbyte_glcd(1, fontrgb[0]);
	    Sendbyte_glcd(1, fontrgb[1]);
	    Sendbyte_glcd(1, fontrgb[2]);
	  }
	  else{
	    Sendbyte_glcd(1, backrgb[0]);
	    Sendbyte_glcd(1, backrgb[1]);
	    Sendbyte_glcd(1, backrgb[2]);
	  }
	  fline=fline>>1;
        }
    }
}

void Draw_string_glcd(uchar x,uchar y,char *str,uchar *fontrgb,uchar *backrgb)
{
  while(*str){
    Draw_chara_glcd(x,y,*str++,fontrgb,backrgb);
    y+=6;
  }
}


void charatest(){

  int i,j;
  uchar x=30,y=30;
  uchar xlen=15,ylen=20;
  uchar *fontrgb=colors_glcd[WHITE_glcd];
  uchar *backrgb=colors_glcd[BLACK_glcd];

  SET_PAGE_ADDRESS_glcd(y,y+ylen-1);
  SET_COLUMN_ADDRESS_glcd(x,x+xlen-1);
  WRITE_MEMORY_START_glcd;
  for(i=0;i<ylen;i++)
    {
      unsigned char *fline=three[i];
      for(j=0;j<xlen;j++)
        {
	  if(fline[j]){
	    Sendbyte_glcd(1, fontrgb[0]);
	    Sendbyte_glcd(1, fontrgb[1]);
	    Sendbyte_glcd(1, fontrgb[2]);
	  }
	  else{
	    Sendbyte_glcd(1, backrgb[0]);
	    Sendbyte_glcd(1, backrgb[1]);
	    Sendbyte_glcd(1, backrgb[2]);
	  }
        }
    }
  Draw_rectangle_glcd(1,1,4,4,colors_glcd[RED_glcd]);
  Draw_rectangle_glcd(124,124,4,4,colors_glcd[GREEN_glcd]);
  Draw_rectangle_glcd(124,1,4,4,colors_glcd[BLUE_glcd]);
  
}

#include<stdio.h>

void Show_rawimage(char *path,uchar w,uchar h)
{
  FILE *fp;
  uchar c;

  fp = fopen(path, "rb" );
  if( fp == NULL ){
    printf( "%sファイルが開けません¥n", path);
    return ;
  }


  uchar i,j,k,l;
  SET_PAGE_ADDRESS_glcd(0,h-1);
  SET_COLUMN_ADDRESS_glcd(0,w-1);
  WRITE_MEMORY_START_glcd;
  k=0;
  fread(&c,sizeof(uchar),1,fp);
  for(i=0;i<h;i++)
    {
      for(j=0;j<w;j++)
        {
	  for(l=0;l<3;l++){
	    if(++k==8)
	      {
		k=0;
		if(!fread(&c,sizeof(uchar),1,fp))goto END;
	      } 
	    Sendbyte_glcd(1, c&0x80);  
	    c=c<<1;
	  }
        }
    }
    END:
  fclose( fp );
  /*
  SET_PAGE_ADDRESS_glcd(3,10);
  SET_COLUMN_ADDRESS_glcd(0,127);
  WRITE_MEMORY_START_glcd;
  for(i=0;i<10;i++)
    {
      for(j=0;j<128;j++)
	Sendbyte_glcd(1,255);
	}
  */
  return ;


}
