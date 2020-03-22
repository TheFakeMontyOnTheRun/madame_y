#include <math.h>
#include <stdint.h>

#include "FixP.h"

#if defined(MSX) || defined(SDLSW)
uint8_t kIntegerPart = 8;
#else
uint8_t kIntegerPart = 8;
#endif


FixInt_t
fixToInt(FixP_t fp) {
    return fp >> kIntegerPart;
}

FixP_t intToFix(

        FixInt_t
        v) {
    return v << kIntegerPart;
}

FixP_t Mul(FixP_t v1, FixP_t v2) {
    return ((v1) * (v2)) >> (kIntegerPart);
}

FixP_t Div(FixP_t v1, FixP_t v2) {
    return (((FixIntDbl_t) v1) * (1 << kIntegerPart)) / v2;
}
