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
    {
      puts("bcm2835 cannot open");
      //quit(0);
    }
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

  SET_PAGE_ADDRESS_glcd(y,y+h-1);
  SET_COLUMN_ADDRESS_glcd(x,x+w-1);
  WRITE_MEMORY_START_glcd;

  while(1){
    frame = cvQueryFrame(capture);
    // (1)画素値（R,G,B）を順次取得し，変更する
    for (y = 0; y < 128; y++) {
      for (x = 0; x < 128; x++) {
	/* 画素値を直接操作する一例 */
	//p[2] = frame->imageData[frame->widthStep * y + x * 3];        // B                                              
	//p[1] = frame->imageData[frame->widthStep * y + x * 3 + 1];    // G                                              
	//p[0] = frame->imageData[frame->widthStep * y + x * 3 + 2];    // R                                             
	//Draw_rectangle_glcd(y,x,1,1,p);
	Sendbytes_glcd(1,frame->imgData);
      }
    }
  }
  cvReleaseImage(&frame);
  return 0;
}

