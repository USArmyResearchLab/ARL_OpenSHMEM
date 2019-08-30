/* oshrun.c
 *
 * Copyright (c) 2019 Brown Deer Technology, LLC. All rights reserved.
 *
 * This file is part of the ARL OpenSHMEM Reference Implementation software
 * package. For license information, see the LICENSE file in the top level
 * directory of the distribution.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "oshrun_sh.h"

int main( int argc, char** argv )
{
	int i;
	size_t maxlen = sysconf(_SC_ARG_MAX);
	size_t sz = tmp_oshrun_sh_len;

	for(i = 1; i < argc; i++) 
		sz += strnlen(argv[i], maxlen) + 1;

	char* cmd = (char*)malloc(sz);

	strncpy(cmd, (char*)tmp_oshrun_sh, tmp_oshrun_sh_len);

	for(i = 1; i < argc; i++) {
		strncat(cmd, " ", 1);
		strncat(cmd, argv[i], strnlen(argv[i], maxlen) + 1);
	}

	int ret = system(cmd);
	return(WEXITSTATUS(ret));
}
