//
//  raspFB: Raspberry Pi FB (Frame Buffer) module
//      xkozima@myu.ac.jp
#include "flamebuffer.h"
//  init: FB 初期化（最初に１回だけ呼び出すこと）

int fd;
volatile unsigned char *frame;
void fb_init (int *width, int *height)
{
  int ret;
  //  フレームバッファ /dev/fb0 を開く（sudo が必要）
  fd = open(FB_DEVICE, O_RDWR);
  if (fd < 0) {
    printf("error: cannot open %s (FB::init)\n", FB_DEVICE);
    exit(-1);
  }
  //  画面情報を取得・チェック
  struct fb_var_screeninfo sinfo;
  ret = ioctl(fd, FBIOGET_VSCREENINFO, &sinfo);
  if (ret < 0) {
    printf("error: cannot get var screen info (FB::init)\n");
    exit(-1);
  }
  printf("fb_width  : %d\n", sinfo.xres);
  printf("fb_height : %d\n", sinfo.yres);
  *width   = sinfo.xres;
  *height  = sinfo.yres;
  if (sinfo.bits_per_pixel != 16) {
    //  16bit でなければエラー
    printf("error: the frame buffer is not 16bpp (FB::init)\n");
    exit(1);
  }
  struct fb_fix_screeninfo finfo;
  ret = ioctl(fd, FBIOGET_FSCREENINFO, &finfo);
  if (ret < 0) {
    printf("error: cannot get fix screen info (FB::init)\n");
    exit(-1);
  }
  printf("fb_linelen: %d\n", finfo.line_length);
  if (finfo.line_length != sinfo.xres * 2) {
    //  １ラインの長さが割り切れないとエラー（高速化のため）
    printf("error: line length must be width*2 (FB::init)\n");
    exit(-1);
  }
  //  /dev/fb0 をメモリ上に mmap
  int size = sinfo.xres * sinfo.yres * 2;
  frame = (unsigned char *)
    mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (frame == NULL) {
    printf("error: cannot map /dev/mem on the memory (FB::init)\n");
    exit(-1);
  }
}
//  fb_get: フレームバッファの内容を取得する
unsigned char *getFrame ()
{
  //  フレームバッファへのポインタを返す
  return (unsigned char *) frame;
}
