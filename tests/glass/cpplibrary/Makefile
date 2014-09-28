.PHONY:all
all:WindowManager

WindowManager:main.o framebuffer.o gl_lcd.o gl_lcd_data.o 
	g++ -Wall -o WindowManager main.o framebuffer.o gl_lcd.o gl_lcd_data.o -lbcm2835 -lm
#`pkg-config --cflags --libs opencv`

main.o : main.cpp
	g++ -Wall -c main.cpp  -lbcm2835 -lm
#`pkg-config --cflags --libs opencv` 

gl_lcd.o : gl_lcd.cpp
	g++ -Wall -c  gl_lcd.cpp  -lbcm2835 -lm

gl_lcd_data.o : gl_lcd_data.c
	g++ -Wall -c gl_lcd_data.c

framebuffer.o:framebuffer.cpp
	g++ -Wall -c framebuffer.cpp

framebuffer.o:framebuffer.h

main.o:gl_lcd.h
gl_lcd_data.o:gl_lcd_data.h
gl_lcd.o : gl_lcd.h

gl_lcd.h : gl_lcd_data.h

.PHONY:clean
clean:
	rm -f gl_lcd.o gl_lcd_data.o main.o framebuffer.o