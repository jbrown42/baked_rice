default: dcas

dcas: main.o Drone.o World.o
	g++ -o dcas -static-libstdc++ main.o Drone.o World.o -lpthread

main.o: src/main.cpp include/Drone.h include/World.h
	g++ -I./include -static-libstdc++ -c src/main.cpp 

Drone.o: src/Drone.cpp include/Drone.h include/World.h
	g++ -I./include -static-libstdc++ -c src/Drone.cpp

World.o: src/World.cpp include/World.h
	g++ -I./include -static-libstdc++ -c src/World.cpp

clean:
	rm dcas main.o Drone.o World.o
