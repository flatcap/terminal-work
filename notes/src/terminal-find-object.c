/*
 * Copyright © 2009 Richard Russon (flatcap)
 *
 * Gnome-terminal is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Gnome-terminal is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "config.h"

#include <string.h>
#include <gconf/gconf-client.h>

#include "terminal-app.h"
#include "terminal-find.h"
#include "terminal-util.h"

/* Our config info lives under: /apps/gnome-terminal/find (CONF_FIND_PREFIX) */
#define CONF_FIND_PREFIX        CONF_PREFIX "/find"

#define PROP_FIND_CHECK_CASE    "check-case"
#define PROP_FIND_CHECK_REGEX   "check-regex"
#define PROP_FIND_CHECK_WHOLE   "check-whole"
#define PROP_FIND_ENTRY_HISTORY "entry-history"
#define PROP_FIND_ENTRY_COUNT   "entry-count"

#define CONF_FIND_CHECK_CASE    CONF_FIND_PREFIX "/" PROP_FIND_CHECK_CASE
#define CONF_FIND_CHECK_REGEX   CONF_FIND_PREFIX "/" PROP_FIND_CHECK_REGEX
#define CONF_FIND_CHECK_WHOLE   CONF_FIND_PREFIX "/" PROP_FIND_CHECK_WHOLE
#define CONF_FIND_ENTRY_HISTORY CONF_FIND_PREFIX "/" PROP_FIND_ENTRY_HISTORY
#define CONF_FIND_ENTRY_COUNT   CONF_FIND_PREFIX "/" PROP_FIND_ENTRY_COUNT

/**
 * struct _TerminalFindClass
 */
struct _TerminalFindClass
{
  GtkObjectClass parent_class;

  /* signal handlers */
};

/**
 * struct _TerminalFind
 */
struct _TerminalFind
{
  GtkObject       parent_instance;

  /* GConf stuff */
  GConfClient    *gconf;
  guint           nid_case;             /* Notify IDs */
  guint           nid_regex;
  guint           nid_whole;

  /* Find Dialog */
  GtkDialog      *dialog;
  GtkCheckButton *check_case;
  GtkCheckButton *check_regex;
  GtkCheckButton *check_whole;
  GtkEntry       *entry;
  gint            entry_max;

  /* Keep track of where we are */
  TerminalScreen *find_screen;
  guint           find_row;
  guint           find_col;
  char           *find_string;
};


static TerminalFind *find = NULL;

G_DEFINE_TYPE (TerminalFind, terminal_find, G_TYPE_OBJECT)

/**
 * terminal_find_init
 */
static void
terminal_find_init (TerminalFind *newfind)
{
  fprintf (stdout, "Entering %s\n", __FUNCTION__); fflush (stdout);
  newfind->gconf = gconf_client_get_default ();

  gconf_client_add_dir (newfind->gconf, CONF_FIND_PREFIX, GCONF_CLIENT_PRELOAD_ONELEVEL, NULL);

  find = newfind;
}

/**
 * terminal_find_finalize
 */
static void
terminal_find_finalize (GObject *object)
{
  fprintf (stdout, "Entering %s\n", __FUNCTION__); fflush (stdout);
  TerminalFind *find = TERMINAL_FIND (object);

  // free things

  if (find->nid_case)
    {
      gconf_client_notify_remove (find->gconf, find->nid_case);
      find->nid_case = 0;
    }
  if (find->nid_regex)
    {
      gconf_client_notify_remove (find->gconf, find->nid_regex);
      find->nid_regex = 0;
    }
  if (find->nid_whole)
    {
      gconf_client_notify_remove (find->gconf, find->nid_whole);
      find->nid_whole = 0;
    }

  gconf_client_remove_dir (find->gconf, CONF_FIND_PREFIX, NULL);

  g_object_unref (find->gconf);
  find->gconf = NULL;

  g_free (find->find_string);
  find->find_string = NULL;

  G_OBJECT_CLASS (terminal_find_parent_class)->finalize (object);

  find = NULL;

  // kill the signal handlers
}

