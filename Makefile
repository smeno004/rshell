output: main.o
	g++ main.cpp -Wall -Werror -ansi -pedantic
	
main.o: main.cpp
	g++ -c main.cpp -Wall -Werror -ansi -pedantic