/*
  Copyright (c) 2018, Electrux
  All rights reserved.
  Using the BSD 3-Clause license for the project,
  main LICENSE file resides in project's root directory.
  Please read that file and understand the license terms
  before using or altering the project.
*/

#include <strings.h>

#include "../include/colors.h"

const char * COLORS[ LAST ] = {
	// Reset to no font customization
	"\033[0m",

	// Red, Green, Yellow, Blue, Magenta, Cyan, White colors
	"\033[0;31m",
	"\033[0;32m",
	"\033[0;33m",
	"\033[0;34m",
	"\033[0;35m",
	"\033[0;36m",
	"\033[0;37m",

	// Bold variants
	"\033[1;31m",
	"\033[1;32m",
	"\033[1;33m",
	"\033[1;34m",
	"\033[1;35m",
	"\033[1;36m",
	"\033[1;37m",

	// Primary, Secondary, Tertiary, and Extra colors
	"\033[0;35m",
	"\033[0;33m",
	"\033[0;37m",
	"\033[0;36m",
};
