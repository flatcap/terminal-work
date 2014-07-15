" " Numeric 123
" map [4~	Gzb
" map OB	j
" map [6~	

" " Numeric 456
" map OD	h
" "map 	zz
" map OC	l

" " Numeric 789
" map [1~	1G
" map OA	k
" map [5~	

" " Numeric 0.
" map [2~	i
" map [3~	x

" Make shift-enter do the right thing
imap OM <CR>

" Stop shift-up & -down moving a page
map O2A OA
map O2B OB

map <F8> yypkf-  23r 11l8r ojer8  24r 29l8r ^f-
map <F7> :tn<CR>

" map ctrl-arrow to arrow
map O5A OA
map O5D OD
map O5C OC
map O5B OB

"silent map <F3> :on<CR>:se ls=1<CR><C-L>
"silent map <F4> :se ls=0<CR>:40vs ntfsrm.h<CR><C-L><C-W><C-W>

let mapleader=','
"map <Leader>f :e terminal-find.c<CR>:set fdm=expr<CR><C-G>
"map <Leader>w :e terminal-window.c<CR>
"map <Leader>a :e terminal-app.c<CR>
"map <Leader>s :e terminal-screen.c<CR>
"map <Leader>u :e terminal-util.c<CR>
"
map <Leader>p :e %:p:s,\..*$,.php,<CR>
map <Leader>j :e %:p:s,\..*$,.js,<CR>
map <Leader>c :e %:p:s,\..*$,.css,<CR>
map <Leader>h :e %:p:s,\..*$,.html,<CR>
map <Leader>s :tabnew<CR>:set bt=nofile<CR>

" Shortcuts to common files
nmap <Leader>t :e terminal.c<CR>
nmap <Leader>m :e main.c<CR>
nmap <Leader>v :e remnants/vte.c<CR>
nmap <Leader>s :e rseq-vte.c<CR>
nmap <Leader>l :ls<CR>
nmap <Leader>b :b<Space>

"noremap <silent> <C-]> :vsplit<CR><C-]>
"nnoremap <silent> <Space> @=(foldlevel('.')?'za':'l')<CR>
"vnoremap <Space> zf

nmap <silent> <Leader>!  :source ~/.vim/debugprint.vim<Bar>:source ~/.vim/static_vte.vim<CR>

