#include <sys/time.h>

#if 1
  struct timeval t;
  gettimeofday (&t, NULL);
  printf ("begin search %ld.%06ld\n", t.tv_sec, t.tv_usec); fflush (stdout);
#endif

#if 1
  gettimeofday (&t, NULL);
  printf ("end  search %ld.%06ld\n", t.tv_sec, t.tv_usec); fflush (stdout);
#endif
