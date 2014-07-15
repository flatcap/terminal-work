// TerminalApp *terminal_app_get()
// TerminalWindow *terminal_app_get_current_window(app)
// TerminalScreen *terminal_window_get_active (window)

/**
 * rich_enumerate_tabs
 */
void rich_enumerate_tabs (TerminalApp *app, TerminalWindow *current_window)
{
	GList *l_windows = NULL;
	GList *l_tmp = NULL;
	GList *l_screens = NULL;
	int i;
	int j;
	int num_windows = 0;
	int num_screens = 0;
	gint pos_w;
	gint pos_s;
	int index_w;
	int index_s;
	TerminalWindow *iter_w = NULL;
	TerminalScreen *iter_s = NULL;
	TerminalScreen *current_screen = NULL;

	l_windows   = terminal_app_get_windows ();		// GtkWindows owned by the TerminalApp
	num_windows = g_list_length (l_windows);		// Number of GtkWindows owned by the TerminalApp

	l_tmp = g_list_find (l_windows, current_window);
	pos_w = g_list_position (l_windows, l_tmp);		// Which GtkWindow are we in the TerminalApp's list?

	printf ("Iterating through windows/screens\n");
	for (i = 0; i < num_windows; i++) {
		index_w = ((pos_w+i) % num_windows);
		iter_w = TERMINAL_WINDOW (g_list_nth_data (l_windows, index_w));	// iter_w is a TerminalWindow
		printf ("%s %p: (%d of %d)\n", GTK_OBJECT_TYPE_NAME (iter_w), iter_w, index_w+1, num_windows);

		l_screens = terminal_window_list_screen_containers (iter_w);	// TerminalScreens owned by the TerminalWindow
		num_screens = g_list_length (l_screens);				// Number of TerminalScreens owned by the TerminalWindow

		// First, find out which item in the list is the current screen
		current_screen = terminal_window_get_active (iter_w);
		pos_s = -1;
		for (j = 0; j < num_screens; j++) {
			iter_s = rich_get_screen (g_list_nth_data (l_screens, j));	// iter_s is a TerminalScreen
			if (iter_s == current_screen) {
				pos_s = j;
				break;
			}
		}

		if (pos_s < 0)
			break;

		//printf ("current screen = %d\n", pos_s);

		for (j = 0; j < num_screens; j++) {
			index_s = ((pos_s+j) % num_screens);
			iter_s = rich_get_screen (g_list_nth_data (l_screens, index_s));	// iter_s is a TerminalScreen
			printf ("    %s %p: (%d of %d) - %s\n", G_OBJECT_TYPE_NAME (iter_s), iter_s, index_s+1, num_screens, terminal_screen_get_title (iter_s));
		}
	}

	printf ("\n");
}


GList * terminal_app_get_windows (void);

GList *
terminal_app_get_windows (void)
{
  return global_app->windows;
}

