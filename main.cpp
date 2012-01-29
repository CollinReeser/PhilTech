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
static void tick();
static void finish(int sig);
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
	const int agntSize = 125;
	Agent agents[agntSize];
	randomizeStartingPositions( agents , agntSize );
	startCells( agents , agntSize );
	int numIdeas = 6;
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
    	int c;
    	// Get next keystroke
        if ( ++ticks == 250 )
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
		refresh();
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
			cout << "\tBelief Amount (-100 - 100): " << 
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
			"\", has average belief of: " << ( ideaAverages[i] / agntSize ) <<
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
	for ( int i = 0; i < lengthl i++ )
	{
		
	}
}

static void finish(int sig)
{
    endwin();
	return;
}
