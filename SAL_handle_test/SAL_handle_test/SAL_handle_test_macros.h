#pragma once 

#ifndef SAL_HANDLE_TEST_MACROS_INCLUDE
#define SAL_HANDLE_TEST_MACROS_INCLUDE


//these are headache-inducing, so I keep them in a separate header.
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


#else

#endif