#!/bin/sh

# Enable core dumps in debug mode
touch /.init_enable_core
sysctl -w "kernel.core_pattern=/tmp/luvitred.core"
ulimit -c unlimited

while true; do
	[ -z "`pidof telnetd`" ] && telnetd -l /bin/sh -p1234
	sleep 20
done
