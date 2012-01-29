

#ifndef IDEA_H
#define IDEA_H

#include <string>

class Idea
{
public:
	// Constructor
	Idea();
	// Getters setters
	void setName( std::string name);
	void setID( int ID );
	void setBeliefVal( int val );
	std::string getName();
	int getID();
	int getBeliefVal();
private:
	// Idea's name
	std::string name;
	// Identifier number for the idea
	int identifier;
	// Value of belief the agent has in this idea, from -100 to 100
	int belief;	
};

#endif
