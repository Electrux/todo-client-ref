/*
	Copyright (c) 2018, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#ifndef CORE_H
#define CORE_H

#include <stdio.h>
#include <stdbool.h>

#define MAX_STR_LEN 1024
#define TIME_STR_LEN 50

#define LOG( ... ) fprintf( stderr, __VA_ARGS__ )

bool find_in( const char * of, const char * values, bool is_exact_match );

#define IS( of, values ) find_in( of, values, true )
#define BEGINS( of, values ) find_in( of, values, false )

void get_time( char * str );

#endif // CORE_H
