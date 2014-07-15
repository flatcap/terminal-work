#-------------------------------------------------------------------------------
# Commands
#
ARCH	= $(shell uname -m)
AWK	= awk
CAT	= cat
CC	= ~/bin/colorgcc.pl
CP	= cp
GEN_GGM	= glib-genmarshal
GEN_GME	= glib-mkenums
GPERF	= gperf
GREP	= grep
LN	= ln
MKDIR	= mkdir -p
MV	= mv
NM	= nm
RM	= rm -fr
SED	= sed
SORT	= LANG=C sort

#-------------------------------------------------------------------------------
# File lists
#

# Things to build
APP_STAT= app
APP_RICH= app2
LIB_RICH= librich.so.1

#OUT	= $(APP_STAT) $(APP_RICH) $(LIB_RICH)
OUT	= $(APP_STAT)

VTE_DIR		= ../git/vte
VTE_SRC_DIR	= $(VTE_DIR)/src

CONFIG	= config.h

# Source directly linked from the vte dir
VTE_SRC	= app.c caps.c debug.c iso2022.c keymap.c matcher.c pty.c reaper.c \
	  ring.c table.c trie.c vte.c vteaccess.c vtebg.c vteconv.c vtedraw.c \
	  vteregex.c vteseq.c vtetc.c vtetree.c vteunistr.c vterowdata.c \
	  vtestream.c

VTE_HDR	= buffer.h caps.h debug.h iso2022.h keymap.h matcher.h pty.h reaper.h \
	  ring.h table.h trie.h vte.h vteaccess.h vtebg.h vteconv.h vtedraw.h \
	  vteint.h vte-private.h vtepty.h vtepty-private.h vteregex.h vtetc.h \
	  vtetree.h vteunistr.h vterowdata.h vtestream-base.h vtestream-file.h \
	  vtestream.h vtedeprecated.h

# Generated source, to be compiled
GEN_SRC	= marshal.c vtetypebuiltins.c
GEN_HDR	= marshal.h vteseq-list.h vtetypebuiltins.h vteversion.h gnome-pty.h \
	  vteseq-2.c vteseq-n.c gnome-pty.h

# Source copied and #included
COPY_SRC = keysyms.c \
	   rseq-gt.c \
	   rseq-unknown.c \
	   rseq-vte.c

# Library definitions
LIB_RICH_DEFS	= librich.ver

LINKS	= gobject gtk src symbols work unitable

# Things we need to compile
COMPILE_SRC	= $(VTE_SRC) $(GEN_SRC)

COMPILE_OBJ	= $(COMPILE_SRC:.c=.o)

SOURCE	= $(VTE_SRC) $(VTE_HDR) $(GEN_SRC) $(GEN_HDR) $(COPY_SRC)

LIB_RICH_OBJ	= caps.o debug.o iso2022.o marshal.o matcher.o pty.o reaper.o \
		  ring.o table.o trie.o vteconv.o vteregex.o vterowdata.o \
		  vtestream.o vtetc.o vtetree.o vteunistr.o

OTHER_OBJ = keymap.o marshal.o vteaccess.o vtebg.o vte.o vteconv.o vtedraw.o vteseq.o vtetypebuiltins.o

#-------------------------------------------------------------------------------
# Compilation flags
#
# General compiler flags
CFLAGS	+= -g
#CFLAGS	+= -O2
CFLAGS	+= -fno-common
#CFLAGS	+= -fprofile-arcs
#CFLAGS	+= -ftest-coverage

# For shared libraries
CFLAGS	+= -fPIC
CFLAGS	+= -DPIC

# Extra definitions
CFLAGS_DEF	+= -DDATADIR='"/home/work/terminal/install/share"'
#CFLAGS_DEF	+= -DG_DISABLE_ASSERT
CFLAGS_DEF	+= -DG_LOG_DOMAIN='"Vte"'
CFLAGS_DEF	+= -DHAVE_CONFIG_H
CFLAGS_DEF	+= -DLIBEXECDIR='"/home/work/terminal/install/libexec"'
CFLAGS_DEF	+= -DLOCALEDIR='"/home/work/terminal/install/share/locale"'
CFLAGS_DEF	+= -DVTE_COMPILATION
#CFLAGS_DEF	+= -D_GNU_SOURCE
CFLAGS_DEF	+= -DVTE_DEBUG

