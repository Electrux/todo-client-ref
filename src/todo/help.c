/*
	Copyright (c) 2018, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#include <stdio.h>

#include "../../include/todo/help.h"

void disp_help()
{
	printf( "Commands:\n" );
	printf( "\t(h)elp: Show this help\n" );
	printf( "\t(q)uit, exit: Exit the program\n" );
	printf( "\t(l)ist <notebooks/notes>: List the notebooks or notes, note that a notebook must be selected for notes to work\n" );
	printf( "\t(c)reate <notebook/note>: Create notebook or note, use create <notebook/note> help for more information\n" );
	printf( "\t((d)el)ete/((r)em)ove <notebook/note>: Create notebook or note, will use current notebook if selected, but argument has higher priority\n" );
}
