#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <cstdio>
#include<string.h>
#include "framebuffer.h"

namespace gl_lcd{


  framebuffer::framebuffer(){
    init();
  }
  framebuffer::~framebuffer(){
    return ;
  }

  /*フレームバッファをマッピングする処理*/
  void framebuffer::init ()
  {
    int fd;//framebuffer file descriptor
    fd = open("/dev/fb0", O_RDWR);
    if (fd < 0){perror("error : framebuffer open :");exit(1);}
  
    struct fb_var_screeninfo vinfo;//スクリーンごとの可変情報
    struct fb_fix_screeninfo finfo;//システム固有情報
  
    if (ioctl(fd, FBIOGET_VSCREENINFO, &vinfo) < 0){perror("error : get fb_var_screeninfo :");exit(1);}
    if(ioctl(fd,FBIOGET_FSCREENINFO,&finfo)<0){perror("error : get fb_var_screeninfo :");exit(1);}

    screen_width  = vinfo.xres;
    screen_height  = vinfo.yres;

    if (vinfo.bits_per_pixel != 16){printf("error: frame buffer must be 16bpp!\n");exit(1);}

    int datalen = screen_width * screen_height * 2;
    frame = (uchar *)mmap(NULL, datalen, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (frame == NULL) {perror("error: cannot map /dev/mem");exit(-1);}
  
    //close(fd);
  }


  char *framebuffer::GetFrameAddress()
  {
    return (char*)frame;
  }

  /*buffにフレームの(0,0)から(w,h)ピクセルまでをコピー
   *RGB565 なので w*h*2 byte必要
   */
  bool framebuffer::FrameCopy(char*buff,int w,int h)
  {
    return FrameCopy(buff,0,0,w,h);
  }
  /*buffにフレームの(sx,sy)から(sx+w,sy+h)ピクセルまでコピー)
    RGB565 なので w*h*2 byte
  */
  bool framebuffer::FrameCopy(char*buff,int sx,int sy,int w,int h)
  {
    uint x,y;
    if(sx+w<screen_width && sy+h<screen_height)
      {    
	for(y=sy;y<sy+h;y++){
	  memcpy(buff+y*w*2,(void*)(frame+y*screen_width*2+sx*2),w*2);
	}
	return true;
      }
    else return false;
  }
}
