#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <stdio.h>
#include "flamebuffer.h"

/*フレームバッファをマッピングする処理*/
void init ()
{
  fd = open("/dev/fb0", O_RDWR);
  if (fd < 0){perror("error : flamebuffer open :");exit(1);}
  
  struct fb_var_screeninfo vinfo;//スクリーンごとの可変情報
  struct fb_fix_screeninfo finfo;//システム固有情報
  
  if (ioctl(fd, FBIOGET_VSCREENINFO, &vinfo) < 0){perror("error : get fb_var_screeninfo :");exit(1);}
  if(ioctl(fd,FBIOGET_FSCREENINFO,&finfo)<0){perror("error : get fb_var_screeninfo :");exit(1);}

  screen_width  = vinfo.xres;
  screen_width  = vinfo.yres;
  if (sinfo.bits_per_pixel != 16){printf("error: frame buffer must be 16bpp!\n");exit(1);}

  int datalen = screen_width * screen_height * 2;
  frame = (uchar *)mmap(NULL, datalen, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (frame == NULL) {perror("error: cannot map /dev/mem");exit(-1);}
}

uchar *GetFrameAddress()
{
  return frame;
}

/*buffにフレームの(0,0)から(w,h)ピクセルまでをコピー
 *RGB565 なので w*h*2 byte必要
 */
bool FrameCopy(uchar*buff,uint w,uint h)
{
  return FrameCopy(buff,0,0,w,h);
}
/*buffにフレームの(sx,sy)から(sx+w,sy+h)ピクセルまでコピー)
  RGB565 なので w*h*2 byte
 */
bool FrameCopy(uchar*buff,uint sx,uint sy,uint w,uint h)
{
  uint x,y;
  if(sx+w<screen_width && sy+h<screen_height)
    {    
      for(y=sy;y<sy+h;y++){
	memcpy(buff+y*w*2,frame+y*screen_width*2+sx*2,w*2);
      }
      return true;
    }
  else return false;
}

