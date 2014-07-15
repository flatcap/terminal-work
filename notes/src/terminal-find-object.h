#ifndef _TERMINAL_FIND_H_
#define _TERMINAL_FIND_H_

/*
 * Copyright © 2009 Richard Russon (flatcap)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "terminal-window.h"
#include <gconf/gconf-client.h>

#define TERMINAL_TYPE_FIND              (terminal_find_get_type ())
#define TERMINAL_FIND(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), TERMINAL_TYPE_FIND, TerminalFind))
#define TERMINAL_FIND_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), TERMINAL_TYPE_FIND, TerminalFindClass))
#define TERMINAL_IS_FIND(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), TERMINAL_TYPE_FIND))
#define TERMINAL_IS_FIND_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), TERMINAL_TYPE_FIND))
#define TERMINAL_FIND_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), TERMINAL_TYPE_FIND, TerminalFindClass))

typedef struct _TerminalFindClass TerminalFindClass;
typedef struct _TerminalFind      TerminalFind;

GType terminal_find_get_type (void);

void terminal_find_display (GtkWindow *window);

#endif // _TERMINAL_FIND_H_

