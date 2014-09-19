#ifndef GL_LCD_H
#define GL_LCD_H

#include "gl_lcd_data.h"

#define DC RPI_BPLUS_GPIO_J8_05
#define SCK RPI_BPLUS_GPIO_J8_23
#define SDA RPI_BPLUS_GPIO_J8_19 
#define CS RPI_BPLUS_GPIO_J8_24
#define RST RPI_BPLUS_GPIO_J8_22

typedef unsigned char uchar;

namespace gl_lcd{

class M014C9163SPI
{
private:
  void SPI_init_glcd();
  void GPIO_init_glcd();
public:

  enum colors{black,red,pink,purple,magenta,bluemcyan,green,lime,yellow,orangle,gray,white};

  inline bool NOP();
  inline bool SOFT_RESET();
  inline bool ENTER_SLEEP_MODE();
  inline bool EXIT_SLEEP_MODED();
  inline bool ENTER_PARTIAL_MODE();
  inline bool ENTER_NORMAL_MODE();
  inline bool EXIT_INVERT_MODE();
  inline bool ENTER_INVERT_MODE();
  inline bool SET_GAMMA_CURVE(uchar one);
  inline bool SET_DISPLAY_OFF();
  inline bool SET_DISPLAY_ON();
  inline bool SET_COLUMN_ADDRESS(uchar x_start,uchar x_end);
  inline bool SET_PAGE_ADDRESS(uchar y_start,uchar y_end);
  inline bool WRITE_MEMORY_START();
  inline bool WRITE_LUT();
  inline bool SET_PARTIAL_AREA(uchar one,uchar two,uchar three,uchar four);
  inline bool SET_SCROLL_AREA(uchar one,uchar two,uchar three,uchar four,uchar five,uchar six);
  inline bool SET_TEAR_OFF();
  inline bool SET_TEAR_ON(uchar one);
  inline bool SET_ADDRESS_MODE(uchar one);
  inline bool SET_SCROLL_START(uchar one,uchar two);
  inline bool EXIT_IDLE_MODE_glcd();
  inline bool ENTER_IDLE_MODE_glcd();
  inline bool SET_PIXEL_FORMAT(uchar format);
  inline bool WRITE_MEMORY_CONTINUE ();
  inline bool SET_TEAR_SCANLINE(uchar one,uchar two);

  void init_glcd();
  void Sendbytes_glcd(char cmd,uchar *data,unsigned int len);
  void Sendbyte_glcd(char cmd,uchar data);

  void Draw_rectangle_glcd(uchar x,uchar y,uchar w,uchar h,uchar *rgb);
  void Draw_line_glcd(uchar xs,uchar ys,uchar xe,uchar ye,uchar w, uchar *rgb);
  void Draw_cycle_glcd(uchar x0,uchar y0,uchar r,uchar w,uchar *rgb);
  void Draw_eclipse(uchar x0,uchar y0,uchar r,uchar a,uchar b,uchar w,uchar *rgb);
  void Reset_H_glcd(void);
  void Draw_chara_glcd(uchar x,uchar y,char chara,uchar *fontrgb,uchar *backrgb);
  void Draw_string_glcd(uchar x,uchar y,char *str,uchar *fontrgb,uchar *backrgb);

  void charatest();
  void Show_rawimage(char *path,unsigned char w,unsigned char h);
}

#endif/* GL_LCD_H */
