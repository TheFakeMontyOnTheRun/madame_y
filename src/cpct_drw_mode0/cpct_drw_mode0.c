//-----------------------------LICENSE NOTICE------------------------------------
//  This file contains code to draw lines and plot points in mode 0 using CPCtelera.
//  Copyright (C) 2016 Ervin Pajor
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//-------------------------------------------------------------------------------
//
// This code is designed to be used with an unmodified mode 0 screen.
// (i.e. 20x25 characters)
//
// The main functions of interest are:
// -----------------------------------
//
// cpct_drw_setLineColour_mode0
// cpct_drw_setClippingBox_mode0
// cpct_drw_plotClipped_mode0
// cpct_drw_plotUnclipped_mode0
// cpct_drw_line_mode0
//
// To use the functions, you MUST include the following code at the top of your main.c:
// ------------------------------------------------------------------------------------
//
// #include <cpctelera.h>
// #include "../../cpct_drw_shared/cpct_drw_shared.h"
// #include "cpct_drw_mode0/cpct_drw_mode0.h"
//
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------

#include "cpct_drw_data_mode0.h"
#include "cpct_drw_mode0.h"

void cpct_drw_1_mode0()__naked{
__asm
ld hl,(_cpct_drw_vram_mode0)

ld a,(_cpct_drw_colourMask_mode0)
ld b,a

ld a,(_cpct_drw_lineMask_mode0)
ld c,a

ret
__endasm;
}

void cpct_drw_2_mode0()__naked{
__asm
ld hl,(_cpct_drw_len_mode0)
dec hl
ld b,l
inc b
ld c,h
inc c

ret
__endasm;
}

void cpct_drw_3_mode0()__naked{
__asm
ex af,af
ld a,(_cpct_drw_remainderX_mode0)
ex af,af

ld a,(_cpct_drw_remainderY_mode0)
.db 0xdd
ld l,a

ret
__endasm;
}

void cpct_drw_4_mode0()__naked{
__asm
ld a,(_cpct_drw_remainderX_mode0)
.db 0xdd
ld h,a

ex af,af
ld a,(_cpct_drw_remainderY_mode0)
ex af,af

ret
__endasm;
}

void cpct_drw_5_mode0()__naked{
__asm
ld de,(_cpct_drw_dx_mode0)
xor a
sub e
ld e,a
sbc a,a
sub d
ld d,a

ret
__endasm;
}

void cpct_drw_6_mode0()__naked{
__asm
ld hl,(_cpct_drw_dx_mode0)
xor a
sub l
ld l,a
sbc a,a
sub h
ld h,a

ret
__endasm;
}

void cpct_drw_7_mode0()__naked{
__asm
ld de,(_cpct_drw_dy_mode0)
xor a
sub e
ld e,a
sbc a,a
sub d
ld d,a

ret
__endasm;
}

void cpct_drw_8_mode0()__naked{
__asm
ld hl,(_cpct_drw_dy_mode0)
xor a
sub l
ld l,a
sbc a,a
sub h
ld h,a

ret
__endasm;
}

void cpct_drw_9_mode0(){
	cpct_drw_len_mode0=cpct_drw_x2_mode0-cpct_drw_x1_mode0+1;
}

void cpct_drw_10_mode0(){
	cpct_drw_len_mode0=cpct_drw_y2_mode0-cpct_drw_y1_mode0+1;
}

void cpct_drw_11_mode0(){
	cpct_drw_len_mode0=cpct_drw_left_mode0-cpct_drw_x1_mode0;
}

void cpct_drw_12_mode0(){
	cpct_drw_len_mode0=cpct_drw_top_mode0-cpct_drw_y1_mode0;
}

void cpct_drw_13_mode0(){
	cpct_drw_len_mode0=cpct_drw_y1_mode0-cpct_drw_bottom_mode0;
}

void cpct_drw_14_mode0(){
	if (cpct_drw_x2_mode0>cpct_drw_right_mode0)
			cpct_drw_x2_mode0=cpct_drw_right_mode0;
}

unsigned char cpct_drw_15_mode0(){
	return (cpct_drw_x1_mode0>cpct_drw_right_mode0);
}

unsigned char cpct_drw_16_mode0(){
	return (cpct_drw_x2_mode0<cpct_drw_left_mode0);
}

unsigned char cpct_drw_17_mode0(){
	return (cpct_drw_y1_mode0<cpct_drw_top_mode0);
}

unsigned char cpct_drw_18_mode0(){
	return (cpct_drw_y1_mode0>cpct_drw_bottom_mode0);
}

unsigned char cpct_drw_19_mode0(){
	return (cpct_drw_y2_mode0<cpct_drw_top_mode0);
}

unsigned char cpct_drw_20_mode0(){
	return (cpct_drw_y2_mode0>cpct_drw_bottom_mode0);
}

void cpct_drw_21_mode0(){
	cpct_drw_yn_mode0=cpct_drw_dx_mode0>>1;
}

void cpct_drw_22_mode0(){
	if (cpct_drw_y2_mode0>cpct_drw_bottom_mode0)
		cpct_drw_y2_mode0=cpct_drw_bottom_mode0;
}

// cpct_drw_setLineColour_mode0 MUST be run at least once before any mode 0 line drawing routines are called
// cpct_drw_populateLineMasks_mode0 MUST have been run before cpct_drw_setLineColour_mode0 is first run

void cpct_drw_setLineColour_mode0(unsigned char colour){
	cpct_drw_cmask_mode0=cpct_drw_lineMasks_mode0[colour];
}

// cpct_drw_plotClipped_mode0 plots a point in mode 0
// clipping is performed to prevent out-of-bounds plotting

