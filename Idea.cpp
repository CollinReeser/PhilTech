
#include <string>
#include "Idea.h"

Idea::Idea()
{
}

void Idea::setName( std::string name )
{
	Idea::name = name;
	return;
}

void Idea::setID( int ID )
{
	identifier = ID;
	return;
}

void Idea::setBeliefVal( int val )
{
	belief = val;
	return;
}

std::string Idea::getName()
{
	return name;
}

int Idea::getID()
{
	return identifier;
}

int Idea::getBeliefVal()
{
	return belief;
}
