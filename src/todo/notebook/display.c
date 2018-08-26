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
#include <hiredis/hiredis.h>

#include "../../../include/core.h"

#include "../../../include/todo/notebook.h"

void handle_display_notebook( redisContext * c )
{
	redisReply * repl;
	bool nb_exists = true;
	repl = redisCommand( c, "EXISTS notebooks" );
	if( repl->integer == 0 ) {
		LOG( "Message: No notebooks exist yet\n" );
		printf( "Error: No notebooks exist yet, create some\n" );
		nb_exists = false;
	}
	freeReplyObject( repl );
	if( !nb_exists ) return;

	repl = redisCommand( c, "LRANGE notebooks 0 -1" );
	if( repl->type == REDIS_REPLY_ARRAY ) {
		printf( "Notebooks:\n" );
		for( int i = 0; i < ( int )repl->elements; ++i ) {
			printf( "\t%d. %s\n", i + 1, repl->element[ i ]->str );
		}
	}
	freeReplyObject( repl );
}
