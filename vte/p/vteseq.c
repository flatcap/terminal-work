/*
 * Copyright (C) 2001-2004 Red Hat, Inc.
 *
 * This is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Library General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <config.h>

#include <limits.h>
#ifdef HAVE_SYS_SYSLIMITS_H
#include <sys/syslimits.h>
#endif

#include <glib.h>

#include "vte.h"
#include "vte-private.h"
#include "vtetc.h"

#define BEL "\007"

/* FUNCTIONS WE USE */

static void
display_control_sequence(const char *name, GValueArray *params)
{
#ifdef VTE_DEBUG
	guint i;
	long l;
	const char *s;
	const gunichar *w;
	GValue *value;
	g_printerr("%s(", name);
	if (params != NULL) {
		for (i = 0; i < params->n_values; i++) {
			value = g_value_array_get_nth(params, i);
			if (i > 0) {
				g_printerr(", ");
			}
			if (G_VALUE_HOLDS_LONG(value)) {
				l = g_value_get_long(value);
				g_printerr("%ld", l);
			} else
			if (G_VALUE_HOLDS_STRING(value)) {
				s = g_value_get_string(value);
				g_printerr("\"%s\"", s);
			} else
			if (G_VALUE_HOLDS_POINTER(value)) {
				w = g_value_get_pointer(value);
				g_printerr("\"%ls\"", (const wchar_t*) w);
			}
		}
	}
	g_printerr(")\n");
#endif
}

/* A couple are duplicated from vte.c, to keep them static... */

/* Some common functions */

/*
 * Sequence handling boilerplate
 */

/* Typedef the handle type */
typedef void (*VteTerminalSequenceHandler) (VteTerminal *terminal, GValueArray *params);

/* Prototype all handlers... */
#define VTE_SEQUENCE_HANDLER(name) \
	static void name (VteTerminal *terminal, GValueArray *params);
#include "vteseq-list.h"
#undef VTE_SEQUENCE_HANDLER

/* THE HANDLERS */

/* Lookup tables */

#define VTE_SEQUENCE_HANDLER(name) name

static const struct vteseq_2_struct *
vteseq_2_lookup (register const char *str, register unsigned int len);
#include"vteseq-2.c"

static const struct vteseq_n_struct *
vteseq_n_lookup (register const char *str, register unsigned int len);
#include"vteseq-n.c"

#undef VTE_SEQUENCE_HANDLER

static VteTerminalSequenceHandler
_vte_sequence_get_handler (const char *name)
{
	int len = strlen (name);

	if (G_UNLIKELY (len < 2)) {
		return NULL;
	} else if (len == 2) {
		const struct vteseq_2_struct *seqhandler;
		seqhandler = vteseq_2_lookup (name, 2);
		return seqhandler ? seqhandler->handler : NULL;
	} else {
		const struct vteseq_n_struct *seqhandler;
		seqhandler = vteseq_n_lookup (name, len);
		return seqhandler ? seqhandler->handler : NULL;
	}
}

/* Handle a terminal control sequence and its parameters. */
void
_vte_terminal_handle_sequence(VteTerminal *terminal,
			      const char *match_s,
			      GQuark match G_GNUC_UNUSED,
			      GValueArray *params)
{
	VteTerminalSequenceHandler handler;

	_VTE_DEBUG_IF(VTE_DEBUG_PARSE)
		display_control_sequence(match_s, params);

	/* Find the handler for this control sequence. */
	handler = _vte_sequence_get_handler (match_s);

	if (handler != NULL) {
		/* Let the handler handle it. */
		handler (terminal, params);
	} else {
		_vte_debug_print (VTE_DEBUG_MISC,
				  "No handler for control sequence `%s' defined.\n",
				  match_s);
	}
}

#include "rseq-vte.c"
#include "rseq-gt.c"
#include "rseq-unknown.c"

