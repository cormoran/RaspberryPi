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
    void SPI_init();
    void GPIO_init();
    void init();
    void end();
  public:

    enum colors{BLACK,RED,PINK,PURPLE,MAGENTA,BLUE,MCYAN,GREEN,LIME,YELLOW,ORANGLE,GRAY,WHITE};


    M014C9163SPI(){
      init();
    }
    ~M014C9163SPI(){
      //他が使ってなければ終わるべし
      //end();
    }

    void Sendbytes(char cmd,char *data,unsigned int len);
    void Sendbyte(char cmd,uchar data);

    void Draw_rectangle(uchar x,uchar y,uchar w,uchar h,uchar *rgb);
    void Draw_line(uchar xs,uchar ys,uchar xe,uchar ye,uchar w, uchar *rgb);
    void Draw_cycle(uchar x0,uchar y0,uchar r,uchar w,uchar *rgb);
    void Draw_eclipse(uchar x0,uchar y0,uchar r,uchar a,uchar b,uchar w,uchar *rgb);
    void Reset_H(void);
    void Draw_chara(uchar x,uchar y,char chara,uchar *fontrgb,uchar *backrgb);
    void Draw_string(uchar x,uchar y,char *str,uchar *fontrgb,uchar *backrgb);

    void charatest();
    void Show_rawimage(char *path,unsigned char w,unsigned char h);


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
}
#endif/* GL_LCD_H */
