/*
 *	Rewrite of DSA1 v3.02_de functions of seg010 (EMS)
 *	Functions rewritten: 8/8 (complete)
 *	Borlandified and identical
 *
 *	Remark: In the original game the EMS functions were written
 *		with Turbo assembler, so look in seg010.asm
 */
#include <string.h>

#include "v302de.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

static unsigned short EMS_installed(void)
{
#if defined(__BORLANDC__)
	unsigned char *IRQ_67;
	char *sig;

	/* get interrupt vector of interrupt 0x67 */
	IRQ_67 = host_readd(0x67 * 4);

	/* Make a pointer to where "EMMXXXX0" stands if EMS is enabled */
	sig = (char*)MK_FP(FP_SEG(IRQ_67), 0x0a);

	/* return 1 if signature is found */
	if (!strncmp(sig, "EMMXXXX0", 8))
		return 1;
#endif

	return 0;
}

static RealPt EMS_get_frame_ptr()
{
#if defined(__BORLANDC__)
	reg_ax = DNG09_SECRETDOOR1;
	CALLBACK_RunRealInt(0x67);

	if (reg_ax == 0)
		return RealMake(reg_bx, 0);

#endif
	return NULL;
}

signed short EMS_get_num_pages_unalloced(void)
{
#if defined(__BORLANDC__)
	reg_ax = 0x04200;
	CALLBACK_RunRealInt(0x67);

	if (reg_ax == 0)
		return reg_bx;

#endif
	return 0;
}

unsigned short EMS_alloc_pages(unsigned short pages)
{
#if defined(__BORLANDC__)
	reg_ax = 0x4300;
	reg_bx = pages;
	CALLBACK_RunRealInt(0x67);

	if (reg_ax == 0)
		return reg_dx;
#endif

	return 0;
}

unsigned short EMS_free_pages(unsigned short handle)
{
#if defined(__BORLANDC__)
	reg_ax = 0x4500;
	reg_dx = handle;
	CALLBACK_RunRealInt(0x67);

	reg_ah = reg_al;
	return reg_ax;
#else
	return 0;
#endif
}

unsigned short EMS_map_memory(unsigned short handle, unsigned short lpage, unsigned char ppage)
{
#if defined(__BORLANDC__)
	reg_ax = 0x4400;
	reg_al = ppage;
	reg_bx = lpage;
	reg_dx = handle;
	CALLBACK_RunRealInt(0x67);

	reg_ah = reg_al;
	return reg_ax;
#else
	return 0;
#endif
}

RealPt EMS_norm_ptr(RealPt p)
{
#if defined(__BORLANDC__)
	RealPt retval;

	retval = RealSeg(p) << 4;
	retval += RealOff(p);
	retval = retval << 12;
	retval |= (RealOff(p) & 0x0f);

	return retval;
#else
	return NULL;
#endif
}

unsigned short EMS_init() {

	if (EMS_installed()) {
		ds_writed(EMS_FRAME_PTR, EMS_get_frame_ptr());
		return 1;
	}

	return 0;
}

#if !defined(__BORLANDC__)
}
#endif
