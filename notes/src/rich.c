#include <stdlib.h>
#include <unistd.h>

#include "config.h"
#include "rich.h"
#include "terminal-screen.h"
#include "terminal-tab-label.h"
#include "terminal-window.h"
#include "terminal-app.h"
#include "vteint.h"
#include "ring.h"
#include "vte-private.h"

VteRowData * _vte_terminal_find_row_data(VteTerminal *terminal, glong row);

const char spaces[] = "                                                                                ";

/**
 * dump_accel_label
 */
void dump_accel_label (GtkWidget *widget, int indent)
{
	const char *text = NULL;
	GtkLabel *label = NULL;

	if (!GTK_IS_ACCEL_LABEL (widget))
		return;

	label = GTK_LABEL (widget);
	text = gtk_label_get_text (label); // or _get_label

	printf ("%16p: %.*s%s '%s'\n", widget, indent, spaces, GTK_OBJECT_TYPE_NAME (widget), text);
}

/**
 * dump_adjustment
 */
void dump_adjustment (GtkObject *object, int indent)
{
	double lower          = 0;
	double page_increment = 0;
	double page_size      = 0;
	double step_increment = 0;
	double upper          = 0;
	double value          = 0;

	if (!GTK_IS_ADJUSTMENT (object))
		return;

	g_object_get (object,
		      "lower", &lower,
		      "page-increment", &page_increment,
		      "page-size", &page_size,
		      "step-increment", &step_increment,
		      "upper", &upper,
		      "value", &value,
		      NULL);

	printf ("%16p: %.*s%s\n", object, indent, spaces, GTK_OBJECT_TYPE_NAME (object));
	printf ("                %.*slower          %6.0f\n", indent, spaces, lower);
	printf ("                %.*spage-increment %6.0f\n", indent, spaces, page_increment);
	printf ("                %.*spage-size      %6.0f\n", indent, spaces, page_size);
	printf ("                %.*sstep-increment %6.0f\n", indent, spaces, step_increment);
	printf ("                %.*supper          %6.0f\n", indent, spaces, upper);
	printf ("                %.*svalue          %6.0f\n", indent, spaces, value);
	printf ("\n");
}

/**
 * dump_button
 */
void dump_button (GtkWidget *widget, int indent)
{
	GtkBin *bin = NULL;
	GtkWidget *child = NULL;

	if (!GTK_IS_BUTTON (widget))
		return;

	printf ("%16p: %.*s%s\n", widget, indent, spaces, GTK_OBJECT_TYPE_NAME (widget));

	bin = GTK_BIN (widget);
	child = gtk_bin_get_child (bin);

	indent += 4;
	dump_widget (child, indent);
}

/**
 * dump_hbox
 */
void dump_hbox (GtkWidget *widget, int indent)
{
	GList *list = NULL;
	int children = 0;
	int i;

	if (!GTK_IS_HBOX (widget))
		return;

	list = gtk_container_get_children (GTK_CONTAINER (widget));
	children = g_list_length (list);

	printf ("%16p: %.*s%s(%d)\n", widget, indent, spaces, GTK_OBJECT_TYPE_NAME (widget), children);

	indent += 4;

	for (i = 0; i < children; i++)
		dump_widget (g_list_nth_data (list, i), indent);
}

/**
 * dump_image
 */
void dump_image (GtkWidget *widget, int indent)
{
	GtkImage *image = NULL;

	if (!GTK_IS_IMAGE (widget))
		return;

	image = GTK_IMAGE (widget);
	printf ("%16p: %.*s%s type %d\n", widget, indent, spaces, GTK_OBJECT_TYPE_NAME (widget), gtk_image_get_storage_type (image));
}

/**
 * dump_image_menu_item
 */
void dump_image_menu_item (GtkWidget *widget, int indent)
{
	GList *list = NULL;
	GtkMenuItem *menu = NULL;
	int children = 0;
	int i;

	if (!GTK_IS_IMAGE_MENU_ITEM (widget))
		return;

	list = gtk_container_get_children (GTK_CONTAINER (widget));
	children = g_list_length (list);

	menu = GTK_MENU_ITEM (widget);
	printf ("%16p: %.*s%s(%d)\n", widget, indent, spaces, GTK_OBJECT_TYPE_NAME (widget), children);

	indent += 4;

	for (i = 0; i < children; i++)
		dump_widget (g_list_nth_data (list, i), indent);
}

/**
 * dump_label
 */
