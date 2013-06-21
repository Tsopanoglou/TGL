#include "tetris.h"
#include "matrix.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>


using namespace tetris;

#pragma region General Functions


TetriminoType to_TT( int type )
{
	switch ( type )
	{
		case 1 : { return TT_O ; break; }
		case 2 : { return TT_T ; break; }
		case 3 : { return TT_L ; break; }
		case 4 : { return TT_J ; break; }
		case 5 : { return TT_I ; break; }
		case 6 : { return TT_S ; break; }
		case 7 : { return TT_Z ; break; }
		case 8 : { return TT_NONE ; break; }
	}

	return TT_NONE;
}// to_TT
int to_Int( TetriminoType type )
{
	switch ( type )
	{
		case TT_O : { return 1 ; break; }
		case TT_T : { return 2 ; break; }
		case TT_L : { return 3 ; break; }
		case TT_J : { return 4 ; break; }
		case TT_I : { return 5 ; break; }
		case TT_S : { return 6 ; break; }
		case TT_Z : { return 7 ; break; }
		case TT_NONE : { return 8; break; }
	}

	return -1;
}// to_Int

//-------------------------------
//  TetrisIO default functions
//-------------------------------

InputPack* p_def_in_func( void )
{
	return NULL ;
}//p_def_in_func
void p_def_out_func( OutputPack* opack )
{

}//p_def_out_func


#pragma endregion

//-------------------------------
//         T E T R I S
//-------------------------------


Tetris::Tetris( void )
{
	// Set variables

	speed = 600;

	tetris.resize( 0 , 0 );
	buffer.resize( 0 , 0 );

	p_gameEnded = true;
	p_gamePaused = false;
	p_gameStarted = false;
	p_gameRunning = false;

	externCodeTimeFactor = 10;

	// Seed random number generator

	srand( time( NULL ) );

	// Set tetrimino associated tetris

	tmino.setAssociatedTetris( &buffer );

}// Tetris::~Tetris
Tetris::~Tetris( void )
{
	// Nothing to do here so far

}// Tetris::~Tetris

int Tetris::newGame( int width , int height )
{
	/*
		Initialize boards
	*/

	tetris.resize( width , height );
	buffer.resize( width , height );

	tetris.set( to_Int(TT_NONE) );
	buffer.set( to_Int(TT_NONE) );

	/*
		IO Packs
	*/

	p_opack.full_tetris = &buffer;
	p_opack.tetris = &tetris;
	p_opack.tmino = &tmino;

	/*
	   Set Variables
	*/

	p_gameRunning = true;
	p_gameStarted = true;
	p_gameEnded = false;
	p_gamePaused = false;

	clock_t start;
	clock_t end;

	/*
		Start playing game
	*/

	p_gameStartTime = clock();

	while ( ! p_gameEnded )
	{
		// Set up a new tetrimino

		tmino.setType( to_TT( ( rand() %  TT_NONE ) + 1 ) );
		tmino.x = width / 2;
		tmino.y = -tmino.board.h();

		while ( tmino.canMove( TMD_DOWN ) )
		{
			if ( ! p_gamePaused )
			{
				tmino.move( TMD_DOWN );

				//
				//	Input Loop
				// 

				p_inputLoopStartTime = clock();

				while ( ( p_inputLoopEndTime = clock () ) - p_inputLoopStartTime <  speed  )
				{
					// Clear output flags
					p_opack.clearFlags();

					// Get input
					p_ipack = IO.callInputFunction();

					// Check for game flow command

					if ( p_ipack->pause )
					{
						p_gamePaused = true;
						continue;
					}

					// Check for move command
					if ( p_ipack->move )
					{
						if ( tmino.canMove( p_ipack->dir ) )
						{
							p_opack.didMove = tmino.move( p_ipack->dir );

							// Call output function
							copyMatrix( &buffer , &tetris );
							tmino.printOnTetris( &tetris );

							//p_startExternalCode();

								IO.callOutputFunction( &p_opack );

							//p_endExternalCode();
						}
					}

					// Check for rotation command
					if ( p_ipack->rotate )
					{
						if ( tmino.canRotate( p_ipack->trnd ) )
						{
							p_opack.didRotate = tmino.rotate( p_ipack->trnd );

							// Send output
							copyMatrix( &buffer , &tetris );
							tmino.printOnTetris( &tetris );

							//p_startExternalCode();
								
								IO.callOutputFunction( &p_opack );

							//p_endExternalCode();
						}
					}
				}
			
				// Copy buffer on tetris
				copyMatrix( &buffer , &tetris );

				// Copy tetrimino on tetris
				tmino.printOnTetris( &tetris );

				// Call output function
				IO.callOutputFunction( &p_opack );
			}
			else
			{
				p_ipack = IO.callInputFunction();
				p_gamePaused = p_ipack->pause;
			}

		}// while canMove

		if ( p_gameLost() )
		{
			p_gameEnded = true;
		}

		// Save new tetris
		tmino.printOnTetris( &tetris );

		// Erase filled lines
		p_eraseFilledLines();

		// Save new buffer
		copyMatrix( &tetris , &buffer );

		IO.callOutputFunction( &p_opack );
	}

	return 0;

}// Tetris::newGame

