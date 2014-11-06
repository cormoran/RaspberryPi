#include <unistd.h>
#include<cstdio>
#include<cstdlib>
#include<ctime>
#include <utility>
#include <string.h>
#include<bcm2835.h>

#include"../../../Library/easy_v1/Glcd.h"
#include"../../../Library/easy_v1/FrameBuffer/framebuffer.h"

using namespace std;

#define SW_g RPI_BPLUS_GPIO_J8_03//green 
#define SW_b1 RPI_BPLUS_GPIO_J8_05 //blue1
#define SW_b2 RPI_BPLUS_GPIO_J8_07 //blue2
#define SW_w1 RPI_BPLUS_GPIO_J8_11 //white1
#define SW_w2 RPI_BPLUS_GPIO_J8_12 //white2


#define LCD_BLOCK_W 5
#define LCD_BLOCK_H 5

Glcd *lcd;
char windowbuffer[128*128*3];


#define WAITTIME 2
#define SLEEPTIME 100000 //us

#define NEXT_BLOCK_X 13
#define NEXT_BLOCK_Y 0

#define SCORE_X 13
#define SCORE_Y 20

#define MAP_X (10+2) //文字　壁2つ
#define MAP_Y (19+1+5) //行　下ガード＋未表示領域
#define MAP_NOT_SHOW_NUM 5
#define BLOCK_H_W 5
#define BLOCK_NUM 7
bool block_can_rot[BLOCK_NUM]={0,1,1,1,1,1,1};
char block_data[BLOCK_NUM][5][5]=
 {
    {{0,0,0,0,0},
     {0,2,2,0,0},
     {0,2,2,0,0},
     {0,0,0,0,0},
     {0,0,0,0,0}
    },
    {{0,0,0,0,0},
     {0,0,3,0,0},
     {0,3,3,3,0},
     {0,0,0,0,0},
     {0,0,0,0,0}
    },
    {{0,0,0,0,0},
     {0,0,0,4,0},
     {0,0,4,4,0},
     {0,0,4,0,0},
     {0,0,0,0,0}
    },
    {{0,0,5,0,0},
     {0,0,5,0,0},
     {0,0,5,0,0},
     {0,0,5,0,0},
     {0,0,0,0,0}
    },
    {{0,0,0,0,0},
     {0,6,0,0,0},
     {0,6,6,0,0},
     {0,0,6,0,0},
     {0,0,0,0,0}
    },
    {{0,0,0,0,0},
     {0,0,7,7,0},
     {0,0,7,0,0},
     {0,0,7,0,0},
     {0,0,0,0,0}
    },
    {{0,0,0,0,0},
     {0,8,8,0,0},
     {0,0,8,0,0},
     {0,0,8,0,0},
     {0,0,0,0,0}
    },
  };

enum Colors{Black,White,Red,Green,Blue,Yellow,Cyan,Magenta,Pink};
unsigned char RGBColor[9][3]
{
  {0,0,0},{55,55,55},{255,0,0},{0,255,0},{0,0,255},{255,255,0},{0,255,255},{255,0,255},{247,171,166}
};

enum Direction{UP,DOWN,RIGHT,LEFT};

typedef struct block
{
  char data[BLOCK_H_W][BLOCK_H_W];
  bool can_rot;
  pair<int,int> point;
  block()
  {
    reset(0);
  }
  void reset(char type)
  {
    char x,y;
    for(y=0;y<BLOCK_H_W;y++)
      for(x=0;x<BLOCK_H_W;x++)
	data[y][x]=block_data[type][y][x];
    can_rot=block_can_rot[type];
    point.first=3;
    point.second=0;
  }
  bool rotate(Direction dir,char map[MAP_Y][MAP_X])
  {
    if(can_rot){
      char next_data[BLOCK_H_W][BLOCK_H_W];
      //取り敢えず回転させてみる
      if(dir==RIGHT)//右移転
	{
	  char x,y;
	  for(y=0;y<BLOCK_H_W;y++)
	    for(x=0;x<BLOCK_H_W;x++)
	      next_data[x][(BLOCK_H_W-1)-y]=data[y][x];
	}
      else if(dir==LEFT)
	{
	  char x,y;
	  for(y=0;y<BLOCK_H_W;y++)
	    for(x=0;x<BLOCK_H_W;x++)
	      next_data[(BLOCK_H_W-1)-x][y]=data[y][x];
	}
      //衝突チェック
      if(can_move(point.first,point.second,next_data,map)){
	char x,y;
	for(y=0;y<BLOCK_H_W;y++)
	  for(x=0;x<BLOCK_H_W;x++)
	    data[y][x]=next_data[y][x];
	return true;
      }
    }
    return false;
  }
  //x,y 左上端座標：負になることもある
  bool can_move(int x,int y,char next_data[BLOCK_H_W][BLOCK_H_W],char map[MAP_Y][MAP_X])
  {
    char i,j;
    for(i=0;i<BLOCK_H_W;i++)
      for(j=0;j<BLOCK_H_W;j++)
	if(x+i>=0 && y+j>=0 && x+i<MAP_X && y+i<MAP_Y)
	  if(next_data[j][i]!=0 && map[y+j][x+i]!=0)
	    return false;
    return true;
  }
  bool move(Direction dir,char map[MAP_Y][MAP_X])
  {
    switch(dir){
    case RIGHT://右
      if(can_move(point.first+1,point.second,data,map)){
	point.first+=1;
	return true;
      }
      break;
    case LEFT://左
      if(can_move(point.first-1,point.second,data,map)){
	point.first-=1;
	return true;
      }
      break;
    case UP://上
      if(can_move(point.first,point.second-1,data,map)){
	point.second-=1;
	return true;
      }
      break;
    case DOWN://下
      if(can_move(point.first,point.second+1,data,map)){
	point.second+=1;
	return true;
      }
      break;
    }
    return false;
  }
}block;


