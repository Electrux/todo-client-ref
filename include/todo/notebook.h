/*
	Copyright (c) 2018, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#ifndef TODO_NOTEBOOK_H
#define TODO_NOTEBOOK_H

#include <hiredis/hiredis.h>

#include "../strvec.h"

void display_notebook( redisContext * c );

void handle_create_notebook( redisContext * c, const struct str_vec * svec, char * curr_nb );
void handle_delete_notebook( redisContext * c, const struct str_vec * svec, char * curr_nb );

#endif // TODO_NOTEBOOK_H