void cpct_drw_plotClipped_mode0(signed int px,signed int py){
	px;
	py;

	__asm
	ld hl,#2
	add hl,sp
	ld de,#_cpct_drw_x1_mode0
	ldi
	ldi
	ld de,#_cpct_drw_y1_mode0
	ldi
	ldi

    ld hl,(_cpct_drw_right_mode0)
    ld de,(_cpct_drw_x1_mode0)
    or a,a
    sbc hl,de
    ret c

	ex de,hl
	ld de,(_cpct_drw_left_mode0)
	or a,a
	sbc hl,de
	ret c

	ld hl,(_cpct_drw_bottom_mode0)
	ld de,(_cpct_drw_y1_mode0)
	or a,a
	sbc hl,de
	ret c

	ex de,hl
	ld bc,(_cpct_drw_top_mode0)
	or a,a
	sbc hl,bc
	ret c
	__endasm;

	__asm
	ld	hl,(_cpct_drw_y1_mode0)
	add	hl, hl
	ld	bc,#_cpct_drw_yAddress
	add	hl,bc

	ld	c,(hl)
	inc	hl
	ld	b,(hl)

	ld hl,(_cpct_drw_x1_mode0)
	ld e,l
	srl h
	rr l
	add hl,bc

	ld a,(_page)
	add a,h
	add a,#0x80
	ld h,a

	ld a,e
	ex de,hl

	ld hl,#_cpct_drw_cmask_mode0
	ld h,(hl)
	ld l,#85

	ld b,a
	rr b
	sla	b
	sub	a,b
    jr z,00931$

	rrc h
	rrc l
    00931$:

    ld a,(de)
    and a,l
    or a,h
	ld (de),a
	__endasm;
}

// cpct_drw_plotUnclipped_mode0 plots a point in mode 0
// clipping is NOT performed, so it is faster than cpct_drw_plotClipped_mode0
// however, attempts to plot outside of screen bounds may lead to unpredictable behaviour

void cpct_drw_plotUnclipped_mode0(signed int px,signed int py){
	px;
	py;

	__asm
	ld hl,#2
	add hl,sp
	ld de,#_cpct_drw_x1_mode0
	ldi
	ldi
	ld de,#_cpct_drw_y1_mode0
	ldi
	ldi

	ld	hl,(_cpct_drw_y1_mode0)
	add	hl, hl
	ld	bc,#_cpct_drw_yAddress
	add	hl,bc

	ld	c,(hl)
	inc	hl
	ld	b,(hl)

	ld hl,(_cpct_drw_x1_mode0)
	ld e,l
	srl h
	rr l
	add hl,bc

	ld a,(_page)
	add a,h
	add a,#0x80
	ld h,a

	ld a,e
	ex de,hl

	ld hl,#_cpct_drw_cmask_mode0
	ld h,(hl)
	ld l,#85

	ld b,a
	rr b
	sla	b
	sub	a,b
    jr z,00931$

	rrc h
	rrc l
    00931$:

    ld a,(de)
    and a,l
    or a,h
	ld (de),a
	__endasm;
}

// cpct_drw_line_mode0 draws a line in mode 0
// lines are clipped to the bounds set by cpct_drw_setClippingBox_mode0

void cpct_drw_line_mode0(signed int px1,signed int py1,signed int px2,signed int py2){
	cpct_drw_x1_mode0=px1;
	cpct_drw_y1_mode0=py1;
	cpct_drw_x2_mode0=px2;
	cpct_drw_y2_mode0=py2;

	// if the entire line is on the outside of one of the clipping box borders, go no further

	if (cpct_drw_discardLine_mode0())
		return;

	// if the line is a single pixel, plot it

	if (cpct_drw_singlePixel_mode0())
		return;

	__asm
	di

	exx
	push bc
	exx

	push ix
	push iy

	ei
	__endasm;

	// if the line is perfectly horizontal, use the fast horizontal routine

	if (cpct_drw_horizontalLine_mode0()){
		cpct_drw_restoreSysRegisters_mode0();
		return;
	}

	// if the line is perfectly vertical, use the fast vertical routine

	if (cpct_drw_verticalLine_mode0()){
		cpct_drw_restoreSysRegisters_mode0();
		return;
	}

	if (cpct_drw_x1_mode0>cpct_drw_x2_mode0){
		cpct_drw_swapX_mode0();
		cpct_drw_swapY_mode0();
	}

	cpct_drw_dx_mode0=cpct_drw_x2_mode0-cpct_drw_x1_mode0;
	cpct_drw_dy_mode0=cpct_drw_y2_mode0-cpct_drw_y1_mode0;

	// if the line is perfectly 1:1 diagonal, use the appropriate fast diagonal routine

	if (cpct_drw_diagonalLineDown_mode0()){
		cpct_drw_restoreSysRegisters_mode0();
		return;
	}

	if (cpct_drw_diagonalLineUp_mode0()){
		cpct_drw_restoreSysRegisters_mode0();
		return;
	}

	// otherwise the appropriate bresenham-based routine is used

	if (cpct_drw_dy_mode0>=0){
		if (cpct_drw_dy_mode0<=cpct_drw_dx_mode0)
			cpct_drw_gentleLineDown_mode0();
		else
			cpct_drw_steepLineDown_mode0();
	}
	else{
		if (-cpct_drw_dy_mode0<=cpct_drw_dx_mode0)
			cpct_drw_gentleLineUp_mode0();
		else
			cpct_drw_steepLineUp_mode0();
	}

	cpct_drw_restoreSysRegisters_mode0();
}

// cpct_drw_setClippingBox_mode0 MUST be run before any mode 0 line drawing routines are called
// it does not check that the specified bounds are within screen limits

// going outside of the following bounds may result in unpredictable behaviour:
// 0 <= x <= 159
// 0 <= y <= 199

void cpct_drw_setClippingBox_mode0(signed int left,signed int right,signed int top,signed int bottom){
	if (right<left){
		cpct_drw_left_mode0=right;
		cpct_drw_right_mode0=left;
	}
	else{
		cpct_drw_left_mode0=left;
		cpct_drw_right_mode0=right;
	}

	if (bottom<top){
		cpct_drw_top_mode0=bottom;
		cpct_drw_bottom_mode0=top;
	}
	else{
		cpct_drw_top_mode0=top;
		cpct_drw_bottom_mode0=bottom;
	}
}

// cpct_drw_populateLineMasks_mode0 MUST be run before any mode 0 line drawing routines are called

