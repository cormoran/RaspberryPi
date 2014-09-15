#include <cv.h>
#include <highgui.h>

#include <stdio.h>
#include <stdlib.h>
#include <bcm2835.h>
#include <math.h>
#include "gl_lcd.h"

int init(){
  //GPIO Init
  if(!bcm2835_init())
    return 1;
  init_glcd();
}

int main(int argc,int **argv)
{
  init();
  CvCapture *capture = 0;
  IplImage *frame = 0;
  double w = 176, h = 144;
  int c,x,y;
  uchar p[3];
  capture = cvCreateCameraCapture(0);
  cvSetCaptureProperty (capture, CV_CAP_PROP_FRAME_WIDTH, w);
  cvSetCaptureProperty (capture, CV_CAP_PROP_FRAME_HEIGHT, h);
  while(1){
    frame = cvQueryFrame(capture);
    // (1)画素値（R,G,B）を順次取得し，変更する
    for (y = 0; y < 128; y++) {
      for (x = 0; x < 128; x++) {
	/* 画素値を直接操作する一例 */
	p[2] = frame->imageData[frame->widthStep * y + x * 3];        // B                                              
	p[1] = frame->imageData[frame->widthStep * y + x * 3 + 1];    // G                                              
	p[0] = frame->imageData[frame->widthStep * y + x * 3 + 2];    // R                                             
	Draw_rectangle_glcd_x(y,x,1,1,p);
      }
    }
    c = cvWaitKey (1);
    if (c == 'q')
      break;
  }
  cvReleaseImage(&frame);
  return 0;
}

