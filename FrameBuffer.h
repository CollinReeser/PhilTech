#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include <curses.h>
#include <signal.h>
#include <locale.h>
#include <sstream>
#include <string>

class GUI
{
public:
	void initGUI();
	void finish();
	void editCell( int y , int x , char ch , int color );
	void editCell( int y , int x , char ch );
private:
	//char[][] buffer;
};

#endif
