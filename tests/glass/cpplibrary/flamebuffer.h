#ifndef FLAMEBUFFER_H
#define FLAMEBUFFER_H

#define uchar unsigned char
#define uint unsigned int

class flamebuffer
{
 private:
  int fd;
  volatile uchar *frame;
  uint screen_width;
  uint screen_height;
 public:

  bool init();
  uchar *GetFrameAddres();
  bool FrameCopy(uchar*buff,uint w,uint h);
  bool FrameCopy(uchar*buff,uint sx,uint sy,uint w,uint h);
}
#endif
