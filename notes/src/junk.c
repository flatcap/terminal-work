  {
    GParamSpec **ps;
    guint n = 0;
    int i;

    printf ("profile: %s\n", GTK_OBJECT_TYPE_NAME (profile));
    ps = g_object_class_list_properties (G_OBJECT_GET_CLASS (profile), &n);
    printf ("%d properties\n", n);
    for (i =  0; i < n; i++) {
            printf ("  %s\n", ps[i]->name);
    }
  }

  GConfClient *client = NULL;
  GError *err = NULL;
  client = gconf_client_get_default ();

  if (0) {
    gboolean do_confirm;
    do_confirm = gconf_client_get_bool (client, CONF_GLOBAL_PREFIX "/confirm_window_close", NULL);
  }

  if (0) {
    gboolean s = FALSE;
    s = gconf_client_dir_exists (client, CONF_SEARCH_PREFIX, &err);

    rich_printf ("key = %s\n", CONF_GLOBAL_PREFIX "/search");
    rich_printf ("search = %d\n", s);

    s = gconf_client_key_is_writable (client, CONF_SEARCH_PREFIX, &err);
    rich_printf ("writeable = %d\n", s);
  }

  if (0) {
    gconf_client_set_bool (client, CONF_SEARCH_PREFIX "/search/wibble", TRUE, &err);
  }

  if (err) {
    rich_printf ("Error: %s\n", err->message);
    g_error_free(err);
    err = NULL;
  }
  g_object_unref (client);


static void
terminal_window_get_property (GObject *object,
                           guint prop_id,
                           GValue *value,
                           GParamSpec *pspec)
{
  //TerminalWindow *window = TERMINAL_WINDOW (object);

  switch (prop_id)
    {
    }
}

static void
terminal_window_set_property (GObject *object,
                            guint prop_id,
                            const GValue *value,
                            GParamSpec *pspec)
{
  //TerminalWindow *window = TERMINAL_WINDOW (object);
  gboolean b = FALSE;

  switch (prop_id)
    {
      case 1:
        b = g_value_get_boolean (value);
        printf ("win_set_prop: %d\n", b);
        break;

      default:
        printf ("win_set_prop: unknown property\n");
        break;
    }
}


  object_class->get_property = terminal_window_get_property;
  object_class->set_property = terminal_window_set_property;

  g_object_class_install_property
    (object_class,
     1,
     g_param_spec_boolean ("check-backwards", NULL, NULL,
                           FALSE,
                           G_PARAM_READWRITE | G_PARAM_STATIC_NAME));


  if (0) {
    GObjectClass *object_class = G_OBJECT_GET_CLASS (dialog);

    printf ("dialog get_property = %p\n", object_class->get_property);
    printf ("dialog set_property = %p\n", object_class->set_property);
  }


    gboolean s = FALSE;
    s = gconf_client_dir_exists (client, CONF_FIND_PREFIX, &err);

    rich_printf ("key = %s\n", CONF_GLOBAL_PREFIX "/find");
    rich_printf ("find = %d\n", s);

    s = gconf_client_key_is_writable (client, CONF_FIND_PREFIX, &err);
    rich_printf ("writeable = %d\n", s);

  GParamSpec **ps;
  guint n = 0;

  ps = g_object_class_list_properties (G_OBJECT_GET_CLASS (dialog), &n);
  printf ("%d properties\n", n);
  int i;
  for (i =  0; i < n; i++) {
    printf ("  %s\n", ps[i]->name);
  }


  printf("Entering: terminal_app_new_terminal\n");
  screen = terminal_window_get_active (window);
  if (screen) {
    //rich_dump_terminal_screen (screen);
    //do_printing(GTK_WIDGET(window));
    //rich_save_buffer (window);
    //rich_wheres_wally(screen);
    //rich_enumerate_tabs (app, window);

    //return NULL;
  }



  {
    GtkDialog         *o1 = NULL;
    GtkWindow         *o2 = NULL;
    GtkBin            *o3 = NULL;
    GtkContainer      *o4 = NULL;
    GtkWidget         *o5 = NULL;
    GtkObject         *o6 = NULL;
    GObject           *o7 = NULL;

    o1 = GTK_DIALOG    (dialog);
    o2 = GTK_WINDOW    (dialog);
    o3 = GTK_BIN       (dialog);
    o4 = GTK_CONTAINER (dialog);
    o5 = GTK_WIDGET    (dialog);
    o6 = GTK_OBJECT    (dialog);

    printf ("GTK_DIALOG     = %p\n", o1);
    printf ("GTK_WINDOW     = %p\n", o2);
    printf ("GTK_BIN        = %p\n", o3);
    printf ("GTK_CONTAINER  = %p\n", o4);
    printf ("GTK_WIDGET     = %p\n", o5);
    printf ("GTK_OBJECT     = %p\n", o6);
  }

QUARKS
static void jim (GQuark key_id, gpointer data, gpointer user_data)
{
  printf ("Quark: %s, %p\n", g_quark_to_string(key_id), data);
}

  GObject *obj = G_OBJECT (widget);
  g_datalist_foreach (&(obj->qdata), jim, NULL);


