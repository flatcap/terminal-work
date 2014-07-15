/* Toggle a terminal mode. */
static void
vte_sequence_handler_set_mode_internal(VteTerminal *terminal,
				       long setting, gboolean value)
{
	switch (setting) {
	case 2:		/* keyboard action mode (?) */
		break;
	case 4:		/* insert/overtype mode */
		terminal->pvt->screen->insert_mode = value;
		break;
	case 12:	/* send/receive mode (local echo) */
		terminal->pvt->screen->sendrecv_mode = value;
		break;
	case 20:	/* automatic newline / normal linefeed mode */
		terminal->pvt->screen->linefeed_mode = value;
		break;
	default:
		break;
	}
}


//------------------------------------------------------------------------------

/* Clear all tab stops. */
static void
vte_sequence_handler_ct (VteTerminal *terminal, GValueArray *params)
{
	if (terminal->pvt->tabstops != NULL) {
		g_hash_table_destroy(terminal->pvt->tabstops);
		terminal->pvt->tabstops = NULL;
	}
}

/* DEC-style device status reports. */
static void
vte_sequence_handler_dec_device_status_report (VteTerminal *terminal, GValueArray *params)
{
	GValue *value;
	VteScreen *screen;
	long param;
	char buf[128];

	screen = terminal->pvt->screen;

	if ((params != NULL) && (params->n_values > 0)) {
		value = g_value_array_get_nth(params, 0);
		if (G_VALUE_HOLDS_LONG(value)) {
			param = g_value_get_long(value);
			switch (param) {
			case 6:
				/* Send the cursor position. */
				g_snprintf(buf, sizeof(buf),
					   _VTE_CAP_CSI "?%ld;%ldR",
					   screen->cursor_current.row + 1 -
					   screen->insert_delta,
					   screen->cursor_current.col + 1);
				vte_terminal_feed_child(terminal, buf, -1);
				break;
			case 15:
				/* Send printer status -- 10 = ready,
				 * 11 = not ready.  We don't print. */
				vte_terminal_feed_child(terminal, _VTE_CAP_CSI "?11n", -1);
				break;
			case 25:
				/* Send UDK status -- 20 = locked,
				 * 21 = not locked.  I don't even know what
				 * that means, but punt anyway. */
				vte_terminal_feed_child(terminal, _VTE_CAP_CSI "?20n", -1);
				break;
			case 26:
				/* Send keyboard status.  50 = no locator. */
				vte_terminal_feed_child(terminal, _VTE_CAP_CSI "?50n", -1);
				break;
			default:
				break;
			}
		}
	}
}

/* Device status reports. The possible reports are the cursor position and
 * whether or not we're okay. */
static void
vte_sequence_handler_device_status_report (VteTerminal *terminal, GValueArray *params)
{
	GValue *value;
	VteScreen *screen;
	long param;
	char buf[128];

	screen = terminal->pvt->screen;

	if ((params != NULL) && (params->n_values > 0)) {
		value = g_value_array_get_nth(params, 0);
		if (G_VALUE_HOLDS_LONG(value)) {
			param = g_value_get_long(value);
			switch (param) {
			case 5:
				/* Send a thumbs-up sequence. */
				vte_terminal_feed_child(terminal, _VTE_CAP_CSI "0n", -1);
				break;
			case 6:
				/* Send the cursor position. */
				g_snprintf(buf, sizeof(buf),
					   _VTE_CAP_CSI "%ld;%ldR",
					   screen->cursor_current.row + 1 -
					   screen->insert_delta,
					   screen->cursor_current.col + 1);
				vte_terminal_feed_child(terminal, buf, -1);
				break;
			default:
				break;
			}
		}
	}
}

/* End insert mode. */
static void
vte_sequence_handler_ei (VteTerminal *terminal, GValueArray *params)
{
	terminal->pvt->screen->insert_mode = FALSE;
}

/* Begin insert mode. */
static void
vte_sequence_handler_im (VteTerminal *terminal, GValueArray *params)
{
	terminal->pvt->screen->insert_mode = TRUE;
}

/* Protect on. */
static void
vte_sequence_handler_mp (VteTerminal *terminal, GValueArray *params)
{
	/* unused; bug 499893
	terminal->pvt->screen->defaults.attr.protect = 1;
	 */
}

