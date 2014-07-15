umask 0007

if [ -n "$DEBUG_APP" ]; then
	export PS1="\\T\\$ "
	unset PROMPT_COMMAND
	unset HISTCONTROL
	unset HISTFILE
	unset HISTFILESIZE
	unset HISTIGNORE
	unset HISTSIZE
	export PROMPT_COMMAND='PROMPT_COUNT=$((PROMPT_COUNT+1)); export PS1=PROMPT_$PROMPT_COUNT\\$\ '
	#export PROMPT_COMMAND=$PROMPT_COMMAND'; if [ $RANDOM -le 25000 ]; then printf "\0337\033[%d;%dH\033[4%dmX\033[m\0338" $((RANDOM%LINES+1)) $((RANDOM%COLUMNS+1)) $((RANDOM%8)); fi'
	T=/tmp/app-$RANDOM
	mkdir -p $T
	cd $T
	alias c="reset; ~/notes/src/streams/list | sed -n 4p"
else

export BZIP2="-9"
export CDPATH=".:..:~"
export EDITOR="/usr/bin/vim"
export FIGNORE=".swp:.o"
export GREP_COLOR="33"
export GREP_OPTIONS="--colour=auto --directories=skip --binary-files=without-match --exclude=tags"
export GZIP="-9"
export HISTCONTROL="ignorespace:erasedups"
export HISTFILE="/home/work/terminal/.bash_history"
export HISTFILESIZE="10000"
export HISTIGNORE="&:;*:\!:%*:bg:cd:cd -:exit:fg:l:la:lc:li:ls:ls -a:ls -al:ls -C:ls -l:m"
export HISTSIZE="10000"
#export HISTTIMEFORMAT="%a %H:%M  "
export HTML_TIDY="/home/work/terminal/.tidy"
export INPUTRC="/home/work/terminal/.inputrc"
export KBUILD_VERBOSE="0"
export LESSCHARSET="iso8859"
export LESS="-eFfiqrX"
export LS_COLORS="no=00:fi=00:di=01;33:ln=36:pi=35:so=35:bd=34:cd=34:ex=31:*.cmd=31:*.exe=31:*.com=31:*.btm=31:*.bat=31:*.rpm=32:*.tar=32:*.xz=32:*.tgz=32:*.bz2=32:*.lzma=32:*.zip=32:*.z=32:*.Z=32:*.gz=32:*.ha=32:*.jpg=35:*.gif=35:*.png=35:*.xbm=35:*.xpm=35:*.bmp=35:*.mkv=35:*.avi=35:*.mpg=35:*.mp3=35:*.mp4=35:*.flv=35:*.a=32:*.swp=30:*.dcf=32:*.dcu=01;35:*.o=30"
export MAKEFLAGS="-j4"
export PAGER="less"
export PATH=".:~/bin:/bin:/usr/bin:/usr/local/bin:/sbin:/usr/sbin"
export TZ="Europe/London"
export WINDOWMANAGER="gnome"
export WORK_DIR="/home/work/terminal"
export WWW_HOME="file:///home/work/terminal/.home/index.html"
export XZ_OPT="-9"

case $(tty) in
	/dev/tty[0-9])
		TERM=linux
		PROMPT_COMMAND='echo -n '
		;;
	/dev/ttyp*|/dev/pts/*)
		TERM=xterm
		PROMPT_COMMAND='echo -n ; echo -ne "\033]0;${PWD/#$HOME/~}\007"'
		;;
	*)	
		TERM=vt100
		PROMPT_COMMAND='echo -n '
		;;
esac

export PROMPT_PREFIX="D"
export PROMPT_COMMAND TERM
PS1="\w\$(parse_git_branch)\\$ "
[ -n "$PROMPT_PREFIX" ] && PS1="[$PROMPT_PREFIX]$PS1"
export PS1

export LESS_TERMCAP_mb=$'\E[01;31m'
export LESS_TERMCAP_md=$'\E[01;34m'
export LESS_TERMCAP_me=$'\E[0m'
export LESS_TERMCAP_se=$'\E[0m'
export LESS_TERMCAP_so=$'\E[01;44;33m'
export LESS_TERMCAP_ue=$'\E[0m'
export LESS_TERMCAP_us=$'\E[01;32m'

unset MAILCHECK

shopt -s autocd
shopt -s cdspell
shopt -s extglob
shopt -s globstar
shopt -s progcomp

for dir in "function.d" "debug.d" "complete.d"; do
	if [ -d "/home/work/terminal/bin/$dir" ]; then
		for file in "/home/work/terminal/bin/$dir/"*; do
			source $file
		done
	fi
done

title

fi
