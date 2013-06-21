#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"

//-------------------------------
//      RELATIVE FUNCTIONS
//-------------------------------


void copyMatrix( matrix* from , matrix* to )
{
	// Resize to matrix 

	if ( ( from->w() != to->w() ) || ( from->h() != to->h() ) )
	{
		to->resize( from->w() , from->h() );
	}

	// Copy items
	for ( int i = 0 ; i < to->h() ; i++ )
	{
		for ( int j = 0 ; j < to->w() ; j++ )
		{
			to->cells[i][j] = from->cells[i][j];
		}
	}

}// copyMatrix

void printMatrix( matrix* mt )
{
	for ( int i = 0 ; i < mt->h() ; i++ )
	{
		for ( int j = 0 ; j < mt->w() ; j++ )
		{
			printf("%d", mt->cells[i][j]);
		}
		printf("\n");
	}

}// printMatrix
void printMatrix( matrix* mt , int filter )
{
	for ( int i = 0 ; i < mt->h() ; i++ )
	{
		for ( int j = 0 ; j < mt->w() ; j++ )
		{
			if ( mt->cells[i][j] != filter )
			{
				printf("%d", mt->cells[i][j]);
			}
		}
		printf("\n");
	}

}// printMatrix
void printMatrix( matrix* mt , int filter , char replacement )
{
	for ( int i = 0 ; i < mt->h() ; i++ )
	{
		for ( int j = 0 ; j < mt->w() ; j++ )
		{
			if ( mt->cells[i][j] != filter )
			{
				printf("%d", mt->cells[i][j]);
			}
			else
			{
				printf("%c", replacement);
			}
		}
		printf("\n");
	}
}// printMatrix


//-------------------------------
//           MATRIX
//-------------------------------


matrix::matrix( int width , int height )
{
	cells = NULL;
	p_w = 0;
	p_h = 0;

	resize( width , height );

}// matrix::matrix

matrix::matrix( void )
{
	cells = NULL;
	p_w = 0;
	p_h = 0;

	resize( 1 , 1 );
}

matrix::~matrix( void )
{
	clear();

}// matrix::~matrix

// public methods

void matrix::resize( int width , int height )
{
	clear();

	// Allocate memory

	cells = ( int** ) malloc ( height * sizeof( int* ) );

	if ( cells == NULL )
	{
		return;
	}

	for ( int i = 0 ; i < height ; i++ )
	{
		cells[i] = NULL;

		cells[i] = ( int* ) malloc ( width * sizeof( int ) );

		if ( cells[i] == NULL )
		{
			
			// Dealocate memory taken so far
			for(int j=i-1;j>=0;j--){if(cells[j]!=NULL){free(cells[j]);}}
			if(cells!=NULL){free(cells);}
			

			return;
		}
	}

	// Set size

	p_w = width;
	p_h = height;

}// matrix::resize

void matrix::set( int set_value )
{
	for ( int i = 0 ; i < p_h ; i++ )
	{
		for ( int j = 0 ; j < p_w ; j++ )
		{
			cells[i][j] = set_value;
		}
	}

}// matrix::clear

int matrix::w( void )
{
	return p_w;

}// matrix::w

int matrix::h( void )
{
	return p_h;

}// matrix::p_h

// private methods

void matrix::clear( void )
{
	// Free memory

	for ( int i = 0 ; i < p_h ; i++ )
	{
		if ( cells[i] != NULL )
		{
			free( cells[i] );
		}
	}

	if ( cells != NULL )
	{
		free( cells );
	}

	cells = NULL;

	p_w = 0;
	p_h = 0;

}// matrix::clear