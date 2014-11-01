#ifdef M014C9163SPI_CPP
#define M014C9163SPI_CPP

class M014C9163SPI
{
 public:
  M014C9163SPI();
  ~M014C9163SPI();
  void Sendbyte(char cmd,uchar data);
  void Sendbytes(char cmd,char *data,unsigned int len);
  void Draw_rectangle(uchar x,uchar y,uchar w,uchar h, uchar *rgb);
  pair<int,int> get_lcd_size();
 private:
  bool SPI_init();
  bool GPIO_init();
  bool init();
  void end();
 public:
  /**/ bool NOP();
  /**/ bool SOFT_RESET();
  /**/ bool ENTER_SLEEP_MODE();
  /**/ bool EXIT_SLEEP_MODED();
  /**/ bool ENTER_PARTIAL_MODE();
  /**/ bool ENTER_NORMAL_MODE();
  /**/ bool EXIT_INVERT_MODE();
  /**/ bool ENTER_INVERT_MODE();
  /**/ bool SET_GAMMA_CURVE(uchar one);
  /**/ bool SET_DISPLAY_OFF();
  /**/ bool SET_DISPLAY_ON();
  /**/ bool SET_COLUMN_ADDRESS(uchar x_start,uchar x_end);
  /**/ bool SET_PAGE_ADDRESS(uchar y_start,uchar y_end);
  /**/ bool WRITE_MEMORY_START();
  /**/ bool WRITE_LUT();
  /**/ bool SET_PARTIAL_AREA(uchar one,uchar two,uchar three,uchar four);
  /**/ bool SET_SCROLL_AREA(uchar one,uchar two,uchar three,uchar four,uchar five,uchar six);
  /**/ bool SET_TEAR_OFF();
  /**/ bool SET_TEAR_ON(uchar one);
  /**/ bool SET_ADDRESS_MODE(uchar one);
  /**/ bool SET_SCROLL_START(uchar one,uchar two);
  /**/ bool EXIT_IDLE_MODE();
  /**/ bool ENTER_IDLE_MODE();
  /**/ bool SET_PIXEL_FORMAT(uchar format);
  /**/ bool WRITE_MEMORY_CONTINUE ();
  /**/ bool SET_TEAR_SCANLINE(uchar one,uchar two);
 
};

#endif