/**
 * terminal_find_config_set
 */
void
terminal_find_config_set (void)
{
  gboolean check_case  = FALSE;
  gboolean check_whole = FALSE;
  gboolean check_regex = FALSE;

  check_case  = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (g_check_case));
  check_whole = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (g_check_whole));
  check_regex = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (g_check_regex));

  gconf_client_set_bool (global_find->gconf, CONF_FIND_CHECK_CASE,  check_case,  NULL);
  gconf_client_set_bool (global_find->gconf, CONF_FIND_CHECK_WHOLE, check_whole, NULL);
  gconf_client_set_bool (global_find->gconf, CONF_FIND_CHECK_REGEX, check_regex, NULL);
}

/**
 * rich_get_screen
 */
TerminalScreen *rich_get_screen (GtkHBox *hbox)
{
	GList *children = NULL;
	gpointer *child = NULL;
	int count;
	int i;

	children = gtk_container_get_children (GTK_CONTAINER (hbox));
	count = g_list_length (children);

	for (i = 0; i < count; i++) {
		child = g_list_nth (children, i)->data;
		if (TERMINAL_IS_SCREEN (child))
			return (TERMINAL_SCREEN (child));
	}

	return NULL;
}


/**
 * rich_always_selected
 */
gboolean
rich_always_selected (VteTerminal *terminal, glong column, glong row, gpointer data)
{
  //printf ("ras: %ld,%ld\n", row, column);
  return TRUE;
}


  {
    VteTerminal   *vterm = NULL;
    GtkAdjustment *adj   = NULL;

    vterm = VTE_TERMINAL (screen);
    adj   = vterm->adjustment;

    int lower;
    int value;
    int upper;
    int psize;

    lower = gtk_adjustment_get_lower (adj);
    value = gtk_adjustment_get_value (adj);
    upper = gtk_adjustment_get_upper (adj);
    psize = gtk_adjustment_get_page_size (adj);

    printf ("lower = %d\n", lower);
    printf ("value = %d\n", value);
    printf ("upper = %d\n", upper);
    printf ("psize = %d\n", psize);
  }


  if (0)
    {
      for (i = 0; i < 15; i++)
        {
          ca = &g_array_index (attrs, VteCharAttributes, i);
          if (0)
            {
              printf ("char = %02x '%c'\n", sel[i], sel[i]);
              printf ("row = %ld, col = %ld\n", ca->row, ca->column);
              printf ("fore = (%02x,%02x,%02x), back = (%02x,%02x,%02x)\n", ca->fore.red>>8, ca->fore.green>>8, ca->fore.blue>>8, ca->back.red>>8, ca->back.green>>8, ca->back.blue>>8);
              printf ("underline = %d, strikethrough = %d\n", ca->underline, ca->strikethrough);
            }
          else
            {
              printf ("%02x '%c' %2ld,%-2ld (%02x,%02x,%02x)\n", (sel[i] & 0xFF), (sel[i] & 0xFF), ca->row, ca->column, ca->fore.red>>8, ca->fore.green>>8, ca->fore.blue>>8);
            }
        }
    }

  if (0)
    {
      glong column = 0;
      glong row = 0;
      vte_terminal_get_cursor_position (vterm, &column, &row);
      printf ("Cursor %ld,%ld\n", row, column);
    }

  char *sel = NULL;
  GArray *attrs = g_array_new (FALSE, TRUE, sizeof (VteCharAttributes));
  VteCharAttributes *ca = NULL;

  sel = vte_terminal_get_text_range (vterm, 0, 0, 99999, 0, terminal_find_aways_selected, NULL, attrs);

  printf ("array = %d\n", attrs->len);
  printf ("sel len = %ld\n", strlen (sel));
  //printf ("sel = >>>%s<<<\n", sel);

  GRegex *regex;
  GMatchInfo *match_info;
  GError *error = NULL;

  regex = g_regex_new (find_string, regex_flags, 0, NULL);

  g_regex_match_full (regex, sel, -1, 0, 0, &match_info, &error);
  while (g_match_info_matches (match_info))
    {
      int start = 0;
      int end = 0;
      gchar *word = g_match_info_fetch (match_info, 0);
      g_match_info_fetch_pos (match_info, 0, &start, &end);
      ca = &g_array_index (attrs, VteCharAttributes, start);
      g_print ("Found: %s at (%ld,%ld)\n", word, ca->row, ca->column);
      g_free (word);
      g_match_info_next (match_info, &error);
      terminal_set_current_row (screen, ca->row);
    }
  g_match_info_free (match_info);
  g_regex_unref (regex);

  if (error != NULL)
    {
      g_printerr ("Error while matching: %s\n", error->message);
      g_error_free (error);
    }

  g_free (sel);
  g_free (find_string);


