.PHONY:all
all:FBapp.out

FBapp.out:FBapp.o Glcd.o M014C9163SPI.o framebuffer.o
	g++ -Wall -o FBapp.out FBapp.o Glcd.o M014C9163SPI.o framebuffer.o -lbcm2835

FBapp.o : FBapp.cpp
	g++ -Wall -c FBapp.cpp  -lbcm2835

Glcd.o : Glcd.cpp
	g++ -Wall -c Glcd.cpp -lbcm2835

M014C9163SPI.o : M014C9163SPI/M014C9163SPI.cpp
	g++ -Wall -c M014C9163SPI/M014C9163SPI.cpp -lbcm2835

framebuffer.o : FrameBuffer/framebuffer.cpp
	g++ -Wall -c FrameBuffer/framebuffer.cpp

FBapp.o : Glcd.h

Glcd.o : M014C9163SPI/M014C9163SPI.h

M014C9163SPI.o:M014C9163SPI/M014C9163SPI.h

framebuffer.o:FrameBuffer/framebuffer.h

.PHONY:clean
clean:
	rm -f *.o
