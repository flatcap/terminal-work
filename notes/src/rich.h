#ifndef _RICH_H_
#define _RICH_H_

//#define __G_I18N_LIB_H__
//#define __G_I18N_H__

#include "config.h"
#include <gtk/gtk.h>
#include "terminal-app.h"
#include "terminal-window.h"
#include "terminal-screen.h"
#include "vte.h"
//#include "vte-private.h"

void dump_accel_label        (GtkWidget *widget, int indent);
void dump_adjustment         (GtkObject *object, int indent);
void dump_button             (GtkWidget *widget, int indent);
void dump_hbox               (GtkWidget *widget, int indent);
void dump_image              (GtkWidget *widget, int indent);
void dump_image_menu_item    (GtkWidget *widget, int indent);
void dump_label              (GtkWidget *widget, int indent);
void dump_menubar            (GtkWidget *widget, int indent);
void dump_notebook           (GtkWidget *widget, int indent);
void dump_terminal_screen    (GtkWidget *widget, int indent);
void dump_terminal_tab_label (GtkWidget *widget, int indent);
void dump_terminal_window    (GtkWidget *widget, int indent);
void dump_vbox               (GtkWidget *widget, int indent);
void dump_vscrollbar         (GtkWidget *widget, int indent);
void dump_widget             (GtkWidget *widget, int indent);
void dump_windows            (GtkWidget *widget);

void rich_save_buffer (TerminalWindow *window);
GString * rich_get_selection (TerminalScreen *screen);
GString * rich_get_buffer (TerminalScreen *screen, int start_line, int end_line);
GString * rich_get_all_buffer (TerminalScreen *screen);
void rich_analyse(VteTerminal *terminal);
void rich_selection_info (VteTerminal *terminal);
void rich_scroll_info (VteTerminal *terminal);
void rich_dump_charcell (void *cc, int indent);
//void rich_dump_charcell (struct vte_charcell *cc, int indent);
void rich_dump_terminal_screen (TerminalScreen *screen);

#endif // _RICH_H_