void cpct_drw_populateLineMasks_mode0(){
	unsigned char i;

	for (i=0;i<16;i++){
		cpct_drw_lineMask_mode0=0;

		if (i&1)
			cpct_drw_lineMask_mode0=128;

		if (i&2)
			cpct_drw_lineMask_mode0|=8;

		if (i&4)
			cpct_drw_lineMask_mode0|=32;

		if (i&8)
			cpct_drw_lineMask_mode0|=2;

		cpct_drw_lineMasks_mode0[i]=cpct_drw_lineMask_mode0;
	}
}

void cpct_drw_restoreSysRegisters_mode0()__naked{
__asm
di

pop iy
pop ix

exx
pop bc
exx

ei
ret
__endasm;
}

void cpct_drw_swapX_mode0()__naked{
__asm
ld hl,(_cpct_drw_x1_mode0)
ld de,(_cpct_drw_x2_mode0)
ld (_cpct_drw_x1_mode0),de
ld (_cpct_drw_x2_mode0),hl
ret
__endasm;
}

void cpct_drw_swapY_mode0()__naked{
__asm
ld hl,(_cpct_drw_y1_mode0)
ld de,(_cpct_drw_y2_mode0)
ld (_cpct_drw_y1_mode0),de
ld (_cpct_drw_y2_mode0),hl
ret
__endasm;
}

unsigned char cpct_drw_discardLine_mode0(){
	if ((cpct_drw_x1_mode0<cpct_drw_left_mode0)&&(cpct_drw_x2_mode0<cpct_drw_left_mode0))
		return 1;

	if ((cpct_drw_x1_mode0>cpct_drw_right_mode0)&&(cpct_drw_x2_mode0>cpct_drw_right_mode0))
		return 1;

	if ((cpct_drw_y1_mode0<cpct_drw_top_mode0)&&(cpct_drw_y2_mode0<cpct_drw_top_mode0))
		return 1;

	if ((cpct_drw_y1_mode0>cpct_drw_bottom_mode0)&&(cpct_drw_y2_mode0>cpct_drw_bottom_mode0))
		return 1;

	return 0;
}

void cpct_drw_prepareLine_mode0(){
	cpct_drw_colourMask_mode0=#85;
	cpct_drw_lineMask_mode0=cpct_drw_cmask_mode0;

	__asm
	ld	a,(_cpct_drw_y1_mode0)
	ld	c,a
	rra
	rra
	rra
	rlca
	rlca
	rlca
	and	a,#0xF8
	ld	b,a
	ld	a,c
	sub	a, b
	ld	(_cpct_drw_remainderY_mode0),a

	ld a,(_cpct_drw_x1_mode0)
	ld d,a
	rr	d
	sla	d
	sub	a, d
	ld	(_cpct_drw_remainderX_mode0),a

    or a,a
    jr z,00931$
    ld hl,#_cpct_drw_lineMask_mode0
    ld a,(_cpct_drw_colourMask_mode0)
    rrc (hl)
    rrca
    ld (_cpct_drw_colourMask_mode0),a
    00931$:

	ld hl,(_cpct_drw_x1_mode0)
	srl h
	rr l
	ex de,hl

	ld	hl,(_cpct_drw_y1_mode0)
	add	hl, hl
	ld	bc,#_cpct_drw_yAddress
	add	hl,bc

	ld	c,(hl)
	inc	hl
	ld	b,(hl)

	ex de,hl
	add hl,bc

	ld a,(_page)
	add a,h
	add a,#0x80
	ld h,a

	ld (_cpct_drw_vram_mode0),hl
	__endasm;
}

unsigned char cpct_drw_singlePixel_mode0(){
	if ((cpct_drw_x1_mode0==cpct_drw_x2_mode0)&&(cpct_drw_y1_mode0==cpct_drw_y2_mode0)){
		cpct_drw_plotUnclipped_mode0(cpct_drw_x1_mode0,cpct_drw_y1_mode0);
		return 1;
	}

	return 0;
}

void cpct_drw_horizontalLine_mode0_asm()__naked{
__asm
exx
call _cpct_drw_1_mode0
ld a,(_cpct_drw_remainderX_mode0)
ld d,a
exx

call _cpct_drw_2_mode0

00850$:

	exx
	ld a,(hl)
	and b
	or c
	ld (hl),a

	ld a,d
	dec	a
	jr	NZ,00800$

	rlc c
	rrc b
	inc hl
	ld d,a
	exx

	djnz 00850$
	jr 00802$

	00800$:
	rrc c
	rrc b
	inc d
	exx

djnz 00850$
00802$:
dec c
jr nz,00850$
ret
__endasm;
}

unsigned char cpct_drw_horizontalLine_mode0(){
	if (cpct_drw_y1_mode0==cpct_drw_y2_mode0){
		if (cpct_drw_x1_mode0>cpct_drw_x2_mode0)
			cpct_drw_swapX_mode0();

		if (cpct_drw_x1_mode0<cpct_drw_left_mode0)
			cpct_drw_x1_mode0=cpct_drw_left_mode0;

		cpct_drw_14_mode0();
		cpct_drw_prepareLine_mode0();
		cpct_drw_9_mode0();
		cpct_drw_horizontalLine_mode0_asm();
		return 1;
	}

	return 0;
}

void cpct_drw_verticalLine_mode0_asm()__naked{
__asm
di

exx
call _cpct_drw_1_mode0
ld de,#0x0800
exx

call _cpct_drw_2_mode0
ld a,(_cpct_drw_remainderY_mode0)

ld (00840$+1),sp
ld sp,#0x0000-#0x37B0

00850$:

	exx
	ex af,af
	ld a,(hl)
	and b
	or c
	ld (hl),a
	ex af,af

	cp 	a, #0x07
	jr nz,00910$

	xor a
	add hl,sp
	exx

	djnz 00850$
	jr 00912$

	00910$:
	inc a
	add hl,de
	exx

djnz 00850$
00912$:
dec c
jr nz,00850$

00840$:
ld sp,#0xFFFF
ei
ret
__endasm;
}

