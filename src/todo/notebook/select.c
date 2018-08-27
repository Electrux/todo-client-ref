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

void handle_select_notebook( redisContext * c, const struct str_vec * svec, char * curr_nb )
{
	if( str_vec_get_count( svec ) < 2 ) {
		LOG( "Message: No argument to select notebook ???\n" );
		printf( "Usage: select notebook <name>\n" );
		return;
	}

	const char * nb = str_vec_get( svec, 1 );

	redisReply * repl;
	repl = redisCommand( c, "EXISTS notebooks:%s", nb );
	if( repl->integer != 1 ) {
		strcpy( curr_nb, "\0" );
		LOG( "Notebook %s does not exist\n", nb );
		printf( "Error: Notebook %s does not exist!\n", nb );
		freeReplyObject( repl );
		return;
	}
	else {
		LOG( "Notebook %s exists, continuing ...\n", nb );
	}
	freeReplyObject( repl );

	LOG( "Message: Selected notebook: %s\n", nb );
	printf( "Success: Selected notebook: %s\n", nb );
	strcpy( curr_nb, nb );
}
