#include <glib.h>
#include <dbus/dbus.h>
#include <dbus/dbus-glib.h>
#include <gio/gio.h>

static gboolean
send_ping (DBusGConnection *bus)
{
	GDBusMessage *message;

	/* Create a new signal "Ping" on the "com.burtonini.dbus.Signal" interface, from the object "/com/burtonini/dbus/ping". */
	message = g_dbus_message_new_signal ("/com/burtonini/dbus/ping", "com.burtonini.dbus.Signal", "Ping");

	/* Append the string "Ping!" to the signal */
	dbus_message_append_args (message, DBUS_TYPE_STRING, "Ping!", DBUS_TYPE_INVALID);

	/* Send the signal */
	dbus_connection_send (bus, message, NULL);

	/* Free the signal now we have finished with it */
	g_object_unref (message);

	/* Tell the user we sent a signal */
	g_print ("Ping!\n");

	/* Return TRUE to tell the event loop we want to be called again */
	return TRUE;
}

int
main (int argc, char **argv)
{
	GMainLoop       *loop  = NULL;
	DBusGConnection *bus   = NULL;
	GError          *error = NULL;

	/* Create a new event loop to run in */
	loop = g_main_loop_new (NULL, FALSE);

	/* Get a connection to the session bus */
	bus = dbus_g_bus_get (DBUS_BUS_SESSION, &error);
	if (!bus) {
		g_warning ("Failed to connect to the D-BUS daemon: %s", error->message);
		g_error_free (error);
		g_main_loop_unref (loop);
		return 1;
	}

	/* Every second call send_ping() with the bus as an argument */
	g_timeout_add (1000, (GSourceFunc) send_ping, bus);

	/* Start the event loop */
	g_main_loop_run (loop);

	/* Clean up */
	g_main_loop_unref (loop);
	dbus_g_connection_unref (bus);
	return 0;
}