unsigned char cpct_drw_verticalLine_mode0(){
	if (cpct_drw_x1_mode0==cpct_drw_x2_mode0){
		if (cpct_drw_y1_mode0>cpct_drw_y2_mode0)
			cpct_drw_swapY_mode0();

		if (cpct_drw_y1_mode0<cpct_drw_top_mode0)
			cpct_drw_y1_mode0=cpct_drw_top_mode0;

		cpct_drw_22_mode0();
		cpct_drw_prepareLine_mode0();
		cpct_drw_10_mode0();
		cpct_drw_verticalLine_mode0_asm();
		return 1;
	}

	return 0;
}

void cpct_drw_diagonalLineDown_mode0_asm()__naked{
__asm
di

call _cpct_drw_3_mode0

exx
call _cpct_drw_1_mode0
ld de,#0x0800
exx

call _cpct_drw_2_mode0

ld (00840$+1),sp
ld sp,#0x0000-#0x37B0

00850$:

	exx
	ld a,(hl)
	and b
	or c
	ld (hl),a

	ex af,af
	dec a
	jr	NZ,00800$

	rlc c
	inc hl
	jr	00801$

	00800$:
	rrc c
	inc a
	inc a

	00801$:
	ex af,af
	rrc b

	.db 0xdd
	ld a,l
	sub a,#0x07
	jr nz,00910$

	.db 0xdd
	ld l,a
	add hl,sp
	exx

	djnz 00850$
	jr 00912$

	00910$:
	.db 0xdd
	inc l
	add hl,de
	exx

djnz 00850$
00912$:
dec c
jr nz,00850$

00840$:
ld sp,#0xFFFF
ei
ret
__endasm;
}

unsigned char cpct_drw_diagonalLineDown_mode0(){
	signed int clip;

	if (cpct_drw_dx_mode0==cpct_drw_dy_mode0){
		if (cpct_drw_x1_mode0<cpct_drw_left_mode0){
			clip=cpct_drw_left_mode0-cpct_drw_x1_mode0;
			cpct_drw_x1_mode0+=clip;
			cpct_drw_y1_mode0+=clip;

			if (cpct_drw_15_mode0())
				return 1;

			if (cpct_drw_18_mode0())
				return 1;
		}

		if (cpct_drw_y1_mode0<cpct_drw_top_mode0){
			clip=cpct_drw_top_mode0-cpct_drw_y1_mode0;
			cpct_drw_x1_mode0+=clip;
			cpct_drw_y1_mode0+=clip;

			if (cpct_drw_15_mode0())
				return 1;

			if (cpct_drw_18_mode0())
				return 1;
		}

		if (cpct_drw_x2_mode0>cpct_drw_right_mode0){
			clip=cpct_drw_x2_mode0-cpct_drw_right_mode0;
			cpct_drw_x2_mode0-=clip;
			cpct_drw_y2_mode0-=clip;

			if (cpct_drw_16_mode0())
				return 1;

			if (cpct_drw_19_mode0())
				return 1;
		}

		if (cpct_drw_y2_mode0>cpct_drw_bottom_mode0){
			clip=cpct_drw_y2_mode0-cpct_drw_bottom_mode0;
			cpct_drw_x2_mode0-=clip;
			cpct_drw_y2_mode0-=clip;

			if (cpct_drw_16_mode0())
				return 1;

			if (cpct_drw_19_mode0())
				return 1;
		}

		cpct_drw_prepareLine_mode0();
		cpct_drw_9_mode0();
		cpct_drw_diagonalLineDown_mode0_asm();
		return 1;
	}

	return 0;
}

void cpct_drw_diagonalLineUp_mode0_asm()__naked{
__asm
di

call _cpct_drw_3_mode0

exx
call _cpct_drw_1_mode0
ld de,#0x0000-#0x0800
exx

call _cpct_drw_2_mode0

ld (00840$+1),sp
ld sp,#0x37B0

00850$:

	exx
	ld a,(hl)
	and b
	or c
	ld (hl),a

	ex af,af
	dec a
	jr	NZ,00800$

	rlc c
	inc hl
	jr	00801$

	00800$:
	rrc c
	inc a
	inc a

	00801$:
	ex af,af
	rrc b

	.db 0xdd
	ld a,l
	or	a, a
	jr	NZ,00920$

	.db 0xdd
	ld l,#0x07
	add hl,sp
	exx

	djnz 00850$
	jr 00922$

	00920$:
	.db 0xdd
	dec l
	add hl,de
	exx

djnz 00850$
00922$:
dec c
jr nz,00850$

00840$:
ld sp,#0xFFFF
ei
ret
__endasm;
}

unsigned char cpct_drw_diagonalLineUp_mode0(){
	signed int clip;

	if (cpct_drw_dx_mode0==-cpct_drw_dy_mode0){
		if (cpct_drw_x1_mode0<cpct_drw_left_mode0){
			clip=cpct_drw_left_mode0-cpct_drw_x1_mode0;
			cpct_drw_x1_mode0+=clip;
			cpct_drw_y1_mode0-=clip;

			if (cpct_drw_15_mode0())
				return 1;

			if (cpct_drw_17_mode0())
				return 1;
		}

		if (cpct_drw_y1_mode0>cpct_drw_bottom_mode0){
			clip=cpct_drw_y1_mode0-cpct_drw_bottom_mode0;
			cpct_drw_x1_mode0+=clip;
			cpct_drw_y1_mode0-=clip;

			if (cpct_drw_15_mode0())
				return 1;

			if (cpct_drw_17_mode0())
				return 1;
		}

		if (cpct_drw_x2_mode0>cpct_drw_right_mode0){
			clip=cpct_drw_x2_mode0-cpct_drw_right_mode0;
			cpct_drw_x2_mode0-=clip;
			cpct_drw_y2_mode0+=clip;

			if (cpct_drw_16_mode0())
				return 1;

			if (cpct_drw_20_mode0())
				return 1;
		}

		if (cpct_drw_y2_mode0<cpct_drw_top_mode0){
			clip=cpct_drw_top_mode0-cpct_drw_y2_mode0;
			cpct_drw_x2_mode0-=clip;
			cpct_drw_y2_mode0+=clip;

			if (cpct_drw_16_mode0())
				return 1;

			if (cpct_drw_20_mode0())
				return 1;
		}

		cpct_drw_prepareLine_mode0();
		cpct_drw_9_mode0();
		cpct_drw_diagonalLineUp_mode0_asm();
		return 1;
	}

	return 0;
}

