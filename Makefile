krayon:
	g++ -Iinclude include/*.cpp src/scene/*.cpp src/*.cpp  -o krayon

clean:
	rm -f *.o krayon