double Tetris::getGameplayTime( void )
{
	return ( clock() - p_gameStartTime ) / 1000.0 ;

}// Tetris::getGameplayTime

bool Tetris::gameEnded( void )
{
	return p_gameEnded;

}// Tetris::gameEnded
bool Tetris::gamePaused( void )
{
	return p_gamePaused;

}// Tetris::gamePaused
bool Tetris::gameRunning( void )
{
	return p_gameRunning;

}// Tetris::gameRunning
bool Tetris::gameStarted( void )
{
	return p_gameStarted;

}// Tetris::gameStarted

bool Tetris::p_gameLost( void )
{
	// Get bottom-most line of tetrimino

	int bottom_most = -1;

	for ( int i = tmino.board.h() - 1 ; i >= 0 ; i-- )
	{
		for ( int j = 0 ; j < tmino.board.w() ; j++ )
		{
			if ( tmino.board.cells[i][j] == tmino.getType() )
			{
				bottom_most = i;
				break;
			}
		}
		if ( bottom_most != -1 )
		{
			break;
		}
	}

	// If bottom_most is above upper border

	//std::cout << std::endl << tmino.y << " " << bottom_most << std::endl;

	//system("pause");

	if ( tmino.y + bottom_most < 0 )
	{
		return true;
	}

	return false;

}// Tetris::p_gameLost
void Tetris::p_eraseFilledLines( void )
{
	p_opack.numOfErasedLines = 0;
	p_opack.didEraseLines = false;

	/*
		1. Create a temp matrix
		2. For each line of tetris, if line is NOT full, copy it to temp
		3. Copy temp back to tetris
	*/

	matrix temp( tetris.w() , tetris.h() );
	temp.set( TT_NONE );
	
	
	int temp_i = tetris.h() - 1;
	int counter = 0;
	
	for ( int i = tetris.h() - 1 ; i >= 0 ; i-- )
	{
		counter = 0;

		for ( int j = 0 ; j < tetris.w() ; j++ )
		{
			if ( ( temp.cells[temp_i][j] = tetris.cells[i][j] ) == TT_NONE )
			{
				counter++;
			}
		}
		
		if ( counter != 0 )
		{
			temp_i--;
		}
		else
		{
			p_opack.numOfErasedLines++;
			p_opack.didEraseLines = true;
		}
	}
	
	copyMatrix( &temp , &tetris );
	
}// Tetris::p_eraseFilledLines

void Tetris::p_startExternalCode( void )
{
	p_externCodeStartTime = clock();

}// Tetris::p_startExternalCode
void Tetris::p_endExternalCode( void )
{
	p_externCodeEndTime = clock();

	p_inputLoopStartTime += ( p_externCodeEndTime - p_externCodeStartTime ) / externCodeTimeFactor;

}// Tetris::p_endExternalCode

//----------------------------------------------------------
//                       TETRIMINO
//----------------------------------------------------------


Tetrimino::Tetrimino( void )
{
	board.resize( 0 , 0 );

	board.set( TT_NONE );

	p_type = TT_NONE;

}// Tetrimino::Tetrimino
Tetrimino::~Tetrimino( void )
{
	// matrix disposal is done inside matrix class

}// Tetrimino::~Tetrimino

void Tetrimino::setAssociatedTetris( matrix* tetris )
{
	p_tetris = tetris;

}// setAssociatedTetris
void Tetrimino::printOnTetris( matrix* tetris )
{
	for ( int i = 0 ; i < board.h() ; i++ )
	{
		for ( int j = 0 ; j < board.w() ; j++ )
		{
			if ( board.cells[i][j] == p_type )
			{
				if ( y + i >= 0 )
				{
					tetris->cells[i + y][j + x] = board.cells[i][j];
				}
			}
		}
	}

}// Tetrimino::printOnTetris

