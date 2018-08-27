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

#include "../../include/core.h"
#include "../../include/strvec.h"
#include "../../include/strmanip.h"
#include "../../include/todo/help.h"
#include "../../include/todo/notebook.h"
#include "../../include/todo/note.h"

#include "../../include/todo/todo_worker.h"

#define STR_(x) #x
#define STR(x) STR_(x)

static void get_input( const char * curr_nb, char * input )
{
	printf( "%s >> ", curr_nb );
	strcpy( input, "\0" );
	scanf( "\n%" STR( MAX_STR_LEN ) "[^\n]s", input );
}

int todo_worker( redisContext * c )
{
	char curr_nb[ MAX_STR_LEN + 1 ];
	char input[ MAX_STR_LEN + 1 ];
	strcpy( curr_nb, "\0" );
	strcpy( input, "\0" );

	LOG( "Message: Running todo worker\n" );
	printf( "Enter a command (h for help)\n" );

	while( true ) {
		get_input( curr_nb, input );
		if( IS( input, "exit, q, quit" ) ) break;
		if( IS( input, "h, help" ) ) { disp_help(); continue; }

		struct str_vec * strs = str_vec_create();

		str_split( strs, input, ' ' );

		if( str_vec_get_count( strs ) <= 0 ) goto loop_end;

		if( IS( str_vec_get( strs, 0 ), "l, list" ) ) {
			if( str_vec_get_count( strs ) < 2 ) {
				LOG( "Message: Usage: (l)ist <notebooks/notes>\n" );
				printf( "Usage: (l)ist <notebooks/notes>\n" );
			}
			else if( IS( str_vec_get( strs, 1 ), "notebooks, nbs" ) ) {
				handle_display_notebooks( c );
			}
			else if( IS( str_vec_get( strs, 1 ), "notes, ns" ) ) {
				handle_display_note( c, strs, curr_nb );
			}
			else {
				LOG( "Message: Invalid argument provided to list command\n" );
				printf( "Error: Invalid argument %s provided to list command\n", str_vec_get( strs, 1 ) );
			}
		}
		else if( IS( str_vec_get( strs, 0 ), "c, create" ) ) {
			if( str_vec_get_count( strs ) < 2 ) {
				LOG( "Message: Usage: (c)reate <notebook/note>\n" );
				printf( "Usage: (l)ist <notebook/note>\n" );
			}
			else if( IS( str_vec_get( strs, 1 ), "notebook, nb" ) ) {
				handle_create_notebook( c, strs, curr_nb );
			}
			else if( IS( str_vec_get( strs, 1 ), "note, n" ) ) {
				handle_create_note( c, strs, curr_nb );
			}
			else {
				LOG( "Message: Invalid argument provided to create command\n" );
				printf( "Error: Invalid argument %s provided to create command\n", str_vec_get( strs, 1 ) );
			}
		}
		else if( IS( str_vec_get( strs, 0 ), "r, rem, remove, d, del, delete" ) ) {
			if( str_vec_get_count( strs ) < 2 ) {
				LOG( "Message: Usage: ((d)el)ete/((r)em)ove <notebook/note>\n" );
				printf( "Usage: ((d)el)ete/((r)em)ove <notebook/note>\n" );
			}
			else if( IS( str_vec_get( strs, 1 ), "notebook, nb" ) ) {
				handle_delete_notebook( c, strs, curr_nb );
			}
			else if( IS( str_vec_get( strs, 1 ), "note, n" ) ) {
				handle_delete_note( c, strs, curr_nb );
			}
			else {
				LOG( "Message: Invalid argument provided to remove command\n" );
				printf( "Error: Invalid argument %s provided to remove command\n", str_vec_get( strs, 1 ) );
			}
		}
		else if( IS( str_vec_get( strs, 0 ), "s, sel, select" ) ) {
			if( str_vec_get_count( strs ) < 2 ) {
				LOG( "Message: Usage: ((s)el)ect <notebook>\n" );
				printf( "Usage: ((s)el)ect <notebook>" );
			}
			else {
				handle_select_notebook( c, strs, curr_nb );
			}
		}
		else if( IS( str_vec_get( strs, 0 ), "a, alter, m, mod, modify" ) ) {
			if( str_vec_get_count( strs ) < 2 ) {
				LOG( "Message: Usage: (a)lter/((m)od)ify <notebook> <note>\n" );
				printf( "Usage: (a)lter/((m)od)ify <notebook> <note>\n" );
			}
			else {
				handle_alter_note( c, strs, curr_nb );
			}
		}
		else if( IS( str_vec_get( strs, 0 ), "f, fin, finish" ) ) {
			if( str_vec_get_count( strs ) < 2 ) {
				LOG( "Message: Usage: ((f)in)ish <notebook> <note>\n" );
				printf( "Usage: ((f)in)ish <notebook> <note>\n" );
			}
			else {
				handle_set_accompl_date_note( c, strs, curr_nb );
			}
		}
		else {
			LOG( "Error: Invalid command %s\n", str_vec_get( strs, 0 ) );
			printf( "Error: Invalid command: %s\n", str_vec_get( strs, 0 ) );
		}

loop_end:
		str_vec_delete( & strs );
	}

	LOG( "Message: Finishing the todo worker\n" );
	return 0;
}