# Warning flags
CFLAGS_WARN	+= -Waggregate-return
CFLAGS_WARN	+= -Wall
CFLAGS_WARN	+= -Wcast-align
#CFLAGS_WARN	+= -Wdeclaration-after-statement
CFLAGS_WARN	+= -Werror-implicit-function-declaration
CFLAGS_WARN	+= -Wextra
CFLAGS_WARN	+= -Wimplicit
CFLAGS_WARN	+= -Winit-self
CFLAGS_WARN	+= -Wmissing-declarations
CFLAGS_WARN	+= -Wmissing-format-attribute
CFLAGS_WARN	+= -Wmissing-prototypes
CFLAGS_WARN	+= -Wnested-externs
CFLAGS_WARN	+= -Wno-missing-field-initializers
CFLAGS_WARN	+= -Wno-switch-enum
CFLAGS_WARN	+= -Wno-unused-parameter
CFLAGS_WARN	+= -Wold-style-definition
CFLAGS_WARN	+= -Wpacked
CFLAGS_WARN	+= -Wpointer-arith
CFLAGS_WARN	+= -Wshadow
CFLAGS_WARN	+= -Wsign-compare
CFLAGS_WARN	+= -Wstrict-aliasing=2
CFLAGS_WARN	+= -Wstrict-prototypes
CFLAGS_WARN	+= -Wunsafe-loop-optimizations
CFLAGS_WARN	+= -Wwrite-strings

# Basic include dirs
CFLAGS_INC	+= -I.
CFLAGS_INC	+= -Iunitable
CFLAGS_INC	+= $(shell pkg-config x11 gobject-2.0 gio-unix-2.0 gdk-2.0 gtk+-2.0 cairo ncurses --cflags)
LDFLAGS	+= $(shell pkg-config x11 gobject-2.0 gio-unix-2.0 gdk-2.0 gtk+-2.0 cairo ncurses --libs)
LDFLAGS	+= -lm

# Definitions
CFLAGS_DEF	+= -DATK_DISABLE_DEPRECATED=1
CFLAGS_DEF	+= -DATK_DISABLE_SINGLE_INCLUDES=1
CFLAGS_DEF	+= -DGDK_DISABLE_DEPRECATED=1
CFLAGS_DEF	+= -DGDK_DISABLE_SINGLE_INCLUDES=1
CFLAGS_DEF	+= -DGDK_PIXBUF_DISABLE_DEPRECATED=1
CFLAGS_DEF	+= -DGDK_PIXBUF_DISABLE_SINGLE_INCLUDES=1
CFLAGS_DEF	+= -DGTK_DISABLE_DEPRECATED=1
CFLAGS_DEF	+= -DGTK_DISABLE_SINGLE_INCLUDES=1
CFLAGS_DEF	+= -DG_DISABLE_DEPRECATED=1
CFLAGS_DEF	+= -DG_DISABLE_SINGLE_INCLUDES=1
CFLAGS_DEF	+= -DPANGO_DISABLE_DEPRECATED=1
CFLAGS_DEF	+= -DPANGO_DISABLE_SINGLE_INCLUDES=1

#CFLAGS_DEF	+= -DVTE_DISABLE_DEPRECATED=1


# Extra linker libs
#LDFLAGS	+= -lgcov

# librich linker flags
LDFLAGS_RICH += -Wl,-soname
LDFLAGS_RICH += -Wl,librich.so.1
LDFLAGS_RICH += -Wl,-version-script
LDFLAGS_RICH += -Wl,librich.ver

#-------------------------------------------------------------------------------
# Build targets
#
all:    $(LINKS) source $(OUT) tags

.c.o:
	$(CC) $(CFLAGS) $(CFLAGS_WARN) $(CFLAGS_INC) $(CFLAGS_DEF) -c $< -o $@

source:	$(SOURCE)

$(LIB_RICH):	$(LIB_RICH_OBJ) $(LIB_RICH_DEFS)
	$(CC) -shared $(LIB_RICH_OBJ) $(LDFLAGS_RICH) -lgio-2.0 -lglib-2.0 -o $@

