#ifndef _TETRIS_H
#define _TETRIS_H

#include "matrix.h"

namespace tetris
{
	//-------------
	// Enumerations
	//-------------

	enum TetriminoRotationTrend { TRT_RIGHT , TRT_LEFT , TRT_NONE };
	enum TetriminoMoveDirection { TMD_RIGHT , TMD_LEFT , TMD_DOWN , TMD_NONE };
	enum TetriminoType { TT_O = 1 , TT_T , TT_L , TT_J , TT_I , TT_S , TT_Z , TT_NONE };

	//--------
	// Classes
	//--------

	class Tetrimino
	{
		public:

			//-------------------------
			// Constructor - Destructor
			//-------------------------

			Tetrimino( void );
			~Tetrimino( void );

			//----------
			// Variables
			//----------

			matrix board;

			int x , y ;

			//--------
			// Methods
			//--------

			void setAssociatedTetris( matrix* tetris );
			void printOnTetris( matrix* tetris );
			
			TetriminoType getType( void );
			bool setType( TetriminoType type );

			bool move( TetriminoMoveDirection dir );
			bool canMove( TetriminoMoveDirection dir );

			bool rotate( TetriminoRotationTrend trend );
			bool canRotate( TetriminoRotationTrend trend );

		private:

			//----------
			// Variables
			//----------
			
			matrix* p_tetris;
			TetriminoType p_type;
	};

	class InputPack
	{
		public:

			//-----------------------
			// Constructor-Destructor
			//-----------------------

			InputPack( void );
			~InputPack( void );

			//---------
			// Controls
			//---------

			bool stop;      // Game state controls
			bool pause;     //
			bool resume;    //

			bool move;      // Tetrimino controls
			bool rotate;    //

			TetriminoMoveDirection dir;
			TetriminoRotationTrend trnd;
			
		private:

			/*
				Do not write anything private. This object is read and written
				only by external functions, so private variables won't be accessible.
			*/
	};

	class OutputPack
	{
		public:

			//-----------------------
			// Constructor-Destructor
			//-----------------------

			OutputPack( void ); 
			~OutputPack( void );

			//--------
			// Methods
			//--------

			void clear( void );
			void clearFlags( void );

			//-------
			// FLAGS
			//-------

			bool didStop;         // Game state flags
			bool didPause;        //
			bool didResume;       //
			bool didEraseLines;   //

			bool didMove;         // Tetrimino control flags
			bool didRotate;       //

			int secsOfGameplay;   // Other flags
			int numOfErasedLines; //

			//----------
			// VARIABLES
			//----------

			Tetrimino* tmino;
			matrix* full_tetris;
			matrix* tetris;

		private:

			/*
				Do not write anything private. This object is read and written
				only by outter classes, so private variables won't be accessible.
			*/
	};

	class TetrisIO
	{
		public:

			//-----------------------
			// Constructor-Destructor
			//-----------------------

			TetrisIO( void );
			~TetrisIO( void );

			//--------
			// Methods
			//--------

			void setInputFunction( InputPack* ( *inFunc )( void ) );     // IO function setters
			void setOutputFunction( void ( *outFunc )( OutputPack* ) );  //
		
			InputPack* callInputFunction( void );                        // IO function callers
			void callOutputFunction( OutputPack* opack );                //
	
		private:

			//-------------------------
			// Pointers to IO Functions
			//-------------------------
	
			InputPack* ( *p_inFunc )( void );
			void ( *p_outFunc )( OutputPack* );
	};

	class Tetris
	{
		public:

			//-------------------------
			// Constructor - Destructor
			//-------------------------

			Tetris( void );
			~Tetris( void );

			//--------
			// Methods
			//--------

			int newGame( int width , int height );

			double getGameplayTime( void );

			bool gamePaused( void );
			bool gameStarted( void );
			bool gameRunning( void );
			bool gameEnded( void );

			//----------
			// Variables
			//----------

			double speed;

			double externCodeTimeFactor;

			//--------
			// Objects
			//--------

			Tetrimino tmino;
			TetrisIO IO;

		private:

			//----------------
			// Tetris matrixes
			//----------------

			matrix tetris;
			matrix buffer;

			//----------
			// Variables
			//----------

			InputPack* p_ipack;
			OutputPack p_opack;

			double p_gameStartTime;       // The time when game has started
			double p_externCodeStartTime; // The time when external code has started
			double p_externCodeEndTime;
			double p_inputLoopStartTime;  // The time when the input loop has started
			double p_inputLoopEndTime;    // the time when the input loop has ended

			//------
			// Flags
			//------

			bool p_gameEnded;
			bool p_gamePaused;
			bool p_gameRunning;
			bool p_gameStarted;

			//--------
			// Methods
			//--------

			bool p_gameLost( void );
			void p_eraseFilledLines( void );

			void p_startExternalCode( void );
			void p_endExternalCode( void );
	};


}// namespace tetris


#endif // _TETRIS_H
