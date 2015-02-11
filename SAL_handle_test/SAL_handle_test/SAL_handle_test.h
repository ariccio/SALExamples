#pragma once

#ifndef SAL_HANDLE_TEST_HEADER_INCLUDE
#define SAL_HANDLE_TEST_HEADER_INCLUDE


#include "SAL_handle_test_include_stdlib.h"//headache-inducing includes, with accompanying warning suppressions.
#include "SAL_handle_test_macros.h"        //headache-inducing macros.


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

static_assert( !SUCCEEDED( E_FAIL ), "bad failure return code for CStyle_GetLastErrorAsFormattedMessage" );

static_assert( SUCCEEDED( S_OK ), "bad success return code for CStyle_GetLastErrorAsFormattedMessage" );

//On returning E_FAIL, call GetLastError for details. That's not my idea!
_Success_( SUCCEEDED( return ) )
const HRESULT CStyle_GetLastErrorAsFormattedMessage( WDS_WRITES_TO_STACK( strSize, chars_written ) PWSTR psz_formatted_error, _In_range_( 128, 32767 ) const rsize_t strSize, _Out_ rsize_t& chars_written, const DWORD error ) {
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

void too_many_args( _In_ _In_range_( 0, INT_MAX ) const int argc ) {
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


void trace_args( _In_ _In_range_( 0, INT_MAX ) const int argc, _In_count_( argc ) _Readable_elements_( argc ) _Deref_prepost_z_ const wchar_t* const argv[ ] ) {
	const auto wpf_res_4 = wprintf( L"arguments passed: \r\n" );
	SAL_HANDLE_TEST_ASSERT_IF_DEBUG_ELSE_UNREFERENCED( wpf_res_4, >= , 0 );
	for ( int i = 0; i < argc; ++i ) {
		const auto wpf_res_5 = wprintf( L"\ti: %i", i );
		SAL_HANDLE_TEST_ASSERT_IF_DEBUG_ELSE_UNREFERENCED( wpf_res_5, >= , 0 );
		TRACE_OUT_C_STYLE( argv[ i ], %s );
		TRACE_OUT_C_STYLE_ENDL( );
		}
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
		return;
		}
	const auto wpf_res = wprintf( L"Error creating file: %u (also, error getting error message)\r\n", last_err );
	SAL_HANDLE_TEST_ASSERT_IF_DEBUG_ELSE_UNREFERENCED( wpf_res, >= , 0 );
	}


#else
#endif