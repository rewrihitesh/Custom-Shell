all:shell


shell: main.o functions.o  pipecmd.o export.o alias.o alarm.o
	g++ main.o functions.o pipecmd.o export.o alias.o alarm.o -o shell

main.o: main.cpp 
	g++ -c main.cpp 

functions.o: functions.cpp
	g++ -c functions.cpp

pipecmd.o: pipecmd.cpp
	g++ -c pipecmd.cpp

export.o: export.cpp
	g++ -c export.cpp

alias.o: alias.cpp
	g++ -c alias.cpp

alarm.o: alarm.cpp
	g++ -c alarm.cpp

rm:
	rm  main.o functions.o pipecmd.o export.o alias.o alarm.o
