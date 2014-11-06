#include<iostream>
#include<unistd.h>
#include<cstdlib>
using namespace std;
int main()
{
  std::cout<<"Process ID : "<<getpid()<<std::endl;
  std::cout<<"Parent ID : "<<getppid()<<std::endl;
  char *ts="12345 as";
  cout<<atoi(ts)+1<<endl;
}
