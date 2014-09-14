#include <cv.h>
#include <highgui.h>

#include <stdio.h>
#include <stdlib.h>
#include <bcm2835.h>
#include <signal.h>
#include <math.h>
#include "gl_lcd.h"

#define PIN RPI_BPLUS_GPIO_J8_07


void signal_callback_handler(int signum)
{
  printf("\ndetect key interrupt\n",signum);
  bcm2835_spi_end();
  bcm2835_close();
  printf("Program exit\n");
  exit(0);
}

int main(void)
{
  //GPIO Init
  if(!bcm2835_init())
    return 1;
  bcm2835_gpio_fsel(PIN,BCM2835_GPIO_FSEL_OUTP);

  init_glcd();
  
  signal(SIGINT, signal_callback_handler);
  printf("press ^C to exit program ...\n");
  
  int x, y;
  uchar p[3];
  int width=128, height=128;
  IplImage *img=0, *hsv_img=0;
  double c, f;
   
  // (1)allocate and initialize an image
  img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
  if(img==0) return -1;
  cvZero(img);
  hsv_img = cvCloneImage(img);
  cvCvtColor(img, hsv_img, CV_BGR2HSV);
  f = cvGetTickFrequency()*1000;
 
  // (2)create hue-value gradation image
  c = cvGetTickCount();
  for(y=0; y<height; y++) {
    for(x=0; x<width; x++) {
      int a = img->widthStep*y+(x*3);
      hsv_img->imageData[a+0] = (x*180/width);
      hsv_img->imageData[a+1] = 255;
      hsv_img->imageData[a+2] = ((height-y)*255/height);
    }
  }
  printf("%f\n", (cvGetTickCount()-c)/f);
 
  cvCvtColor(hsv_img, img, CV_HSV2BGR);
  
  // (1)画素値（R,G,B）を順次取得し，変更する
  for (y = 0; y < img->height; y++) {
    for (x = 0; x < img->width; x++) {
      /* 画素値を直接操作する一例 */
      p[0] = img->imageData[img->widthStep * y + x * 3];        // B
      p[1] = img->imageData[img->widthStep * y + x * 3 + 1];    // G
      p[2] = img->imageData[img->widthStep * y + x * 3 + 2];    // R

      Draw_rectangle_glcd_x(x,y,1,1,p);
    }
  }
  
  // (3)show the iamge, and quit when any key pressed
  cvNamedWindow ("Gradation", CV_WINDOW_AUTOSIZE);
  cvShowImage ("Gradation", img);
  cvWaitKey (0);
 
  cvDestroyWindow("Gradation");
  cvReleaseImage(&img);

  int i=0,j,k;
  while (1)
    {

      Draw_eclipse_glcd(60,60,40,1,2,1,i++ +2%13);
      Draw_string_glcd(60,30,"Hello World!",GREEN_glcd,BLACK_glcd);
      if(i==13)i=0;
    }
}

