.PHONY:all
all:WindowManager

WindowManager:main.o gl_lcd.o gl_lcd_data.o flamebuffer.o
	gcc -o WindowManager main.o gl_lcd.o gl_lcd_data.o flamebuffer.o -lbcm2835 -lm
#`pkg-config --cflags --libs opencv`

main.o : main.c
	gcc -c main.c  -lbcm2835 -lm
#`pkg-config --cflags --libs opencv` 

gl_lcd.o : gl_lcd.c
	gcc -c  gl_lcd.c  -lbcm2835 -lm

gl_lcd_data.o : gl_lcd_data.c
	gcc -c gl_lcd_data.c

flamebuffer.o:flamebuffer.c
	gcc -c flamebuffer.c

flamebuffer.o:flamebuffer.h

main.o:gl_lcd.h
gl_lcd_data.o:gl_lcd_data.h
gl_lcd.o : gl_lcd.h

gl_lcd.h : gl_lcd_data.h

.PHONY:clean
clean:
	rm -f gl_lcd.o gl_lcd_data.o main.o flamebuffer.o