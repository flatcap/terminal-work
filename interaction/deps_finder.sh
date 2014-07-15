#!/bin/bash

FILE="$1"

[ -f "$FILE" ] || exit 1

for i in $(cat $FILE); do
	BASE=${i##*/}.dep
	[ -f $BASE ] && continue
	echo $BASE
	ldd $i | grep " => /" | sed -e 's/.* => //' -e 's/ .*//' > $BASE
done

