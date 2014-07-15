parse_git_branch()
{
	(
	BRANCH=$(git branch 2> /dev/null | sed -e '/^[^*]/d' -e 's/* \(.*\)/(\1)/')
	if [ -z "$BRANCH" ]; then
		DIR=$(pwd -P | sed 's!\(.*\)/\([[:alpha:]]\+\)[[:digit:]]*$!\1/git/\2!')
		[ -d "$DIR" ] && cd "$DIR" > /dev/null
		BRANCH=$(git branch 2> /dev/null | sed -e '/^[^*]/d' -e 's/* \(.*\)/(\1)/')
	fi
	echo $BRANCH
	)
}

