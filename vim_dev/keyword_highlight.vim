" Keyword highlighter
" Copyright 2010 Richard Russon (flatcap)

let s:key_list   = []
let s:key_init   = 0
let s:key_group  = 'key_group'
let s:key_colour = 'ctermbg=blue'

function! Keyword_Clear()
	if (s:key_init == 1)
		execute 'syntax clear ' s:key_group
	endif
endfunction

function! Keyword_Highlight()
	call Keyword_Clear()
	for item in s:key_list
		execute 'syntax keyword ' s:key_group item
	endfor
	if (!empty (s:key_list))
		let s:key_init = 1
	endif
	execute 'highlight ' s:key_group s:key_colour
endfunction

function! Keyword_Add(name)
	let i = index(s:key_list, a:name)
	if (i < 0)
		call add (s:key_list, a:name)
	endif
	call Keyword_Highlight()
endfunction

function! Keyword_Remove(name)
	let i = index(s:key_list, a:name)
	if (i >= 0)
		unlet s:key_list[i]
	endif
	call Keyword_Highlight()
endfunction

function! Keyword_Toggle(name)
	let i = index(s:key_list, a:name)
	if (i < 0)
		call Keyword_Add(a:name)
	else
		call Keyword_Remove(a:name)
	endif
endfunction

call Keyword_Clear()

noremap <2-LeftMouse> <LeftMouse>:call Keyword_Toggle("<C-R><C-W>")<CR>

