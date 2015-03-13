/*
Alexander Riccio
This demonstrates using SAL and static analysis to detect usage of an invalidated handle. This is a recreation of some stress test code for altWinDirStat, where I noticed the neat usage of SAL.
*/

#include "SAL_handle_test.h"


void close_single_handle( _In_ const HANDLE& the_handle ) {
	//"If the function succeeds, the return value is nonzero."
	const BOOL close_res = CloseHandle( the_handle );
	assert( close_res != 0 );
	if ( close_res != 0 ) {
		return;
		}
	const rsize_t err_buff_size = 512;
	wchar_t err_buff[ err_buff_size ] = { 0 };
	const auto last_err = GetLastError( );
	rsize_t chars_written = 0;
	const HRESULT err_res = CStyle_GetLastErrorAsFormattedMessage( err_buff, err_buff_size, chars_written, last_err );
	assert( SUCCEEDED( err_res ) );
	if ( SUCCEEDED( err_res ) ) {
		const auto wpf_res = wprintf( L"Error closing handle: %s\r\n", err_buff );
		SAL_HANDLE_TEST_ASSERT_IF_DEBUG_ELSE_UNREFERENCED( wpf_res, >= , 0 );
		return;
		}
	const auto wpf_res = wprintf( L"Error closing handle: %u (also, error getting error message)\r\n", last_err );
	SAL_HANDLE_TEST_ASSERT_IF_DEBUG_ELSE_UNREFERENCED( wpf_res, >= , 0 );
	}


void handle_failed_to_create_event_already_exists( _In_ const HANDLE& fileHandle ) {
	const auto wpf_res = wprintf( L"Error creating event: event already exists!\r\n" );
	SAL_HANDLE_TEST_ASSERT_IF_DEBUG_ELSE_UNREFERENCED( wpf_res, >= , 0 );
	close_single_handle( fileHandle );
	}

void handle_failed_to_create_event( const DWORD last_err, _In_ const HANDLE& fileHandle ) {
	const rsize_t err_buff_size = 512;
	wchar_t err_buff[ err_buff_size ] = { 0 };
	rsize_t chars_written = 0;
	const HRESULT err_res = CStyle_GetLastErrorAsFormattedMessage( err_buff, err_buff_size, chars_written, last_err );
	if ( SUCCEEDED( err_res ) ) {
		const auto wpf_res = wprintf( L"Error creating event: %s\r\n", err_buff );
		SAL_HANDLE_TEST_ASSERT_IF_DEBUG_ELSE_UNREFERENCED( wpf_res, >= , 0 );
		close_single_handle( fileHandle );
		return;
		}
	const auto wpf_res = wprintf( L"Error creating event: %u (also, error getting error message)\r\n", last_err );
	SAL_HANDLE_TEST_ASSERT_IF_DEBUG_ELSE_UNREFERENCED( wpf_res, >= , 0 );
	close_single_handle( fileHandle );
	}


_Pre_satisfies_( fileHandle != INVALID_HANDLE_VALUE )
_Pre_satisfies_( handle_event != NULL )
_At_( handle_event, _Pre_valid_ )
_At_( fileHandle, _Pre_valid_ )
_At_( handle_event, _Post_invalid_ )
_At_( fileHandle, _Post_invalid_ )
void close_handles( const HANDLE& handle_event, const HANDLE& fileHandle ) {
	assert( fileHandle != INVALID_HANDLE_VALUE );
	assert( handle_event != NULL );
	close_single_handle( handle_event );
	close_single_handle( fileHandle );
	}


//this uses the annotated handle-closing function
void CreateFileAndEvent_ThenClose_WithAnnotations( _In_z_ PCWSTR newStr ) {

	const HANDLE fileHandle = CreateFileW( newStr, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL );

	const HANDLE handle_event = CreateEventW( NULL, TRUE, FALSE, NULL );
	const auto last_err = GetLastError( );
	if ( handle_event == NULL ) {
		handle_failed_to_create_event( last_err, fileHandle );
		return;
		}
	if ( last_err == ERROR_ALREADY_EXISTS ) {
		handle_failed_to_create_event_already_exists( fileHandle );
		return;
		}
	

	close_handles( handle_event, fileHandle );
	//handle is no longer valid

	//C6001	Using uninitialized memory '*fileHandle'.
	//	239 '*fileHandle' is not initialized
	//	251 '*fileHandle' is an Input to 'GetHandleInformation' (declared at c:\program files (x86)\windows kits\8.1\include\um\handleapi.h:79)
	//	251 '*fileHandle' is used, but may not have been initialized

	DWORD dummy;
	GetHandleInformation( fileHandle, &dummy );
	return;
	}

//this doesn't use the aforementioned annotations
void CreateFileAndEvent_ThenClose_NoAnnotations( _In_z_ PCWSTR newStr ) {

	const HANDLE fileHandle = CreateFileW( newStr, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL );

	const HANDLE handle_event = CreateEventW( NULL, TRUE, FALSE, NULL );
	const auto last_err = GetLastError( );
	if ( handle_event == NULL ) {
		handle_failed_to_create_event( last_err, fileHandle );
		return;
		}
	if ( last_err == ERROR_ALREADY_EXISTS ) {
		handle_failed_to_create_event_already_exists( fileHandle );
		return;
		}
	

	CloseHandle( fileHandle );
	CloseHandle( handle_event );
	//handle is no longer valid
	
	//no complaint
	DWORD dummy;
	GetHandleInformation( fileHandle, &dummy );
	return;
	}



int wmain( _In_ _In_range_( 0, INT_MAX ) int argc, _In_count_( argc ) _Readable_elements_( argc ) _Deref_prepost_z_ wchar_t* argv[ ] ) {
	if ( argc == 1 ) {
		usage( );
		return 0;
		}

	trace_args( argc, argv );

	assert( argv[ argc ] == NULL );
	TRACE_OUT_C_STYLE_ENDL( );
	if ( argc > 2 ) {
		too_many_args( argc );
		return -1;
		}

	CreateFileAndEvent_ThenClose_WithAnnotations( argv[ 1 ] );
	

	CreateFileAndEvent_ThenClose_NoAnnotations( argv[ 1 ] );

	return 0;
	}