void dump_label (GtkWidget *widget, int indent)
{
	const char *text = NULL;
	GtkLabel *label = NULL;

	if (!GTK_IS_LABEL (widget))
		return;

	label = GTK_LABEL (widget);
	text = gtk_label_get_text (label); // or _get_label

	printf ("%16p: %.*s%s '%s'\n", widget, indent, spaces, GTK_OBJECT_TYPE_NAME (widget), text);
}

/**
 * dump_menubar
 */
void dump_menubar (GtkWidget *widget, int indent)
{
	GList *list = NULL;
	int children = 0;
	int i;

	if (!GTK_IS_MENU_BAR (widget))
		return;

	list = gtk_container_get_children (GTK_CONTAINER (widget));
	children = g_list_length (list);

	printf ("%16p: %.*s%s(%d)\n", widget, indent, spaces, GTK_OBJECT_TYPE_NAME (widget), children);

	if (1) {
		indent += 4;

		for (i = 0; i < children; i++)
			dump_widget (g_list_nth_data (list, i), indent);
	}
}

/**
 * dump_notebook
 */
void dump_notebook (GtkWidget *widget, int indent)
{
	GList *list = NULL;
	GtkWidget *label;
	GtkNotebook *notebook = NULL;
	int children = 0;
	int i;

	if (!GTK_IS_NOTEBOOK (widget))
		return;

	notebook = GTK_NOTEBOOK (widget);
	children = gtk_notebook_get_n_pages (notebook);

	printf ("%16p: %.*s%s(%d)\n", widget, indent, spaces, GTK_OBJECT_TYPE_NAME (widget), children);

	indent += 4;

	list = gtk_container_get_children (GTK_CONTAINER (widget));
	for (i = 0; i < children; i++) {
		label = gtk_notebook_get_tab_label (notebook, g_list_nth_data (list, i));
		dump_widget (label, indent);

		dump_widget (g_list_nth_data (list, i), indent);
	}
}

/**
 * dump_terminal_screen
 */
void dump_terminal_screen (GtkWidget *widget, int indent)
{
	if (!TERMINAL_IS_SCREEN (widget))
		return;

	printf ("%16p: %.*s%s\n", widget, indent, spaces, GTK_OBJECT_TYPE_NAME (widget));
}

/**
 * dump_terminal_tab_label
 */
void dump_terminal_tab_label (GtkWidget *widget, int indent)
{
	GList *list = NULL;
	int children = 0;
	int i;

	if (!TERMINAL_IS_TAB_LABEL (widget))
		return;

	list = gtk_container_get_children (GTK_CONTAINER (widget));
	children = g_list_length (list);

	printf ("%16p: %.*s%s(%d)\n", widget, indent, spaces, GTK_OBJECT_TYPE_NAME (widget), children);

	indent += 4;

	for (i = 0; i < children; i++)
		dump_widget (g_list_nth_data (list, i), indent);
}

/**
 * dump_terminal_window
 */
void dump_terminal_window (GtkWidget *widget, int indent)
{
	GtkBin *bin = NULL;
	GtkWidget *child = NULL;

	if (!TERMINAL_IS_WINDOW (widget))
		return;

	printf ("%16p: %.*s%s(1)\n", widget, indent, spaces, GTK_OBJECT_TYPE_NAME (widget));

	indent += 4;
	bin = GTK_BIN (widget);
	child = gtk_bin_get_child (bin);

	dump_widget (child, indent);
}

/**
 * dump_vbox
 */
void dump_vbox (GtkWidget *widget, int indent)
{
	GList *list = NULL;
	int children = 0;
	int i;

	if (!GTK_IS_VBOX (widget))
		return;

	list = gtk_container_get_children (GTK_CONTAINER (widget));
	children = g_list_length (list);

	printf ("%16p: %.*s%s(%d)\n", widget, indent, spaces, GTK_OBJECT_TYPE_NAME (widget), children);

	indent += 4;

	for (i = 0; i < children; i++)
		dump_widget (g_list_nth_data (list, i), indent);
}

/**
 * dump_vscrollbar
 */
void dump_vscrollbar (GtkWidget *widget, int indent)
{
	if (!GTK_IS_VSCROLLBAR (widget))
		return;

	printf ("%16p: %.*s%s\n", widget, indent, spaces, GTK_OBJECT_TYPE_NAME (widget));
}

/**
 * dump_widget
 */
