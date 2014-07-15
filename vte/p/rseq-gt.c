static gboolean
vte_parse_color (const char *spec, GdkColor *color)
{
	gchar *spec_copy = (gchar *) spec;
	gboolean retval = FALSE;

	/* gdk_color_parse doesnt handle all XParseColor formats.  It only
	 * supports the #RRRGGGBBB format, not the rgb:RRR/GGG/BBB format.
	 * See: man XParseColor */

	if (g_ascii_strncasecmp (spec_copy, "rgb:", 4) == 0) {
		gchar *cur, *ptr;

		spec_copy = g_strdup (spec);
		cur = spec_copy;
		ptr = spec_copy + 3;

		*cur++ = '#';
		while (*ptr++)
			if (*ptr != '/')
				*cur++ = *ptr;
		*cur++ = '\0';
	}

	retval = gdk_color_parse (spec_copy, color);

	if (spec_copy != spec)
		g_free (spec_copy);

	return retval;
}

/* Check how long a string of unichars is.  Slow version. */
static gssize
vte_unichar_strlen(gunichar *c)
{
	int i;
	for (i = 0; c[i] != 0; i++) ;
	return i;
}

/* Convert a wide character string to a multibyte string */
static gchar *
vte_ucs4_to_utf8 (VteTerminal *terminal, const guchar *in)
{
	gchar *out = NULL;
	guchar *buf = NULL, *bufptr = NULL;
	gsize inlen, outlen;
	VteConv conv;

	conv = _vte_conv_open ("UTF-8", VTE_CONV_GUNICHAR_TYPE);

	if (conv != VTE_INVALID_CONV) {
		inlen = vte_unichar_strlen ((gunichar *) in) * sizeof (gunichar);
		outlen = (inlen * VTE_UTF8_BPC) + 1;

		_vte_buffer_set_minimum_size (terminal->pvt->conv_buffer, outlen);
		buf = bufptr = terminal->pvt->conv_buffer->data;

		if (_vte_conv (conv, &in, &inlen, &buf, &outlen) == (size_t) -1) {
			_vte_debug_print (VTE_DEBUG_IO,
					  "Error converting %ld string bytes (%s), skipping.\n",
					  (long) _vte_buffer_length (terminal->pvt->outgoing),
					  g_strerror (errno));
			bufptr = NULL;
		} else {
			out = g_strndup ((gchar *) bufptr, buf - bufptr);
		}
	}

	_vte_conv_close (conv);

	return out;
}

/* Set icon/window titles. */
static void
vte_sequence_handler_set_title_internal(VteTerminal *terminal,
					GValueArray *params,
					gboolean icon_title,
					gboolean window_title)
{
	GValue *value;
	char *title = NULL;

	if (icon_title == FALSE && window_title == FALSE)
		return;

	/* Get the string parameter's value. */
	value = g_value_array_get_nth(params, 0);
	if (value) {
		if (G_VALUE_HOLDS_LONG(value)) {
			/* Convert the long to a string. */
			title = g_strdup_printf("%ld", g_value_get_long(value));
		} else
		if (G_VALUE_HOLDS_STRING(value)) {
			/* Copy the string into the buffer. */
			title = g_value_dup_string(value);
		} else
		if (G_VALUE_HOLDS_POINTER(value)) {
			title = vte_ucs4_to_utf8 (terminal, g_value_get_pointer (value));
		}
		if (title != NULL) {
			char *p, *validated;
			const char *end;

			/* Validate the text. */
			g_utf8_validate(title, strlen(title), &end);
			validated = g_strndup(title, end - title);

			/* No control characters allowed. */
			for (p = validated; *p != '\0'; p++) {
				if ((*p & 0x1f) == *p) {
					*p = ' ';
				}
			}

			/* Emit the signal */
			if (window_title) {
				g_free (terminal->pvt->window_title_changed);
				terminal->pvt->window_title_changed = g_strdup (validated);
			}

			if (icon_title) {
				g_free (terminal->pvt->icon_title_changed);
				terminal->pvt->icon_title_changed = g_strdup (validated);
			}

			g_free (validated);
			g_free(title);
		}
	}
}

/* Emit a "refresh-window" signal. */
static void
vte_terminal_emit_refresh_window(VteTerminal *terminal)
{
	_vte_debug_print(VTE_DEBUG_SIGNALS,
			"Emitting `refresh-window'.\n");
	g_signal_emit_by_name(terminal, "refresh-window");
}