/**
 * terminal_find_class_init
 */
static void
terminal_find_class_init (TerminalFindClass *klass)
{
  fprintf (stdout, "Entering %s\n", __FUNCTION__); fflush (stdout);
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = terminal_find_finalize;
}

/**
 * terminal_find_get_object
 */
static TerminalFind *
terminal_find_get_object (void)
{
  fprintf (stdout, "Entering %s\n", __FUNCTION__); fflush (stdout);
  if (!find)
    find = g_object_new (TERMINAL_TYPE_FIND, NULL);

  return find;
}


/**
 * terminal_find_history_get_model
 */
static GtkTreeModel *
terminal_find_history_get_model (GtkEntry *entry)
{
  fprintf (stdout, "Entering %s\n", __FUNCTION__); fflush (stdout);
  // create the completion and model if necessary
  GtkEntryCompletion *comp = NULL;
  GtkTreeModel *model = NULL;

  comp = gtk_entry_get_completion (entry);
  if (!comp)
    return NULL;

  model = gtk_entry_completion_get_model (comp);

  return model;
}

/**
 * terminal_find_history_load
 */
static void
terminal_find_history_load (GtkEntry *entry)
{
  fprintf (stdout, "Entering %s\n", __FUNCTION__); fflush (stdout);
  GtkTreeModel *model = NULL;
  GtkTreeIter iter;
  GSList *list = NULL;
  GSList *curr = NULL;
  int i;
  int count;

  model = terminal_find_history_get_model (entry);
  printf ("model = %p (%s)\n", model, GTK_OBJECT_TYPE_NAME (model));

  find->entry_max = gconf_client_get_int (find->gconf, CONF_FIND_ENTRY_COUNT, NULL);
  printf ("%s: max = %d\n", __FUNCTION__, find->entry_max);

  list = gconf_client_get_list (find->gconf, CONF_FIND_ENTRY_HISTORY, GCONF_VALUE_STRING, NULL);
  count = g_slist_length (list);
  printf ("%s: list = %d\n", __FUNCTION__, count);

  count = MIN (count, find->entry_max);
  curr = list;
  for (i = 0; i < count; i++)
    {
      gtk_list_store_append (GTK_LIST_STORE (model), &iter);
      gtk_list_store_set (GTK_LIST_STORE (model), &iter, 0, curr->data, -1);
      curr = g_slist_next (curr);
    }
}

/**
 * terminal_find_history_save
 */
static void
terminal_find_history_save (GtkEntry *entry)
{
  fprintf (stdout, "Entering %s\n", __FUNCTION__); fflush (stdout);
  GtkTreeModel *model = NULL;
  GtkTreeIter iter;
  GSList *list = NULL;
  int i;
  int count;
  char *str = NULL;

  model = terminal_find_history_get_model (entry);

  printf ("model = %p (%s)\n", model, GTK_OBJECT_TYPE_NAME (model));

  gtk_tree_model_get_iter_first (model, &iter);
  count = gtk_tree_model_iter_n_children (model, NULL);
  printf ("%s: completion: %d count\n", __FUNCTION__, count);

  // truncate
  count = MIN (count, find->entry_max);

  for (i = 0; i < count; i++)
    {
      GValue value = { 0 };
      str = NULL;

      gtk_tree_model_get_value (model, &iter, 0, &value);
      str = g_strdup (g_value_get_string (&value));
      //printf ("%s: item = %s\n", __FUNCTION__, str);
      list = g_slist_append (list, str);

      g_value_unset (&value);

      //gboolean
      gtk_tree_model_iter_next (model, &iter);
    }

  printf ("list has %d count\n", g_slist_length (list));

  gconf_client_set_int  (find->gconf, CONF_FIND_ENTRY_COUNT,   find->entry_max, NULL);
  gconf_client_set_list (find->gconf, CONF_FIND_ENTRY_HISTORY, GCONF_VALUE_STRING, list, NULL);

  for (i = 0; i < count; i++)
    {
      str = g_slist_nth_data (list, i);
      //printf ("list: %s\n", str);
      g_free (str);
    }

  g_slist_free (list);
}