void put_window()
{
  lcd->SendData_RGB666(windowbuffer,128*128*3);
}

bool draw_rect(int x,int y,int w,int h,unsigned char *rgb)
{
  x*=LCD_BLOCK_W+1;
  y*=LCD_BLOCK_H+1;
  if(x>=0 && y>=0 && x+w<=128 && y+h<=128)
    {
      int i,j;
      for(i=y;i<y+h;i++)
	for(j=x*3;j<(x+w)*3;j+=3)
	  {
	    memcpy(windowbuffer+j+128*i*3,rgb,3);
	  }
      return false;
    }
  return true;
}

void fill_buffer(int color)
{
  draw_rect(0,0,128,128,RGBColor[color]);
}

void show_map(char map[MAP_Y][MAP_X])
{
  int x,y;
  for(y=0;y<MAP_Y-MAP_NOT_SHOW_NUM;y++)
    for(x=0;x<MAP_X;x++){
      draw_rect(x,y,LCD_BLOCK_W,LCD_BLOCK_H,RGBColor[map[y+MAP_NOT_SHOW_NUM][x]]);
    }
}

void show_map(block *myblock,char map[MAP_Y][MAP_X])
{
  int x,y;
  for(y=0;y<MAP_Y-MAP_NOT_SHOW_NUM;y++)
    for(x=0;x<MAP_X;x++){
      draw_rect(x,y,LCD_BLOCK_W,LCD_BLOCK_H,RGBColor[map[y+MAP_NOT_SHOW_NUM][x]]);      
    }

  for(y=0;y<BLOCK_H_W;y++)
    for(x=0;x<BLOCK_H_W;x++)
      if(myblock->data[y][x]!=0 && myblock->point.second-MAP_NOT_SHOW_NUM+y>=0){
	draw_rect(myblock->point.first+x,myblock->point.second-MAP_NOT_SHOW_NUM+y,LCD_BLOCK_W,LCD_BLOCK_H,RGBColor[myblock->data[y][x]]);
      }
}

void show_nextblock(int block_num)
{
  int x,y;
  for(y=0;y<BLOCK_H_W;y++)
    for(x=0;x<BLOCK_H_W;x++)
	{
	  draw_rect(x+NEXT_BLOCK_X,y+NEXT_BLOCK_Y,LCD_BLOCK_W,LCD_BLOCK_H,RGBColor[block_data[block_num][y][x]]);      
	}
}

void show_score(int score)
{
  //attrset(COLOR_PAIR(0));
  //mvprintw(SCORE_Y,SCORE_X,"SCORE:%d",score);
  return ;
}

void game_pause()
{
  while(!bcm2835_gpio_lev(SW_g));
  usleep(500000);
  while(true)
    {
      usleep(100000);
      if(!bcm2835_gpio_lev(SW_b1))break;
      if(!bcm2835_gpio_lev(SW_b2))break;
      if(!bcm2835_gpio_lev(SW_w1))break;
      if(!bcm2835_gpio_lev(SW_w2))break;
      if(!bcm2835_gpio_lev(SW_g))
	{
	  //end
	  lcd->MySemop(false);
	  delete lcd;
	  exit(0);
	}
    }
  return;
}

int init()
{
  srand(time(NULL));//乱数列初期化
  return 0;
}