void cpct_drw_gentleLineDown_mode0(){
	__asm
	ld hl,(_cpct_drw_bottom_mode0)
	ld a,l
	ld (00890$+1),a
	ld a,h
	ld (00891$+1),a
	__endasm;

	cpct_drw_21_mode0();

	if (cpct_drw_x1_mode0<cpct_drw_left_mode0){
		cpct_drw_11_mode0();

		__asm
		di

		ld iy,(_cpct_drw_y1_mode0)
		call _cpct_drw_2_mode0
		call _cpct_drw_5_mode0

		ld hl,(_cpct_drw_dy_mode0)
		add hl,de
		ld (00841$+1),sp
		ld sp,hl

		ld hl,(_cpct_drw_yn_mode0)

		00860$:

			add hl,sp
			jr nc,00813$

			inc iy
			djnz 00860$
			jr 00833$

			00813$:
			sbc hl,de

		djnz 00860$
		00833$:
		dec  c
		jr nz,00860$

		ld (_cpct_drw_y1_mode0),iy
		ld (_cpct_drw_yn_mode0),hl

		ld hl,(_cpct_drw_left_mode0)
		ld (_cpct_drw_x1_mode0),hl

		00841$:
		ld sp,#0xFFFF
		ei
		__endasm;

		if (cpct_drw_15_mode0())
			return;

		if (cpct_drw_18_mode0())
			return;
	}

	if (cpct_drw_y1_mode0<cpct_drw_top_mode0){
		cpct_drw_12_mode0();

		__asm
		di

		ld ix,(_cpct_drw_x1_mode0)
		ld bc,(_cpct_drw_len_mode0)
		call _cpct_drw_5_mode0

		ld hl,(_cpct_drw_dy_mode0)
		add hl,de
		ld (00842$+1),sp
		ld sp,hl

		ld hl,(_cpct_drw_yn_mode0)

		00861$:

			add hl,sp
			jr nc,00812$

			dec bc
			inc ix

			ld a,c
			or a
			jr nz,00861$
			jr 00832$

			00812$:
			sbc hl,de

		inc ix

		ld a,c
		or a
		jr nz,00861$

		00832$:
		ld a,b
		or a
		jr nz,00861$

		ld (_cpct_drw_x1_mode0),ix
		ld (_cpct_drw_yn_mode0),hl

		ld hl,(_cpct_drw_top_mode0)
		ld (_cpct_drw_y1_mode0),hl

		00842$:
		ld sp,#0xFFFF
		ei
		__endasm;

		if (cpct_drw_15_mode0())
			return;

		if (cpct_drw_18_mode0())
			return;
	}

	cpct_drw_14_mode0();
	cpct_drw_prepareLine_mode0();
	cpct_drw_9_mode0();

	if (cpct_drw_y2_mode0>cpct_drw_bottom_mode0){
		__asm
		di

		call _cpct_drw_3_mode0
		ld iy,(_cpct_drw_y1_mode0)

		exx
		call _cpct_drw_1_mode0
		ld de,#0x0800
		exx

		call _cpct_drw_2_mode0
		call _cpct_drw_6_mode0

		ld (00840$+1),sp
		ld sp,hl

		ld de,(_cpct_drw_dy_mode0)
		add hl,de
		ex de,hl

		ld hl,(_cpct_drw_yn_mode0)

		00850$:

			exx
			ld a,(hl)
			and b
			or c
			ld (hl),a

			ex af,af
			dec a
			jr	NZ,00800$

			rlc c
			inc hl
			jr	00801$

			00800$:
			rrc c
			inc a
			inc a

			00801$:
			ex af,af
			rrc b
			exx

			add hl,de
			jr nc,00811$

			.db 0xfd
			ld a,l
			00890$:
			cp #0xFF
			jr nz,00892$

			.db 0xfd
			ld a,h
			00891$:
			cp #0xFF
			jr z,00840$

			00892$:
			inc iy

			exx
			.db 0xdd
			ld a,l
			sub a, #0x07
			jr nz,00910$

			ld de,#0x0000-#0x37B0
			add hl,de
			ld de,#0x0800
			.db 0xdd
			ld l,a
			exx

			djnz 00850$
			jr 00831$

			00910$:
			add hl,de
			.db 0xdd
			inc l
			exx

			djnz 00850$
			jr 00831$

			00811$:
			sbc hl,sp

		djnz 00850$
		00831$:
		dec c
		jr nz,00850$

		00840$:
		ld sp,#0xFFFF
		ei
		ret
		__endasm;
	}

	__asm
	di

	call _cpct_drw_3_mode0

	exx
	call _cpct_drw_1_mode0
	ld de,#0x0800
	exx

	call _cpct_drw_2_mode0
	call _cpct_drw_6_mode0

	ld (00845$+1),sp
	ld sp,hl

	ld de,(_cpct_drw_dy_mode0)
	add hl,de
	ex de,hl

	ld hl,(_cpct_drw_yn_mode0)

	00855$:

		exx
		ld a,(hl)
		and b
		or c
		ld (hl),a

		ex af,af
		dec a
		jr	NZ,00805$

		rlc c
		inc hl
		jr	00806$

		00805$:
		rrc c
		inc a
		inc a

		00806$:
		ex af,af
		rrc b
		exx

		add hl,de
		jr nc,00816$

		exx
		.db 0xdd
		ld a,l
		sub a, #0x07
		jr nz,00915$

		ld de,#0x0000-#0x37B0
		add hl,de
		ld de,#0x0800
		.db 0xdd
		ld l,a
		exx

		djnz 00855$
		jr 00836$

		00915$:
		add hl,de
		.db 0xdd
		inc l
		exx

		djnz 00855$
		jr 00836$

		00816$:
		sbc hl,sp

	djnz 00855$
	00836$:
	dec c
	jr nz,00855$

	00845$:
	ld sp,#0xFFFF
	ei
	ret
	__endasm;
}

