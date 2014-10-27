#ifndef GLAPHIC_LCD_H
#define GLAPHIC_LCD_H

//interface 
class glaphic_lcd
{
 public:
  virtual ~glaphic_lcd();
  virtual void SendData(char *data,unsigned int len)=0;
  virtual pair<int,int> get_lcd_size()=0;
  virtual glaphic_lcd* Create()=0;
  virtual void setting(unsigned long flg)=0;
}


#endif/* GLAPHIC_LCD_H */
