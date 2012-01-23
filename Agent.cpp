

#include "Agent.h"
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// This is the basic constructor for an agent, leading to a purely random
// agent. Use another constructor for deliberately initializing values
Agent::Agent()
{
	if ( !randChecker )
	{
		srand( time(0) );
		randChecker = true;
	}
	int rng = rand() % 100;
	// Compiler will reduce these constants down so that math does't occur in
	// runtime, but the expressions are kept here for clarity
	// 1 is red
	if ( rng < ( 100 - 6 * ( 100 / 7 ) ) )
	{
		color = 1;
	}
	// 2 is green
	else if ( rng < ( 100 - 5 * ( 100 / 7 ) ) )
	{
		color = 2;
	}
	// 3 is yellow
	else if ( rng < ( 100 - 4 * ( 100 / 7 ) ) )
	{
		color = 3;
	}
	// 4 is blue
	else if ( rng < ( 100 - 3 * ( 100 / 7 ) ) )
	{
		color = 4;
	}
	// 5 is cyan
	else if ( rng < ( 100 - 2 * ( 100 / 7 ) ) )
	{
		color = 5;
	}
	// Color constant 6 is black, and my personal terminal has a very dark
	// background. 7 is white.
	else
	{
		color = 7;
	}
	rng = rand() % 100;
	if ( rng < 90 )
	{
		rep = "\u263A";
	}
	else
	{
		rep = "\u2639";
	}
    randomizeAgent();
}

// This is the initializer for a random population. Randomly chooses a
// charisma and intelligence and maximum talking points value, as well as
// the talking points recharge factor.
// Also initializes a random number of ideas to a random value. Use this
// fucntion for a purely random agent
void Agent::randomizeAgent()
{
    return;
}
// This is a one sided exchange, where this agent spends points and does
// checks to influence the agent he is speaking with
void Agent::oneSidedConvo( Agent other )
{
    return;
}
// Might not use this, but here for completeness of idea, this is as above
// except it is a two sided conversation. One agent is the starter, but then
// after this agent does their check, it forces a call to the other agents
// one sided convo function, to emulate a two sided conversation
void Agent::twoSidedConvo( Agent other )
{
    return;
}
// GetColor
int Agent::getColor()
{
	return color;
}

// Get string representation
std::string Agent::getRep()
{
	return rep;
}
