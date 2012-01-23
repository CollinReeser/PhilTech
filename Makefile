all:	main

main:	main.cpp Agent.cpp Agent.h Idea.cpp Idea.h FrameBuffer.cpp\
		FrameBuffer.h
	g++ -o simulator main.cpp Agent.cpp Idea.cpp FrameBuffer.cpp -lncursesw \
	-std=gnu++0x -O1
	
#main.o:	main.cpp
#	g++ -c main.cpp
	
#Agent:	Agent.o Idea

#Agent.o:	Agent.cpp Agent.h
#	g++ -c Agent.cpp
	
#Idea:	Idea.o

#Idea.o:	Idea.cpp Idea.h
#	g++ -c Idea.cpp

clean:
	rm simulator
