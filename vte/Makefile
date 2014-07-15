#-------------------------------------------------------------------------------
# Commands
#
ARCH	= $(shell uname -m)
AWK	= awk
CAT	= cat
#CC	= ~/bin/colorgcc.pl
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
#APPSRC = vteapp.c
APPSRC = app-plain.c
#APPSRC = app-cairo.c

OUT	= app

VTE_DIR		= ../git/vte
VTE_SRC_DIR	= $(VTE_DIR)/src

CONFIG	= config.h

# Source directly linked from the vte dir
VTE_SRC	= caps.c debug.c iso2022.c keymap.c matcher.c pty.c reaper.c \
	  ring.c table.c trie.c vte.c vteaccess.c vtebg.c vteconv.c vtedraw.c \
	  vteregex.c vteseq.c vtetc.c vtetree.c vteunistr.c vterowdata.c \
	  vtestream.c

VTE_HDR	= buffer.h caps.h debug.h iso2022.h keymap.h matcher.h pty.h reaper.h \
	  ring.h table.h trie.h vte.h vteaccess.h vtebg.h vteconv.h vtedraw.h \
	  vteint.h vte-private.h vtepty.h vtepty-private.h vteregex.h vtetc.h \
	  vtetree.h vteunistr.h vterowdata.h vtestream-base.h vtestream-file.h \
	  vtestream.h vtedeprecated.h

# Generated source, to be compiled
GEN_SRC	= app.c marshal.c vtetypebuiltins.c
GEN_HDR	= marshal.h vteseq-list.h vtetypebuiltins.h vteversion.h gnome-pty.h \
	  vteseq-2.c vteseq-n.c gnome-pty.h

# Source copied and #included
COPY_SRC = keysyms.c

#LINKS	= gobject gtk src symbols work unitable
LINKS	= unitable

# Things we need to compile
COMPILE_SRC	= $(VTE_SRC) $(GEN_SRC)

COMPILE_OBJ	= $(COMPILE_SRC:.c=.o)

SOURCE	= $(VTE_SRC) $(VTE_HDR) $(GEN_SRC) $(GEN_HDR) $(COPY_SRC)

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
CFLAGS	+= -DDATADIR='"/home/work/terminal/install/share"'
#CFLAGS	+= -DG_DISABLE_ASSERT
CFLAGS	+= -DG_LOG_DOMAIN='"Vte"'
CFLAGS	+= -DHAVE_CONFIG_H
CFLAGS	+= -DLIBEXECDIR='"/home/work/terminal/install/libexec"'
CFLAGS	+= -DLOCALEDIR='"/home/work/terminal/install/share/locale"'
CFLAGS	+= -DTERMCAPDIR='"/home/work/terminal/git/vte/termcaps"'
CFLAGS	+= -DVTE_COMPILATION
#CFLAGS	+= -D_GNU_SOURCE
CFLAGS	+= -DVTE_DEBUG

CFLAGS	+= -DATK_DISABLE_DEPRECATED=1
CFLAGS	+= -DG_DISABLE_DEPRECATED=1
CFLAGS	+= -DGDK_DISABLE_DEPRECATED=1
CFLAGS	+= -DGDK_PIXBUF_DISABLE_DEPRECATED=1
CFLAGS	+= -DGTK_DISABLE_DEPRECATED=1
CFLAGS	+= -DPANGO_DISABLE_DEPRECATED=1
#CFLAGS	+= -DVTE_DISABLE_DEPRECATED=1

# Warning flags
CFLAGS	+= -Waggregate-return
CFLAGS	+= -Wall
CFLAGS	+= -Wcast-align
#CFLAGS	+= -Wdeclaration-after-statement
CFLAGS	+= -Werror-implicit-function-declaration
CFLAGS	+= -Wextra
CFLAGS	+= -Wimplicit
CFLAGS	+= -Winit-self
CFLAGS	+= -Wmissing-declarations
CFLAGS	+= -Wmissing-format-attribute
CFLAGS	+= -Wmissing-prototypes
CFLAGS	+= -Wnested-externs
CFLAGS	+= -Wno-missing-field-initializers
CFLAGS	+= -Wno-switch-enum
CFLAGS	+= -Wno-unused-parameter
CFLAGS	+= -Wold-style-definition
CFLAGS	+= -Wpacked
CFLAGS	+= -Wpointer-arith
CFLAGS	+= -Wshadow
CFLAGS	+= -Wsign-compare
CFLAGS	+= -Wstrict-aliasing=2
CFLAGS	+= -Wstrict-prototypes
CFLAGS	+= -Wunsafe-loop-optimizations
CFLAGS	+= -Wwrite-strings

# Basic include dirs
CFLAGS	+= -I.
CFLAGS	+= -Iunitable
CFLAGS	+= $(shell pkg-config x11 gobject-2.0 gio-unix-2.0 gdk-2.0 gtk+-2.0 cairo ncurses --cflags)
LDFLAGS	+= $(shell pkg-config x11 gobject-2.0 gio-unix-2.0 gdk-2.0 gtk+-2.0 cairo ncurses --libs)
LDFLAGS	+= -lm

# Dummy for pty.c
CFLAGS	+= -I$(VTE_DIR)/doc

# Extra linker libs
#LDFLAGS	+= -lgcov

#-------------------------------------------------------------------------------
# Build targets
#
all:    $(LINKS) $(OUT) tags

.c.o:
	$(CC) $(CFLAGS) $(CFLAGS_WARN) $(CFLAGS_INC) $(CFLAGS_DEF) -c $< -o $@

source:	$(SOURCE)

$(OUT):	app.o $(COMPILE_OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

tags:   $(SOURCE) force
	ctags *.[ch]

force:

clean:
	$(RM) $(COMPILE_OBJ)
	$(RM) $(OUT)
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

app.c:
	$(LN) $(VTE_SRC_DIR)/$(APPSRC) $@

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

