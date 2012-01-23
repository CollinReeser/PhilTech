// This will be the main driving program for the behavioral simulator

#include <iostream>
#include <curses.h>
#include <signal.h>
#include <locale.h>
#include <sstream>
#include <string>
#include "Agent.h"

static void finish(int sig);
static void bufferPrint();
static void editCell( int y , int x , char ch , int color );
static void editCell( int y , int x , char ch );
static void finish(int sig);
struct cell
{
	std::string ch;
	int color;
	bool occupied;
};
cell buffer[25][80];

int main(int argc , char** argv)
{
	Agent* agents[150];
	int agntSize = 150;
	std::cout << "Hello, world!\n";
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
		attrset( COLOR_PAIR( agents[i]->getColor() ) );
		attron(A_BOLD);
		addstr( agents[i]->getRep().c_str() );
		attroff(A_BOLD);
	}
	for (;;)
    {
    	// Get next keystroke
        
        int c = getch();
		if ( c == KEY_END )
		{
			break;
		}
		else if ( c == KEY_UP )
		{
		}
		bufferPrint();
		refresh();
	}
	finish(0);
	
	
	
	return 0;
}

static void bufferPrint()
{
	for ( int i = 0; i < 25; i++ )
	{
		for ( int j = 0; j < 80; j++ )
		{
			move( i , j );
			attrset( COLOR_PAIR( buffer[i][j].color ) );
			addstr( buffer[i][j].ch.c_str() );
		}
	}
	return;
}

static void editCell( int y , int x , std::string ch , int color , bool occ )
{
	buffer[y][x] = { ch , color , occ };
	return;
}

static void editCell( int y , int x , std::string ch )
{
	return;
}

static void finish(int sig)
{
    endwin();
	return;
}
