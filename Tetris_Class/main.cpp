#include "tetris.h"
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

#define WIDTH 10
#define HEIGHT 20
#define KEYBOARD_SPECIAL_CHARS 224
#define KEYBOARD_ARROW_DOWN 80
#define KEYBOARD_ARROW_RIGHT 77
#define KEYBOARD_ARROW_LEFT 75

using namespace std;
using namespace tetris;

// Create a tetris objects
Tetris trt;
InputPack ipack;
OutputPack* tetris_output = NULL;

void getOutput( OutputPack* opack )
{
	if ( tetris_output == NULL )
	{
		tetris_output = opack ;
	}

	// Clear screen
	if ( system("clear") )
	{
		system("cls");
	}
	
	// Print tetris
	printMatrix( opack->tetris , TT_NONE , ' ' );

	// Print time
	printf("Time : %f\n" , trt.getGameplayTime() );

	// Print erased lines
	if ( opack->didEraseLines )
	{
		printf("Deleted Rows : %d\n" , opack->numOfErasedLines );
	}
}

InputPack* sendInput( void ) 
{
	trt.externCodeTimeFactor = 1000;

	int user_input;

	ipack.rotate = ipack.move = false;

	if ( kbhit() )
	{
		user_input = getch();

		if ( ! trt.gamePaused() )
		{
			if ( user_input == KEYBOARD_SPECIAL_CHARS )
			{
				switch ( getch() )
				{
					case KEYBOARD_ARROW_RIGHT : { ipack.move = true; ipack.dir = TMD_RIGHT; break; }
					case KEYBOARD_ARROW_LEFT  : { ipack.move = true; ipack.dir = TMD_LEFT;  break; }
					case KEYBOARD_ARROW_DOWN  : { ipack.move = true; ipack.dir = TMD_DOWN;  break; }
				}
			}
			else if ( user_input == ' ' )
			{
				ipack.rotate = true;
				ipack.trnd = TRT_LEFT;
			}
		}

		if ( user_input == 'p' || user_input == 'P' )
		{
			if ( trt.gamePaused() )
			{
				ipack.pause = false;
			}
			else
			{
				ipack.pause = true;
			}
		}

	}
	fflush( stdin );
	return &ipack;
}


int main( void )
{
	// Set IO functions
	trt.IO.setInputFunction( &sendInput );
	trt.IO.setOutputFunction( &getOutput );

	// Set tetris speed in milliseconds
	trt.speed = 400;
	
	// Start playing game	
	trt.newGame( WIDTH , HEIGHT );

	return 0;
}
