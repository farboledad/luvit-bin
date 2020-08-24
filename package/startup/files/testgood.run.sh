#!/bin/sh
echo "good starting $$"
for i in 1 2 3; do
	echo "good running $$ $i"
	sleep 5
done
echo "good exiting $$"
