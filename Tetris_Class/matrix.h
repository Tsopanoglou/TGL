#ifndef _MATRIX_H
#define _MATRIX_H


class matrix
{
	public:
		
		//-------------------------
		// Constructor - Destructor
		//-------------------------

		matrix( int width , int height );
		matrix( void );
		~matrix( void );

		//----------
		// Variables
		//----------

		int** cells;

		//--------
		// Methods
		//--------
		
		void resize( int width , int height );
		void set( int set_value );
		int w( void );
		int h( void );

	private:

		//----------
		// Variables
		//----------

		int p_w;
		int p_h;

		//--------
		// Methods
		//--------

		void clear( void );
};


void copyMatrix( matrix* from , matrix* to );
void printMatrix( matrix* mt );
void printMatrix( matrix* mt , int filter );
void printMatrix( matrix* mt , int filter , char replacement );


#endif // _MATRIX_H