/* No-op. */
static void
vte_sequence_handler_noop (VteTerminal *terminal, GValueArray *params)
{
}

/* Request terminal attributes. */
static void
vte_sequence_handler_request_terminal_parameters (VteTerminal *terminal, GValueArray *params)
{
	vte_terminal_feed_child(terminal, "\e[?x", -1);
}

/* Unset certain terminal attributes. */
static void
vte_sequence_handler_reset_mode (VteTerminal *terminal, GValueArray *params)
{
	guint i;
	long setting;
	GValue *value;
	if ((params == NULL) || (params->n_values == 0)) {
		return;
	}
	for (i = 0; i < params->n_values; i++) {
		value = g_value_array_get_nth(params, i);
		if (!G_VALUE_HOLDS_LONG(value)) {
			continue;
		}
		setting = g_value_get_long(value);
		vte_sequence_handler_set_mode_internal(terminal, setting, FALSE);
	}
}

/* Send terminal ID. */
static void
vte_sequence_handler_return_terminal_id (VteTerminal *terminal, GValueArray *params)
{
	vte_sequence_handler_send_primary_device_attributes (terminal, params);
}

/* Request terminal attributes. */
static void
vte_sequence_handler_return_terminal_status (VteTerminal *terminal, GValueArray *params)
{
	vte_terminal_feed_child(terminal, "", 0);
}

/* Send primary device attributes. */
static void
vte_sequence_handler_send_primary_device_attributes (VteTerminal *terminal, GValueArray *params)
{
	/* Claim to be a VT220 with only national character set support. */
	vte_terminal_feed_child(terminal, "\e[?62;9;c", -1);
}

/* Send secondary device attributes. */
static void
vte_sequence_handler_send_secondary_device_attributes (VteTerminal *terminal, GValueArray *params)
{
	char **version;
	char buf[128];
	long ver = 0, i;
	/* Claim to be a VT220, more or less.  The '>' in the response appears
	 * to be undocumented. */
	version = g_strsplit(VERSION, ".", 0);
	if (version != NULL) {
		for (i = 0; version[i] != NULL; i++) {
			ver = ver * 100;
			ver += atol(version[i]);
		}
		g_strfreev(version);
	}
	g_snprintf(buf, sizeof (buf), _VTE_CAP_ESC "[>1;%ld;0c", ver);
	vte_terminal_feed_child(terminal, buf, -1);
}

/* Set certain terminal attributes. */
static void
vte_sequence_handler_set_mode (VteTerminal *terminal, GValueArray *params)
{
	guint i;
	long setting;
	GValue *value;
	if ((params == NULL) || (params->n_values == 0)) {
		return;
	}
	for (i = 0; i < params->n_values; i++) {
		value = g_value_array_get_nth(params, i);
		if (!G_VALUE_HOLDS_LONG(value)) {
			continue;
		}
		setting = g_value_get_long(value);
		vte_sequence_handler_set_mode_internal(terminal, setting, TRUE);
	}
}

/* Set tab stop in the current column. */
static void
vte_sequence_handler_st (VteTerminal *terminal, GValueArray *params)
{
	if (terminal->pvt->tabstops == NULL) {
		terminal->pvt->tabstops = g_hash_table_new(NULL, NULL);
	}
	_vte_terminal_set_tabstop(terminal,
				 terminal->pvt->screen->cursor_current.col);
}

/* Clear tabs selectively. */
static void
vte_sequence_handler_tab_clear (VteTerminal *terminal, GValueArray *params)
{
	GValue *value;
	long param = 0;

	if ((params != NULL) && (params->n_values > 0)) {
		value = g_value_array_get_nth(params, 0);
		if (G_VALUE_HOLDS_LONG(value)) {
			param = g_value_get_long(value);
		}
	}
	if (param == 0) {
		_vte_terminal_clear_tabstop(terminal,
					   terminal->pvt->screen->cursor_current.col);
	} else
	if (param == 3) {
		if (terminal->pvt->tabstops != NULL) {
			g_hash_table_destroy(terminal->pvt->tabstops);
			terminal->pvt->tabstops = NULL;
		}
	}
}

