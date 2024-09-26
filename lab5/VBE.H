#ifndef _VBE_H_
#define _VBE_H_

#include <lcom/lcf.h>
#include <machine/int86.h>

#define VBE_1024x768_IC 0x105 //8 bits per pixel
#define VBE_640x480_DC 0x110 //15((1:)5:5:5)
#define VBE_800x600_DC 0x115 //24 (8:8:8)
#define VBE_1280x1024_DC 0x11A //16 (5:6:5)
#define VBE_1152x864_DC 0x14C //32 ((8:)8:8:8)
 
#define VBE_MODE_SET 0x4F02
#define VBE_LINEAR_FB BIT(14)

#define DIRECT_COLOR 0x06 //DC
#define INDEXED_COLOR 0x105 //IC

#define BIOS_SET_VIDEOMODE 0x00
#define BIOS_VIDEOCARD_SERV 0x10

#endif 

/*

Function |	AH |	AL |	Ref in VBE standard |	Notes
Set VBE mode |	0x4F |	0x02 |	page 25 |	mode is passed in register BX, which should have bit 14set;
Return VBE Mode Info |	0x4F | 0x01 |	page 16 |	mode is passed on CX must provide address for returned info in registers ES:DI;
Return VBE Controller Information |	0x4F |	0x00 |	page 12 |	must provide address for returned info in registers ES:DI;
BIOS Set Video Mode |	0x00 |	< video mode > |		not a VBE function, but a BIOS function
set AL to 0x03 to return to Minix's default text mode ;

*/