static void
rich (GtkWidget *w, gpointer user_data)
{
  printf ("Destroying widget = %p (%s)\n", w, GTK_OBJECT_TYPE_NAME (w));
}

  g_signal_connect (find->dialog,      "destroy", G_CALLBACK (rich), NULL);
  g_signal_connect (find->check_case,  "destroy", G_CALLBACK (rich), NULL);
  g_signal_connect (find->check_whole, "destroy", G_CALLBACK (rich), NULL);
  g_signal_connect (find->check_regex, "destroy", G_CALLBACK (rich), NULL);
  g_signal_connect (find->entry,       "destroy", G_CALLBACK (rich), NULL);
  g_signal_connect (button_close,      "destroy", G_CALLBACK (rich), NULL);
  g_signal_connect (button_find,       "destroy", G_CALLBACK (rich), NULL);


  /* Disconnect all the signal handlers */
  g_signal_handlers_disconnect_by_func (find->dialog,      G_CALLBACK (terminal_find_destroyed_cb), NULL);
  g_signal_handlers_disconnect_by_func (find->dialog,      G_CALLBACK (terminal_find_response_cb),  window);
  g_signal_handlers_disconnect_by_func (find->entry,       G_CALLBACK (terminal_find_clear_cb),     NULL);
  g_signal_handlers_disconnect_by_func (find->check_case,  G_CALLBACK (terminal_find_toggled_cb),   NULL);
  g_signal_handlers_disconnect_by_func (find->check_regex, G_CALLBACK (terminal_find_toggled_cb),   NULL);
  g_signal_handlers_disconnect_by_func (find->check_whole, G_CALLBACK (terminal_find_toggled_cb),   NULL);
  g_signal_handlers_disconnect_by_func (find->entry,       G_CALLBACK (terminal_find_text_cb),      NULL);

/**
 * rich
 */
static void
rich (GtkWidget *w, gpointer user_data)
{
  printf ("Destroying widget = %p (%s)\n", w, GTK_OBJECT_TYPE_NAME (w));
}

/**
 * rich2
 */
static void
rich2 (GtkWidget *w, gpointer user_data)
{
  printf ("Entering %s\n", __FUNCTION__); fflush (stdout);
  printf ("Object has died: %p (%s)\n", w, GTK_OBJECT_TYPE_NAME (w)); fflush (stdout);

  printf ("dialog      = %p (%s)\n", dialog, GTK_OBJECT_TYPE_NAME (dialog)); fflush (stdout);
  //printf ("check_case  = %p (%s)\n", check_case, GTK_OBJECT_TYPE_NAME(check_case)); fflush (stdout);
  //printf ("check_regex = %p (%s)\n", check_regex, GTK_OBJECT_TYPE_NAME(check_regex)); fflush (stdout);
  //printf ("check_whole = %p (%s)\n", check_whole, GTK_OBJECT_TYPE_NAME(check_whole)); fflush (stdout);
  printf ("entry       = %p (%s)\n", entry, GTK_OBJECT_TYPE_NAME(entry)); fflush (stdout);
  printf ("killing dialog\n"); fflush (stdout);
#if 0
  GtkWindow *window = gtk_window_get_transient_for (GTK_WINDOW (dialog));
  printf ("window      = %p (%s)\n", window, GTK_OBJECT_TYPE_NAME(window)); fflush (stdout);
  gtk_window_set_transient_for (GTK_WINDOW (dialog), NULL);
  window = gtk_window_get_transient_for (GTK_WINDOW (dialog));
  printf ("window      = %p (%s)\n", window, GTK_OBJECT_TYPE_NAME(window)); fflush (stdout);
#endif
#if 1
  if (dialog)
    g_object_unref (dialog);
#endif
  printf ("done\n"); fflush (stdout);
}


#if 1
  g_signal_connect (dialog,       "destroy", G_CALLBACK (rich), NULL);
  g_signal_connect (check_case,   "destroy", G_CALLBACK (rich), NULL);
  g_signal_connect (check_whole,  "destroy", G_CALLBACK (rich), NULL);
  g_signal_connect (check_regex,  "destroy", G_CALLBACK (rich), NULL);
  g_signal_connect (entry,        "destroy", G_CALLBACK (rich), NULL);
  g_signal_connect (button_close, "destroy", G_CALLBACK (rich), NULL);
  g_signal_connect (button_find,  "destroy", G_CALLBACK (rich), NULL);
#endif

#if 1
  //gtk_widget_destroyed (GTK_WIDGET (dialog),      (GtkWidget **) &dialog);
  //gtk_widget_destroyed (GTK_WIDGET (entry),       (GtkWidget **) &entry);
  gtk_widget_destroyed (GTK_WIDGET (check_case),  (GtkWidget **) &check_case);
  gtk_widget_destroyed (GTK_WIDGET (check_regex), (GtkWidget **) &check_regex);
  gtk_widget_destroyed (GTK_WIDGET (check_whole), (GtkWidget **) &check_whole);
#endif

  TerminalApp *app = terminal_app_get();
  g_signal_connect (app, "quit", G_CALLBACK (rich2), dialog);

