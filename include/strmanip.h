/*
	Copyright (c) 2018, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.

	Please read that file and understand the license terms
	before using or altering the project.
*/

#ifndef STRMANIP_H
#define STRMANIP_H

#include "strvec.h"

void str_split( struct str_vec * svec, const char * str, const char delim );

#endif // STRMANIP_H
