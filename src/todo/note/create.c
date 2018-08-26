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

void handle_create_note( redisContext * c, const struct str_vec * svec, char * curr_nb )
{
	if( str_vec_get_count( svec ) < 5 ) {
		LOG( "Message: No argument to create note ??? ??? ??? ???\n" );
		printf( "Usage: create note <notebook> <name> <info> <priority>\n" );
		return;
	}

	int base_pos;
	const char * nb;
	if( str_vec_get_count( svec ) == 5 ) {
		base_pos = 0;
		nb = curr_nb;
	}
	else {
		base_pos = 1;
		nb = str_vec_get( svec, base_pos + 1 );
	}

	if( str_vec_get_count( svec ) == 5 && strlen( curr_nb ) == 0 ) {
		LOG( "Message: Only 3 arguments specified, but there is no current notebook selected\n" );
		printf( "Error: No current notebook selected!\n" );
		return;
	}
	const char * note_title = str_vec_get( svec, base_pos + 2 );
	const char * note_info = str_vec_get( svec, base_pos + 3 );
	const char * note_prio = str_vec_get( svec, base_pos + 4 );
	if( !IS( note_prio, "chill, cool, important, essential, sweating, fuck" ) ) {
		LOG( "Message: Invalid priority: %s specified\n", note_prio );
		printf( "Error: Priority must be one of chill, cool, important, essential, sweating, or fuck; invalid priority %s specified\n", note_prio );
		return;
	}
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

	repl = redisCommand( c, "EXISTS notebooks:%s:titles:%s", nb, note_title );
	if( repl->integer == 0 ) {
		LOG( "Message: Note %s does not exist, continuing ...\n", note_title );
	}
	else {
		LOG( "Message: Note %s already exists, cannot create again ...\n", note_title );
		printf( "Error: Note %s already exists!\n", note_title );
		freeReplyObject( repl );
		return;
	}
	freeReplyObject( repl );

	repl = redisCommand( c, "RPUSH notebooks:%s:titles %s", nb, note_title );
	if( repl->integer > 0 ) {
		LOG( "Message: Successfully added the note to the list of notes\n" );
	}
	freeReplyObject( repl );

	repl = redisCommand( c, "HSET notebooks:%s:titles:%s info %s", nb, note_title, note_info );
	if( repl->integer != 1 ) {
		LOG( "Error: Some error occurred setting info, value %lld\n", repl->integer );
		printf( "Error: Some error with %lld code occurred while setting info\n", repl->integer );
		freeReplyObject( repl );
		return;
	}
	freeReplyObject( repl );

	char time[ TIME_STR_LEN + 1 ];
	get_time( time );
	repl = redisCommand( c, "HSET notebooks:%s:titles:%s date %s", nb, note_title, time );
	if( repl->integer != 1 ) {
		LOG( "Error: Some error occurred setting date, value %lld\n", repl->integer );
		printf( "Error: Some error with %lld code occurred while setting date\n", repl->integer );
		freeReplyObject( repl );
		return;
	}
	freeReplyObject( repl );

	repl = redisCommand( c, "HSET notebooks:%s:titles:%s priority %s", nb, note_title, note_prio );
	if( repl->integer != 1 ) {
		LOG( "Error: Some error occurred setting priority, value %lld\n", repl->integer );
		printf( "Error: Some error with %lld code occurred while setting priority\n", repl->integer );
		freeReplyObject( repl );
		return;
	}
	freeReplyObject( repl );

	return;
}
