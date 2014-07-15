#!/bin/bash

get_package_info()
{
	[ -z "$2" -o -n "$3" ] && return
	local dir="$1"
	local url="$2"

	mkdir -p "$dir"

	wget -O "$dir".txt.bz2 "$url" || return

	pushd "$dir"
	bzip2 -dc ../"$dir".txt.bz2 | awk -f ../split_packages.awk
	grep -l libvte * > ../"$dir"-vte.txt
	popd
}


get_package_info stable   http://www.mirrorservice.org/sites/ftp.debian.org/debian/dists/stable/main/binary-amd64/Packages.bz2
get_package_info unstable http://www.mirrorservice.org/sites/ftp.debian.org/debian/dists/unstable/main/binary-amd64/Packages.bz2
get_package_info testing  http://www.mirrorservice.org/sites/ftp.debian.org/debian/dists/testing/main/binary-amd64/Packages.bz2