$(APP_STAT):	app.o $(COMPILE_OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

$(APP_RICH):	app.o $(OTHER_OBJ) $(LIB_RICH)
	$(CC) $^ -o $@ debug.o $(LDFLAGS)

#$(COMPILE_OBJ): $(SOURCE)

tags:   $(SOURCE) force
	ctags *.[ch]

force:

clean:
	$(RM) $(COMPILE_OBJ)
	$(RM) $(OUT)
	$(RM) $(LIB_RICH_DEFS)
	#$(RM) *.gcno *.gcda *.gcov

distclean: clean
	#$(RM) $(CONFIG)
	$(RM) $(VTE_SRC) $(VTE_HDR)
	$(RM) $(GEN_SRC) $(GEN_HDR)
	$(RM) $(COPY_SRC)
	$(RM) tags typescript
	$(RM) caller-*
	$(RM) $(LINKS)
	$(RM) unitable

#-------------------------------------------------------------------------------
# Symlinks, etc
#
links:	$(LINKS)

$(VTE_SRC) $(VTE_HDR) $(COPY_SRC):
	$(LN) $(VTE_SRC_DIR)/$@

gnome-pty.h:
	$(LN) $(VTE_DIR)/gnome-pty-helper/$@

gobject:
	$(LN) -s /usr/src/dev/glib-2.24.1/gobject

gtk:
	$(LN) -s /usr/src/dev/gtk+-2.20.1/gtk

symbols:
	$(LN) -s /home/work/terminal/notes/symbols

src:
	$(LN) -s /home/work/terminal/git/vte/src $@

work:
	$(LN) -s /home/work/terminal/vte $@

unitable:
	$(MKDIR) $@
	$(CP) -l $(VTE_SRC_DIR)/unitable.* $@

#-------------------------------------------------------------------------------
# Generated files
#
$(CONFIG):	$(VTE_DIR)/$(CONFIG)
	$(GREP) "^#define" $(VTE_DIR)/$(CONFIG) | $(SORT) > $@

marshal.c:
	$(GEN_GGM) --prefix=_vte_marshal --header --body --internal $(VTE_SRC_DIR)/marshal.list > $@

marshal.h:
	$(GEN_GGM) --prefix=_vte_marshal --header --internal $(VTE_SRC_DIR)/marshal.list > $@

vtetypebuiltins.c: vte.h vtepty.h
	$(GEN_GME) --template $(VTE_SRC_DIR)/vtetypebuiltins.c.template vte.h vtepty.h > $@

vtetypebuiltins.h: vte.h vtepty.h
	$(GEN_GME) --template $(VTE_SRC_DIR)/vtetypebuiltins.h.template vte.h vtepty.h > $@

vteseq-list.h:
	$(SED) -n '/^[^#].*\<VTE_SEQUENCE_HANDLER\>/s/.*, //p' $(VTE_SRC_DIR)/vteseq-{2,n}.gperf | LANG=C sort -u > $@

vteseq-2.c:
	$(GPERF) -m 100 --no-strlen $(VTE_SRC_DIR)/vteseq-2.gperf > $@

vteseq-n.c:
	$(GPERF) -m 100 $(VTE_SRC_DIR)/vteseq-n.gperf > $@

vteversion.h:
	@VTE_MAJ=$(shell $(SED) -n '1{s/^.*\[\([0-9]\+\).*/\1/;p;q}' $(VTE_DIR)/configure.in);	\
	 VTE_MIN=$(shell $(SED) -n '2{s/^.*\[\([0-9]\+\).*/\1/;p;q}' $(VTE_DIR)/configure.in);	\
	 VTE_MIC=$(shell $(SED) -n '3{s/^.*\[\([0-9]\+\).*/\1/;p;q}' $(VTE_DIR)/configure.in);	\
	$(SED) -e "s/@VTE_MAJOR_VERSION@/$$VTE_MAJ/" -e "s/@VTE_MINOR_VERSION@/$$VTE_MIN/" -e "s/@VTE_MICRO_VERSION@/$$VTE_MIC/" $(VTE_SRC_DIR)/vteversion.h.in > $@

# Generated lib data
librich.ver: $(LIB_RICH_OBJ)
	echo "{ global:" > $@
	$(NM) -B $(LIB_RICH_OBJ) | grep -wof symbols/symbols_librich | sort -u | sed 's/$$/;/' >> $@
	echo "local: *; };" >> $@

# Caller tags - where all the librich functions get used
caller-tags:
	$(GREP) -nof symbols/symbols_librich $$($(CAT) symbols/src_for_vte) | $(AWK) -F: '{print $$3"\t"$$1"\t"$$2";\"\tf"}' | $(SORT) > $@

caller-bp:
	$(GREP) -nof symbols/symbols_librich $$($(CAT) symbols/src_for_vte) | $(AWK) -F: '{print "b "$$1":"$$2}' | $(SORT) > $@

