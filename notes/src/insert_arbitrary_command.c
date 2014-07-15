/* Send locally-encoded characters to the child. */
static gboolean
vte_terminal_io_write(GIOChannel *channel,
		      GIOCondition condition,
		      VteTerminal *terminal)
{

...

	fd = g_io_channel_unix_get_fd(channel);

+       insert arbitrary command
+       count = write(fd, "cd /etc", 7);

	count = write(fd, terminal->pvt->outgoing->bytes,

...

