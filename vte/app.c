#include <stdlib.h>
#include <gtk/gtk.h>
#include "vte.h"

/**
 * delete_event - window was killed
 */
static void
delete_event (GtkWidget *window, GdkEvent *event, gpointer terminal)
{
	gtk_widget_destroy (window);
	gtk_main_quit ();
}

/**
 * child_exited - bash has died
 */
static void
child_exited (GtkWidget *terminal, gpointer window)
{
	gtk_widget_destroy (GTK_WIDGET (window));
	gtk_main_quit ();
}

/**
 * key_event - notification for key press
 */
static gboolean
key_event (GtkWidget *window, GdkEventKey *event, gpointer terminal)
{
	/*
	if (event->type == GDK_KEY_PRESS)
		printf ("[WIN] key pressed:  '%c' (%d,%d)\n", event->keyval, event->state, event->keyval);
	else
		printf ("[WIN] key released: '%c' (%d,%d)\n", event->keyval, event->state, event->keyval);
	*/
	//printf ("    type = %d\n", event->type); // 8 = GDK_KEY_PRESS (GdkEventType)
	//printf ("    window = %p\n", event->window);
	//printf ("    state = %d\n", event->state); // 1 Shift, 4 Ctrl, 8 Alt (GdkModifierType)
	//printf ("    keyval = %c\n", event->keyval); // Character <gdk/gdkkeysyms.h>
	//printf ("    length = %d\n", event->length);
	//printf ("    string = %s\n", event->string); // deprecated
	//printf ("    hardware_keycode = %d\n", event->hardware_keycode);
	//printf ("    group = %d\n", event->group);
	//printf ("    is_modifier = %d\n", event->is_modifier);

	return FALSE;
}

/**
 * button_event - notification for button press
 */
static gboolean
button_event (GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
	/*
	if (event->type == GDK_BUTTON_PRESS)
		printf ("[WIN] button pressed:  %d (%.0f,%.0f)\n", event->button, event->x, event->y);
	else
		printf ("[WIN] button released: %d (%.0f,%.0f)\n", event->button, event->x, event->y);
	*/
	//printf ("    type = %d\n", event->type);
	//printf ("    window = %p\n", event->window);
	//printf ("    send_event = %d\n", event->send_event);
	//printf ("    time = %u\n", event->time);
	//printf ("    x = %f\n", event->x);
	//printf ("    y = %f\n", event->y);
	//printf ("    axes = %p\n", event->axes);
	//printf ("    state = %d\n", event->state);
	//printf ("    button = %d\n", event->button);
	//printf ("    device = %p\n", event->device);
	//printf ("    x_root = %f\n", event->x_root);
	//printf ("    y_root = %f\n", event->y_root);

	return FALSE;
}

/**
 * main
 */
int
main (int argc, char **argv)
{
	GtkWidget *window;
	GtkWidget *widget;
	VteTerminal *terminal;
	//const char *command_argv[] = { "/bin/bash", NULL };
	const char *command_argv[] = { "/home/work/terminal/readline/bash-4.2/rash", NULL };
	const char *env_add[] = { "DEBUG_APP=TRUE", NULL };

	gtk_init (NULL, NULL);
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

	widget = vte_terminal_new();
	terminal = VTE_TERMINAL (widget);
	vte_terminal_set_scrollback_lines (terminal, 1000);

        gtk_container_add (GTK_CONTAINER (window), widget);

	vte_terminal_fork_command_full (terminal, VTE_PTY_DEFAULT, NULL, (char**) command_argv, (char**) env_add, G_SPAWN_SEARCH_PATH, NULL, NULL, NULL, NULL);

	g_signal_connect (widget, "child-exited", G_CALLBACK (child_exited), window);
	g_signal_connect (window, "delete-event", G_CALLBACK (delete_event), widget);

	g_signal_connect       (window, "key-press-event",   G_CALLBACK (key_event), window);
	g_signal_connect_after (window, "key-release-event", G_CALLBACK (key_event), window);

	g_signal_connect (window, "button-press-event",   G_CALLBACK (button_event), window);
	g_signal_connect (window, "button-release-event", G_CALLBACK (button_event), window);

	g_object_add_weak_pointer (G_OBJECT (widget), (gpointer*) &widget);
	g_object_add_weak_pointer (G_OBJECT (window), (gpointer*) &window);

	gtk_window_move (GTK_WINDOW (window), 1300, 100);
	gtk_widget_realize (widget);
	gtk_widget_show_all (window);
	gtk_window_present (GTK_WINDOW (window));

	gtk_main();

	g_assert (widget == NULL);
	g_assert (window == NULL);

	return 0;
}
