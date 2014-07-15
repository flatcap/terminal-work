// gcc -o jim{,.c} -lreadline
#include <stdio.h>
#include <stdlib.h>

#include <readline/readline.h>
#include <readline/history.h>

int my_startup (void)
{
        //printf ("startup\n");
        return 0;
}

int my_pre_input (void)
{
        //printf ("pre_input\n");
        return 0;
}

int my_event (void)
{
        //printf ("event\n");
        if (rl_line_buffer[rl_point] == 'r') {
                rl_done = 1;
        }
        return 0;
}

int main (int argc, char *argv[])
{
	char *buf;

	rl_bind_key ('\t', rl_abort);	   //disable auto-complete

        rl_startup_hook   = my_startup;
        rl_pre_input_hook = my_pre_input;
        rl_event_hook     = my_event;

        //rl_num_chars_to_read = 1;
	while ((buf = readline (">> ")) != NULL) {
		if (strcmp (buf, "quit") == 0) {
			free (buf);
			break;
		}

		printf ("[%s]\n", buf);

		if (buf[0] != 0)
			add_history (buf);

		free (buf);
	}

	return 0;
}