/**
 * terminal_find_history_add
 */
static void
terminal_find_history_add (GtkEntry *entry,
                           const char *str)
{
  fprintf (stdout, "Entering %s\n", __FUNCTION__); fflush (stdout);
  GtkTreeModel *model = NULL;
  GtkTreeIter iter;
  int i;
  int children;

  model = terminal_find_history_get_model (entry);

  children = gtk_tree_model_iter_n_children (model, NULL);
  printf ("%s: %d children\n", __FUNCTION__, children);

  gtk_tree_model_get_iter_first (model, &iter);
  for (i = 0; i < children; i++)
    {
      GValue value = { 0 };
      gtk_tree_model_get_value (model, &iter, 0, &value);
      printf ("    item: %s\n", g_value_get_string (&value));
      if (!g_strcmp0 (str, g_value_get_string (&value)))
        {
          gtk_list_store_remove (GTK_LIST_STORE (model), &iter);
          g_value_unset (&value);
          break;
        }
      gtk_tree_model_iter_next (model, &iter);
      g_value_unset (&value);
    }

  gtk_list_store_prepend (GTK_LIST_STORE (model), &iter);
  gtk_list_store_set (GTK_LIST_STORE (model), &iter, 0, str, -1);

  children = gtk_tree_model_iter_n_children (model, NULL);
  printf ("%s: %d/%d children\n", __FUNCTION__, children, find->entry_max);

  // truncate
  while (children > find->entry_max)
    {
      gtk_tree_model_iter_nth_child (model, &iter, NULL, find->entry_max);
      gtk_list_store_remove (GTK_LIST_STORE (model), &iter);
      children = gtk_tree_model_iter_n_children (model, NULL);
      printf ("removed item\n");
    }
}

/**
 * terminal_find_history_init
 */
static void
terminal_find_history_init (GtkEntry *entry)
{
  fprintf (stdout, "Entering %s\n", __FUNCTION__); fflush (stdout);
  GtkEntryCompletion *comp = NULL;
  GtkTreeModel *model = NULL;

  // add completion
  // add model
  // populate model

  /* set up completion */
  comp = gtk_entry_completion_new ();

  gtk_entry_completion_set_inline_selection (comp, TRUE);
  gtk_entry_completion_set_text_column (comp, 0);

  gtk_entry_set_completion (entry, comp);

  model = GTK_TREE_MODEL (gtk_list_store_new (1, G_TYPE_STRING));
  gtk_entry_completion_set_model (comp, model);

  // The GtkEntry's holding the references now.
  g_object_unref (comp);
  g_object_unref (model);

  terminal_find_history_load (entry);
}


/**
 * terminal_find_aways_selected
 */
static gboolean
terminal_find_aways_selected (VteTerminal *terminal,
                              glong column,
                              glong row,
                              gpointer data)
{
  return TRUE;
}

/**
 * terminal_find_get_row
 */
static char *
terminal_find_get_row (TerminalScreen *screen,
                       int row,
                       int col,
                       GArray *attrs)
{
  fprintf (stdout, "Entering %s\n", __FUNCTION__); fflush (stdout);
  VteTerminal *vterm = NULL;
  char        *sel   = NULL;

  if (!screen)
    return NULL;

  vterm = VTE_TERMINAL (screen);

  sel = vte_terminal_get_text_range (vterm, row, col, row, 99999, terminal_find_aways_selected, NULL, attrs);

  return sel;
}

/**
 * terminal_find_do_search
 */
