filetype plugin indent on " load filetype plugins/indent settings

" (XXX: #VIM/tpope warns the line below could break things)
set iskeyword+=_,$,@,%,# " none of these are word dividers

au BufRead,BufNewFile *.rb,*.rhtml set shiftwidth=2 
au BufRead,BufNewFile *.rb,*.rhtml set softtabstop=2 

au BufNewFile,BufRead *.ahk setf ahk 


use joinspaces and cpoptions=j only for .txt
add cpoptions=m (showmatch)