void dump_widget (GtkWidget *widget, int indent)
{
	// Terminal classes need to be before more primitive classes
	if      (TERMINAL_IS_SCREEN (widget))     dump_terminal_screen (widget, indent);
	else if (TERMINAL_IS_TAB_LABEL (widget))  dump_terminal_tab_label (widget, indent);
	else if (TERMINAL_IS_WINDOW (widget))     dump_terminal_window (widget, indent);
	else if (GTK_IS_ACCEL_LABEL (widget))     dump_accel_label (widget, indent);
	else if (GTK_IS_BUTTON (widget))          dump_button (widget, indent);
	else if (GTK_IS_HBOX (widget))            dump_hbox (widget, indent);
	else if (GTK_IS_IMAGE (widget))            dump_image (widget, indent);
	else if (GTK_IS_IMAGE_MENU_ITEM (widget)) dump_image_menu_item (widget, indent);
	else if (GTK_IS_LABEL (widget))           dump_label (widget, indent);
	else if (GTK_IS_MENU_BAR (widget))        dump_menubar (widget, indent);
	else if (GTK_IS_NOTEBOOK (widget))        dump_notebook (widget, indent);
	else if (GTK_IS_VBOX (widget))            dump_vbox (widget, indent);
	else if (GTK_IS_VSCROLLBAR (widget))      dump_vscrollbar (widget, indent);
	else
		printf ("%16p: %.*sNEW TYPE: %s\n", widget, indent, spaces, GTK_OBJECT_TYPE_NAME (widget));
}

/**
 * dump_windows
 */
void dump_windows (GtkWidget *widget)
{
	GtkWidget *old = NULL;
	TerminalApp *app = NULL;

	// Find the top of the tree of windows.
	do {
		old = widget;
		widget = gtk_widget_get_parent (widget);
	}
	while (widget);

	dump_widget (old, 0);

	app = terminal_app_get();
	printf ("TerminalApp = %16p\n", app);
}


/**
 * rich_get_selection
 */
GString * rich_get_selection (TerminalScreen *screen)
{
	VteTerminal *vterm = NULL;
	char *str = NULL;
	GString *gs = NULL;

	vterm = VTE_TERMINAL (screen);
	if (!vterm)
		return NULL;

	str = _vte_terminal_get_selection (vterm);

	gs = g_string_new (str);
	free (str);

	return gs;
}

/**
 * rich_get_buffer
 */
GString * rich_get_buffer (TerminalScreen *screen, int start_line, int end_line)
{
	VteTerminal *vterm = NULL;
	VteTerminalPrivate *vpriv = NULL;
	VteRing *ring = NULL;
	VteRowData *row_data = NULL;
	struct vte_charcell *cc = NULL;
	GString *gs = NULL;
	int i, j;

	vterm = VTE_TERMINAL (screen);
	vpriv = vterm->pvt;
	ring = vpriv->screen->row_data;

	for (i = ring->delta; i <= ring->cached_item; i++) {
		row_data = _vte_terminal_find_row_data (vterm, i);

		gs = g_string_new(NULL);

		for (j = 0; j < row_data->cells->len; j++) {
			cc = &g_array_index (row_data->cells, struct vte_charcell, j);
			if (!cc->c)
				break;

			g_string_append_unichar (gs, cc->c);
			j += (cc->attr.columns - 1);
		}

		g_string_append (gs, "\n");
	}

	return gs;
}

/**
 * rich_get_all_buffer
 */
GString * rich_get_all_buffer (TerminalScreen *screen)
{
	return rich_get_buffer (screen, -1, -1);
}

/**
 * rich_save_buffer
 */
void rich_save_buffer (TerminalWindow *window)
{
	TerminalScreen *screen = NULL;
	VteTerminal *vterm = NULL;
	FILE *f = NULL;
	GString *gs = NULL;

	screen = terminal_window_get_active (window);
	vterm = VTE_TERMINAL (screen);

	f = fopen ("/home/work/terminal/out.txt", "w");

	if (vte_terminal_get_has_selection (vterm))
		gs = rich_get_selection (screen);
	else
		gs = rich_get_all_buffer (screen);

	fwrite (gs->str, gs->len, 1, f);
	g_string_free (gs, TRUE);
	fclose (f);
}

/**
 * rich_analyse
 */
