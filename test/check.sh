#!/bin/bash

CORES=16

red="\\033[1;31m"
grn="\\033[1;32m"
clr="\\033[0m"

rstatus=0
for x in *.x; do
	if [ -e "$x" ]; then
	for ((n=1; n<=$CORES; n++)); do
			printf "%-3d%-27.27s " $n "$x .........................."
			../tools/oshrun -np $n ./$x 2>&1 | grep -q -i "ERROR"
			xstatus=( ${PIPESTATUS[*]} )
			if [ ${xstatus[0]} != 0 ]; then
				echo -e "[${red}RETURNED ${xstatus[0]}${clr}]"
				rstatus=${xstatus[0]}
			else
				if [ ${xstatus[1]} = 0 ]; then
					echo -e "[${red}ERROR${clr}]"
					rstatus=${xstatus[1]}
				else
					echo -e "[${grn}pass${clr}]"
				fi
			fi
	done
	else
		echo -e "$x not found. Try 'make' first."
		break
	fi
done

exit $rstatus
