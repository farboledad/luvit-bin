#!/bin/sh

# disable core files. Safety if device previously had a debug build.
rm /.init_enable_core &> /dev/null

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

if [ -n "$RUNCOMMANDS" ]
then
	while true; do
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
	done
else
	echo "no startups found"
fi