void rich_analyse(VteTerminal *terminal)
{
	//printf ("%s:%s(%d)\n",__FILE__, __FUNCTION__, __LINE__);
	VteRowData *row_data = _vte_terminal_find_row_data (terminal, 0);

	printf ("row_data\n");
	printf ("    cells    = %p\n", row_data->cells);
	printf ("    len      = %d\n", row_data->cells->len);
	printf ("    wrapped  = %i\n", row_data->soft_wrapped);

	int i;
	struct vte_charcell *cc = NULL;
	GString *gs = NULL;
	gs = g_string_new(NULL);

	printf ("    data     = ");
	printf ("    CharCell (%ld):\n", sizeof (struct vte_charcell));
	for (i = 0; i < row_data->cells->len; i++) {
		cc = &g_array_index (row_data->cells, struct vte_charcell, i);
		//rich_dump_charcell (cc, 8);
		if ((i > 20) && (i < 30)) {
			cc->c = '0' + i - 20;
			cc->attr.fore = 1;
		}
		_vte_invalidate_cells (terminal, 21, 9, 0, 1); //column_start, column_count, row_start, row_count)
		g_string_append_unichar (gs, cc->c);
		i += (cc->attr.columns - 1);
	}

	_vte_terminal_select_text(terminal, 5, 10, 60, 15, 0, 0);
	printf ("Line: %s\n", gs->str);
}

/**
 * rich_selection_info
 */
void rich_selection_info (VteTerminal *terminal)
{
	VteTerminalPrivate *pvt = terminal->pvt;

	//GArray *word_chars;
	//char *selection;

	printf ("has_selection        %d\n", pvt->has_selection);
	printf ("selecting            %d\n", pvt->selecting);
	printf ("selecting_restart    %d\n", pvt->selecting_restart);
	printf ("selecting_had_delta  %d\n", pvt->selecting_had_delta);
	printf ("selection_block_mode %d\n", pvt->selection_block_mode);

	printf ("selection type: ");
	switch (pvt->selection_type) {
		case selection_type_char:
			printf ("char\n");
			break;
		case selection_type_word:
			printf ("word\n");
			break;
		case selection_type_line:
			printf ("line\n");
			break;
	}

	printf ("event coords origin (%ld,%ld)\n", pvt->selection_origin.x, pvt->selection_origin.y);
	printf ("event coords last   (%ld,%ld)\n", pvt->selection_last.x, pvt->selection_last.y);

	printf ("cell coords start   (%ld,%ld)\n", pvt->selection_start.row, pvt->selection_start.col);
	printf ("cell coords end     (%ld,%ld)\n", pvt->selection_end.row, pvt->selection_end.col);
	printf ("\n");
}

/**
 * rich_scroll_info
 */
void rich_scroll_info (VteTerminal *terminal)
{
#if 1
	double lower, page_increment, page_size, step_increment, upper, value;
	GtkAdjustment *adj = terminal->adjustment;
	GtkObject *obj = GTK_OBJECT (adj);
	g_object_get (obj,
		      "lower", &lower,
		      "page-increment", &page_increment,
		      "page-size", &page_size,
		      "step-increment", &step_increment,
		      "upper", &upper,
		      "value", &value,
		      NULL);

	printf ("lower          %6.0f\n", lower);
	printf ("page-increment %6.0f\n", page_increment);
	printf ("page-size      %6.0f\n", page_size);
	printf ("step-increment %6.0f\n", step_increment);
	printf ("upper          %6.0f\n", upper);
	printf ("value          %6.0f\n", value);
	printf ("\n");
#endif
#if 1
	VteTerminalPrivate *pvt = terminal->pvt;
	struct _VteScreen *screen = pvt->screen;
	printf ("scrolling region (%d,%d)\n", screen->scrolling_region.start, screen->scrolling_region.end);
	printf ("scroll delta     %ld\n", screen->scroll_delta);
	printf ("insert delta     %ld\n", screen->insert_delta);
	printf ("\n");
#endif
}

/**
 * rich_dump_charcell
 */
