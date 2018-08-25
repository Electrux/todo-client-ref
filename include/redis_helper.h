/*
	Copyright (c) 2018, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#ifndef REDIS_HELPER_H
#define REDIS_HELPER_H

#include <hiredis/hiredis.h>

#define MAX_AUTH_ATTEMPTS 10

int redis_authenticate( redisContext * c );

#endif // REDIS_HELPER_H
