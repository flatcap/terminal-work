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
