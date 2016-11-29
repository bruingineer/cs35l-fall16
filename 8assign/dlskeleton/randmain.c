/* 
 * randmain.c
 * josh c
 * 
 * randmain.c should contain the main program that glues together
 * everything else. It should include randcpuid.h (as the corresponding
 * module should be linked statically) but not randlib.h (as the
 * corresponding module should be linked after main starts up).
 * Depending on whether randcpuid reports that the hardware supports
 * the RDRAND instruction, this main program should dynamically link the
 * hardware-oriented or software-oriented implementation of randlib, doing
 * the dynamic linking via dlopen and dlsym. Also, the main program should
 * call dlclose to clean up before exiting. Like randall, if any function
 * called by the main program fails, the main program should report an
 * error and exit with nonzero status.
 * 
 */

#include "randcpuid.h"
#include <stdbool.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h>
#include <dlfcn.h>

/* Main program, which outputs N bytes of random data.  */
int
main (int argc, char **argv)
{
  /* Check arguments.  */
  bool valid = false;
  long long nbytes;
  if (argc == 2)
    {
      char *endptr;
      errno = 0;
      nbytes = strtoll (argv[1], &endptr, 10);
      if (errno)
	perror (argv[1]);
      else
	valid = !*endptr && 0 <= nbytes;
    }
  if (!valid)
    {
      fprintf (stderr, "%s: usage: %s NBYTES\n", argv[0], argv[0]);
      return 1;
    }

  /* If there's no work to do, don't worry about which library to use.  */
  if (nbytes == 0)
    return 0;

  /* Now that we know we have work to do, arrange to use the
     appropriate library.  */
  unsigned long long (*rand64) (void);
  void *implement_rand64;

  if (rdrand_supported ())
    {
      implement_rand64 = dlopen("randlibhw.so", RTLD_LAZY);
    }

  else
    {
      implement_rand64 = dlopen("randlibsw.so", RTLD_LAZY);
    }

  char* error = dlerror();
  if(error) {
    fprintf(stderr, "Error when dynamically linking %s\n", error);
  }

  rand64 = dlsym(implement_rand64, "rand64");

  int wordsize = sizeof rand64 ();
  int output_errno = 0;

  do
    {
      unsigned long long x = rand64 ();
      size_t outbytes = nbytes < wordsize ? nbytes : wordsize;
      if (fwrite (&x, 1, outbytes, stdout) != outbytes)
	{
	  output_errno = errno;
	  break;
	}
      nbytes -= outbytes;
    }
  while (0 < nbytes);

  if (fclose (stdout) != 0)
    output_errno = errno;

  if (output_errno)
    {
      errno = output_errno;
      perror ("output");
      return 1;
    }

  dlclose(implement_rand64);
  return 0;
}
