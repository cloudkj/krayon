krayon: inc.o
	g++ -Iinclude src/scene/*.cpp src/*.cpp  -o krayon inc.o

inc.o:
	g++ -c -Iinclude include/*.cpp -o inc.o

clean:
	rm -f *.o krayon
