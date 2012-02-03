// This will be the main driving program for the behavioral simulator

#include <iostream>
#include <curses.h>
#include <signal.h>
#include <locale.h>
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <sstream>
#include "Agent.h"
using namespace std;

static void finish(int sig);
static void bufferPrint( Agent* agents );
static void editCell( int y , int x , char ch , int color );
static void startCells( Agent* agents , int length );
static void editCell( int y , int x , int newY , int newX );
static void randomizeStartingPositions( Agent* agents , int length );
static void moveRandomly( Agent* agents , int length );
static void initSimpleIdeas( Agent* agents , int length , int numIdeas );
static void exchangeIdeas( Agent* agents , int length );
static void tick();
static void finish(int sig);
static void resetTalked( Agent* agents , int length );
struct cell
{
	std::string ch;
	int color;
	bool occupied;
};
cell buffer[25][80];
int bufferNew[25][80];

int main(int argc , char** argv)
{
	const int MAX_IDEAS = 6;
	int agntSize = 125;
	int numIdeas = 1;
	int ticksTillPause = 200;
	int msTicks = 0;
	vector<string> cmd;
	// If we have command line arguments
	if ( argc > 1 )
	{
		for ( int i = 1; i < argc; i++ )
		{
			string tmp( argv[i] );
			cmd.push_back( tmp );
		}
	}
	for ( unsigned int i = 0; i < cmd.size(); i += 2 )
	{
		if ( cmd.at(i).compare( "agents" ) == 0 )
		{
			if ( i + 1 < cmd.size() )
			{
				stringstream ss;
				int agentCmd;
				ss.str( cmd.at( i+1 ) );
				ss >> agentCmd;
				agntSize = agentCmd;
				if ( agentCmd < 1 )
				{
					cerr << "ERR: There must be at least one agent." << endl;
					return 0;
				}
				if ( ss.fail() )
				{
					cerr << "Argument \"" << cmd.at(i) << "\" found malformed "
						<< "integer: " << cmd.at(i+1) << endl;
					return 0;
				}
			}
			else
			{
				cerr << "Argument \"" << cmd.at(i) << "\" expects integer." <<
					endl;
				return 0;
			}
		}
		else if ( cmd.at(i).compare( "ideas" ) == 0 )
		{
			if ( i + 1 < cmd.size() )
			{
				stringstream ss;
				int ideaCmd;
				ss.str( cmd.at( i+1 ) );
				ss >> ideaCmd;
				if ( ideaCmd > MAX_IDEAS )
				{
					cerr << "ERR: Requested number of ideas, " << ideaCmd << 
						" is greater than the max number of ideas ever\n" <<
						"thought: " << MAX_IDEAS << endl;
					return 0;
				}
				else if ( ideaCmd < 1 )
				{
					cerr << "ERR: Agents must have at least one idea." << endl;
					return 0;
				}
				numIdeas = ideaCmd;
				if ( ss.fail() )
				{
					cerr << "Argument \"" << cmd.at(i) << "\" found malformed "
						<< "integer: " << cmd.at(i+1) << endl;
					return 0;
				}
			}
			else
			{
				cerr << "Argument \"" << cmd.at(i) << "\" expects integer." <<
					endl;
				return 0;
			}
		}
		else if ( cmd.at(i).compare( "ticks" ) == 0 )
		{
			if ( i + 1 < cmd.size() )
			{
				stringstream ss;
				int ticksCmd;
				ss.str( cmd.at( i+1 ) );
				ss >> ticksCmd;
				ticksTillPause = ticksCmd;
				if ( ticksCmd < 1 )
				{
					cerr << "ERR: There must be at least one frame of " <<
						"simulation: " << cmd.at(i+1) << " chosen frames." << endl;
					return 0;
				}
				if ( ss.fail() )
				{
					cerr << "Argument \"" << cmd.at(i) << "\" found malformed "
						<< "integer: " << cmd.at(i+1) << endl;
					return 0;
				}
			}
			else
			{
				cerr << "Argument \"" << cmd.at(i) << "\" expects integer." <<
					endl;
				return 0;
			}
		}
		else if ( cmd.at(i).compare( "msTicks" ) == 0 )
		{
			if ( i + 1 < cmd.size() )
			{
				stringstream ss;
				int msticksCmd;
				ss.str( cmd.at( i+1 ) );
				ss >> msticksCmd;
				msTicks = msticksCmd;
				if ( msticksCmd < 0 )
				{
					cerr << "ERR: Negative time is silly." << endl;
					return 0;
				}
				if ( ss.fail() )
				{
					cerr << "Argument \"" << cmd.at(i) << "\" found malformed "
						<< "integer: " << cmd.at(i+1) << endl;
					return 0;
				}
			}
			else
			{
				cerr << "Argument \"" << cmd.at(i) << "\" expects integer." <<
					endl;
				return 0;
			}
		}
		else
		{
			cerr << "ERR: Malformed command line argument: " << cmd.at(i) << 
				endl;
			return 0;
		}
	}
	
	
	Agent agents[agntSize];
	randomizeStartingPositions( agents , agntSize );
	startCells( agents , agntSize );
	initSimpleIdeas( agents , agntSize , numIdeas );
	
	
	/*
	for ( int i = 0; i < agntSize; i++ )
	{
		agents[i] = new Agent();
		std::cout << agents[i]->getColor() << "\n";
		std::cout << agents[i]->getRep().c_str() << "\n";
	}
	*/
	
	
	
	// Allow unicode characters
	setlocale(LC_ALL,"");
    (void)signal(SIGINT, finish);      /* arrange interrupts to terminate */
	(void)initscr();      /* initialize the curses library */
    keypad(stdscr, TRUE);  /* enable keyboard mapping */
    (void) nonl();         /* tell curses not to do NL->CR/NL on output */
    (void) cbreak();       /* take input chars one at a time, no wait for \n */
    (void) noecho();		// Key presses don't echo
    use_default_colors();
	if (has_colors())
    {
        start_color();
		 /*
         * Simple color assignment, often all we need.  Color pair 0 cannot
		 * be redefined.  This example uses the same value for the color
		 * pair as for the foreground color, though of course that is not
		 * necessary:
         */
        init_pair(1, COLOR_RED,     -1); // -1 is default background
        init_pair(2, COLOR_GREEN,   -1);
        init_pair(3, COLOR_YELLOW,  -1);
        init_pair(4, COLOR_BLUE,    -1);
        init_pair(5, COLOR_CYAN,    -1);
        init_pair(6, COLOR_BLACK,   -1);
        init_pair(7, COLOR_WHITE,   -1);
    }
    attron(A_BOLD);
	int col = 0;
	int row = 0;
	for ( int i = 0; i < agntSize; i++ )
	{
		move( ( ( col == 0 ) ? ( ( ++row > 23 ) ? row = 0 : row ) : row ) , 
			( ( ++col > 79 ) ? col = 0 : col ) );
		//attrset( agents[i]->getColor() );
		attrset( COLOR_PAIR( agents[i].getColor() ) );
		attron(A_BOLD);
		addstr( agents[i].getRep().c_str() );
		attroff(A_BOLD);
	}
	int ticks = 0;
	for (;;)
    {
    	int c = '\0';
    	// Get next keystroke
        if ( ++ticks % ticksTillPause == 0 )
        {
        	c = getch();
        }
		if ( c == KEY_END )
		{
			break;
		}
		else if ( c == KEY_UP )
		{
		}
		bufferPrint(agents);
		moveRandomly( agents , agntSize );
		exchangeIdeas( agents , agntSize );
		refresh();
		if ( msTicks != 0 )
    	{
    		clock_t then = clock() + msTicks * 1000;
  			while ( then > clock() )
  			{
  			}
  		}
  		resetTalked( agents , agntSize );
	}
	finish(0);
	int ideaAverages[agents[0].numIdeas];
	for ( int i = 0; i < agents[0].numIdeas; i++ )
	{
		ideaAverages[i] = 0;
	}
	for ( int i = 0; i < agntSize; i++ )
	{
		cout << "Agent: " << i << endl;
		for ( int j = 0; j < agents[i].numIdeas; j++ )
		{
			cout << "\tID: " << agents[i].ideas[j].getID();
			cout << "\tIdea: \"" << agents[i].ideas[j].getName() << "\""<< endl;
			cout << "\tBelief Amount (0 - 100): " << 
				agents[i].ideas[j].getBeliefVal();
			cout << endl;
			if ( j < agents[i].numIdeas - 1 )
			{
				cout << "\t----------------------------------------" << endl;
			}
			ideaAverages[j] += agents[i].ideas[j].getBeliefVal();
		}
	}
	cout << "\n----------------------------------------" <<
		"----------------------------------------" <<
		"----------------------------------------" <<
		"----------------------------------------\n" << endl;
	for ( int i = 0; i < agents[0].numIdeas; i++ )
	{
		cout << "Idea ID " << i << ", \"" << agents[0].ideas[i].getName() <<
			"\", has an average belief of: " << ( ideaAverages[i] / agntSize ) <<
			endl;
	}
	cout << "\nEND OF SIMULATION\n" << endl;
	
	return 0;
}