void cpct_drw_steepLineDown_mode0(){
	__asm
	ld hl,(_cpct_drw_right_mode0)
	ld a,l
	ld (00890$+1),a
	ld a,h
	ld (00891$+1),a
	__endasm;

	cpct_drw_xn_mode0=cpct_drw_dy_mode0>>1;

	if (cpct_drw_y1_mode0<cpct_drw_top_mode0){
		cpct_drw_12_mode0();

		__asm
		di

		ld ix,(_cpct_drw_x1_mode0)
		call _cpct_drw_2_mode0
		call _cpct_drw_7_mode0

		ld hl,(_cpct_drw_dx_mode0)
		add hl,de
		ld (00841$+1),sp
		ld sp,hl

		ld hl,(_cpct_drw_xn_mode0)

		00860$:

			add hl,sp
			jr nc,00813$

			inc ix
			djnz 00860$
			jr 00833$

			00813$:
			sbc hl,de

		djnz 00860$
		00833$:
		dec  c
		jr nz,00860$

		ld (_cpct_drw_x1_mode0),ix
		ld (_cpct_drw_xn_mode0),hl

		ld hl,(_cpct_drw_top_mode0)
		ld (_cpct_drw_y1_mode0),hl

		00841$:
		ld sp,#0xFFFF
		ei
		__endasm;

		if (cpct_drw_15_mode0())
			return;

		if (cpct_drw_18_mode0())
			return;
	}

	if (cpct_drw_x1_mode0<cpct_drw_left_mode0){
		cpct_drw_11_mode0();

		__asm
		di

		ld iy,(_cpct_drw_y1_mode0)
		ld bc,(_cpct_drw_len_mode0)
		call _cpct_drw_7_mode0

		ld hl,(_cpct_drw_dx_mode0)
		add hl,de
		ld (00842$+1),sp
		ld sp,hl

		ld hl,(_cpct_drw_xn_mode0)

		00861$:

			add hl,sp
			jr nc,00812$

			dec bc
			inc iy

			ld a,c
			or a
			jr nz,00861$
			jr 00832$

			00812$:
			sbc hl,de

		inc iy

		ld a,c
		or a
		jr nz,00861$

		00832$:
		ld a,b
		or a
		jr nz,00861$

		ld (_cpct_drw_y1_mode0),iy
		ld (_cpct_drw_xn_mode0),hl

		ld hl,(_cpct_drw_left_mode0)
		ld (_cpct_drw_x1_mode0),hl

		00842$:
		ld sp,#0xFFFF
		ei
		__endasm;

		if (cpct_drw_15_mode0())
			return;

		if (cpct_drw_18_mode0())
			return;
	}

	cpct_drw_22_mode0();
	cpct_drw_prepareLine_mode0();
	cpct_drw_10_mode0();

	if (cpct_drw_x2_mode0>cpct_drw_right_mode0){
		__asm
		di

		call _cpct_drw_4_mode0
		ld iy,(_cpct_drw_x1_mode0)

		exx
		call _cpct_drw_1_mode0
		ld de,#0x0800
		exx

		call _cpct_drw_2_mode0
		call _cpct_drw_7_mode0

		ld hl,(_cpct_drw_dx_mode0)
		add hl,de
		ld (00840$+1),sp
		ld sp,hl

		ld hl,(_cpct_drw_xn_mode0)

		00850$:

			exx
			ld a,(hl)
			and b
			or c
			ld (hl),a

			ex af,af
			cp a, #0x07
			jr nz,00910$

			ld de,#0x0000-#0x37B0
			add hl,de
			ld de,#0x0800
			xor a
			jr	00911$

			00910$:
			add hl,de
			inc a

			00911$:
			ex af,af
			exx

			add hl,sp
			jr nc,00811$

			.db 0xfd
			ld a,l
			00890$:
			cp #0xFF
			jr nz,00892$

			.db 0xfd
			ld a,h
			00891$:
			cp #0xFF
			jr z,00840$

			00892$:
			inc iy

			exx
			rrc b
			.db 0xdd
			ld a,h
			dec	a
			jr	NZ,00800$

			rlc c
			inc hl
			.db 0xdd
			ld h,a
			exx

			djnz 00850$
			jr 00831$

			00800$:
			rrc c
			.db 0xdd
			inc h
			exx

			djnz 00850$
			jr 00831$

			00811$:
			sbc hl,de

		djnz 00850$
		00831$:
		dec c
		jr nz,00850$

		00840$:
		ld sp,#0xFFFF
		ei
		ret
		__endasm;
	}

	__asm
	di

	call _cpct_drw_4_mode0

	exx
	call _cpct_drw_1_mode0
	ld de,#0x0800
	exx

	call _cpct_drw_2_mode0
	call _cpct_drw_7_mode0

	ld hl,(_cpct_drw_dx_mode0)
	add hl,de
	ld (00845$+1),sp
	ld sp,hl

	ld hl,(_cpct_drw_xn_mode0)

	00855$:

		exx
		ld a,(hl)
		and b
		or c
		ld (hl),a

		ex af,af
		cp a, #0x07
		jr nz,00915$

		ld de,#0x0000-#0x37B0
		add hl,de
		ld de,#0x0800
		xor a
		jr	00916$

		00915$:
		add hl,de
		inc a

		00916$:
		ex af,af
		exx

		add hl,sp
		jr nc,00816$

		exx
		rrc b
		.db 0xdd
		ld a,h
		dec	a
		jr	NZ,00805$

		rlc c
		inc hl
		.db 0xdd
		ld h,a
		exx

		djnz 00855$
		jr 00836$

		00805$:
		rrc c
		.db 0xdd
		inc h
		exx

		djnz 00855$
		jr 00836$

		00816$:
		sbc hl,de

	djnz 00855$
	00836$:
	dec c
	jr nz,00855$

	00845$:
	ld sp,#0xFFFF
	ei
	ret
	__endasm;
}

