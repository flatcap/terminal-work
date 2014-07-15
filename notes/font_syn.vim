let name = 'font_key'
let fn_list = [
	\ 'font_info_measure_font',
	\ 'font_info_allocate',
	\ 'font_info_find_for_context',
	\ 'font_info_create_for_context',
	\ 'font_info_create_for_screen',
	\ 'font_info_create_for_widget',
	\ '_vte_draw_set_text_font',
	\ 'vte_terminal_ensure_font',
	\ 'vte_terminal_set_font_full_internal',
\ ]

for key in fn_list
	execute 'syntax keyword ' . name . ' ' . key
endfor

highlight name ctermfg=213

