/*
	Copyright (c) 2018, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#ifndef TODO_TODO_WORKER_H
#define TODO_TODO_WORKER_H

#include <hiredis/hiredis.h>

int todo_worker( redisContext * c );

#endif // TODO_TODO_WORKER_H
