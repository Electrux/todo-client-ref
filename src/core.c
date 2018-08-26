/*
	Copyright (c) 2018, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <time.h>

static inline bool is_whitespc( const char c ) { return c == ',' || c == ' ' || c == '\0'; }

bool find_in( const char * of, const char * values, bool is_exact_match )
{
        int len_val = strlen( values );
        char tmp_str[ 100 ];
        int tmp_ctr = 0;
        for( int i = 0; i <= len_val; ++i ) {
                if( is_whitespc( values[ i ] ) ) {
                        if( tmp_ctr == 0 ) continue;
                        tmp_str[ tmp_ctr ] = '\0';
                        if( is_exact_match ) {
                                if( strcasecmp( of, tmp_str ) == 0 ) return true;
                        }
                        else {
                                if( strncasecmp( of, tmp_str, strlen( tmp_str ) ) == 0 ) return true;
                        }
                        strcpy( tmp_str, "\0" );
                        tmp_ctr = 0;
                        continue;
                }
                tmp_str[ tmp_ctr++ ] = values[ i ];
        }
        return false;
}

void get_time( char * str )
{
	time_t t = time( NULL );
	struct tm tm = * localtime( & t );

	sprintf( str, "%d-%d-%d, %d:%d:%d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec );
}
