#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

typedef struct _row {
	long text_offset;
	long attr_offset;
} Row;

typedef struct _attr {
	unsigned long offset;

	unsigned int fragment: 1;
	unsigned int columns: 4;
	unsigned int bold: 1;
	unsigned int fore: 9;
	unsigned int back: 9;
	unsigned int standout: 1;
	unsigned int underline: 1;
	unsigned int strikethrough: 1;
	unsigned int reverse: 1;
	unsigned int blink: 1;
	unsigned int half: 1;
	unsigned int invisible: 1;
} Attr;

void
print_attr (int i, Attr *attr)
{
	int fg, bg;

	printf ("%3d  %4lu  ", i, attr->offset);

	fg = attr->fore;
	bg = attr->back;
	if (fg >= 256) fg = 7;
	if (bg >= 256) bg = 0;

	printf ("%2d %c%c%c%c%c%c%c%c%c [38;5;%dm%03d[0m [48;5;%dm%03d[0m\n",
		attr->columns,
		attr->invisible     ? 'i' : ' ',
		attr->bold          ? 'b' : ' ',
		attr->reverse       ? 'r' : ' ',
		attr->underline     ? 'u' : ' ',
		attr->strikethrough ? 's' : ' ',
		attr->fragment      ? 'f' : ' ',
		attr->standout      ? 'S' : ' ',
		attr->blink         ? 'B' : ' ',
		attr->half          ? 'h' : ' ',
		fg, attr->fore,
		bg, attr->back);
}

int
main (int argc, char *argv[])
{
	int fd_text = -1;
	int fd_attr = -1;
	int fd_row = -1;
	int count;
	Row row;
	Attr attr;
	int i;
	char *text = NULL;
	int offset = 0;
	int len;

	if (argc != 4) {
		printf ("Usage: %s text attr row\n", argv[0]);
		return 1;
	}

	fd_text = open (argv[1], O_RDONLY);
	if (fd_text < 0) {
		printf ("Couldn't open text stream '%s'\n", argv[1]);
		return 1;
	}

	fd_attr = open (argv[2], O_RDONLY);
	if (fd_attr < 0) {
		printf ("Couldn't open attr stream '%s'\n", argv[2]);
		return 1;
	}

	fd_row = open (argv[3], O_RDONLY);
	if (fd_row < 0) {
		printf ("Couldn't open row stream '%s'\n", argv[3]);
		return 1;
	}

	/*
	printf ("text stream: %s\n", argv[1]);
	printf ("attr stream: %s\n", argv[2]);
	printf ("row  stream: %s\n", argv[3]);
	printf ("\n");
	*/

	text = malloc (4096);
	if (!text) {
		printf ("malloc failed\n");
		return 1;
	}

	count = read (fd_text, text, 4096);
	if (count < 0) {
		printf ("text read failed\n");
		return 1;
	}

	for (i = 0; i < 4096; i++) {
		if (text[i] == '\n')
			text[i] = 0;
	}

	printf ("attroff  off  len     text\n");
	for (i = 0; i < 15; i++) {
		count = read (fd_row, &row, sizeof (row));
		if (count < sizeof (row)) {
			break;
			printf ("Row read failed\n");
			return 1;
		}

		len = strlen (&text[row.text_offset])+1;
		printf ("%5ld   %4u  %3u     %s\n", row.attr_offset, offset, len, &text[row.text_offset]);
		offset += len;
		//printf ("attr: %ld\n", row.attr_offset);
	}
	printf ("\n");

	printf ("file  off cols   attrs   fg bg\n");
	for (i = 0; i < 50; i++) {
		count = read (fd_attr, &attr, sizeof (attr));
		if (count < sizeof (attr)) {
			break;
			printf ("Attr read failed\n");
			return 1;
		}

		print_attr (i*sizeof (Attr), &attr);
	}

	free (text);
	close (fd_text);
	close (fd_attr);
	close (fd_row);
	return 0;
}

