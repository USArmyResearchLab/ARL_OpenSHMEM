#!/bin/sh

# oshrun.sh
#
# Copyright (c) 2019 Brown Deer Technology, LLC. All rights reserved.
#
# This file is part of the ARL OpenSHMEM Reference Implementation software
# package. For license information, see the LICENSE file in the top level
# directory of the distribution.
#
#

#trap "kill_cleanup" INT TERM ERR

version() {
	echo "oshrun " \
		__VERSION_STRING" ("__VERSION_NAME")" \
		"build "__BUILD_STRING;
	echo "Copyright (C) 2019 Brown Deer Technology";
}

usage() {
	echo "Usage: oshrun <runner options> -np <#> <program> <program arguments>";
	echo "The arguments for oshrun are:";
	echo "  <runner options>     Options passed to underlying launcher:";
	echo "     -h, --help        Show this usage information.";
	echo "     --version         Show version information.";
	echo "  -np <#>              The number of PEs to be used in the execution.";
	echo "  <program>            The program executable to be launched.";
	echo "  <program arguments>  Flags and other parameters to pass to the program.";
}

ret_status=0

run_id=

child_pids=

normal_cleanup() {
	rm -f /dev/shm/shmem_init.$run_id.*
}

kill_cleanup() {
	echo "Terminating job ..."
	for p in $child_pids
	do
		kill -9 $p
	done
	normal_cleanup
}

main() {

	npes=1
	prog=""

	while [[ $# > 0 ]]
	do

		arg="$1"

		case $arg in

			-h|--help) usage;
			;;

			--version) version;
			;;

			-c|-n|--n|-np|--np) shift; npes=$1;
			;;

			-*) echo "Bad option: $1"; usage; exit -1;
			;;

			*) prog="$*"; break;
			;;

		esac

	shift

	done

	if [ $npes -lt 1 -o $npes -ge 65536 ];
	then
		echo oshrun: error: npes out of range
		exit -1
	fi

	if [ "$prog" = "" ];
	then
		echo oshrun: error: no program specified
		exit -1
	fi

	run_id=$$

	i=0; while [ $i -lt $npes ]; do
		SHMEM_RUN_ID=$$ SHMEM_MY_PE=$i SHMEM_N_PES=$npes $prog &
		child_pids+=" $!"
		i=$(( i + 1 ))
	done

	for p in $child_pids
	do
		wait ${p} ; child_pids[${p}]=$?
		if [[ ${child_pids[${p}]} -ne 0 ]]; then
			ret_status=${child_pids[${p}]}
			echo -e "[${p}] Exited with status: ${ret_status}"
		fi
	done

	normal_cleanup
	exit ${ret_status}
}

main $*