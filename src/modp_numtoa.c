/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*- */
/* vi: set expandtab shiftwidth=4 tabstop=4: */

#include "modp_numtoa.h"

#include <stdint.h>
#include <stdio.h>
#include <math.h>

// other interesting references on num to string convesion
// http://www.jb.man.ac.uk/~slowe/cpp/itoa.html
// and http://www.ddj.com/dept/cpp/184401596?pgno=6

/**
 * Powers of 10
 * 10^0 to 10^9
 */
static const double pow10[] = {0, 10, 100, 1000, 10000, 100000, 1000000,
							   10000000, 100000000, 1000000000};

static void strreverse(char* begin, char* end)
{
    char aux;
    while (end > begin)
        aux = *end, *end-- = *begin, *begin++ = aux;
}

void modp_itoa10(int32_t value, char* str)
{
    char* wstr=str;
    int sign;
    // Take care of sign
    if ((sign=value) < 0) value = -value;
    // Conversion. Number is reversed.
    do *wstr++ = (48 + (value % 10)); while( value /= 10);
    if (sign < 0) *wstr++ = '-';
    *wstr='\0';

    // Reverse string
    strreverse(str,wstr-1);
}

void modp_uitoa10(uint32_t value, char* str)
{
    char* wstr=str;
    // Conversion. Number is reversed.
    do *wstr++ = 48 + (value % 10); while (value /= 10);
    *wstr='\0';
    // Reverse string
    strreverse(str, wstr-1);
}

void modp_dtoa(double value, char* str, int prec)
{
    const double thres_min = 1.0/2048.0;
    char* wstr=str;
    int sign;

    if (prec < 0) {
        prec = 0;
    } else if (prec > 9) {
        prec = 9;
    }

    if ((sign = value) < 0) value = -value;
	int whole = (int) value;
	double tmp = (value - whole) * pow10[prec];
    uint32_t frac = (uint32_t)(tmp);
	if (tmp - frac > 0.5) {
		++frac;
	}

    if (whole > ((1u<<31)-1) || (frac > 0 && frac < thres_min)) {
		sprintf(str, "%e", (sign < 0) ? -value : value);
		return;
	}

	int count = prec;
	// now do fractional part, as an unsigned number
    do {
		--count;
		*wstr++ = 48 + (frac % 10);
	} while (frac /= 10);
	// add extra 0s
	while (count-- > 0) *wstr++ = '0';
	// add decimal
	*wstr++ = '.';

	// do whole part
    // Take care of sign
    // Conversion. Number is reversed.
    do *wstr++ = 48 + (whole % 10); while (whole /= 10);
    if (sign < 0) *wstr++ = '-';
    *wstr='\0';
    strreverse(str,wstr-1);
}

#include <stdio.h>
#include <time.h>

void testDtoA()
{
	double v[] = {0, 0.1, 0.01, 0.001, 0.5, 1, 1.0001, 1000000, 1000000.1,
				  123456789.123456, 1e200, 1e-10,1e-200, 1u<<31, (1u<<31) -1};

	char buf[100];
	int i;
	int max = sizeof(v)/sizeof(double);
	for (i = 0; i < max; ++i) {
		modp_dtoa(v[i], buf, 7);
		printf("%5.5f == %s\n", v[i], buf);
	}
}

