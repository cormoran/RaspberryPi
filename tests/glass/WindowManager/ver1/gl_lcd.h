#ifndef GL_LCD_H
#define GL_LCD_H

#ifdef __cplusplus
extern "C" {
#endif


#include "gl_lcd_data.h"

#define DC RPI_BPLUS_GPIO_J8_05
#define SCK RPI_BPLUS_GPIO_J8_23
#define SDA RPI_BPLUS_GPIO_J8_19 
#define CS RPI_BPLUS_GPIO_J8_24
#define RST RPI_BPLUS_GPIO_J8_22

#define NOP_glcd                    Sendbyte_glcd(0,0x00)
#define SOFT_RESET_glcd             Sendbyte_glcd(0,0x01)
#define ENTER_SLEEP_MODE_glcd       Sendbyte_glcd(0,0x10)
#define EXIT_SLEEP_MODED_glcd       Sendbyte_glcd(0,0x11)
#define ENTER_PARTIAL_MODE_glcd     Sendbyte_glcd(0,0x12)
#define ENTER_NORMAL_MODE_glcd      Sendbyte_glcd(0,0x13)
#define EXIT_INVERT_MODE_glcd       Sendbyte_glcd(0,0x20)
#define ENTER_INVERT_MODE_glcd      Sendbyte_glcd(0,0x21)
#define SET_GAMMA_CURVE_glcd(one)   (Sendbyte_glcd(0,0x26),Sendbyte_glcd(1,one))
#define SET_DISPLAY_OFF_glcd        Sendbyte_glcd(0,0x28)
#define SET_DISPLAY_ON_glcd         Sendbyte_glcd(0,0x29)
#define SET_COLUMN_ADDRESS_glcd(\
	        x_start,x_end)      (Sendbyte_glcd(0,0x2a),Sendbyte_glcd(1,0),Sendbyte_glcd(1,x_start),Sendbyte_glcd(1,0),Sendbyte_glcd(1,x_end))
#define SET_PAGE_ADDRESS_glcd(\
	 	  y_start,y_end)    (Sendbyte_glcd(0,0x2b),Sendbyte_glcd(1,0),Sendbyte_glcd(1,y_start),Sendbyte_glcd(1,0),Sendbyte_glcd(1,y_end))
#define WRITE_MEMORY_START_glcd     Sendbyte_glcd(0,0x2c)
#define WRITE_LUT_glcd              Sendbyte_glcd(0,0x2d)
#define SET_PARTIAL_AREA_glcd(\
		 one,two,three,				\
                four)               (Sendbyte_glcd(0,0x30),Sendbyte_glcd(1,one),Sendbyte_glcd(1,two),Sendbyte_glcd(1,three),Sendbyte_glcd(1,four))
#define SET_SCROLL_AREA_glcd(\
                one,two,three,\
                four,five,six)      (Sendbyte_glcd(0,0x33),Sendbyte_glcd(1,one),Sendbyte_glcd(1,two),Sendbyte_glcd(1,three),Sendbyte_glcd(1,four),Sendbyte_glcd(1,five),Sendbyte_glcd(1,six))
#define SET_TEAR_OFF_glcd           Sendbyte_glcd(0,0x34)
#define SET_TEAR_ON_glcd(one)       (Sendbyte_glcd(0,0x35),Sendbyte_glcd(1,one))
#define SET_ADDRESS_MODE_glcd(one)  (Sendbyte_glcd(0,0x36),Sendbyte_glcd(1,one))
#define SET_SCROLL_START_glcd(\
	      	one,two)            (Sendbyte_glcd(0,0x37),Sendbyte_glcd(1,one),Sendbyte_glcd(1,two))
#define EXIT_IDLE_MODE_glcd         Sendbyte_glcd(0,0x38)
#define ENTER_IDLE_MODE_glcd        Sendbyte_glcd(0,0x39)
#define SET_PIXEL_FORMAT_glcd(one)  (Sendbyte_glcd(0,0x3a),Sendbyte_glcd(1,one))
#define WRITE_MEMORY_CONTINUE_glcd  Sendbyte_glcd(0,0x3c)
#define SET_TEAR_SCANLINE_glcd(\
		one,two)            (Sendbyte_glcd(0,0x44),Sendbyte_glcd(1,one),Sendbyte_glcd(1,two))

  //Color Number Definition
#define BLACK_glcd 0
#define WHITE_glcd 12
#define RED_glcd 1
#define GREEN_glcd 7
#define BLUE_glcd 5
#define YELLOW_glcd 9
#define PINK_glcd 2
#define PURPLE_glcd 3
#define MAGENTA_glcd 4
#define CYAN_glcd 6
#define LIME_glcd 8
#define ORANGE_glcd 10
#define GRAY_glcd 11

typedef unsigned char uchar;

extern  void init_glcd(void);
extern  void Sendbytes_glcd(char cmd,uchar *data,unsigned int len);
extern  void Sendbyte_glcd(char cmd,uchar data);

extern  void Draw_rectangle_glcd(uchar x,uchar y,uchar w,uchar h,uchar *rgb);
extern  void Draw_line_glcd(uchar xs,uchar ys,uchar xe,uchar ye,uchar w, uchar *rgb);
extern  void Draw_cycle_glcd(uchar x0,uchar y0,uchar r,uchar w,uchar *rgb);
extern  void Draw_eclipse(uchar x0,uchar y0,uchar r,uchar a,uchar b,uchar w,uchar *rgb);
extern  void Reset_H_glcd(void);
extern  void Draw_chara_glcd(uchar x,uchar y,char chara,uchar *fontrgb,uchar *backrgb);
extern  void Draw_string_glcd(uchar x,uchar y,char *str,uchar *fontrgb,uchar *backrgb);

  extern void charatest(void);
  extern  void Show_rawimage(char *path,unsigned char w,unsigned char h);
#ifdef __cplusplus
}
#endif

#endif/* GL_LCD_H */
