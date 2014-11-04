#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#define uchar unsigned char
#define uint unsigned int
namespace gl_lcd{
  class framebuffer
  {
  private:
  
    volatile uchar *frame;//framebuffer head address
    uint screen_width;
    uint screen_height;
    void init();
    void end();
  public:
    framebuffer();
    ~framebuffer();
    char *GetFrameAddress();
    bool FrameCopy(char*buff,int w,int h);
    bool FrameCopy(char*buff,int sx,int sy,int w,int h);
  };
}
#endif
