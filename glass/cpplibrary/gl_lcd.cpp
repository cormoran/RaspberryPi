typedef unsigned char uchar;

#include"gl_lcd.h"
#include<cmath>
#include<bcm2835.h>
#include<cstdio>
#include<cstdlib>
#include<string.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<unistd.h>

namespace gl_lcd{


  M014C9163SPI::M014C9163SPI(char AppMode){
    if(init())exit(1);
    //if(ShareMemInit(AppMode))exit(1);
  }
  M014C9163SPI::~M014C9163SPI(){
    end();
  }
  
  bool M014C9163SPI::init(){
    if(GPIO_init())return 1;//error
    if(SPI_init())return 1;//error
    delay(120);

    bcm2835_gpio_write(RST,HIGH);
    delay(120);
    bcm2835_gpio_write(RST,LOW);
    delay(1);
    bcm2835_gpio_write(RST,HIGH);
    delay(120);
    EXIT_SLEEP_MODED();
    delay(200);
    SET_DISPLAY_ON();
    SET_ADDRESS_MODE(0b00001000);
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
    Draw_rectangle(0, 0, 129,129, RGB_Colors[BLACK]);
    return 0;
  }

  bool M014C9163SPI::SPI_init(){
    bcm2835_spi_begin();
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      //transfer MSB first 
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);                   //
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_8);     //SPI Clock is 125MHz/8  NOTE:Max is 125MHx/2,Min is /65536
    bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                      //select CS0
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);      //CS0 active LOW  
    return 0;
  }

  bool M014C9163SPI::GPIO_init(){
    if(!bcm2835_init()){
      printf("cannot open bcm");
      return 1;
    }
    bcm2835_gpio_fsel(RST,BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(DC,BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_write(RST,HIGH);
    bcm2835_gpio_write(DC,LOW);
    return 0;
  }


  void M014C9163SPI::end(){
    bcm2835_spi_end();
    if(!bcm2835_close())perror("bcm2835_close error");

    /*if(shmctl(id, IPC_RMID, 0)==-1)
      perror("shmctl error");
    */
  }  

  void M014C9163SPI::Sendbyte(char cmd,uchar data){
    //if(*(ShareMemoryAddress+1)==MyWindowID){
      bcm2835_gpio_write(DC, cmd&0x01);
      bcm2835_spi_transfer(data);
      //}
  }
  void M014C9163SPI::Sendbytes(char cmd,char *data,unsigned int len)
  {
    //if(*(ShareMemoryAddress+1)==MyWindowID){
      bcm2835_gpio_write(DC,cmd&0x01);
      bcm2835_spi_writenb(data,len);
      //}
  }
  
  //Senddata by RGB 565 Mode
  void M014C9163SPI::SendFrame(char *buf,uint len)
  {
    //if(*(ShareMemoryAddress+1)==MyWindowID){
      SET_PIXEL_FORMAT(0b01010101);//RGB565モードにする
      SET_PAGE_ADDRESS(0,WINDOWPX_H-1);
      SET_COLUMN_ADDRESS(0,WINDOWPX_W-1);
      WRITE_MEMORY_START();
      Sendbytes(1,buf,len);
      //}
  }
  void M014C9163SPI::Draw_rectangle(uchar x,uchar y,uchar w,uchar h, uchar *rgb)
  {
    uchar i,j;
    SET_PAGE_ADDRESS(y,y+h-1);
    SET_COLUMN_ADDRESS(x,x+w-1);
    WRITE_MEMORY_START();
    for(i = 0; i <h; i++)
      for(j=0;j<w;j++)
	{
	  Sendbyte(1, rgb[0]);
	  Sendbyte(1, rgb[1]);
	  Sendbyte(1, rgb[2]);
	}
  }


  /**/ bool M014C9163SPI::NOP(){
    Sendbyte(0,0x00);
    return 0;
  }
  /**/ bool M014C9163SPI::SOFT_RESET(){
    Sendbyte(0,0x01);
    return 0;
  }
  /**/ bool M014C9163SPI::ENTER_SLEEP_MODE(){
    Sendbyte(0,0x10);
    return 0;
  }
  /**/ bool M014C9163SPI::EXIT_SLEEP_MODED(){
    Sendbyte(0,0x11);
    return 0;
  }
  /**/ bool M014C9163SPI::ENTER_PARTIAL_MODE(){
    Sendbyte(0,0x12);
    return 0;
  }
  /**/ bool M014C9163SPI::ENTER_NORMAL_MODE(){
    Sendbyte(0,0x13);
    return 0;
  }
  /**/ bool M014C9163SPI::EXIT_INVERT_MODE(){
    Sendbyte(0,0x20);
    return 0;
  }
  /**/ bool M014C9163SPI::ENTER_INVERT_MODE(){
    Sendbyte(0,0x21);
    return 0;
  }
  /**/ bool M014C9163SPI::SET_GAMMA_CURVE(uchar one){
    Sendbyte(0,0x26);
    Sendbyte(1,one);
    return 0;
  }
  /**/ bool M014C9163SPI::SET_DISPLAY_OFF(){
    Sendbyte(0,0x28);
    return 0;
  }
  /**/ bool M014C9163SPI::SET_DISPLAY_ON(){
    Sendbyte(0,0x29);
    return 0;
  }
  /**/ bool M014C9163SPI::SET_COLUMN_ADDRESS(uchar x_start,uchar x_end){
    Sendbyte(0,0x2a);
    Sendbyte(1,0);
    Sendbyte(1,x_start);
    Sendbyte(1,0);
    Sendbyte(1,x_end);
    return 0;
  }
  /**/ bool M014C9163SPI::SET_PAGE_ADDRESS(uchar y_start,uchar y_end){
    Sendbyte(0,0x2b);
    Sendbyte(1,0);
    Sendbyte(1,y_start);
    Sendbyte(1,0);
    Sendbyte(1,y_end);
    return 0;
  }
  /**/ bool M014C9163SPI::WRITE_MEMORY_START(){
    Sendbyte(0,0x2c);
    return 0;
  }
  /**/ bool M014C9163SPI::WRITE_LUT(){
    Sendbyte(0,0x2d);
    return 0;
  }
  /**/ bool M014C9163SPI::SET_PARTIAL_AREA(uchar one,uchar two,uchar three,uchar four){
    Sendbyte(0,0x30);
    Sendbyte(1,one);
    Sendbyte(1,two);
    Sendbyte(1,three);
    Sendbyte(1,four);
    return 0;
  }
  /**/ bool M014C9163SPI::SET_SCROLL_AREA(uchar one,uchar two,uchar three,uchar four,uchar five,uchar six){
    Sendbyte(0,0x33);
    Sendbyte(1,one);
    Sendbyte(1,two);
    Sendbyte(1,three);
    Sendbyte(1,four);
    Sendbyte(1,five);
    Sendbyte(1,six);
    return 0;
  }
  /**/ bool M014C9163SPI::SET_TEAR_OFF(){
    Sendbyte(0,0x34);
    return 0;
  }
  /**/ bool M014C9163SPI::SET_TEAR_ON(uchar one){
    Sendbyte(0,0x35);
    Sendbyte(1,one);
    return 0;
  }
  /**/ bool M014C9163SPI::SET_ADDRESS_MODE(uchar one){
    Sendbyte(0,0x36);
    Sendbyte(1,one);
    return 0;
  }
  /**/ bool M014C9163SPI::SET_SCROLL_START(uchar one,uchar two){
    Sendbyte(0,0x37);
    Sendbyte(1,one);
    Sendbyte(1,two);
    return 0;
  }
  /**/ bool M014C9163SPI::EXIT_IDLE_MODE(){
    Sendbyte(0,0x38);
    return 0;
  }
  /**/ bool M014C9163SPI::ENTER_IDLE_MODE(){
    Sendbyte(0,0x39);
    return 0;
  }
  /**/ bool M014C9163SPI::SET_PIXEL_FORMAT(uchar format){
    Sendbyte(0,0x3a);
    Sendbyte(1,format);
    return 0;
  }
  /**/ bool M014C9163SPI::WRITE_MEMORY_CONTINUE ()
  {
    Sendbyte(0,0x3c);
    return 0;
  }
  /**/ bool M014C9163SPI::SET_TEAR_SCANLINE(uchar one,uchar two)
  {
    Sendbyte(0,0x44);
    Sendbyte(1,one);
    Sendbyte(1,two);
    return 0;
  }

}
