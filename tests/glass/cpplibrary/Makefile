.PHONY:all
all:glass

glass:main.o gl_lcd.o gl_lcd_data.o
	gcc -o glass main.o gl_lcd.o gl_lcd_data.o `pkg-config --cflags --libs opencv` -lbcm2835 -lm

main.o : main.c
	gcc `pkg-config --cflags --libs opencv` -c glass main.c  -lbcm2835 -lm

gl_lcd.o : gl_lcd.c
	gcc -c  gl_lcd.c  -lbcm2835 -lm

gl_lcd_data.o : gl_lcd_data.c
	gcc -c gl_lcd_data.c

main.o:gl_lcd.h
gl_lcd_data.o:gl_lcd_data.h
gl_lcd.o : gl_lcd.h

gl_lcd.h : gl_lcd_data.h

.PHONY:clean
clean:
	rm -f gl_lcd.o gl_lcd_data.o main.o