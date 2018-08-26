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
#include "../../../include/colors.h"
#include "../../../include/strvec.h"

#include "../../../include/todo/note.h"

static const char * get_priority_color( const char * prio )
{
	if( strcmp( prio, "chill" ) == 0 ) { return COLORS[ GREEN ]; }
	if( strcmp( prio, "cool" ) == 0 ) { return COLORS[ CYAN ]; }
	if( strcmp( prio, "important" ) == 0 ) { return COLORS[ MAGENTA ]; }
	if( strcmp( prio, "essential" ) == 0 ) { return COLORS[ BLUE ]; }
	if( strcmp( prio, "sweating" ) == 0 ) { return COLORS[ YELLOW ]; }
	if( strcmp( prio, "fuck" ) == 0 ) { return COLORS[ RED ]; }

	return COLORS[ RESET ];
}

static void get_and_display_note( redisContext * c, const int i, const char * nb, const char * note )
{
	redisReply * repl;
	repl = redisCommand( c, "EXISTS notebooks:%s:titles:%s", nb, note );
	if( repl->integer == 0 ) {
		LOG( "Warning: Note %s from notebook %s does not exist, removing from notebook list\n", note, nb );
		freeReplyObject( repl );
		repl = redisCommand( c, "LREM notebooks:%s:titles 0 %s", nb, note );
		if( repl->integer > 0 ) {
			LOG( "Message: Note %s removed from notebook list %s\n", note, nb );
		}
		else {
			LOG( "Message: What the ****? Note %s does not exist in the notebook %s list? How did we get here then?!\n", note, nb );
		}
		freeReplyObject( repl );
		return;
	}
	freeReplyObject( repl );

	// get note info
	char note_info[ MAX_STR_LEN + 1 ];
	char note_date[ TIME_STR_LEN + 1 ];
	char note_date_accompl[ TIME_STR_LEN + 1 ];
	char note_prio[ MAX_PRIO_LEN + 1 ];

	LOG( "Message: Fetching note information ...\n" );
	repl = redisCommand( c, "HGET notebooks:%s:titles:%s info", nb, note );
	strcpy( note_info, repl->str );
	freeReplyObject( repl );
	LOG( "Message: Fetched note information: %s\n", note_info );

	LOG( "Message: Fetching note creation date ...\n" );
	repl = redisCommand( c, "HGET notebooks:%s:titles:%s date", nb, note );
	strcpy( note_date, repl->str );
	freeReplyObject( repl );
	LOG( "Message: Fetched note creation date: %s\n", note_date );

	bool accomplished;

	LOG( "Message: Checking if note has been accomplished ...\n" );
	repl = redisCommand( c, "HEXISTS notebooks:%s:titles:%s accomplish_date", nb, note );
	accomplished = repl->integer;
	freeReplyObject( repl );
	LOG( "Message: Note %s has not been finished\n", note );

	if( accomplished ) {
		LOG( "Message: Fetching note accomplish date ...\n" );
		repl = redisCommand( c, "HGET notebooks:%s:titles:%s accomplish_date", nb, note );
		strcpy( note_date_accompl, repl->str );
		freeReplyObject( repl );
		LOG( "Message: Fetched note accomplish date: %s\n", note_date_accompl );
	}

	LOG( "Message: Fetching note priority ...\n" );
	repl = redisCommand( c, "HGET notebooks:%s:titles:%s priority", nb, note );
	strcpy( note_prio, repl->str );
	freeReplyObject( repl );
	LOG( "Message: Fetched note priority: %s\n", note_prio );

	if( accomplished ) {
		printf( "\t%d -\t%s\t%s\t%s\t%s\t%s%s%s\n", i + 1, note, note_info,
			note_date, note_date_accompl, get_priority_color( note_prio ),
			note_prio, COLORS[ RESET ] );
	}
	else {
		printf( "\t%d -\t%s\t%s\t%s\t\t\t\t\t%s%s%s\n", i + 1, note, note_info,
			note_date, get_priority_color( note_prio ),
			note_prio, COLORS[ RESET ] );
	}
}

void handle_display_note( redisContext * c, const struct str_vec * svec, char * curr_nb )
{
	if( str_vec_get_count( svec ) < 3 && strlen( curr_nb ) == 0 ) {
		LOG( "Message: No argument to list note ??? nor is any notebook selected\n" );
		printf( "Usage: list note <notebook>, notebook argument can be omitted if a notebook is selected\n" );
		return;
	}

	const char * nb = ( str_vec_get_count( svec ) < 3 ) ? curr_nb : str_vec_get( svec, 2 );

	redisReply * repl;
	bool nb_exists = true;
	repl = redisCommand( c, "EXISTS notebooks:%s", nb );
	if( repl->integer == 0 ) {
		LOG( "Message: No notebook by the name %s exists\n", nb );
		printf( "Error: No notebook exists by the name %s\n", nb );
		if( nb == curr_nb ) strcpy( curr_nb, "\0" );
		nb_exists = false;
	}
	freeReplyObject( repl );
	if( !nb_exists ) return;

	repl = redisCommand( c, "EXISTS notebooks:%s:titles", nb );
	if( repl->integer == 0 ) {
		LOG( "Message: No note exists in %s\n", nb );
		printf( "Error: Notebook %s is empty!\n", nb );
		freeReplyObject( repl );
		return;
	}
	freeReplyObject( repl );

	repl = redisCommand( c, "LRANGE notebooks:%s:titles 0 -1", nb );
	LOG( "Message: Displaying %zu notes ...\n", repl->elements );
	printf( "Notes:\n" );
	printf( "\tNo. -\tName\tInfo\tDate created\t\tDate accomplished\t\tPriority\n\n" );
	for( int i = 0; i < ( int )repl->elements; ++i ) {
		const char * note_title = repl->element[ i ]->str;
		get_and_display_note( c, i, nb, note_title );
	}
	freeReplyObject( repl );

	return;
}