static void initSimpleIdeas( Agent* agents , int length , int numIdeas )
{
	for ( int i = 0; i < length; i++ )
	{
		agents[i].initIdeas(numIdeas);
	}
}

static void bufferPrint( Agent* agents )
{
	for ( int i = 0; i < 25; i++ )
	{
		for ( int j = 0; j < 80; j++ )
		{
			if ( bufferNew[i][j] == -1 )
			{
				move( i , j );
				attrset( COLOR_PAIR( 6 ) );
				addstr( " " );
			}
			else
			{
				move( i , j );
				attrset( COLOR_PAIR( agents[ bufferNew[i][j] ].getColor() ) );
				attron(A_BOLD);
				addstr( agents[ bufferNew[i][j] ].getRep().c_str() );
				attroff(A_BOLD);
			}
		}
	}
	return;
}

static void editCell( int y , int x , std::string ch , int color , bool occ )
{
	buffer[y][x] = { ch , color , occ };
	return;
}

static void startCells( Agent* agents , int length )
{
	for ( int i = 0; i < 25; i++ )
	{
		for ( int j = 0; j < 80; j++ )
		{
			bufferNew[i][j] = -1;
		}
	}
	for ( int i = 0; i < length; i++ )
	{
		bufferNew[agents[i].getY()][agents[i].getX()] = i;
	}
	return;
}

