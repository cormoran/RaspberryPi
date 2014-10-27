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

IplImage* get128image(int argc,int **argv)
{
  IplImage *sorceimage=0,*resizedimage;
  char *imagename;
  imagename = argc > 1 ? argv[1] : "./none.png";
  sorceimage = cvLoadImage(imagename, CV_LOAD_IMAGE_COLOR);
  //sorceimage = cvLoadImage(imagename,CV_LOAD_IMAGE_GRAYSCALE );
  if(sorceimage == 0)
    return 0;
  resizedimage=cvCreateImage(cvSize(128,128),IPL_DEPTH_8U,3);
  cvResize(sorceimage,resizedimage,CV_INTER_NN);
  cvRelease(&sorceimage);
  return resizedimage;
}

int main(int argc,int **argv)
{
  init();
      ENTER_INVERT_MODE_glcd;
  int x, y;
  uchar p[3];
  IplImage *img=get128image(argc,argv);
  if (img==0)
    return 1;
    
  // (1)画素値（R,G,B）を順次取得し，変更する
  for (y = 0; y < img->height; y++) {
    for (x = 0; x < img->width; x++) {
      /* 画素値を直接操作する一例 */
      p[2] = img->imageData[img->widthStep * y + x * 3];        // B                                              
      p[1] = img->imageData[img->widthStep * y + x * 3 + 1];    // G                                              
      p[0] = img->imageData[img->widthStep * y + x * 3 + 2];    // R                                               
      Draw_rectangle_glcd(x,y,1,1,p);
    }
  }

  cvWaitKey (0);
  cvReleaseImage(&img);
  return 0;
}
