#pragma warning( disable: 4820 )//bytes padding added
#pragma warning( disable: 4514 )//unreferenced inline function removed
#pragma warning( push, 4 )
#pragma warning( disable: 4668 )//some crap not defined as preprocessor macro
#include <cstdio>
#include <intrin.h>
#include <Windows.h>
#pragma warning( pop )

_Ret_range_( 2, 20 )
const DWORD return_some_number( _In_ _In_range_( 1, 10 ) const DWORD in ) {
	return ( in * 2 );
	}

/*_Success_( return != 0 )*/ /*_Always_( _When_( Mask > 0, _Ret_range_( 1, UCHAR_MAX ) ) )*/ /*_On_failure_( _At_( Index, _Post_invalid_ ) )*/
_Pre_satisfies_( Mask > 0 )
UCHAR my_bit_scan_reverse( _Out_ DWORD* const Index, _In_ const DWORD Mask ) {
	if ( Mask == 0 ) {
		return 0;
		}
	__asm bsr eax, Mask;
	__asm mov ecx, Index
	__asm mov [ecx], eax
	return 1;
	}

#pragma intrinsic(_BitScanReverse)

int main( ) {
	const ULONG mask = 12;
		{
		ULONG index = 0;
		const UCHAR is_non_zero = _BitScanReverse( &index, mask );

		//C6102: Using 'index' from failed function call at line '36'.
		printf( "char is_non_zero: %c, unsigned long value of is_non_zero: %lu, index: %lu\r\n", is_non_zero, static_cast< unsigned long >( is_non_zero ), index );
		}
		{
		ULONG index = 0;
		
		//analyze doesn't complain
		const UCHAR is_non_zero = my_bit_scan_reverse( &index, mask );
		printf( "char is_non_zero: %c, unsigned long value of is_non_zero: %lu, index: %lu\r\n", is_non_zero, static_cast< unsigned long >( is_non_zero ), index );
		}
		{
		ULONG index = 0;
		const ULONG alt_mask = 0;
		
		//C28020: The expression '_Param_(2)>0' is not true at this call.
		const UCHAR is_non_zero = my_bit_scan_reverse( &index, alt_mask );
		printf( "char is_non_zero: %c, unsigned long value of is_non_zero: %lu, index: %lu\r\n", is_non_zero, static_cast< unsigned long >( is_non_zero ), index );
		}
		
		{
		ULONG index = 0;
		const ULONG alt_mask = return_some_number( 7 );
		const UCHAR is_non_zero = my_bit_scan_reverse( &index, alt_mask );

		//analyze doesn't complain
		printf( "char is_non_zero: %c, unsigned long value of is_non_zero: %lu, index: %lu\r\n", is_non_zero, static_cast< unsigned long >( is_non_zero ), index );
		}



	}