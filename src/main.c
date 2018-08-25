/*
	Copyright (c) 2018, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <hiredis/hiredis.h>

#include "../include/core.h"
#include "../include/redis_helper.h"
#include "../include/todo/todo_worker.h"

#define DEFAULT_IP "127.0.0.1"
#define DEFAULT_PORT 6379

int main( int argc, char ** argv )
{
	if( argc < 2 ) LOG( "Warning: IP and Port not provided, using default ones\n" );

	const char * ip = ( argc >= 2 ) ? argv[ 1 ] : DEFAULT_IP;
	const int port = ( argc >= 3 ) ? atoi( argv[ 2 ] ) : DEFAULT_PORT;

	LOG( "Message: Connecting to %s:%d ...\n", ip, port );

	struct timeval timeout = { 1, 500000 };
	redisContext * c = redisConnectWithTimeout( ip, port, timeout );

	if( c == NULL ) {
		LOG( "Error: Could not allocate redisContext, exiting\n" );
		return 1;
	}
	else if( c->err ) {
		LOG( "Error: redisContext allocated with error: %d, described as %s\n", c->err, c->errstr );
		redisFree( c );
		return c->err;
	}

	redisReply * repl;
	repl = redisCommand( c, "PING" );
	LOG( "Message: Ping server, received: %s\n", repl->str );
	bool auth_reqd;
	if( strcmp( repl->str, "PONG" ) == 0 ) {
		auth_reqd = false;
		LOG( "Message: Authentication not required, continuing ...\n" );
	}
	else if( strcmp( repl->str, "NOAUTH Authentication required." ) == 0 ) {
		auth_reqd = true;
		LOG( "Message: Authentication required, calling authentication function ...\n" );
	}
	else {
		LOG( "Error: Invalid response for Ping: %s, exiting\n", repl->str );
		freeReplyObject( repl );
		redisFree( c );
		return 1;
	}
	freeReplyObject( repl );

	if( auth_reqd && redis_authenticate( c ) != 0 ) {
		LOG( "Error: Failed authentication, exiting\n" );
		printf( "Authentication failed! Exiting!\n" );
		redisFree( c );
		return 1;
	}

	int ret_val = todo_worker( c );

	redisFree( c );
	return ret_val;
}