/*
  キー入力を反映する関数
  入力がなければfalse
*/
char key_input(block *myblock,char map[MAP_Y][MAP_X])
{
  char keyinput=0;
  if(!bcm2835_gpio_lev(SW_b1))keyinput='s';
  else if(!bcm2835_gpio_lev(SW_b2))keyinput='a';
  if(!bcm2835_gpio_lev(SW_w1))keyinput='w';
  else if(!bcm2835_gpio_lev(SW_w2))keyinput='z';
  if(!bcm2835_gpio_lev(SW_g))keyinput='p';

  if(keyinput)
    {
      switch(keyinput){
      case 's':
	myblock->move(RIGHT,map);break;
      case 'a':
	myblock->move(LEFT,map);break;
      case 'w':
	myblock->rotate(LEFT,map);break;
      case 'z':
	myblock->move(DOWN,map);break;
      case 'p':
	game_pause();break;
      default:
	return false;
      }
      return true;
    }
  return false;
}

void lock_myblock(block *myblock,char map[MAP_Y][MAP_X])
{
  int x,y;
  for(y=0;y<BLOCK_H_W;y++)
    for(x=0;x<BLOCK_H_W;x++)
      if(myblock->data[y][x]!=0)map[myblock->point.second+y][myblock->point.first+x]=myblock->data[y][x];
}

/*
  揃ったラインを消す関数
  return:消したライン数
*/
int delete_lines(char map[MAP_Y][MAP_X])
{
  int x,y,cnt=0;
  for(y=MAP_Y-2;y>=0;y--){
    bool flg=true;
    for(x=1;x<MAP_X-1;x++)
      if(map[y][x]==0)flg=false;
    if(flg)cnt++;
    else if(cnt)
      for(x=1;x<MAP_X-1;x++)
	map[y+cnt][x]=map[y][x];
  }
  return cnt;
}

void gameover(char map[MAP_Y][MAP_X])
{
  int x,y;
  for(y=0;y<MAP_Y;y++)
    for(x=0;x<MAP_X;x++)
      map[y][x]=1;
  show_map(map);
}

int play(int difficulty)
{
  int next_myblock_num=0;
  char map[MAP_Y][MAP_X];
  block myblock;
  int score=0;
 //マップリセット
  int x,y;
  for(y=0;y<MAP_Y;y++)
    for(x=0;x<MAP_X;x++)
      {
	if(x==0 || x== MAP_X-1)map[y][x]=1;
	else if(y==MAP_Y-1)map[y][x]=1;
	else map[y][x]=0;
      }

  while(true)
    {
      fill_buffer(Black);
      for(int i=0;i<WAITTIME-difficulty;i++)
	{
	  key_input(&myblock,map);
	  //タイム追加
	  usleep(SLEEPTIME);
	}
      if(myblock.move(DOWN,map)==false){
	//下に落とせなかったら固定
	lock_myblock(&myblock,map);
	//上端で落とせなかったら終わり
	if(myblock.point.second<=MAP_NOT_SHOW_NUM){
	  gameover(map);
	  return 0;
	}
	myblock.reset(next_myblock_num);
	next_myblock_num=rand()%BLOCK_NUM;
      }
      score+=delete_lines(map)*10;

      show_map(&myblock,map);

      show_nextblock(next_myblock_num);
      show_score(score);
      put_window();
    }
}




int main()
{
  bcm2835_init();
  lcd=new Glcd;


  bcm2835_gpio_fsel(SW_g, BCM2835_GPIO_FSEL_INPT);
  bcm2835_gpio_fsel(SW_w1, BCM2835_GPIO_FSEL_INPT);
  bcm2835_gpio_fsel(SW_w2, BCM2835_GPIO_FSEL_INPT);
  bcm2835_gpio_fsel(SW_b1, BCM2835_GPIO_FSEL_INPT);
  bcm2835_gpio_fsel(SW_b2, BCM2835_GPIO_FSEL_INPT);
  bcm2835_gpio_set_pud(SW_g, BCM2835_GPIO_PUD_UP);
  bcm2835_gpio_set_pud(SW_w1, BCM2835_GPIO_PUD_UP);
  bcm2835_gpio_set_pud(SW_w2, BCM2835_GPIO_PUD_UP);
  bcm2835_gpio_set_pud(SW_b1, BCM2835_GPIO_PUD_UP);
  bcm2835_gpio_set_pud(SW_b2, BCM2835_GPIO_PUD_UP);

  lcd->MySemop(true);
  init();
  play(0);
  lcd->MySemop(false);
  delete lcd;
  return 0;
}
