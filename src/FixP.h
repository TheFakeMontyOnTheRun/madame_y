#ifndef FIXP_H
#define FIXP_H

#if defined(MSX) || defined(SDLSW)
#define FixP_t int16_t
#define FixInt_t int16_t
#define FixIntDbl_t int32_t
#else
#define FixP_t int16_t
#define FixInt_t int16_t
#define FixIntDbl_t int32_t
#endif




FixInt_t fixToInt( FixP_t fp);

FixP_t intToFix( FixInt_t v);

FixP_t Mul( FixP_t v1, FixP_t v2);

FixP_t Div( FixP_t v1,  FixP_t v2);

#endif
