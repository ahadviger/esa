PROJECT = esa
SOURCE = main.cpp esa.cpp childtable.hpp lcpinterval.hpp

all: main

main: 
	g++ main.cpp -o main

clean:
	-rm -f $(PROJECT)