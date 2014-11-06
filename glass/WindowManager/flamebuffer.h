//
//  raspFB: Raspberry Pi FB (Frame Buffer) module
//      xkozima@myu.ac.jp
#ifndef FB_H
#define FB_H

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <stdio.h>
#define FB_DEVICE "/dev/fb0"            //  FB デバイス

  void fb_init(int *width, int *height);
  unsigned char *getFrame();
  extern int fd;
  extern volatile unsigned char *frame;

#endif
