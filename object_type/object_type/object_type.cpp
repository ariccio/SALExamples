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

//inspired by ITEMTYPE in older versions of WDS.
enum ITEMTYPE {
	IT_FILE,
	IT_FOLDER
	};

//inspired by CItem/CItemBranch in WDS.
struct ItemObject {
	ItemObject( const ITEMTYPE type_in, const std::wstring name_in ) : m_type( type_in ), m_name( std::move( name_in ) ) { }

	ItemObject& operator=( const ItemObject& in ) = delete;


	const std::wstring m_name;
	const ITEMTYPE m_type;
	};

//WTF THIS DOESN'T WORK
_Pre_satisfies_( item.m_type == ITEMTYPE::IT_FOLDER )
void ListFiles( const ItemObject& item ) {
	wprintf( L"Files in %s:\r\n", item.m_name.c_str( ) );
	//for ( const auto& file : item.m_files ) {
	//	wprintf( L"\tfile name: %s\r\n", file.m_name.c_str( ) );
	//	}
	}


void example( ) {
	ItemObject some_damned_object( ITEMTYPE::IT_FOLDER, L"my damned folder!" );
	ListFiles( some_damned_object );

	ItemObject some_other_damned_object( ITEMTYPE::IT_FILE, L"my damned file!" );
	ListFiles( some_other_damned_object );
	}

int main( ) {
	example( );

	}