TetriminoType Tetrimino::getType( void )
{
	return p_type;

}// Tetrimino::getType
bool Tetrimino::setType( TetriminoType type )
{
	//Dispose previous tetrimino board

	board.set( TT_NONE );

	//Create the selected Tetrimino
	switch ( p_type = type )
	{
		case TT_O :
			{
				// Resize tetrimino
				board.resize( 4 , 4 );

				// If memory was allocated, create shape on board
				if ( board.cells != NULL )
				{
					board.cells[1][1] = board.cells[2][1] = board.cells[1][2] = board.cells[2][2] = TT_O; 
				}

				break;
			}
		case TT_T :
			{
				// Resize Tetrimino
				board.resize( 5 , 5 );

				// If memory was allocated, create shape on board
				if ( board.cells != NULL )
				{
					board.cells[2][1] = board.cells[2][2] = board.cells[2][3] = board.cells[1][2] = TT_T; 
				}

				break;
			}
		case TT_L :
			{
				// Resize Tetrimino
				board.resize( 5 , 5 );

				// If memory was allocated, create shape on board
				if ( board.cells != NULL )
				{
					board.cells[1][1] = board.cells[2][1] = board.cells[3][1] = board.cells[3][2] = TT_L; 
				}

				break;
			}
		case TT_J :
			{
				// Resize Tetrimino
				board.resize( 5 , 5 );

				// If memory was allocated, create shape on board
				if ( board.cells != NULL )
				{
					board.cells[1][2] = board.cells[2][2] = board.cells[3][2] = board.cells[3][1] = TT_J; 
				}

				break;
			}
		case TT_I :
			{
				// Resize Tetrimino
				board.resize( 6 , 6 );

				// If memory was allocated, create shape on board
				if ( board.cells != NULL )
				{
					board.cells[1][2] = board.cells[2][2] = board.cells[3][2] = board.cells[4][2] = TT_I; 
				}

				break;
			}
		case TT_S :
			{
				// Resize Tetrimino
				board.resize( 5 , 5 );

				// If memory was allocated, create shape on board
				if ( board.cells != NULL )
				{
					board.cells[2][1] = board.cells[2][2] = board.cells[1][2] = board.cells[1][3] = TT_S; 
				}

				break;
			}
		case TT_Z :
			{
				// Resize Tetrimino
				board.resize( 5 , 5 );

				// If memory was allocated, create shape on board
				if ( board.cells != NULL )
				{
					board.cells[1][1] = board.cells[1][2] = board.cells[2][2] = board.cells[2][3] = TT_Z; 
				}

				break;
			}

	}// switch ( type )

	//Notify about method success

	if ( board.cells == NULL ) 
	{
		return false;
	}

	return true;

}// Tetrimino::setType

bool Tetrimino::move( TetriminoMoveDirection dir )
{
	switch ( dir )
	{
		case TMD_DOWN  : { y++ ; return 1; }
		case TMD_LEFT  : { x-- ; return 1; }
		case TMD_RIGHT : { x++ ; return 1; }
	}

	return 0;

}// Tetrimino::move
bool Tetrimino::canMove( TetriminoMoveDirection dir )
{
	int jvar = 0;
	int ivar = 0;

	switch ( dir )
	{
		case TMD_DOWN  : { ivar =  1; break; }
		case TMD_LEFT  : { jvar = -1; break; }
		case TMD_RIGHT : { jvar =  1; break; }
	}

	/*
		Check if the shape is inside borders ( except top )
	*/

	int left_most = -1;
	int right_most = -1;
	int bottom_most = -1;

	// Get left_most

	for ( int j = 0 ; j < board.w() ; j++ )
	{
		for ( int i = 0 ; i < board.h() ; i++ )
		{
			if ( board.cells[i][j] == p_type )
			{
				left_most = j;
				break;
			}
		}
		if ( left_most != -1 )
		{
			break;
		}
	}

	// Get right_most

	for ( int j = board.w() - 1 ; j >= 0 ; j-- )
	{
		for ( int i = 0 ; i < board.h() ; i++ )
		{
			if ( board.cells[i][j] == p_type )
			{
				right_most = j;
				break;
			}
		}
		if ( right_most != -1 )
		{
			break;
		}
	}

	// Get botom_most

	for ( int i = board.h() - 1 ; i >= 0 ; i-- )
	{
		for ( int j = 0 ; j < board.w() ; j++ )
		{
			if ( board.cells[i][j] == p_type )
			{
				bottom_most = i;
				break;
			}
		}
		if ( bottom_most != -1 )
		{
			break;
		}
	}

	// Check borders

	if ( x + jvar + left_most < 0 ) { return false; }                   // Left border
	if ( x + jvar + right_most > p_tetris->w() - 1 ) { return false; }  // Right border
	if ( y + ivar + bottom_most > p_tetris->h() - 1 ) { return false; } // Bottom border

	/*
	   ****
	   ***I
	   **II
	   **I*
	   ****
	*/

	/*
		Check if there are any other tetriminos at the new location
	*/
	
	for ( int i = 0 ; i < board.h() ; i++ )
	{
		for ( int j = 0 ; j < board.w() ; j++ )
		{
			if ( board.cells[i][j] == p_type )
			{
				if ( y + ivar + i >= 0 )
				{
					if ( p_tetris->cells[y + ivar + i][x + jvar + j] != TT_NONE )
					{
						return false;
					}
				}
			}
		}
	}

	return true;

}// Tetrimino::canMove

