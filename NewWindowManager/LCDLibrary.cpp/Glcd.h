#ifndef GLCD_H
#define GLCD_H

//セマフォ実装済みのLCDコントロールクラス
//同時に使用しようとした時は待機になる
class Glcd : public M014C9163SPI
{
private:
  bool Can_use_lcd;//LCD使用権を持っているか（セマフォの話）
  int semid;
  key_t semkey;
  void MySemop(bool lock);  
public:
  Glcd();
  ~Glcd();
  void Sendbyte(char cmd,uchar data);
  void Sendbytes(char cmd,char *data,unsigned int len);
  void Draw_rectangle(uchar x,uchar y,uchar w,uchar h, uchar *rgb);
};

#endif
