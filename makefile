all:run

run:main.o cbc.o Des.o DESmath.o key.o
	g++ -std=c++11 main.o key.o Des.o DESmath.o cbc.o -o run -lcrypto

main.o:main.cpp
	g++ -std=c++11 -c main.cpp -lssl  -lcrypto

cbc.o:cbc.cpp cbc.h
	g++ -std=c++11 -c cbc.cpp 

Des.o:Des.cpp Des.h
	g++ -std=c++11 -c Des.cpp

key.o:key.cpp key.h
	g++ -std=c++11 -c key.cpp


DESmath.o:DESmath.cpp DESmath.h
	g++ -std=c++11 -c DESmath.cpp

clean:
	rm -rf *o program2
