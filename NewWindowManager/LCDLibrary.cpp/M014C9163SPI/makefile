.PHONY:all
all:LibraryTest

LibraryTest:LibraryTest.o M014C9163SPI.o
	g++ -Wall -o LibraryTest LibraryTest.o M014C9163SPI.o -lbcm2835

LibraryTest.o : LibraryTest.cpp
	g++ -Wall -c LibraryTest.cpp  -lbcm2835

M014C9163SPI.o : M014C9163SPI.cpp
	g++ -Wall -c M014C9163SPI.cpp -lbcm2835

LibraryTest.o:M014C9163SPI.h
M014C9163SPI.o:M014C9163SPI.h

.PHONY:clean
clean:
	rm -f *.o
