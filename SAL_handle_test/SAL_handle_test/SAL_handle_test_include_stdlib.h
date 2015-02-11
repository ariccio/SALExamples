#pragma once

#ifndef SAL_HANDLE_TEST_INCLUDE_STDLIB_INCLUDE
#define SAL_HANDLE_TEST_INCLUDE_STDLIB_INCLUDE

//these are headache-inducing, so I keep them in a separate header.

//Windows headers DO NOT compile cleanly >:(

#pragma warning( disable: 4514 ) //"unreferenced inline function has been removed"
#pragma warning( disable: 4710 ) //"function not inlined"

#pragma warning( push )
#pragma warning( disable: 4820 ) //"'[SOME NUMBER]' bytes padding added after data member"

#pragma warning( disable: 4350 ) //"behavior change"
#pragma warning( disable: 4668 ) //"'_WIN32_WINNT_WINTHRESHOLD' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'", "'NTDDI_WIN7SP1' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'"

#include <cstdio>
#include <string>
#include <windows.h>
#pragma warning( pop )

#include <utility>
#include <assert.h>


#else

#endif