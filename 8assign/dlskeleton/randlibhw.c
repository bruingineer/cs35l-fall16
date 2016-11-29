/* 
 * randlibhw.c
 * josh c
 *
 * contains the hardware implementation of the random number generator.
 * It should start by including randlib.h and should implement the
 * interface described by randlib.h.
 *
 */

#include "randlib.h"
#include <immintrin.h>

/* Initialize the hardware rand64 implementation.  */


/* Return a random value, using hardware operations.  */
static unsigned long long
hardware_rand64 (void)
{
  unsigned long long int x;
  while (! _rdrand64_step (&x))
    continue;
  return x;
}

/* Finalize the hardware rand64 implementation.  */




