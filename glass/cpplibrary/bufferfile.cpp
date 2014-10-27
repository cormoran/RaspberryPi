  bool M014C9163SPI::ShareMemInit(char AppMode){
    key_t key = ftok("/home/pi/Programs/RaspberryPi/tests/glass/ShareMemoryKeyFile",1);
    char *adr;

    switch(AppMode){
    case 1://Mode.Master:
      id = shmget(key, 8, IPC_CREAT|IPC_EXCL|0666);//8byteŠm•ÛAŠù‚É‚ ‚Á‚½‚ç¸”s
      if(id == -1){perror("shmget error");return 1;}
      adr = (char *)shmat(id, NULL, 0);
      if(adr == (void *)-1){
	perror("shmat error");
	if(shmctl(id, IPC_RMID, 0)==-1)
	  perror("shmctl error");
	return 1;
      }
      memset(adr,0,8);//ƒƒ‚ƒŠ‰Šú‰»
      ShareMemoryAddress=adr;
      MyWindowID=0;

      break;
    case 0://Mode.Slave:
      id = shmget(key,8,0);
      if(id == -1){perror("shmget error");return 1;}
      adr = (char *)shmat(id,NULL,0);
      if(adr == (void *)-1){
	perror("shmat error");
	if(shmctl(id, IPC_RMID, 0)==-1)
	  perror("shmctl error");
	return 1;
      }
      ShareMemoryAddress=adr;
      *adr+=1;//slave‚É‚Â‚«sharemem‚Ì0”Ô’n‚ª1up‚·‚é
      MyWindowID=*adr;

      break;
    default:
      printf("Error:invailed number");
      return 1;
    }
    return 0;
  }




  /*’¼ü•`ÊŠÖ”
   * (xs,ys)‚©‚ç(xe,ye)‚Ü‚Å‘¾‚³w‚ÅFcolor‚Ì’¼ü‚ğˆø‚­
   */
  void M014C9163SPI::Draw_line(uchar xs,uchar ys,uchar xe,uchar ye, uchar w,uchar *rgb)
  {
    //Bresenham‚Å’¼ü•`Ê
    uchar dx,dy;
    char sx,sy;
    int slope;
    if(xs<xe){
      sx=1;
      dx=xe-xs;
    }
    else{
      sx=-1;
      dx=xs-xe;
    }
    if(ys<ye){
      sy=1;
      dy=ye-ys;
    }
    else{
      sy=-1;
      dy=ys-ye;
    }
    slope=dx-dy;

    while(1){
      Draw_rectangle(xs,ys,w,w,rgb);
      if(xs==xe && ys==ye)break;
      if(slope*2>-dy){
	slope-=dy;
	xs+=sx;
      }
      if(slope*2<dx){
	slope+=dx;
	ys+=sy;
      }
    }

    return;
  }

  void M014C9163SPI::Draw_cycle(uchar x0,uchar y0,uchar r,uchar w,uchar *rgb)
  {
    uchar x = r;
    uchar y = 0;
    int F = -2 * r + 3;
    while ( x >= y ) {
      Draw_rectangle( x0 + x, y0 + y,w,w, rgb );
      Draw_rectangle( x0 - x, y0 + y,w,w, rgb );
      Draw_rectangle( x0 + x, y0 - y,w,w, rgb );
      Draw_rectangle( x0 - x, y0 - y,w,w, rgb );
      Draw_rectangle( x0 + y, y0 + x,w,w, rgb );
      Draw_rectangle( x0 - y, y0 + x,w,w, rgb );
      Draw_rectangle( x0 + y, y0 - x,w,w, rgb );
      Draw_rectangle( x0 - y, y0 - x,w,w, rgb );
      if(F>=0){
	x--;
	F -= 4 * x;
      }
      y++;
      F += 4 * y + 2;
    }
  }

  void M014C9163SPI::Draw_eclipse(uchar x0,uchar y0,uchar r,uchar a,uchar b,uchar w, uchar *rgb)
  {
    uchar x = (int)( (double)r / sqrt( (double)a ) );
    int y = 0;
    double d = sqrt( (double)a ) * (double)r;
    int F = (int)( -2.0 * d ) +     a + 2 * b;
    int H = (int)( -4.0 * d ) + 2 * a     + b;

    while (1) {
      Draw_rectangle( x0 + x, y0 + y,w,w, rgb );
      Draw_rectangle( x0 - x, y0 + y,w,w, rgb );
      Draw_rectangle( x0 + x, y0 - y,w,w, rgb );
      Draw_rectangle( x0 - x, y0 - y,w,w, rgb );

      if(!x)break;

      if ( F >= 0 ) {
	--x;
	F -= 4 * a * x;
	H -= 4 * a * x - 2 * a;
      }
      if ( H < 0 ) {
	++y;
	F += 4 * b * y + 2 * b;
	H += 4 * b * y;
      }
    }
  }
  

  void M014C9163SPI::Draw_chara(uchar x,uchar y,char chara,uchar *fontrgb, uchar *backrgb)
  {
    char charanum=chara-' ';
    char i,j;
    SET_PAGE_ADDRESS(y,y+5-1);
    SET_COLUMN_ADDRESS(x,x+8-1);
    WRITE_MEMORY_START();
    for(i=0;i<5;i++)
      {
	unsigned char fline=Font[charanum][i];
	for(j=0;j<8;j++)
	  {
	    if(fline & 0x01){
	      Sendbyte(1, fontrgb[0]);
	      Sendbyte(1, fontrgb[1]);
	      Sendbyte(1, fontrgb[2]);
	    }
	    else{
	      Sendbyte(1, backrgb[0]);
	      Sendbyte(1, backrgb[1]);
	      Sendbyte(1, backrgb[2]);
	    }
	    fline=fline>>1;
	  }
      }
  }

  void M014C9163SPI::Draw_string(uchar x,uchar y,char *str,uchar *fontrgb,uchar *backrgb)
  {
    while(*str){
      Draw_chara(x,y,*str++,fontrgb,backrgb);
      y+=6;
    }
  }
