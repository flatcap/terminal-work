
static gboolean
vte_terminal_io_write(GIOChannel *channel,
		      GIOCondition condition,
		      VteTerminal *terminal)
{
	gssize count;
	int fd;
	gboolean leave_open;

        //printf ("c = %d\n", terminal->pvt->outgoing->bytes[0]);
        if (terminal->pvt->outgoing->bytes[0] == 10)
          rich_analyse(terminal);

        //Q printf("Entering: vte_terminal_io_write: '%.*s' (%d)\n", (int)_vte_buffer_length(terminal->pvt->outgoing), terminal->pvt->outgoing->bytes, (int)_vte_buffer_length(terminal->pvt->outgoing));
        /*
        int i;
        printf ("HEX: ");
        for (i = 0; i < (int)_vte_buffer_length(terminal->pvt->outgoing); i++) {
          printf ("%02x ", (guint8)terminal->pvt->outgoing->bytes[i]);
        }
        printf ("\n");
        */


_vte_terminal_insert_char(VteTerminal *terminal, gunichar c,
			 gboolean insert, gboolean invalidate_now)
{
        char buffer[6];
        memset (buffer, 0, sizeof(buffer));
        /*
        int len =
        */
        g_unichar_to_utf8 (c, buffer);

        //Q printf("Entering: _vte_terminal_insert_char '%s'\n", buffer);
        /*
        int j;
        printf ("HEX: ");
        for (j = 0; j < len; j++) {
          printf ("%02x ", (guint8)buffer[j]);
        }
        printf ("\n");
        */

	struct vte_charcell_attr attr;
	VteRowData *row;
	long col;
	int columns, i;
	VteScreen *screen;
	gboolean line_wrapped = FALSE; /* cursor moved before char inserted */

	screen = terminal->pvt->screen;
	insert |= screen->insert_mode;
	invalidate_now |= insert;

        //Q printf ("_vte_terminal_insert_char:\n");
        /*
        printf ("    shell   = %s\n", terminal->pvt->shell);
        printf ("    master  = %d\n", terminal->pvt->pty_master);
        printf ("    pty_pid = %d\n", terminal->pvt->pty_pid);
        */
        //printf ("    terminal  %p\n", terminal);
        //printf ("    pvt       %p\n", terminal->pvt);
        //printf ("    screen    %p\n", terminal->pvt->screen);
        //printf ("    row_data  %p\n", terminal->pvt->screen->row_data);

        //printf ("        delta       %ld\n", terminal->pvt->screen->row_data->delta);
        //printf ("        length      %ld\n", terminal->pvt->screen->row_data->length);
        //printf ("        max         %ld\n", terminal->pvt->screen->row_data->max);
        //printf ("        cached_item %ld\n", terminal->pvt->screen->row_data->cached_item);
        //printf ("        cached_data %p\n", terminal->pvt->screen->row_data->cached_data);
        //printf ("        array       %p\n", terminal->pvt->screen->row_data->array);
        //printf ("        user_data   %p\n", terminal->pvt->screen->row_data->user_data);

        //VteRowData *row_data = _vte_terminal_find_row_data (terminal, 0);
        VteRowData *row_data = _vte_terminal_find_row_data (terminal, terminal->pvt->screen->row_data->length-1);
        //Q printf ("row_data = %p\n", row_data);
        if (row_data) {
          //Q printf ("    cells    = %p\n", row_data->cells);
          //Q printf ("    len      = %d\n", row_data->cells->len);
          //printf ("    wrapped  = %i\n", row_data->soft_wrapped);

          struct vte_charcell *cc = NULL;
          GString *gs = NULL;
          gs = g_string_new(NULL);

          //printf ("    data     = ");
          //Q printf ("    CharCell (%ld):\n", sizeof (struct vte_charcell));
          for (i = 0; i < row_data->cells->len; i++)
          {
            cc = &g_array_index (row_data->cells, struct vte_charcell, i);
            //rich_dump_charcell (cc, 8);
            g_string_append_unichar (gs, cc->c);
            i += (cc->attr.columns - 1);
          }

          //Q printf ("        >>%s<<\n", gs->str);
          g_string_free (gs, TRUE);

        }