//void rich_dump_charcell (struct vte_charcell *cc, int indent)
void rich_dump_charcell (void *ccv, int indent)
{
	char buffer[6];
	struct vte_charcell *cc = ccv;

	if (!cc)
		return;

	memset (buffer, 0, sizeof(buffer));
	g_unichar_to_utf8 (cc->c, buffer);

	printf ("%.*scc               %p\n", indent, spaces, cc);
	printf ("%.*s  char           '%s' (%d)\n", indent, spaces, buffer, cc->c);
	printf ("%.*s  columns        %d\n", indent, spaces, cc->attr.columns);
	//printf ("%.*s  fore           %d = (%02x,%02x,%02x)\n", indent, spaces, cc->attr.fore, terminal->pvt->palette[cc->attr.fore].red >> 8, terminal->pvt->palette[cc->attr.fore].green >> 8, terminal->pvt->palette[cc->attr.fore].blue >> 8);
	//printf ("%.*s  back           %d = (%02x,%02x,%02x)\n", indent, spaces, cc->attr.back, terminal->pvt->palette[cc->attr.back].red >> 8, terminal->pvt->palette[cc->attr.back].green >> 8, terminal->pvt->palette[cc->attr.back].blue >> 8);
	printf ("%.*s  fore           %d\n", indent, spaces, cc->attr.fore);
	printf ("%.*s  back           %d\n", indent, spaces, cc->attr.back);
	printf ("%.*s  fragment       %d\n", indent, spaces, cc->attr.fragment);
	printf ("%.*s  standout       %d\n", indent, spaces, cc->attr.standout);
	printf ("%.*s  underline      %d\n", indent, spaces, cc->attr.underline);
	printf ("%.*s  strikethrough  %d\n", indent, spaces, cc->attr.strikethrough);
	printf ("%.*s  reverse        %d\n", indent, spaces, cc->attr.reverse);
	printf ("%.*s  blink          %d\n", indent, spaces, cc->attr.blink);
	printf ("%.*s  half           %d\n", indent, spaces, cc->attr.half);
	printf ("%.*s  bold           %d\n", indent, spaces, cc->attr.bold);
	printf ("%.*s  invisible      %d\n", indent, spaces, cc->attr.invisible);
	printf ("\n");
}

/**
 * rich_dump_terminal_screen
 */
void rich_dump_terminal_screen (TerminalScreen *screen)
{
	VteTerminal *vterm = NULL;
	VteTerminalPrivate *vpriv = NULL;
	VteRing *ring = NULL;

	if (!screen)
		return;

	vterm = VTE_TERMINAL (screen);
	vpriv = vterm->pvt;
	ring = vpriv->screen->row_data;

	printf ("TerminalScreen info:\n");
	printf ("    char_width   = %ld\n", vterm->char_width);
	printf ("    char_height  = %ld\n", vterm->char_height);
	printf ("    char_ascent  = %ld\n", vterm->char_ascent);
	printf ("    char_descent = %ld\n", vterm->char_descent);
	printf ("    row_count    = %ld\n", vterm->row_count);
	printf ("    column_count = %ld\n", vterm->column_count);
	printf ("    window_title = %s\n", vterm->window_title);
	printf ("    icon_title   = %s\n", vterm->icon_title);
	printf ("\n");

	printf ("    terminal  %p\n", vterm);
	printf ("    pvt       %p\n", vterm->pvt);
	printf ("    screen    %p\n", vterm->pvt->screen);
	printf ("    row_data  %p\n", vterm->pvt->screen->row_data);
	printf ("        delta       %ld\n", ring->delta);
	printf ("        length      %ld\n", ring->length);
	printf ("        max         %ld\n", ring->max);
	printf ("        cached_item %ld\n", ring->cached_item);
	printf ("        cached_data %p\n", ring->cached_data);
	printf ("        array       %p\n", ring->array);
	printf ("        user_data   %p\n", ring->user_data);
	printf ("\n");

	printf ("    vpriv %p\n", vpriv);
	printf ("        word_chars           %p\n", vpriv->word_chars);
	printf ("        has_selection        %d\n", vpriv->has_selection);
	printf ("        selecting            %d\n", vpriv->selecting);
	printf ("        selecting_restart    %d\n", vpriv->selecting_restart);
	printf ("        selecting_had_delta  %d\n", vpriv->selecting_had_delta);
	printf ("        selection_block_mode %d\n", vpriv->selection_block_mode);
	printf ("        selection            %s\n", vpriv->selection);
	printf ("        selection_type       %d\n", vpriv->selection_type);
	printf ("        sel origin coords    %ld,%ld\n", vpriv->selection_origin.x, vpriv->selection_origin.y);
	printf ("        sel last   coords    %ld,%ld\n", vpriv->selection_last.x, vpriv->selection_last.y);
	printf ("        sel start cell       %ld,%ld\n", vpriv->selection_start.row, vpriv->selection_start.col);
	printf ("        sel end cell         %ld,%ld\n", vpriv->selection_end.row, vpriv->selection_end.col);
	printf ("\n");
}


/* vim: set ts=8 sw=8 noet fdl=0: */