/* Emit a "deiconify-window" signal. */
static void
vte_terminal_emit_deiconify_window(VteTerminal *terminal)
{
	_vte_debug_print(VTE_DEBUG_SIGNALS,
			"Emitting `deiconify-window'.\n");
	g_signal_emit_by_name(terminal, "deiconify-window");
}

/* Emit a "iconify-window" signal. */
static void
vte_terminal_emit_iconify_window(VteTerminal *terminal)
{
	_vte_debug_print(VTE_DEBUG_SIGNALS,
			"Emitting `iconify-window'.\n");
	g_signal_emit_by_name(terminal, "iconify-window");
}

/* Emit a "raise-window" signal. */
static void
vte_terminal_emit_raise_window(VteTerminal *terminal)
{
	_vte_debug_print(VTE_DEBUG_SIGNALS,
			"Emitting `raise-window'.\n");
	g_signal_emit_by_name(terminal, "raise-window");
}

/* Emit a "lower-window" signal. */
static void
vte_terminal_emit_lower_window(VteTerminal *terminal)
{
	_vte_debug_print(VTE_DEBUG_SIGNALS,
			"Emitting `lower-window'.\n");
	g_signal_emit_by_name(terminal, "lower-window");
}

/* Emit a "maximize-window" signal. */
static void
vte_terminal_emit_maximize_window(VteTerminal *terminal)
{
	_vte_debug_print(VTE_DEBUG_SIGNALS,
			"Emitting `maximize-window'.\n");
	g_signal_emit_by_name(terminal, "maximize-window");
}

/* Emit a "restore-window" signal. */
static void
vte_terminal_emit_restore_window(VteTerminal *terminal)
{
	_vte_debug_print(VTE_DEBUG_SIGNALS,
			"Emitting `restore-window'.\n");
	g_signal_emit_by_name(terminal, "restore-window");
}

/* Emit a "move-window" signal.  (Pixels.) */
static void
vte_terminal_emit_move_window(VteTerminal *terminal, guint x, guint y)
{
	_vte_debug_print(VTE_DEBUG_SIGNALS,
			"Emitting `move-window'.\n");
	g_signal_emit_by_name(terminal, "move-window", x, y);
}

/* Emit a "resize-window" signal.  (Pixels.) */
static void
vte_terminal_emit_resize_window(VteTerminal *terminal,
				guint width, guint height)
{
	_vte_debug_print(VTE_DEBUG_SIGNALS,
			"Emitting `resize-window'.\n");
	g_signal_emit_by_name(terminal, "resize-window", width, height);
}

//------------------------------------------------------------------------------

/* End alternate character set. */
static void
vte_sequence_handler_ae (VteTerminal *terminal, GValueArray *params)
{
	terminal->pvt->screen->alternate_charset = FALSE;
}

/* Start using alternate character set. */
static void
vte_sequence_handler_as (VteTerminal *terminal, GValueArray *params)
{
	terminal->pvt->screen->alternate_charset = TRUE;
}

/* Beep. */
static void
vte_sequence_handler_bl (VteTerminal *terminal, GValueArray *params)
{
	_vte_terminal_beep (terminal);
	g_signal_emit_by_name(terminal, "beep");
}

/* Start using alternate character set. */
static void
vte_sequence_handler_eA (VteTerminal *terminal, GValueArray *params)
{
	vte_sequence_handler_ae (terminal, params);
}

/* Visible bell. */
static void
vte_sequence_handler_vb (VteTerminal *terminal, GValueArray *params)
{
	_vte_terminal_visible_beep (terminal);
}

/* Cursor visible. */
static void
vte_sequence_handler_ve (VteTerminal *terminal, GValueArray *params)
{
	terminal->pvt->cursor_visible = TRUE;
}

/* Cursor invisible. */
static void
vte_sequence_handler_vi (VteTerminal *terminal, GValueArray *params)
{
	terminal->pvt->cursor_visible = FALSE;
}

/* Cursor standout. */
static void
vte_sequence_handler_vs (VteTerminal *terminal, GValueArray *params)
{
	terminal->pvt->cursor_visible = TRUE; /* FIXME: should be *more*
						 visible. */
}

