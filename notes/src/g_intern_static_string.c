#include <stdio.h>
#include <string.h>
#include <glib.h>

char *rich = "Richard";
char *rob  = "Robert";

int
main (int argc, char *argv[])
{
	const char *r1 = g_intern_static_string (rich);
	const char *r2 = g_intern_static_string (rob);

	char *r3 = strdup (rich);
	const char *r4 = g_intern_string (r3);

	printf ("r1 = %p, %s\n", r1, r1);
	printf ("r2 = %p, %s\n", r2, r2);
	printf ("r3 = %p, %s\n", r3, r3);
	printf ("r4 = %p, %s\n", r4, r4);
	return 0;
}
