#ifndef FIXP_H
#define FIXP_H


#define FixP_t int16_t


int16_t fixToInt( FixP_t fp);

FixP_t intToFix( int16_t v);

FixP_t Mul( FixP_t v1, FixP_t v2);

FixP_t Div( FixP_t v1,  FixP_t v2);

#endif
