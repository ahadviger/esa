CC = g++

all: esa ipm

esa: main.cpp esa.cpp
	$(CC) main.cpp esa.cpp -o esa_main

ipm: ipm_example.cpp esa.cpp ipm.cpp
	$(CC) esa.cpp ipm.cpp ipm_example.cpp -o ipm_main

clean:
	rm ipm_main esa_main