static void
terminal_find_do_search (TerminalScreen *screen)
{
  fprintf (stdout, "Entering %s\n", __FUNCTION__); fflush (stdout);
  VteTerminal   *vterm = NULL;
  GtkAdjustment *adj   = NULL;
  char *row = NULL;
  char *fstr = NULL;
  int lower;
  int value;
  int upper;
  int psize;
  int range;
  int i;
  int rownum;
  gboolean find_case;
  gboolean find_regex;
  gboolean find_whole;
  gboolean str_changed;

  GRegexCompileFlags regex_flags = 0;

  if (!screen || !find)
    return;

  vterm = VTE_TERMINAL (screen);
  adj = vterm->adjustment;

  fstr = g_strdup (gtk_entry_get_text (find->entry));

  str_changed = (g_strcmp0 (find->find_string, fstr) != 0);

  if (str_changed)
    {
      g_free (find->find_string);
      find->find_string = fstr;
    }
  else
    {
      g_free (fstr);
    }

  terminal_find_history_add (find->entry, find_string);

  /* Some measures of the screen */
  lower = gtk_adjustment_get_lower (adj);       /* First line of the buffer */
  value = gtk_adjustment_get_value (adj);       /* First visible line of the buffer */
  upper = gtk_adjustment_get_upper (adj);       /* Last line of the buffer */
  psize = gtk_adjustment_get_page_size (adj);   /* Number of lines on screen */
  range = upper - lower + 1;

  printf ("lower = %d\n", lower);
  printf ("value = %d\n", value);
  printf ("upper = %d\n", upper);
  printf ("psize = %d\n", psize);
  printf ("range = %d\n", range);

  printf ("XXX: %d + %d >= %d\n", find->find_row, psize, upper);
  if ((find->find_row + psize) >= upper)
    {
      printf ("previous result is already on screen\n");
    }
  else
    {
      if ((find->find_screen != screen) ||          /* User changed tab */
          (find->find_row != value)     ||          /* Screen focus has moved */
          (str_changed))                            /* Different search string */
        {
          find->find_screen = screen;
          find->find_row    = value;
          find->find_col    = 0;
        }
    }

  printf ("check_case = %p\n",  find->check_case);
  printf ("check_regex = %p\n", find->check_regex);
  printf ("check_whole = %p\n", find->check_whole);
  printf ("marker\n");
  fflush (stdout);

  printf ("check_case = %p (%s)\n",  find->check_case,  GTK_OBJECT_TYPE_NAME (find->check_case));
  printf ("check_regex = %p (%s)\n", find->check_regex, GTK_OBJECT_TYPE_NAME (find->check_regex));
  printf ("check_whole = %p (%s)\n", find->check_whole, GTK_OBJECT_TYPE_NAME (find->check_whole));
  fflush (stdout);

  find_case  = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (find->check_case));
  find_regex = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (find->check_regex));
  find_whole = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (find->check_whole));

  /* Generate a regex for searching */
  if (!find_case)
    regex_flags |= G_REGEX_CASELESS;

  if (!find_regex)
    {
      fstr = find_string;
      find_string = g_regex_escape_string (fstr, -1);
      g_free (fstr);
    }

  if (find_whole)
    {
      fstr = find_string;
      find_string = g_strdup_printf ("\\b%s\\b", fstr); // Perl RE
      g_free (fstr);
    }

  printf ("find string = '%s'\n", find_string);

  GRegex *regex;
  GMatchInfo *match_info;
  GError *error = NULL;
  VteCharAttributes *ca = NULL;
  GArray *attrs = g_array_new (FALSE, TRUE, sizeof (VteCharAttributes));
  int colnum = 0;

  regex = g_regex_new (find_string, regex_flags, 0, NULL);

  for (i = 0; i < range; i++)
    {
      if (i == 0)
        colnum = find->find_col;
      else
        colnum = 0;

      /* search from find->find_row, not value, just in case the previous match is on screen */
      /* search from value->upper, lower->value-1 */
      //rownum = ((i + value - lower) % range) + lower;
      rownum = ((i + find->find_row - lower) % range) + lower;

      row = terminal_find_get_row (screen, rownum, colnum, attrs);
      //printf ("row %i: %s", rownum, row);

      g_regex_match_full (regex, row, -1, 0, 0, &match_info, &error);
      g_free (row);

      if (error != NULL)
        {
          g_printerr ("Error while matching: %s\n", error->message);
          g_error_free (error);
          break;
        }

      if (!g_match_info_matches (match_info))
        continue;

      int start = 0;
      int end = 0;
      gchar *word = g_match_info_fetch (match_info, 0);
      g_match_info_fetch_pos (match_info, 0, &start, &end);
      ca = &g_array_index (attrs, VteCharAttributes, start);
      g_print ("Found: %s at (%ld,%ld)\n", word, ca->row, ca->column);

      int len = strlen (word);
      g_free (word);
      //g_match_info_next (match_info, &error);
      gtk_adjustment_set_value (adj, ca->row);
      find->find_row = ca->row;
      find->find_col = ca->column + 1;

      // SELECT TEXT

      _vte_terminal_select_text (vterm, ca->column, ca->row, ca->column + len - 1, ca->row, 0, 0);
      _vte_invalidate_all (vterm);

      break;
    }

  g_match_info_free (match_info);
  g_regex_unref (regex);
}


