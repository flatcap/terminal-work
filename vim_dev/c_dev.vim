" Enable some C syntax features
"let g:c_space_errors=1
let g:c_gnu=1

highlight link cCharacter	cString
highlight link cFormat		cString
highlight link cSpecial		cString

highlight link cFloat		cNumber
highlight link cOctal		cNumber
highlight link cOctalZero	cNumber
highlight link cFloat		cNumber

set path=.,/usr/include,/usr/include/glib-2.0,/usr/lib64/glib-2.0/include,/usr/include/gtk-2.0,/usr/lib64/gtk-2.0/include,/usr/include/cairo,/usr/include/pango-1.0