void cpct_drw_gentleLineUp_mode0(){
	__asm
	ld hl,(_cpct_drw_top_mode0)
	ld a,l
	ld (00890$+1),a
	ld a,h
	ld (00891$+1),a
	__endasm;

	cpct_drw_21_mode0();

	if (cpct_drw_x1_mode0<cpct_drw_left_mode0){
		cpct_drw_11_mode0();

		__asm
		di

		ld iy,(_cpct_drw_y1_mode0)
		call _cpct_drw_5_mode0
		call _cpct_drw_2_mode0
		call _cpct_drw_8_mode0
		add hl,de

		ld (00841$+1),sp
		ld sp,hl

		ld hl,(_cpct_drw_yn_mode0)

		00860$:

			add hl,sp
			jr nc,00813$

			dec iy
			djnz 00860$
			jr 00833$

			00813$:
			sbc hl,de

		djnz 00860$
		00833$:
		dec  c
		jr nz,00860$

		ld (_cpct_drw_y1_mode0),iy
		ld (_cpct_drw_yn_mode0),hl

		ld hl,(_cpct_drw_left_mode0)
		ld (_cpct_drw_x1_mode0),hl

		00841$:
		ld sp,#0xFFFF
		ei
		__endasm;

		if (cpct_drw_15_mode0())
			return;

		if (cpct_drw_17_mode0())
			return;
	}

	if (cpct_drw_y1_mode0>cpct_drw_bottom_mode0){
		cpct_drw_13_mode0();

		__asm
		di

		ld ix,(_cpct_drw_x1_mode0)
		call _cpct_drw_5_mode0
		call _cpct_drw_8_mode0
		add hl,de

		ld (00842$+1),sp
		ld sp,hl

		ld bc,(_cpct_drw_len_mode0)
		ld hl,(_cpct_drw_yn_mode0)

		00861$:

			add hl,sp
			jr nc,00812$

			dec bc
			inc ix
			ld a,c
			or a
			jr nz,00861$
			jr 00832$

			00812$:
			sbc hl,de

		inc ix

		ld a,c
		or a
		jr nz,00861$

		00832$:
		ld a,b
		or a
		jr nz,00861$

		ld (_cpct_drw_x1_mode0),ix
		ld (_cpct_drw_yn_mode0),hl

		ld hl,(_cpct_drw_bottom_mode0)
		ld (_cpct_drw_y1_mode0),hl

		00842$:
		ld sp,#0xFFFF
		ei
		__endasm;

		if (cpct_drw_15_mode0())
			return;

		if (cpct_drw_17_mode0())
			return;
	}

	cpct_drw_14_mode0();
	cpct_drw_prepareLine_mode0();
	cpct_drw_9_mode0();

	if (cpct_drw_y2_mode0<cpct_drw_top_mode0){
		__asm
		di

		call _cpct_drw_3_mode0
		ld iy,(_cpct_drw_y1_mode0)

		exx
		call _cpct_drw_1_mode0
		ld de,#0x0000-#0x0800
		exx

		call _cpct_drw_2_mode0
		call _cpct_drw_7_mode0
		call _cpct_drw_6_mode0

		ld (00840$+1),sp
		ld sp,hl

		add hl,de
		ex de,hl

		ld hl,(_cpct_drw_yn_mode0)

		00850$:

			exx
			ld a,(hl)
			and b
			or c
			ld (hl),a

			ex af,af
			dec a
			jr	NZ,00800$

			rlc c
			inc hl
			jr	00801$

			00800$:
			rrc c
			inc a
			inc a

			00801$:
			ex af,af
			rrc b
			exx

			add hl,de
			jr nc,00811$

			.db 0xfd
			ld a,l
			00890$:
			cp #0xFF
			jr nz,00892$

			.db 0xfd
			ld a,h
			00891$:
			cp #0xFF
			jr z,00840$

			00892$:
			dec iy

			exx
			.db 0xdd
			ld a,l
			or	a, a
			jr	NZ,00920$

			ld de,#0x37B0
			add hl,de
			ld de,#0x0000-#0x0800
			.db 0xdd
			ld l,#0x07
			exx

			djnz 00850$
			jr	00831$

			00920$:
			add hl,de
			.db 0xdd
			dec l
			exx

			djnz 00850$
			jr 00831$

			00811$:
			sbc hl,sp

		djnz 00850$
		00831$:
		dec c
		jr nz,00850$

		00840$:
		ld sp,#0xFFFF
		ei
		ret
		__endasm;
	}

	__asm
	di

	call _cpct_drw_3_mode0

	exx
	call _cpct_drw_1_mode0
	ld de,#0x0000-#0x0800
	exx

	call _cpct_drw_2_mode0
	call _cpct_drw_7_mode0
	call _cpct_drw_6_mode0

	ld (00845$+1),sp
	ld sp,hl

	add hl,de
	ex de,hl

	ld hl,(_cpct_drw_yn_mode0)

	00855$:

		exx
		ld a,(hl)
		and b
		or c
		ld (hl),a

		ex af,af
		dec a
		jr	NZ,00805$

		rlc c
		inc hl
		jr	00806$

		00805$:
		rrc c
		inc a
		inc a

		00806$:
		ex af,af
		rrc b
		exx

		add hl,de
		jr nc,00816$

		exx
		.db 0xdd
		dec l
		jp p,00925$

		ld de,#0x37B0
		add hl,de
		ld de,#0x0000-#0x0800
		.db 0xdd
		ld l,#0x07
		exx

		djnz 00855$
		jr	00836$

		00925$:
		add hl,de
		exx

		djnz 00855$
		jr 00836$

		00816$:
		sbc hl,sp

	djnz 00855$
	00836$:
	dec c
	jr nz,00855$

	00845$:
	ld sp,#0xFFFF
	ei
	ret
	__endasm;
}