bool Tetrimino::rotate( TetriminoRotationTrend trend )
{
   /* 
      ROTATION ALGORYTHM
                        
      1.Copy board to temp
      2.Clear board     
      3.Select rotation direction
      4.Copy temp back to board rotated
   */  

	matrix temp;

	temp.set( TT_NONE );

	// Copy board on temp

	copyMatrix( &board , &temp );

	// Clear board

	board.set( TT_NONE );

	// Select rotation direction

	switch ( trend )
	{
		case TRT_LEFT :
			{
				// Copy temp back to board rotated
				for ( int i = 0 ; i < board.h() ; i++ )
				{
					for ( int j = 0 ; j < board.w() ; j++ )
					{
						if ( temp.cells[i][j] == p_type )
						{
							board.cells[board.h() - j - 1][i] = temp.cells[i][j];
						}
					}
				}
				return 1;
			}
		case TRT_RIGHT :
			{
				// Copy temp back to shape rotated
				for ( int i = 0 ; i < board.h(); i++ )
				{
					for ( int j = 0 ; j < board.w() ; j++ )
					{
						if ( temp.cells[i][j] == p_type )
						{
							board.cells[j][board.w() - i - 1] = temp.cells[i][j];
						}
					}
				}
				return 1;
			}
	}

}// Tetrimino::rotate
bool Tetrimino::canRotate( TetriminoRotationTrend trend )
{
	// Temp tetrimino to test rotation
	Tetrimino temp;
	temp.x = x;
	temp.y = y;
	temp.setType(p_type);
	copyMatrix( &board , &temp.board );
	temp.setAssociatedTetris( p_tetris );

	
	// Rotate temp
	temp.rotate( trend );


	// Check for colitions
	return temp.canMove( TMD_NONE );

}// Tetrimino::canRotate


//----------------------------------------------------------
//                       INPUTPACK
//----------------------------------------------------------


InputPack::InputPack( void )
{
	// Preset commands

	stop = false;
	pause = false;
	resume = false;

	move = false;
	rotate = false;

	dir = TMD_NONE;
	trnd = TRT_NONE;

}// InputPack::InputPack
InputPack::~InputPack( void )
{
	//Nothing  to do here so far

}// InputPacK::~InputPack


//----------------------------------------------------------
//                      OUTPUTPACK
//----------------------------------------------------------


OutputPack::OutputPack( void )
{
	// Set pointers to NULL

	full_tetris = NULL;
	tetris = NULL;
	tmino = NULL;

	// Preset flags

	didStop = false;
	didPause = false;
	didResume = false;

    didMove = false;
	didRotate = false;

	secsOfGameplay = 0;
	numOfErasedLines = 0;

}// OutputPack::OutputPack
OutputPack::~OutputPack( void )
{
	//Nothing to do here so far

}// OutputPack::~OutputPack

void OutputPack::clear( void )
{
	// Set pointers to NULL

	full_tetris = NULL;
	tetris = NULL;
	tmino = NULL;

	// Preset flags

	clearFlags();

	//Other

	secsOfGameplay = 0;
	numOfErasedLines = 0;

}// OutputPack::clear
void OutputPack::clearFlags( void )
{
	// Preset flags

	didStop = false;
	didPause = false;
	didResume = false;

    didMove = false;
	didRotate = false;

}// OutputPack::clearFlags


//----------------------------------------------------------
//                        TETRISIO
//----------------------------------------------------------


TetrisIO::TetrisIO( void )
{
	// Set function pointers to default functions

	p_inFunc = & p_def_in_func;
	p_outFunc = & p_def_out_func;

}// TetrisIO::TetrisIO
TetrisIO::~TetrisIO( void )
{
	//Nothing to do here so far

}// Tetris::~TetrisIO

void TetrisIO::setInputFunction( InputPack* ( *inFunc )( void ) )
{
	p_inFunc = inFunc;

}// TetrisIO::setInputFunction
void TetrisIO::setOutputFunction( void ( *outFunc )( OutputPack* ) )
{
	p_outFunc = outFunc;

}// TetrisIO::setOutputFunction

InputPack* TetrisIO::callInputFunction( void )
{
	return p_inFunc();

}// TetrisIO::callInputFunction
void TetrisIO::callOutputFunction( OutputPack* opack )
{
	p_outFunc( opack );

}// TetrisIO::callOutputFunction


