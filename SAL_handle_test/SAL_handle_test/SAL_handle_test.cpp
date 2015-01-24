/*
Alexander Riccio
This demonstrates using SAL and static analysis to detect usage of an invalidated handle. This is a recreation of some stress test code for altWinDirStat, where I noticed the neat usage of SAL.
*/

#include <Windows.h>
#include <stdio.h>
#include <utility>
#include <assert.h>


#ifndef WDS_WRITES_TO_STACK
//This is a macro I developed for (alt)WinDirStat, which improves the safety of C-Style text processing
#define WDS_WRITES_TO_STACK( strSize, chars_written ) _Out_writes_z_( strSize ) _Pre_writable_size_( strSize ) _Post_readable_size_( chars_written ) _Pre_satisfies_( strSize >= chars_written ) _Post_satisfies_( _Old_( chars_written ) <= chars_written )
#else
#error already defined!
#endif


#ifdef NDEBUG
//this macro allows me to compile with warnings as errors, and assert conditions for variables that are otherwise unreferenced
#define SAL_HANDLE_TEST_ASSERT_IF_DEBUG_ELSE_UNREFERENCED( var_name, cmp_op, cond ) UNREFERENCED_PARAMETER( var_name )
#else
//this macro allows me to compile with warnings as errors, and assert conditions for variables that are otherwise unreferenced
#define SAL_HANDLE_TEST_ASSERT_IF_DEBUG_ELSE_UNREFERENCED( var_name, cmp_op, cond ) assert( ( ( var_name ) cmp_op ( cond ) ) )
#endif


#ifdef NDEBUG
//this macro conveniently formats and prints `x` as a string, followed by the actual value of `x`;
#define TRACE_OUT_C_STYLE( x, fmt_spec ) wprintf( L"\r\n\t\t" L#x L" = `" L#fmt_spec L"` ", ##x )
#define TRACE_OUT_C_STYLE_ENDL( ) wprintf( L"\r\n" )
#else
//this macro conveniently formats and prints `x` as a string, followed by the actual value of `x`;
#define TRACE_OUT_C_STYLE( x, fmt_spec ) assert( wprintf( L"\r\n\t\t" L#x L" = `" L#fmt_spec L"` ", ##x ) >= 0 )
#define TRACE_OUT_C_STYLE_ENDL( ) assert( wprintf( L"\r\n" ) >= 0 )
#endif


void write_bad_fmt_msg( _Out_writes_z_( 41 ) _Pre_writable_size_( 42 ) _Post_readable_size_( chars_written ) PWSTR psz_fmt_msg, _Out_ rsize_t& chars_written ) {
	psz_fmt_msg[ 0 ] = L'F';
	psz_fmt_msg[ 1 ] = L'o';
	psz_fmt_msg[ 2 ] = L'r';
	psz_fmt_msg[ 3 ] = L'm';
	psz_fmt_msg[ 4 ] = L'a';
	psz_fmt_msg[ 5 ] = L't';
	psz_fmt_msg[ 6 ] = L'M';
	psz_fmt_msg[ 7 ] = L'e';
	psz_fmt_msg[ 8 ] = L's';
	psz_fmt_msg[ 9 ] = L's';
	psz_fmt_msg[ 10 ] = L'a';
	psz_fmt_msg[ 11 ] = L'g';
	psz_fmt_msg[ 12 ] = L'e';
	psz_fmt_msg[ 13 ] = L' ';
	psz_fmt_msg[ 14 ] = L'f';
	psz_fmt_msg[ 15 ] = L'a';
	psz_fmt_msg[ 16 ] = L'i';
	psz_fmt_msg[ 17 ] = L'l';
	psz_fmt_msg[ 18 ] = L'e';
	psz_fmt_msg[ 19 ] = L'd';
	psz_fmt_msg[ 20 ] = L' ';
	psz_fmt_msg[ 21 ] = L't';
	psz_fmt_msg[ 22 ] = L'o';
	psz_fmt_msg[ 23 ] = L' ';
	psz_fmt_msg[ 24 ] = L'f';
	psz_fmt_msg[ 25 ] = L'o';
	psz_fmt_msg[ 26 ] = L'r';
	psz_fmt_msg[ 27 ] = L'm';
	psz_fmt_msg[ 28 ] = L'a';
	psz_fmt_msg[ 29 ] = L't';
	psz_fmt_msg[ 30 ] = L' ';
	psz_fmt_msg[ 31 ] = L'a';
	psz_fmt_msg[ 32 ] = L'n';
	psz_fmt_msg[ 33 ] = L' ';
	psz_fmt_msg[ 34 ] = L'e';
	psz_fmt_msg[ 35 ] = L'r';
	psz_fmt_msg[ 36 ] = L'r';
	psz_fmt_msg[ 37 ] = L'o';
	psz_fmt_msg[ 38 ] = L'r';
	psz_fmt_msg[ 39 ] = L'!';
	psz_fmt_msg[ 40 ] = 0;
	chars_written = 41;
	}

