void cpct_drw_1_mode1();
void cpct_drw_2_mode1();
void cpct_drw_3_mode1();
void cpct_drw_4_mode1();
void cpct_drw_5_mode1();
void cpct_drw_6_mode1();
void cpct_drw_7_mode1();
void cpct_drw_8_mode1();
void cpct_drw_9_mode1();
void cpct_drw_10_mode1();
void cpct_drw_11_mode1();
void cpct_drw_12_mode1();
void cpct_drw_13_mode1();
void cpct_drw_14_mode1();
unsigned char cpct_drw_15_mode1();
unsigned char cpct_drw_16_mode1();
unsigned char cpct_drw_17_mode1();
unsigned char cpct_drw_18_mode1();
unsigned char cpct_drw_19_mode1();
unsigned char cpct_drw_20_mode1();
void cpct_drw_21_mode1();
void cpct_drw_22_mode1();

void cpct_drw_setLineColour_mode1(unsigned char);
void cpct_drw_plotClipped_mode1(signed int,signed int);
void cpct_drw_plotUnclipped_mode1(signed int,signed int);
void cpct_drw_line_mode1(signed int,signed int,signed int,signed int);
void cpct_drw_setClippingBox_mode1(signed int,signed int,signed int,signed int);

void cpct_drw_populateLineMasks_mode1();
void cpct_drw_restoreSysRegisters_mode1();
void cpct_drw_swapX_mode1();
void cpct_drw_swapY_mode1();

unsigned char cpct_drw_discardLine_mode1();
void cpct_drw_prepareLine_mode1();
unsigned char cpct_drw_singlePixel_mode1();
void cpct_drw_horizontalLine_mode1_asm();
unsigned char cpct_drw_horizontalLine_mode1();
void cpct_drw_verticalLine_mode1_asm();
unsigned char cpct_drw_verticalLine_mode1();
void cpct_drw_diagonalLineDown_mode1_asm();
unsigned char cpct_drw_diagonalLineDown_mode1();
void cpct_drw_diagonalLineUp_mode1_asm();
unsigned char cpct_drw_diagonalLineUp_mode1();
void cpct_drw_gentleLineDown_mode1();
void cpct_drw_steepLineDown_mode1();
void cpct_drw_gentleLineUp_mode1();
void cpct_drw_steepLineUp_mode1();
