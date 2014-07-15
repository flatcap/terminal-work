#!/bin/bash

WAIT="0.1"
echo -en "\033"; sleep $WAIT
echo -en "]"; sleep $WAIT
echo -en "2"; sleep $WAIT
echo -en ";"; sleep $WAIT
echo -en "$RANDOM"; sleep $WAIT
for ((i = 0; i < 210000; i++)); do
	echo -en "$i;"
	#sleep $WAIT
done
echo -en "\007"; sleep $WAIT
echo

