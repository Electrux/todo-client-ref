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

#include "../../../include/todo/note.h"

void handle_delete_note( redisContext * c, const struct str_vec * svec, char * curr_nb )
{
	if( str_vec_get_count( svec ) < 4 && strlen( curr_nb ) == 0 ) {
		LOG( "Message: No argument to delete note ??? ???, nor any current notebook to delete from\n" );
		printf( "Usage: delete note <notebook> <title>, note that notebook isn't required if there is one currently selected, which it isn't right now\n" );
		return;
	}

	bool using_curr_nb = false;
	if( str_vec_get_count( svec ) < 4 ) {
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

	const char * note = str_vec_get( svec, ( ( using_curr_nb ) ? 2 : 3 ) );

	LOG( "Message: Deleting note from notebook list: %s ...\n", nb );
	repl = redisCommand( c, "LREM notebooks:%s:titles 0 %s", nb, note );
	if( repl->integer <= 0 ) {
		LOG( "Error: Note %s does not exist in notebook %s\n", note, nb );
		printf( "Error: Note %s does not exist in notebook %s\n", note, nb );
		freeReplyObject( repl );
		return;
	}
	freeReplyObject( repl );

	LOG( "Message: Deleting note information ...\n" );
	repl = redisCommand( c, "DEL notebooks:%s:titles:%s", nb, note );
	if( repl->integer == 1 ) {
		LOG( "Message: Successfully deleted the note information\n" );
	}
	else {
		LOG( "Warning: Could not delete the note information %s\n", note );
	}
	freeReplyObject( repl );

	LOG( "Message: Successfully deleted note %s\n", note );
	printf( "Success: Note %s has been deleted\n", note );
}
