/*
	Copyright (c) 2018, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#include <string.h>
#include <hiredis/hiredis.h>

#include "../../../include/core.h"
#include "../../../include/strvec.h"

#include "../../../include/todo/notebook.h"

void handle_create_notebook( redisContext * c, const struct str_vec * svec, char * curr_nb )
{
	if( str_vec_get_count( svec ) < 3 ) {
		LOG( "Message: No argument to create notebook ???\n" );
		printf( "Usage: create notebook <name>\n" );
		return;
	}

	const char * nb = str_vec_get( svec, 2 );

	strcpy( curr_nb, nb );

	redisReply * repl;
	repl = redisCommand( c, "EXISTS notebooks:%s", nb );
	if( repl->integer == 1 ) {
		LOG( "Notebook %s already exists\n", nb );
		printf( "Error: Notebook %s already exists\n", nb );
		freeReplyObject( repl );
		return;
	}
	else {
		LOG( "Notebook %s does not exist, continuing ...\n", nb );
	}
	freeReplyObject( repl );

	char date[ TIME_STR_LEN + 1 ];
	get_time( date );

	repl = redisCommand( c, "HSET notebooks:%s date_created %s", nb, date );
	if( repl->integer == 1 ) {
		LOG( "Message: Date_created for notebook %s is set successfully\n", nb );
	}
	else {
		strcpy( curr_nb, "\0" );
		printf( "Notebook set date created failed, returned: %lld", repl->integer );
		freeReplyObject( repl );
		return;
	}
	freeReplyObject( repl );

	repl = redisCommand( c, "HSET notebooks:%s date_modified %s", nb, date );
	if( repl->integer == 1 ) {
		LOG( "Message: Date_modified for notebook %s is set successfully\n", nb );
	}
	else {
		strcpy( curr_nb, "\0" );
		printf( "Notebook set date modified failed, returned: %lld", repl->integer );
		freeReplyObject( repl );
		return;
	}
	freeReplyObject( repl );

	repl = redisCommand( c, "RPUSH notebooks %s", nb );
	if( repl->integer == 1 ) {
		LOG( "Message: Notebook %s is added successfully\n", nb );
	}
	else {
		strcpy( curr_nb, "\0" );
		printf( "Notebook creation failed, returned: %lld", repl->integer );
		freeReplyObject( repl );
		return;
	}
	freeReplyObject( repl );

	printf( "Notebook %s created successfully!\n", nb );
}
