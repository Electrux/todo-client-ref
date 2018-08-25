/*
	Copyright (c) 2018, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <hiredis/hiredis.h>

#include "../include/core.h"

#include "../include/redis_helper.h"

int redis_authenticate( redisContext * c )
{
	LOG( "Message: Starting server authentication ...\n" );

	bool is_auth = false;
	redisReply * repl;
	int attempt_ctr = 0;

	printf( "Enter server authentication password: " );
	while( !is_auth && attempt_ctr < MAX_AUTH_ATTEMPTS ) {
		// \n in beginning to counter the skipped \n at the end of input
		char * pass = getpass( "" );
		if( strcmp( pass, "\x03" ) == 0 ) {
			LOG( "Message: ctrl-c encountered, stopping\n" );
			break;
		}
		LOG( "Message: Attempting server login with password: %s\n", pass );
		repl = redisCommand( c, "AUTH %s", pass );
		LOG( "Message: Received reply: %s\n", repl->str );
		if( strcmp( repl->str, "OK" ) == 0 ) {
			is_auth = true;
			LOG( "Message: Server authentication successful, continuing ...\n" );
		}
		else {
			++attempt_ctr;
			if( attempt_ctr < MAX_AUTH_ATTEMPTS ) {
				LOG( "Message: Incorrect password, please try again\n" );
				printf( "Enter server authentication password (remaining attemps: %d): ", MAX_AUTH_ATTEMPTS - attempt_ctr );
			}
		}
		freeReplyObject( repl );
	}

	return !is_auth;
}