void write_BAD_FMT( _Out_writes_z_( 8 ) _Pre_writable_size_( 8 ) _Post_readable_size_( 8 ) PWSTR pszFMT, _Out_ rsize_t& chars_written ) {
	pszFMT[ 0 ] = 'B';
	pszFMT[ 1 ] = 'A';
	pszFMT[ 2 ] = 'D';
	pszFMT[ 3 ] = '_';
	pszFMT[ 4 ] = 'F';
	pszFMT[ 5 ] = 'M';
	pszFMT[ 6 ] = 'T';
	pszFMT[ 7 ] = 0;
	chars_written = 8;
	}


//On returning E_FAIL, call GetLastError for details. That's not my idea!
_Success_( SUCCEEDED( return ) ) HRESULT CStyle_GetLastErrorAsFormattedMessage( WDS_WRITES_TO_STACK( strSize, chars_written ) PWSTR psz_formatted_error, _In_range_( 128, 32767 ) const rsize_t strSize, _Out_ rsize_t& chars_written, const DWORD error ) {
	//const auto err = GetLastError( );
	const auto err = error;
	const auto ret = FormatMessageW( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, err, MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ), psz_formatted_error, static_cast< DWORD >( strSize ), NULL );
	if ( ret != 0 ) {
		chars_written = ret;
		return S_OK;
		}
	if ( strSize > 41 ) {
		write_bad_fmt_msg( psz_formatted_error, chars_written );
		return E_FAIL;
		}
	else if ( strSize > 8 ) {
		write_BAD_FMT( psz_formatted_error, chars_written );
		return E_FAIL;
		}
	chars_written = 0;
	return E_FAIL;
	}

void handle_invalid_handle_value( ) {
	const rsize_t err_buff_size = 512;
	const auto last_err = GetLastError( );
	wchar_t err_buff[ err_buff_size ] = { 0 };
	rsize_t chars_written = 0;
	const HRESULT err_res = CStyle_GetLastErrorAsFormattedMessage( err_buff, err_buff_size, chars_written, last_err );
	if ( SUCCEEDED( err_res ) ) {
		const auto wpf_res = wprintf( L"Error creating file: %s\r\n", err_buff );
		SAL_HANDLE_TEST_ASSERT_IF_DEBUG_ELSE_UNREFERENCED( wpf_res, >= , 0 );
		}
	else {
		const auto wpf_res = wprintf( L"Error creating file: %u (also, error getting error message)\r\n", last_err );
		SAL_HANDLE_TEST_ASSERT_IF_DEBUG_ELSE_UNREFERENCED( wpf_res, >= , 0 );
		}
	}

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
		}
	else {
		const auto wpf_res = wprintf( L"Error closing handle: %u (also, error getting error message)\r\n", last_err );
		SAL_HANDLE_TEST_ASSERT_IF_DEBUG_ELSE_UNREFERENCED( wpf_res, >= , 0 );
		}
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
		}
	else {
		const auto wpf_res = wprintf( L"Error creating event: %u (also, error getting error message)\r\n", last_err );
		SAL_HANDLE_TEST_ASSERT_IF_DEBUG_ELSE_UNREFERENCED( wpf_res, >= , 0 );
		}
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


void trace_args( _In_ _In_range_( 0, INT_MAX ) const int& argc, _In_count_( argc ) _Readable_elements_( argc ) _Deref_prepost_z_ const wchar_t* const argv[ ] ) {
	const auto wpf_res_4 = wprintf( L"arguments passed: \r\n" );
	SAL_HANDLE_TEST_ASSERT_IF_DEBUG_ELSE_UNREFERENCED( wpf_res_4, >= , 0 );
	for ( int i = 0; i < argc; ++i ) {
		const auto wpf_res_5 = wprintf( L"\ti: %i", i );
		SAL_HANDLE_TEST_ASSERT_IF_DEBUG_ELSE_UNREFERENCED( wpf_res_5, >= , 0 );
		TRACE_OUT_C_STYLE( argv[ i ], %s );
		TRACE_OUT_C_STYLE_ENDL( );
		}
	}


void too_many_args( _In_ _In_range_( 0, INT_MAX ) const int& argc ) {
	const auto wpf_res_6 = wprintf( L"You passed %i (too many) arguments. Please pass only the directory to open.\r\n", argc );
	SAL_HANDLE_TEST_ASSERT_IF_DEBUG_ELSE_UNREFERENCED( wpf_res_6, >= , 0 );
	}

void usage( ) {
	const auto wpf_res_1 = wprintf( L"no arguments supplied, displaying usage.\r\n" );
	const auto wpf_res_2 = wprintf( L"usage:  `\"C:\\path\\to\\a\\directory\\to\\be\\opened\"`\r\n" );
	const auto wpf_res_3 = wprintf( L"example: \"C:\\Users\\Alexander Riccio\\Documents\\test_junk_dir\"\r\n" );
	SAL_HANDLE_TEST_ASSERT_IF_DEBUG_ELSE_UNREFERENCED( wpf_res_1, >= , 0 );
	SAL_HANDLE_TEST_ASSERT_IF_DEBUG_ELSE_UNREFERENCED( wpf_res_2, >= , 0 );
	SAL_HANDLE_TEST_ASSERT_IF_DEBUG_ELSE_UNREFERENCED( wpf_res_3, >= , 0 );
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
