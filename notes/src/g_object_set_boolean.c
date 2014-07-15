/**
 * set_boolean
 */
gboolean
set_boolean (GtkWidget *widget, gchar *name, gboolean new_val)
{
	GObject *obj;
	GValue value;
	gboolean old_val;

	memset(&value, 0, sizeof(value));
	g_value_init (&value, G_TYPE_BOOLEAN);

	obj = G_OBJECT (widget);
	g_object_get_property (obj, name, &value);
	old_val = g_value_get_boolean (&value);

	g_value_set_boolean (&value, new_val);
	g_object_set_property (obj, name, &value);

	return old_val;
}

