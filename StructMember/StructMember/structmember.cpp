#include <sal.h>
#include <cstdio>
#include <cassert>
#include <stdexcept>

struct somestruct {
	int member;
	bool flag;
	};

_Pre_satisfies_( in->flag )
void dosomething( _In_ somestruct* const in ) {
	//Not detected at compile time
	assert(in-> flag);
	if ( !in->flag ) {
		throw std::invalid_argument( "Function preconditions not met!" );
		}
	}

_Pre_satisfies_( in->flag )
_When_( in->member > 0, _At_( some_number, _In_ ) )
_When_( in->member <= 0, _At_( some_number, _Out_ ) )
void dosomethingelse( _In_ somestruct* const in, int* const some_number ) {
	//Not detected at compile time
	assert(in-> flag);
	if ( !in->flag ) {
		throw std::invalid_argument( "Function preconditions not met!" );
		}
	if ( in->member <= 0 ) {
		( *some_number ) = in->member;
		}
	else {
		in->member = ( *some_number );
		}
	}


int main( ) {
	somestruct the_struct = { 0, false };

	//invalid use
	dosomething( &the_struct );

	//valid use
	the_struct.flag = true;
	dosomething( &the_struct );

	//correctly use some_int as an _In_ parameter
	the_struct.member = 1;
	int some_int = 5;
	dosomethingelse( &the_struct, &some_int );

	//correctly use some_other_int as an _Out_ parameter
	the_struct.member = -1;
	int some_other_int;
	dosomethingelse( &the_struct, &some_other_int );


	//incorrectly use yet_another_int as an _In_ parameter
	the_struct.member = 1;
	int yet_another_int;
	dosomethingelse( &the_struct, &yet_another_int );

	}