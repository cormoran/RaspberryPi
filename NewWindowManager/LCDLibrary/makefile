.PHONY:all
all:Glcd_Test.out

Glcd_Test.out:Glcd_Test.o Glcd.o M014C9163SPI.o
	g++ -Wall -o Glcd_Test.out Glcd_Test.o Glcd.o M014C9163SPI.o -lbcm2835

Glcd_Test.o : Glcd_Test.cpp
	g++ -Wall -c Glcd_Test.cpp  -lbcm2835

Glcd.o : Glcd.cpp
	g++ -Wall -c Glcd.cpp -lbcm2835

M014C9163SPI.o : M014C9163SPI/M014C9163SPI.cpp
	g++ -Wall -c M014C9163SPI/M014C9163SPI.cpp -lbcm2835

Glcd_Test.o : Glcd.h

Glcd.o : M014C9163SPI/M014C9163SPI.h

M014C9163SPI.o:M014C9163SPI/M014C9163SPI.h

.PHONY:clean
clean:
	rm -f *.o