/**
 * window_died
 */
static void
window_died (GtkWidget *w, gpointer user_data)
{
  printf ("WINDOW DIED = %p (%s)\n", w, GTK_OBJECT_TYPE_NAME (w));
}

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
  //TerminalFind *f = TERMINAL_FIND (user_data);
  printf ("Object has died: %p (%s)\n", w, GTK_OBJECT_TYPE_NAME (w)); fflush (stdout);
  
  if (!find)
    return;

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
  if (find && find->dialog)
    g_object_unref (find->dialog);
#endif
  printf ("done\n"); fflush (stdout);
}



/**
 * terminal_find_response_cb
 */
static void
terminal_find_response_cb (GtkWidget *dialog,
                           int response,
                           TerminalWindow *window)
{
  fprintf (stdout, "Entering %s\n", __FUNCTION__); fflush (stdout);
  printf ("Reponse Find Dialog %d\n", response);

  /* Might get GTK_RESPONSE_CLOSE or GTK_RESPONSE_DELETE_EVENT */
  if (response != GTK_RESPONSE_APPLY)
    {
      gtk_widget_destroy (dialog);
      return;
    }

  terminal_find_do_search (terminal_window_get_active (window));
}

/**
 * terminal_find_clear_cb
 */
static void
terminal_find_clear_cb (GtkEntry *entry,
                        GtkEntryIconPosition icon_pos,
                        GdkEvent *event,
                        gpointer user_data)
{
  fprintf (stdout, "Entering %s\n", __FUNCTION__); fflush (stdout);
  gtk_entry_set_text (entry, "");
}

/**
 * terminal_find_toggled_cb
 */
static void
terminal_find_toggled_cb (GtkToggleButton *togglebutton,
                          gpointer *data)
{
  fprintf (stdout, "Entering %s\n", __FUNCTION__); fflush (stdout);
  gboolean b;
  printf ("toggled\n");

  b = gtk_toggle_button_get_active (togglebutton);

  if (togglebutton == GTK_TOGGLE_BUTTON (find->check_case))
    gconf_client_set_bool (find->gconf, CONF_FIND_CHECK_CASE, b, NULL);
  else if (togglebutton == GTK_TOGGLE_BUTTON (find->check_regex))
    gconf_client_set_bool (find->gconf, CONF_FIND_CHECK_REGEX, b, NULL);
  else if (togglebutton == GTK_TOGGLE_BUTTON (find->check_whole))
    gconf_client_set_bool (find->gconf, CONF_FIND_CHECK_WHOLE, b, NULL);
}

/**
 * terminal_find_text_cb
 */
