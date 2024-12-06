# Please implement your Makefile rules and targets below.
# Customize this file to define how to build your project.
all: clean  link 

link: compile
	g++ -o bin/simulation bin/main.o bin/Simulation.o bin/Action.o bin/Plan.o bin/SelectionPolicy.o bin/Facility.o bin/Settlement.o bin/Auxiliary.o

compile: src/Auxiliary.cpp src/Settlement.cpp src/Facility.cpp src/SelectionPolicy.cpp src/Plan.cpp src/Action.cpp src/Simulation.cpp src/main.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/main.o src/main.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Action.o src/Action.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Plan.o src/Plan.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/SelectionPolicy.o src/SelectionPolicy.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Simulation.o src/Simulation.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Facility.o src/Facility.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Settlement.o src/Settlement.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Auxiliary.o src/Auxiliary.cpp

clean:
	rm -rf bin/*

c: all
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./bin/simulation config_file.txt