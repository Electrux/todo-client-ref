/*
	Copyright (c) 2018, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#include <string.h>


#include "../include/core.h"
#include "../include/strvec.h"
#include "../include/strmanip.h"

void str_split( struct str_vec * svec, const char * str, const char delim )
{
	if( svec == NULL ) return;

	char tmp[ MAX_STR_LEN + 1 ];
	int tmp_ctr = 0;
	int len = strlen( str );

	int in_quote = 0;
	for( int i = 0; i < len; ++i ) {
		if( str[ i ] == '\"' && in_quote != 1 ) {
			in_quote = ( in_quote ) ? 0 : 2;
			continue;
		}
		if( str[ i ] == '\'' && in_quote != 2 ) {
			in_quote = ( in_quote ) ? 0 : 1;
			continue;
		}
		if( str[ i ] == delim && !in_quote ) {
			if( tmp_ctr <= 0 ) continue;
			tmp[ tmp_ctr ] = '\0';
			str_vec_add( svec, tmp );
			strcpy( tmp, "\0" );
			tmp_ctr = 0;
			continue;
		}
		tmp[ tmp_ctr++ ] = str[ i ];
	}
	if( tmp_ctr > 0 ) {
		tmp[ tmp_ctr ] = '\0';
		str_vec_add( svec, tmp );
	}
}