static void
terminal_find_text_cb (GtkEntry   *entry,
                       GParamSpec *pspec,
                       GtkWidget  *button)
{
  fprintf (stdout, "Entering %s\n", __FUNCTION__); fflush (stdout);
  gboolean has_text;

  has_text = gtk_entry_get_text_length (entry) > 0;
  gtk_entry_set_icon_sensitive (entry, GTK_ENTRY_ICON_SECONDARY, has_text);
  gtk_widget_set_sensitive (button, has_text);
}

/**
 * terminal_find_check_cb
 */
static void
terminal_find_check_cb (GConfClient *client,
                        guint cnxn_id,
                        GConfEntry *entry,
                        gpointer button)
{
  fprintf (stdout, "Entering %s\n", __FUNCTION__); fflush (stdout);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (button), gconf_value_get_bool (entry->value));
}

/**
 * terminal_find_destroyed_cb
 */
static void
terminal_find_destroyed_cb (GtkWidget *dialog,
                            gpointer   window)
{
  fprintf (stdout, "Entering %s\n", __FUNCTION__); fflush (stdout);
  //find->dialog = NULL;

  terminal_find_history_save (find->entry);

  /* Disconnect all the signal handlers */
#if 0
  g_signal_handlers_disconnect_by_func (find->dialog,      G_CALLBACK (terminal_find_destroyed_cb), NULL);
  g_signal_handlers_disconnect_by_func (find->dialog,      G_CALLBACK (terminal_find_response_cb),  window);
  g_signal_handlers_disconnect_by_func (find->entry,       G_CALLBACK (terminal_find_clear_cb),     NULL);
  g_signal_handlers_disconnect_by_func (find->check_case,  G_CALLBACK (terminal_find_toggled_cb),   NULL);
  g_signal_handlers_disconnect_by_func (find->check_regex, G_CALLBACK (terminal_find_toggled_cb),   NULL);
  g_signal_handlers_disconnect_by_func (find->check_whole, G_CALLBACK (terminal_find_toggled_cb),   NULL);
  g_signal_handlers_disconnect_by_func (find->entry,       G_CALLBACK (terminal_find_text_cb),      NULL);
#endif

  TerminalApp *app;

  app = terminal_app_get();

  fprintf (stdout, "    find->dialog = %p (%s)\n", find->dialog, GTK_OBJECT_TYPE_NAME (find->dialog)); fflush (stdout);
  g_signal_handlers_disconnect_by_func (app, G_CALLBACK (rich2), find->dialog);

  // destroy TerminalFind object, too
  g_object_unref (find);
  find = NULL;
}


/**
 * terminal_find_display
 */
