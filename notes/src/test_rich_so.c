#include <stdio.h>
#include "jim.h"

#if 1                   // Working
#include "buffer.h"
#include "caps.h"
#include "debug.h"
#include "gnome-pty.h"
#include "iso2022.h"
#include "matcher.h"
#include "pty.h"
#include "reaper.h"
#include "ring.h"
#include "table.h"
#include "trie.h"
#include "vterowdata.h"
#include "vtestream-base.h"
#include "vtestream-file.h"
#include "vtestream.h"
#include "vtetc.h"
#include "vteunistr.h"
#include "vteconv.h"
#include "vtepty.h"
#include "vtepty-private.h"
#include "vteregex.h"
#include "vtetree.h"
#include "vteversion.h"
#include "marshal.h"
#endif

#if 0                   // Broken
#include "keymap.h"
#include "vteaccess.h"
#include "vtebg.h"
#include "vtedraw.h"
#include "vte.h"
#include "vteint.h"
#include "vte-private.h"
#include "vtetypebuiltins.h"
#endif

int main (int argc, char *argv[])
{
        printf ("hello world\n");
}