/* Set the application or normal keypad. */
static void
vte_sequence_handler_application_keypad (VteTerminal *terminal, GValueArray *params)
{
	_vte_debug_print(VTE_DEBUG_KEYBOARD,
			"Entering application keypad mode.\n");
	terminal->pvt->keypad_mode = VTE_KEYMODE_APPLICATION;
}

/* change color in the palette */
static void
vte_sequence_handler_change_color (VteTerminal *terminal, GValueArray *params)
{
	gchar **pairs, *str = NULL;
	GValue *value;
	GdkColor color;
	guint idx, i;

	if (params != NULL && params->n_values > 0) {
		value = g_value_array_get_nth (params, 0);

		if (G_VALUE_HOLDS_STRING (value))
			str = g_value_dup_string (value);
		else if (G_VALUE_HOLDS_POINTER (value))
			str = vte_ucs4_to_utf8 (terminal, g_value_get_pointer (value));

		if (! str)
			return;

		pairs = g_strsplit (str, ";", 0);
		if (! pairs) {
			g_free (str);
			return;
		}

		for (i = 0; pairs[i] && pairs[i + 1]; i += 2) {
			idx = strtoul (pairs[i], (char **) NULL, 10);

			if (idx >= VTE_DEF_FG)
				continue;

			if (vte_parse_color (pairs[i + 1], &color)) {
				terminal->pvt->palette[idx].red = color.red;
				terminal->pvt->palette[idx].green = color.green;
				terminal->pvt->palette[idx].blue = color.blue;
			} else if (strcmp (pairs[i + 1], "?") == 0) {
				gchar buf[128];
				g_snprintf (buf, sizeof (buf),
					    _VTE_CAP_OSC "4;%u;rgb:%04x/%04x/%04x" BEL, idx,
					    terminal->pvt->palette[idx].red,
					    terminal->pvt->palette[idx].green,
					    terminal->pvt->palette[idx].blue);
				vte_terminal_feed_child (terminal, buf, -1);
			}
		}

		g_free (str);
		g_strfreev (pairs);

		/* emit the refresh as the palette has changed and previous
		 * renders need to be updated. */
		vte_terminal_emit_refresh_window (terminal);
	}
}

/* Change the color of the cursor */
static void
vte_sequence_handler_change_cursor_color (VteTerminal *terminal, GValueArray *params)
{
	gchar *name = NULL;
	GValue *value;
	GdkColor color;

	if (params != NULL && params->n_values > 0) {
		value = g_value_array_get_nth (params, 0);

		if (G_VALUE_HOLDS_STRING (value))
			name = g_value_dup_string (value);
		else if (G_VALUE_HOLDS_POINTER (value))
			name = vte_ucs4_to_utf8 (terminal, g_value_get_pointer (value));

		if (! name)
			return;

		if (vte_parse_color (name, &color))
			vte_terminal_set_color_cursor (terminal, &color);
		else if (strcmp (name, "?") == 0) {
			gchar buf[128];
			g_snprintf (buf, sizeof (buf),
				    _VTE_CAP_OSC "12;rgb:%04x/%04x/%04x" BEL,
				    terminal->pvt->palette[VTE_CUR_BG].red,
				    terminal->pvt->palette[VTE_CUR_BG].green,
				    terminal->pvt->palette[VTE_CUR_BG].blue);
			vte_terminal_feed_child (terminal, buf, -1);
		}

		g_free (name);
	}
}

/* Set the terminal encoding. */
static void
vte_sequence_handler_local_charset (VteTerminal *terminal, GValueArray *params)
{
	G_CONST_RETURN char *locale_encoding;
	g_get_charset(&locale_encoding);
	vte_terminal_set_encoding(terminal, locale_encoding);
}

static void
vte_sequence_handler_normal_keypad (VteTerminal *terminal, GValueArray *params)
{
	_vte_debug_print(VTE_DEBUG_KEYBOARD,
			"Leaving application keypad mode.\n");
	terminal->pvt->keypad_mode = VTE_KEYMODE_NORMAL;
}

/* Set both the window and icon titles to the same string. */
static void
vte_sequence_handler_set_icon_and_window_title (VteTerminal *terminal, GValueArray *params)
{
	vte_sequence_handler_set_title_internal(terminal, params, TRUE, TRUE);
}