static void editCell( int y , int x , int newY , int newX )
{
	bufferNew[newY][newX] = bufferNew[y][x];
	bufferNew[y][x] = -1;
	return;
}

static void randomizeStartingPositions( Agent* agents, int length )
{
	srand( time(0) );
	for ( int i = 0; i < length; i++ )
	{
		for ( int j = 0; j < length; j++ )
		{
			if ( i == j )
			{
				continue;
			}
			while ( agents[i].getX() == agents[j].getX() &&
				agents[i].getY() == agents[j].getY() )
			{
				agents[j].setX( rand() % 80 );
				agents[j].setY( rand() % 25 );
			}
		}
	}
	return;
}

static void moveRandomly( Agent* agents, int length )
{
	int newX = 0;
	int newY = 0;
	for ( int i = 0; i < length; i++ )
	{
		do {
			newX = ( rand() % 3 ) - 1;
			newY = ( rand() % 3 ) - 1;
		}
		while( ( agents[i].getY() + newY < 0 || agents[i].getY() + newY > 24 ||
			agents[i].getX() + newX < 0 || agents[i].getX() + newX > 79 ) ||
			//( newY == 0 && newX == 0 ) ||
			bufferNew[agents[i].getY() + newY][agents[i].getX() + newX] != -1 );
		editCell( agents[i].getY() , agents[i].getX() , 
			agents[i].getY() + newY , agents[i].getX() + newX );
		//cout << "Agent number: " << i << endl;
		//cout << "\tCurrent: (" << agents[i].getX() << " , " << agents[i].getY() << ")" << endl;
		//cout << "\tNext: (" << newX << " , " << newY << ")" << endl;
		agents[i].setY( agents[i].getY() + newY );
		agents[i].setX( agents[i].getX() + newX );
	}
	return;
}

static void exchangeIdeas( Agent* agents , int length )
{
	for ( int i = 0; i < length; i++ )
	{
		for ( int j = -1; j < 2; j++ )
		{
			for ( int k = -1; k < 2; k++)
			{
				if ( j == 0 && k == 0 )
				{
					continue;
				}
				if ( agents[i].getY() + j > -1 && agents[i].getY() + j < 25 &&
					agents[i].getX() + k > -1 && agents[i].getX() + k < 80 && 
					bufferNew[agents[i].getY() + j][agents[i].getX() + k] != -1)
				{
					//cout << "Y: " << agents[i].getY() + j << " ";
					//cout << "X: " << agents[i].getX() + k << endl;
					agents[i].simplestExchange( //agents[i] );
						agents[
						bufferNew[agents[i].getY() + j][agents[i].getX() + k]
						] );
					break;
				}
			}
		}
	}
}

static void resetTalked( Agent* agents , int length )
{
	for ( int i = 0; i < length; i++ )
	{
		agents[i].resetTalked();
	}
	return;
}

static void finish(int sig)
{
    endwin();
	return;
}
