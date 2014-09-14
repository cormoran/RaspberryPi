#include <opencv2/opencv.hpp>
 
int main()
{
  cv::VideoCapture cam(0);
  while(true)
    {
      cv::Mat img;
      cam >> img;
      cv::imshow("img", img);
      char key=cv::waitKey(1);
      if(key=='q') break;
    }
  return 0;
}
