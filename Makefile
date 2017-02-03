all:
	g++ -std=c99 -fopenmp -olab2 lab2.cpp

run:
	./lab2

clean:
	rm lab2