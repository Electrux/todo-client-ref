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

void handle_delete_notebook( redisContext * c, const struct str_vec * svec, char * curr_nb )
{
	if( str_vec_get_count( svec ) < 3 && strlen( curr_nb ) == 0 ) {
		LOG( "Message: No argument to delete notebook ???, nor any current notebook to delete\n" );
		printf( "Usage: delete notebook <name>, note that name isn't required if there is a notebook currently selected, which it isn't right now\n" );
		return;
	}

	bool using_curr_nb = false;
	if( str_vec_get_count( svec ) < 3 ) {
		using_curr_nb = true;
	}
	const char * nb = ( using_curr_nb ) ? curr_nb : str_vec_get( svec, 2 );

	redisReply * repl;
	repl = redisCommand( c, "EXISTS notebooks:%s", nb );
	if( repl->integer != 1 ) {
		LOG( "Notebook %s does not exist\n", nb );
		printf( "Error: Notebook %s does not exist!\n", nb );
		if( using_curr_nb ) strcpy( curr_nb, "\0" );
		freeReplyObject( repl );
		return;
	}
	else {
		LOG( "Notebook %s exists, continuing ...\n", nb );
	}
	freeReplyObject( repl );

	LOG( "Message: Deleting all notes in notebook %s ...\n", nb );
	repl = redisCommand( c, "LRANGE notebooks:%s:titles 0 -1", nb );
	for( int i = 0; i < ( int )repl->elements; ++i ) {
		LOG( "Message: Erasing note: %s ...\n", repl->element[ i ]->str );
		redisReply * tmprepl = redisCommand( c, "DEL notebooks:%s:titles:%s", nb, repl->element[ i ]->str );
		if( tmprepl->integer != 1 ) {
			LOG( "Warning: Failed to delete note %s, continuing ...\n", repl->element[ i ]->str );
		}
		freeReplyObject( tmprepl );
	}
	freeReplyObject( repl );

	LOG( "Message: Deleting note list from notebook ...\n" );
	repl = redisCommand( c, "DEL notebooks:%s:titles", nb );
	if( repl->integer == 1 ) {
		LOG( "Message: Successfully deleted note list\n" );
	}
	else {
		LOG( "Warning: Could not delete the note list of notebook %s\n", nb );
	}
	freeReplyObject( repl );

	LOG( "Message: Deleting notebook information ...\n" );
	repl = redisCommand( c, "DEL notebooks:%s", nb );
	if( repl->integer == 1 ) {
		LOG( "Message: Successfully deleted notebook information\n" );
	}
	else {
		LOG( "Warning: Could not delete the information of notebook %s\n", nb );
	}
	freeReplyObject( repl );

	LOG( "Message: Deleting notebook from notebooks list ...\n" );
	repl = redisCommand( c, "LREM notebooks 0 %s", nb );
	if( repl->integer >= 1 ) {
		LOG( "Message: Successfully deleted notebook from list\n" );
	}
	else {
		LOG( "Warning: Could not delete the notebook %s from list\n", nb );
	}
	freeReplyObject( repl );

	if( using_curr_nb || strcmp( curr_nb, str_vec_get( svec, 2 ) ) == 0 ) {
		strcpy( curr_nb, "\0" );
	}
	printf( "Notebook %s deleted successfully!\n", nb );
}
