
for i in range(0,255)
	let term = "XXX_".i
	execute 'syntax match '.term.' "\<XXX_'.i.'\>"'
	execute 'highlight '.term.' ctermfg='.i
endfor

