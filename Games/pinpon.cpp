#include <ncurses.h>
#include <unistd.h>
#include<cstdio>
#include<cstdlib>
#include<ctime>
#include <utility>
#include <string>
using namespace std;

#define TIMEOUT 10 //キーボード入力タイムアウト ms
#define WAITTIME 20 //TIMEOUT * 1ms

#define MAP_W 128
#define MAP_H 128


void show_map(char map[MAP_Y][MAP_X])
{
  int x,y;
  for(y=0;y<MAP_Y-MAP_NOT_SHOW_NUM;y++)
    for(x=0;x<MAP_X;x++){
      attrset(COLOR_PAIR(map[y+MAP_NOT_SHOW_NUM][x]));
      mvaddstr(y,x," ");
    }
}

void curses_init()
{
  initscr();//window初期化
  noecho();//キー入力を表示しない
  cbreak();//キー入力をすぐに受け付ける
  curs_set(0);//カーソル非表示
  
  start_color();//カラーの初期化？
  /*色ペアの設定　(番号,前景色,背景色)
    0は前白,後黒になってるらしい*/
  init_pair(1,COLOR_BLACK,COLOR_WHITE);
  init_pair(2, COLOR_BLACK, COLOR_RED);
  init_pair(3, COLOR_BLACK, COLOR_GREEN);
  init_pair(4,COLOR_BLACK,COLOR_BLUE);
  init_pair(5,COLOR_BLACK,COLOR_YELLOW);
  init_pair(6,COLOR_BLACK,COLOR_CYAN);
  init_pair(7,COLOR_BLACK,COLOR_MAGENTA);
  init_pair(8,COLOR_BLACK,COLOR_BLUE);
  
  bkgd(COLOR_PAIR(0));//ターミナルの背景黒、文字白
  wtimeout(stdscr,TIMEOUT);//getchのタイムアウト設定
  keypad(stdscr, TRUE); //矢印キー使用する
}  

int init()
{
  curses_init();
  srand(time(NULL));//乱数列初期化
  return 0;
}

/*
  キー入力を反映する関数
  入力がなければfalse
*/
bool key_input(block *myblock,char map[MAP_Y][MAP_X])
{
  int keyinput;
  keyinput=getch();
  switch(keyinput){
  case KEY_RIGHT:
    myblock->move(RIGHT,map);break;
  case KEY_LEFT:
    myblock->move(LEFT,map);break;
  case KEY_UP:
    myblock->rotate(LEFT,map);break;
  case KEY_DOWN:
    myblock->move(DOWN,map);break;
  case 'c':
    endwin();
    exit(0);
  default :return false;
  }
  return true;
}
int main()
{


  init();
  play(0);
  endwin();
}