void cpct_drw_steepLineUp_mode0(){
	__asm
	ld hl,(_cpct_drw_right_mode0)
	ld a,l
	ld (00890$+1),a
	ld a,h
	ld (00891$+1),a
	__endasm;

	cpct_drw_xn_mode0=-cpct_drw_dy_mode0>>1;

	if (cpct_drw_y1_mode0>cpct_drw_bottom_mode0){
		cpct_drw_13_mode0();

		__asm
		di

		ld ix,(_cpct_drw_x1_mode0)
		ld de,(_cpct_drw_dy_mode0)
		call _cpct_drw_2_mode0

		ld hl,(_cpct_drw_dx_mode0)
		add hl,de
		ld (00841$+1),sp
		ld sp,hl

		ld hl,(_cpct_drw_xn_mode0)

		00860$:

			add hl,sp
			jr nc,00812$

			inc ix
			djnz 00860$
			jr 00832$

			00812$:
			sbc hl,de

		djnz 00860$
		00832$:
		dec  c
		jr nz,00860$

		ld (_cpct_drw_x1_mode0),ix
		ld (_cpct_drw_xn_mode0),hl

		ld hl,(_cpct_drw_bottom_mode0)
		ld (_cpct_drw_y1_mode0),hl

		00841$:
		ld sp,#0xFFFF
		ei
		__endasm;

		if (cpct_drw_15_mode0())
			return;

		if (cpct_drw_17_mode0())
			return;
	}

	if (cpct_drw_x1_mode0<cpct_drw_left_mode0){
		cpct_drw_11_mode0();

		__asm
		di

		ld iy,(_cpct_drw_y1_mode0)
		ld de,(_cpct_drw_dy_mode0)
		ld bc,(_cpct_drw_len_mode0)

		ld hl,(_cpct_drw_dx_mode0)
		add hl,de
		ld (00842$+1),sp
		ld sp,hl

		ld hl,(_cpct_drw_xn_mode0)

		00861$:

			add hl,sp
			jr nc,00813$

			dec bc
			dec iy
			ld a,c
			or a
			jr nz,00861$
			jr 00833$

			00813$:
			sbc hl,de

		dec iy

		ld a,c
		or a
		jr nz,00861$

		00833$:
		ld a,b
		or a
		jr nz,00861$

		ld (_cpct_drw_y1_mode0),iy
		ld (_cpct_drw_xn_mode0),hl

		ld hl,(_cpct_drw_left_mode0)
		ld (_cpct_drw_x1_mode0),hl

		00842$:
		ld sp,#0xFFFF
		ei
		__endasm;

		if (cpct_drw_15_mode0())
			return;

		if (cpct_drw_17_mode0())
			return;
	}

	if (cpct_drw_y2_mode0<cpct_drw_top_mode0)
		cpct_drw_y2_mode0=cpct_drw_top_mode0;

	cpct_drw_prepareLine_mode0();
	cpct_drw_len_mode0=cpct_drw_y1_mode0-cpct_drw_y2_mode0+1;

	if (cpct_drw_x2_mode0>cpct_drw_right_mode0){
		__asm
		di

		call _cpct_drw_4_mode0
		ld iy,(_cpct_drw_x1_mode0)

		exx
		call _cpct_drw_1_mode0
		ld de,#0x0000-#0x0800
		exx

		call _cpct_drw_2_mode0
		ld de,(_cpct_drw_dy_mode0)

		ld hl,(_cpct_drw_dx_mode0)
		add hl,de
		ld (00840$+1),sp
		ld sp,hl

		ld hl,(_cpct_drw_xn_mode0)

		00850$:

			exx
			ld a,(hl)
			and b
			or c
			ld (hl),a

			ex af,af
			dec a
			jp p,00920$

			ld de,#0x3FB0
			add hl,de
			ld de,#0x0000-#0x0800
			ld a,#0x07

			00920$:
			add hl,de

			ex af,af
			exx

			add hl,sp
			jr nc,00811$

			.db 0xfd
			ld a,l
			00890$:
			cp #0xFF
			jr nz,00892$

			.db 0xfd
			ld a,h
			00891$:
			cp #0xFF
			jr z,00840$

			00892$:
			inc iy

			exx
			rrc b
			.db 0xdd
			ld a,h
			dec	a
			jr	NZ,00800$

			rlc c
			inc hl
			.db 0xdd
			ld h,a
			exx

			djnz 00850$
			jr 00831$

			00800$:
			rrc c
			.db 0xdd
			inc h
			exx

			djnz 00850$
			jr 00831$

			00811$:
			sbc hl,de

		djnz 00850$
		00831$:
		dec c
		jr nz,00850$

		00840$:
		ld sp,#0xFFFF
		ei
		ret
		__endasm;
	}

	__asm
	di

	call _cpct_drw_4_mode0

	exx
	call _cpct_drw_1_mode0
	ld de,#0x0000-#0x0800
	exx

	call _cpct_drw_2_mode0
	ld de,(_cpct_drw_dy_mode0)

	ld hl,(_cpct_drw_dx_mode0)
	add hl,de
	ld (00845$+1),sp
	ld sp,hl

	ld hl,(_cpct_drw_xn_mode0)

	00855$:

		exx
		ld a,(hl)
		and b
		or c
		ld (hl),a

		ex af,af
		dec a
		jp p,00925$

		ld de,#0x3FB0
		add hl,de
		ld de,#0x0000-#0x0800
		ld a,#0x07

		00925$:
		add hl,de

		ex af,af
		exx

		add hl,sp
		jr nc,00816$

		exx
		rrc b
		.db 0xdd
		ld a,h
		dec	a
		jr	NZ,00805$

		rlc c
		inc hl
		.db 0xdd
		ld h,a
		exx

		djnz 00855$
		jr 00836$

		00805$:
		rrc c
		.db 0xdd
		inc h
		exx

		djnz 00855$
		jr 00836$

		00816$:
		sbc hl,de

	djnz 00855$
	00836$:
	dec c
	jr nz,00855$

	00845$:
	ld sp,#0xFFFF
	ei
	ret
	__endasm;
}