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

void handle_alter_note( redisContext * c, const struct str_vec * svec, char * curr_nb )
{
	if( str_vec_get_count( svec ) < 5 ) {
		LOG( "Message: No argument to alter note ??? ??? ??? ???\n" );
		printf( "Usage: alter note <notebook> <note_title> <key> <value>, where notebook can be omitted if one is already selected\n" );
		return;
	}

	if( str_vec_get_count( svec ) == 5 && strlen( curr_nb ) == 0 ) {
		LOG( "Message: Only 3 arguments specified, but there is no current notebook selected\n" );
		printf( "Error: No current notebook selected!\n" );
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

	const char * note_title = str_vec_get( svec, base_pos + 2 );
	const char * note_key = str_vec_get( svec, base_pos + 3 );
	const char * note_val = str_vec_get( svec, base_pos + 4 );

	if( !IS( note_key, "info, priority" ) ) {
		LOG( "Message: Invalid property %s specified to alter\n", note_key );
		printf( "Error: Invalid property %s specified to alter in the note\n", note_key );
		return;
	}

	if( strcmp( note_key, "priority" ) == 0 && !IS( note_val, "chill, cool, important, essential, sweating, fuck" ) ) {
		LOG( "Message: Invalid priority: %s specified\n", note_val );
		printf( "Error: Priority must be one of chill, cool, important, essential, sweating, or fuck; invalid priority %s specified\n", note_val );
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
		LOG( "Message: Note %s does not exist, cannot continue\n", note_title );
		printf( "Error: Note %s does not exist!\n", note_title );
		freeReplyObject( repl );
		return;
	}
	freeReplyObject( repl );

	repl = redisCommand( c, "HSET notebooks:%s:titles:%s %s %s", nb, note_title, note_key, note_val );
	if( repl->integer != 1 ) {
		LOG( "Error: Some error occurred altering data, value %lld\n", repl->integer );
		printf( "Error: Some error with %lld code occurred while altering data\n", repl->integer );
		freeReplyObject( repl );
		return;
	}
	freeReplyObject( repl );

	LOG( "Message: Successfully updated note %s\n", note_title );
	printf( "Success: Updated %s in note %s\n", note_key, note_title );
	return;
}

void handle_set_accompl_date_note( redisContext * c, const struct str_vec * svec, char * curr_nb )
{
	if( str_vec_get_count( svec ) < 2 ) {
		LOG( "Message: No argument to finish ??? ???\n" );
		printf( "Usage: finish <notebook> <note_title>, where notebook can be omitted if one is already selected\n" );
		return;
	}

	if( str_vec_get_count( svec ) == 2 && strlen( curr_nb ) == 0 ) {
		LOG( "Message: Only 1 argument specified, but there is no current notebook selected\n" );
		printf( "Error: No current notebook selected!\n" );
		return;
	}

	int base_pos;
	const char * nb;
	if( str_vec_get_count( svec ) == 2 ) {
		base_pos = 0;
		nb = curr_nb;
	}
	else {
		base_pos = 1;
		nb = str_vec_get( svec, base_pos );
	}

	const char * note_title = str_vec_get( svec, base_pos + 1 );

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
		LOG( "Message: Note %s does not exist, cannot continue\n", note_title );
		printf( "Error: Note %s does not exist!\n", note_title );
		freeReplyObject( repl );
		return;
	}
	freeReplyObject( repl );

	char time[ TIME_STR_LEN + 1 ];
	get_time( time );
	repl = redisCommand( c, "HSET notebooks:%s:titles:%s accomplish_date %s", nb, note_title, time );
	if( repl->integer != 1 ) {
		LOG( "Error: Some error occurred in adding accomplish date, value %lld\n", repl->integer );
		printf( "Error: Some error with %lld code occurred while adding accomplish date\n", repl->integer );
		freeReplyObject( repl );
		return;
	}
	freeReplyObject( repl );

	LOG( "Message: Successfully added accomplish date for note %s\n", note_title );
	printf( "Success: Updated accomplish date in note %s\n", note_title );
	return;
}
