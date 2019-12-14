#include <math.h>
#include <stdint.h>

#include "FixP.h"


uint8_t kIntegerPart = 8;


int16_t
fixToInt( FixP_t fp) {
	return fp >> kIntegerPart;
}

FixP_t intToFix(
		
		int16_t
		v ) {
	return v << kIntegerPart;
}

FixP_t Mul( FixP_t v1,  FixP_t v2) {
  return ((v1 >> 3) * (v2 >> 3)) >> 2;
}

FixP_t Div( FixP_t v1,  FixP_t v2) {
  return (((int32_t)v1) * (1 << kIntegerPart)) / v2;
}