void
terminal_find_display (GtkWindow *window)
{
  fprintf (stdout, "Entering %s\n", __FUNCTION__); fflush (stdout);
  GtkButton *button_close = NULL;
  GtkButton *button_find  = NULL;
  gboolean find_case;
  gboolean find_regex;
  gboolean find_whole;

  if (!find)
    terminal_find_get_object ();

  if (find->dialog)
    {
      gtk_window_set_transient_for (GTK_WINDOW (find->dialog), window);
      gtk_window_present (GTK_WINDOW (find->dialog));
      return;
    }

  printf ("terminal_find_display\n");

  printf ("window = %p (%s)\n", window, GTK_OBJECT_TYPE_NAME (window));
  if (!terminal_util_load_builder_file ("terminal-find.ui",
                                        "dialog-find",     &find->dialog,
                                        "check-case",      &find->check_case,
                                        "check-whole",     &find->check_whole,
                                        "check-regex",     &find->check_regex,
                                        "entry-find",      &find->entry,
                                        "button-close",    &button_close,
                                        "button-find",     &button_find,
                                        NULL))
    return;

  find_case  = gconf_client_get_bool (find->gconf, CONF_FIND_CHECK_CASE,  NULL);
  find_regex = gconf_client_get_bool (find->gconf, CONF_FIND_CHECK_REGEX, NULL);
  find_whole = gconf_client_get_bool (find->gconf, CONF_FIND_CHECK_WHOLE, NULL);

  printf ("check_case = %p (%s)\n",  find->check_case,  GTK_OBJECT_TYPE_NAME (find->check_case));
  printf ("check_regex = %p (%s)\n", find->check_regex, GTK_OBJECT_TYPE_NAME (find->check_regex));
  printf ("check_whole = %p (%s)\n", find->check_whole, GTK_OBJECT_TYPE_NAME (find->check_whole));

  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (find->check_case),  find_case);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (find->check_regex), find_regex);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (find->check_whole), find_whole);

  terminal_find_history_init (find->entry);

  gtk_entry_set_icon_from_stock (find->entry, GTK_ENTRY_ICON_SECONDARY, GTK_STOCK_CLEAR);
  gtk_entry_set_icon_sensitive (find->entry, GTK_ENTRY_ICON_SECONDARY, FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET (button_find), FALSE);

  nid_case  = gconf_client_notify_add (gconf, CONF_FIND_CHECK_CASE,  terminal_find_check_cb, check_case,  NULL, NULL);
  nid_regex = gconf_client_notify_add (gconf, CONF_FIND_CHECK_REGEX, terminal_find_check_cb, check_regex, NULL, NULL);
  nid_whole = gconf_client_notify_add (gconf, CONF_FIND_CHECK_WHOLE, terminal_find_check_cb, check_whole, NULL, NULL);

  g_signal_connect (dialog,      "destroy",      G_CALLBACK (terminal_find_destroyed_cb), NULL);
  g_signal_connect (dialog,      "response",     G_CALLBACK (terminal_find_response_cb),  window);
  g_signal_connect (entry,       "icon-press",   G_CALLBACK (terminal_find_clear_cb),     NULL);
  g_signal_connect (check_case,  "toggled",      G_CALLBACK (terminal_find_toggled_cb),   NULL);
  g_signal_connect (check_regex, "toggled",      G_CALLBACK (terminal_find_toggled_cb),   NULL);
  g_signal_connect (check_whole, "toggled",      G_CALLBACK (terminal_find_toggled_cb),   NULL);
  g_signal_connect (entry,       "notify::text", G_CALLBACK (terminal_find_text_cb),      button_find);

  g_signal_connect (window,      "destroy",  G_CALLBACK (window_died), NULL);

#if 1
  g_signal_connect (find->dialog,      "destroy", G_CALLBACK (rich), NULL);
  g_signal_connect (find->check_case,  "destroy", G_CALLBACK (rich), NULL);
  g_signal_connect (find->check_whole, "destroy", G_CALLBACK (rich), NULL);
  g_signal_connect (find->check_regex, "destroy", G_CALLBACK (rich), NULL);
  g_signal_connect (find->entry,       "destroy", G_CALLBACK (rich), NULL);
  g_signal_connect (button_close,      "destroy", G_CALLBACK (rich), NULL);
  g_signal_connect (button_find,       "destroy", G_CALLBACK (rich), NULL);
#endif

#if 0
  gtk_widget_destroyed (GTK_WIDGET (find->dialog),      (GtkWidget **) &find->dialog);
  gtk_widget_destroyed (GTK_WIDGET (find->entry),       (GtkWidget **) &find->entry);
  gtk_widget_destroyed (GTK_WIDGET (find->check_case),  (GtkWidget **) &find->check_case);
  gtk_widget_destroyed (GTK_WIDGET (find->check_regex), (GtkWidget **) &find->check_regex);
  gtk_widget_destroyed (GTK_WIDGET (find->check_whole), (GtkWidget **) &find->check_whole);
#endif

  TerminalApp *app = terminal_app_get();
  g_signal_connect (app, "quit", G_CALLBACK (rich2), find->dialog);

  gtk_window_set_transient_for (GTK_WINDOW (find->dialog), GTK_WINDOW (window));
  gtk_window_present (GTK_WINDOW (find->dialog));
}


