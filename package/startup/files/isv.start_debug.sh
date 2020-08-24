#!/bin/sh

# Enable core dumps in debug mode
touch /.init_enable_core
sysctl -w "kernel.core_pattern=/tmp/luvitred.core"
ulimit -c unlimited

INITCOMMANDS=$(find . -maxdepth 1 -name '*.init.sh' -exec basename {} \;)

if [ -n "$INITCOMMANDS" ]
then
	for COMMAND in $INITCOMMANDS
	do
		echo "starting $COMMAND"
		"./$COMMAND"
	done
else
	echo "no init commands found"
fi

RUNCOMMANDS=$(find . -maxdepth 1 -name '*.run.sh' -exec basename {} \;)

while true; do
	[ -z "`pidof telnetd`" ] && telnetd -l /bin/sh -p1234
	if [ -n "$RUNCOMMANDS" ]
	then
		for COMMAND in $RUNCOMMANDS
		do
			# for testing on PC this requires pidof -x
			if [ -z "`pidof $COMMAND`" ]
			then
				echo "starting $COMMAND"
				"./$COMMAND" &
			fi
		done
		sleep 20
	fi
done
