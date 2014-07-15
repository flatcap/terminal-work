#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curses.h>
#include <term.h>

int main (int argc, char *argv[])
{
	char buffer[10];
	char *ptr = buffer;
	char *res = NULL;
	int i;
	int ret;

	memset (buffer, 0, sizeof (buffer));

	ret = tgetent (NULL, "linux");
	if (ret <= 0)
		return 1;

	res = tgetstr ("cl", &ptr);

        printf ("'cl' = ");
	for (i = 0; i < sizeof (buffer); i++) {
		if (buffer[i] == 27)
			printf ("<esc>");
		else
			printf ("%c", buffer[i]);
	}
	printf ("\n");

	return 0;
}
