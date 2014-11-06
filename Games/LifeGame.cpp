/*メモ
  show_mapとreadinitfleにIOが集積


*/
#include <unistd.h>
#include<cstdio>
#include<cstdlib>
#include<iostream>
#include<fstream>
#include<ctime>
#include <utility>
#include <string>
#include<algorithm>
using namespace std;

#define rep(i,j) for(i=0;i<j;i++)
#define TIMEOUT 10 //キーボード入力タイムアウト ms
#define WAITTIME 20 //TIMEOUT * 1ms
#define MAP_W 10
#define MAP_H 10
bool map[MAP_H][MAP_W];

void show_map()
{
  printf("\033[%dD\033%dA",MAP_W,MAP_H);
  int x,y;
  rep(y,MAP_H){
    rep(x,MAP_W)
      {
	if(map[y][x])printf("\033[41m ");
	else printf("\033[49m ");
	printf("\033[49m");
      }
    printf("\n");
  }
}

//引数のファイル名のファイルを読み込み、マップを作成
void readinitfile(char filepath[])
{
  string filename;
  bool flg=false;
  ifstream ifs(filepath);
  if(ifs.fail()){
    printf("error %s can't read.\n",filepath);
    exit(0);
  }
  
  string str;
  int x,y=-1,mini;
  while(getline(ifs,str) && ++y<MAP_H)
    {
      mini=str.size()>MAP_W ? MAP_W:str.size();
      for(x=0;x<mini;x++)
	{
	  map[y][x]= str[x]=='0' ? false :true;
	}
    }
}

//引数のマップについて(x,y)の周りの生存数を返す
int around_count(int x,int y,bool maptemp[MAP_H][MAP_W])
{
  int i,j,cnt=0;
  if(y-1>=0)//上列
    {
      if(x-1>=0 && maptemp[y-1][x-1])cnt++;
      if(maptemp[y-1][x])++cnt;
      if(x+1<MAP_W && maptemp[y-1][x+1])++cnt;
    }
  if(y+1<MAP_H)//下列
    {
      if(x-1>=0 && maptemp[y+1][x-1])cnt+=1;
      if(maptemp[y+1][x])cnt+=1;
      if(x+1<MAP_W && maptemp[y+1][x+1])cnt+=1;
    }
  if(x-1>=0 && maptemp[y][x-1])cnt+=1;
  if(x+1<MAP_W && maptemp[y][x+1])cnt+=1;
  //if(cnt)cout<<cnt<<endl;
  return cnt;
}


//周囲の状況に応じて生死変化
void change()
{
  int x,y,i;
  bool map_temp[MAP_H][MAP_W];
  rep(y,MAP_H)
    rep(x,MAP_W)
    map_temp[y][x]=map[y][x];
  rep(y,MAP_H)
    rep(x,MAP_W)
    {
      switch(around_count(x,y,map_temp)){
      case 0://過疎で死亡
      case 1:
	map[y][x]=false;
	break;
      case 2://平安
	break;
      case 3://常に生存
	map[y][x]=true;
	break;
      default ://過密で死亡(>=4)
	map[y][x]=false;
      }
    }
}

int main(int argc,char *argv[])
{    
  int x,y;
  //初期化
  rep(y,MAP_H)
    rep(x,MAP_W)
    map[y][x]=false;
  if(argc==2)
    readinitfile(argv[1]);

  while(true)
    {
      show_map();
      change();
      usleep(500000);
    }
}