/* Set one or the other. */
static void
vte_sequence_handler_set_icon_title (VteTerminal *terminal, GValueArray *params)
{
	vte_sequence_handler_set_title_internal(terminal, params, TRUE, FALSE);
}

static void
vte_sequence_handler_set_window_title (VteTerminal *terminal, GValueArray *params)
{
	vte_sequence_handler_set_title_internal(terminal, params, FALSE, TRUE);
}

static void
vte_sequence_handler_utf_8_charset (VteTerminal *terminal, GValueArray *params)
{
	vte_terminal_set_encoding(terminal, "UTF-8");
}

/* Window manipulation control sequences.  Most of these are considered
 * bad ideas, but they're implemented as signals which the application
 * is free to ignore, so they're harmless. */
static void
vte_sequence_handler_window_manipulation (VteTerminal *terminal, GValueArray *params)
{
	GdkScreen *gscreen;
	VteScreen *screen;
	GValue *value;
	GtkWidget *widget;
	char buf[128];
	long param, arg1, arg2;
	gint width, height;
	guint i;

	widget = &terminal->widget;
	screen = terminal->pvt->screen;

	for (i = 0; ((params != NULL) && (i < params->n_values)); i++) {
		arg1 = arg2 = -1;
		if (i + 1 < params->n_values) {
			value = g_value_array_get_nth(params, i + 1);
			if (G_VALUE_HOLDS_LONG(value)) {
				arg1 = g_value_get_long(value);
			}
		}
		if (i + 2 < params->n_values) {
			value = g_value_array_get_nth(params, i + 2);
			if (G_VALUE_HOLDS_LONG(value)) {
				arg2 = g_value_get_long(value);
			}
		}
		value = g_value_array_get_nth(params, i);
		if (!G_VALUE_HOLDS_LONG(value)) {
			continue;
		}
		param = g_value_get_long(value);
		switch (param) {
		case 1:
			_vte_debug_print(VTE_DEBUG_PARSE,
					"Deiconifying window.\n");
			vte_terminal_emit_deiconify_window(terminal);
			break;
		case 2:
			_vte_debug_print(VTE_DEBUG_PARSE,
					"Iconifying window.\n");
			vte_terminal_emit_iconify_window(terminal);
			break;
		case 3:
			if ((arg1 != -1) && (arg2 != -2)) {
				_vte_debug_print(VTE_DEBUG_PARSE,
						"Moving window to "
						"%ld,%ld.\n", arg1, arg2);
				vte_terminal_emit_move_window(terminal,
							      arg1, arg2);
				i += 2;
			}
			break;
		case 4:
			if ((arg1 != -1) && (arg2 != -1)) {
				_vte_debug_print(VTE_DEBUG_PARSE,
						"Resizing window "
						"(to %ldx%ld pixels).\n",
						arg2, arg1);
				vte_terminal_emit_resize_window(terminal,
								arg2 +
								terminal->pvt->inner_border.left +
								terminal->pvt->inner_border.right,
								arg1 +
								terminal->pvt->inner_border.top +
								terminal->pvt->inner_border.bottom);
				i += 2;
			}
			break;
		case 5:
			_vte_debug_print(VTE_DEBUG_PARSE, "Raising window.\n");
			vte_terminal_emit_raise_window(terminal);
			break;
		case 6:
			_vte_debug_print(VTE_DEBUG_PARSE, "Lowering window.\n");
			vte_terminal_emit_lower_window(terminal);
			break;
		case 7:
			_vte_debug_print(VTE_DEBUG_PARSE,
					"Refreshing window.\n");
			_vte_invalidate_all(terminal);
			vte_terminal_emit_refresh_window(terminal);
			break;
		case 8:
			if ((arg1 != -1) && (arg2 != -1)) {
				_vte_debug_print(VTE_DEBUG_PARSE,
						"Resizing window "
						"(to %ld columns, %ld rows).\n",
						arg2, arg1);
				vte_terminal_emit_resize_window(terminal,
								arg2 * terminal->char_width +
								terminal->pvt->inner_border.left +
								terminal->pvt->inner_border.right,
								arg1 * terminal->char_height +
								terminal->pvt->inner_border.top +
								terminal->pvt->inner_border.bottom);
				i += 2;
			}
			break;
		case 9:
			switch (arg1) {
			case 0:
				_vte_debug_print(VTE_DEBUG_PARSE,
						"Restoring window.\n");
				vte_terminal_emit_restore_window(terminal);
				break;
			case 1:
				_vte_debug_print(VTE_DEBUG_PARSE,
						"Maximizing window.\n");
				vte_terminal_emit_maximize_window(terminal);
				break;
			default:
				break;
			}
			i++;
			break;
		case 11:
			/* If we're unmapped, then we're iconified. */
			g_snprintf(buf, sizeof(buf),
				   _VTE_CAP_CSI "%dt",
				   1 + !gtk_widget_get_mapped(widget));
			_vte_debug_print(VTE_DEBUG_PARSE,
					"Reporting window state %s.\n",
					gtk_widget_get_mapped(widget) ?
					"non-iconified" : "iconified");
			vte_terminal_feed_child(terminal, buf, -1);
			break;
		case 13:
			/* Send window location, in pixels. */
			gdk_window_get_origin(widget->window,
					      &width, &height);
			g_snprintf(buf, sizeof(buf),
				   _VTE_CAP_CSI "3;%d;%dt",
				   width + terminal->pvt->inner_border.left,
                                   height + terminal->pvt->inner_border.top);
			_vte_debug_print(VTE_DEBUG_PARSE,
					"Reporting window location"
					"(%d++,%d++).\n",
					width, height);
			vte_terminal_feed_child(terminal, buf, -1);
			break;
		case 14:
			/* Send window size, in pixels. */
			g_snprintf(buf, sizeof(buf),
				   _VTE_CAP_CSI "4;%d;%dt",
				   widget->allocation.height -
                                       (terminal->pvt->inner_border.top +
                                        terminal->pvt->inner_border.bottom),
				   widget->allocation.width -
                                       (terminal->pvt->inner_border.left +
                                        terminal->pvt->inner_border.right));
			_vte_debug_print(VTE_DEBUG_PARSE,
					"Reporting window size "
					"(%dx%dn",
					width - (terminal->pvt->inner_border.left + terminal->pvt->inner_border.right),
					height - (terminal->pvt->inner_border.top + terminal->pvt->inner_border.bottom));
			vte_terminal_feed_child(terminal, buf, -1);
			break;
		case 18:
			/* Send widget size, in cells. */
			_vte_debug_print(VTE_DEBUG_PARSE,
					"Reporting widget size.\n");
			g_snprintf(buf, sizeof(buf),
				   _VTE_CAP_CSI "8;%ld;%ldt",
				   terminal->row_count,
				   terminal->column_count);
			vte_terminal_feed_child(terminal, buf, -1);
			break;
		case 19:
			_vte_debug_print(VTE_DEBUG_PARSE,
					"Reporting screen size.\n");
			gscreen = gtk_widget_get_screen(widget);
			height = gdk_screen_get_height(gscreen);
			width = gdk_screen_get_width(gscreen);
			g_snprintf(buf, sizeof(buf),
				   _VTE_CAP_CSI "9;%ld;%ldt",
				   height / terminal->char_height,
				   width / terminal->char_width);
			vte_terminal_feed_child(terminal, buf, -1);
			break;
		case 20:
			/* Report the icon title. */
			_vte_debug_print(VTE_DEBUG_PARSE,
				"Reporting icon title.\n");
			g_snprintf (buf, sizeof (buf),
				    _VTE_CAP_OSC "L%s" _VTE_CAP_ST,
				    terminal->icon_title);
			vte_terminal_feed_child(terminal, buf, -1);
			break;
		case 21:
			/* Report the window title. */
			_vte_debug_print(VTE_DEBUG_PARSE,
					"Reporting window title.\n");
			g_snprintf (buf, sizeof (buf),
				    _VTE_CAP_OSC "l%s" _VTE_CAP_ST,
				    terminal->window_title);
			vte_terminal_feed_child(terminal, buf, -1);
			break;
		default:
			if (param >= 24) {
				_vte_debug_print(VTE_DEBUG_PARSE,
						"Resizing to %ld rows.\n",
					       	param);
				/* Resize to the specified number of
				 * rows. */
				vte_terminal_emit_resize_window(terminal,
								terminal->column_count * terminal->char_width +
                                                                terminal->pvt->inner_border.left +
                                                                terminal->pvt->inner_border.right,
								param * terminal->char_height +
								terminal->pvt->inner_border.top +
                                                                terminal->pvt->inner_border.bottom);
			}
			break;
		}
	}
}

