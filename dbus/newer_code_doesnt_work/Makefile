CC	= gcc
PROGS	= dbus-search dbus-ping-send dbus-ping-listen # multiple window_resize rte term_resize simple_terminal
OBJ	= # cache.o debug.o detab.o list.o options.o view.o
CFLAGS	= -g -Wall

#CFLAGS  += `pkg-config --cflags ncurses gtk+-3.0`
#LDFLAGS += `pkg-config --libs   ncurses gtk+-3.0`
 
CFLAGS  += `pkg-config --cflags dbus-glib-1 glib-2.0`
LDFLAGS += `pkg-config --libs   dbus-glib-1 glib-2.0`

all:	$(PROGS)

clean:
	$(RM) $(PROGS) $(OBJ) tags

tags:
	ctags *.[ch]

multiple:	$(OBJ)

