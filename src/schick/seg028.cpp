/*
 *	Rewrite of DSA1 v3.02_de functions of seg028 (file loader: maps, npcs, special textures, dialogs, fight backgrounds)
 *	Functions rewritten: 19/19 (complete)
 *
 *	Functions called rewritten: 16/16
 *	Functions uncalled rewritten: 3/3
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg028.cpp
 */
#include <string.h>

#if defined(__BORLANDC__)
#include <DOS.H>
#include <IO.H>
#else
#include <unistd.h>
#endif

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg004.h"
#include "seg008.h"
#include "seg009.h"
#include "seg010.h"
#include "seg026.h"
#include "seg028.h"
#include "seg066.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

void prepare_dungeon_area(void)
{
	signed short index;
	Bit32u v1;
	Bit32u v2;
	HugePt buf;

	signed short l_si;
	signed short handle;

	index = ds_readbs(DUNGEON_INDEX) + ARCHIVE_FILE_DNGS_DTX;

	if (ds_readbs(DNG_AREA_LOADED) != ds_readbs(DUNGEON_INDEX)) {

		load_area_description(1);
		ds_writeb(CITY_AREA_LOADED, -1);
		load_dungeon_ddt();
	}

	load_tx(index);

	if ((ds_readws(AREA_PREPARED) == -1) || (ds_readws(AREA_PREPARED) == 1)) {

		set_var_to_zero();
		ds_writew(CURRENT_ANI, -1);

		l_si = (ds_readbs(DUNGEON_INDEX) == DUNGEONS_TOTENSCHIFF) ? ARCHIVE_FILE_SHIPSL_NVF :
			(((ds_readbs(DUNGEON_INDEX) == DUNGEONS_VERFALLENE_HERBERGE) ||
				(ds_readbs(DUNGEON_INDEX) == DUNGEONS_RUINE_DES_SCHWARZMAGIERS) ||
				(ds_readbs(DUNGEON_INDEX) == DUNGEONS_KULTSTAETTE_DES_NAMENLOSEN) ||
				(ds_readbs(DUNGEON_INDEX) == DUNGEONS_PIRATENHOEHLE) ||
				(ds_readbs(DUNGEON_INDEX) == DUNGEONS_ZWERGENFESTE) ||
				(ds_readbs(DUNGEON_INDEX) == DUNGEONS_VERLASSENE_MINE) ||
				(ds_readbs(DUNGEON_INDEX) == DUNGEONS_ZWINGFESTE) ||
				(ds_readbs(DUNGEON_INDEX) == DUNGEONS_HYGGELIKS_RUINE)) ? ARCHIVE_FILE_MARBLESL_NVF : ARCHIVE_FILE_STONESL_NVF);

		ds_writeb(DUNGEON_GFX_STYLE, (l_si == ARCHIVE_FILE_SHIPSL_NVF) ? 0 : ((l_si == ARCHIVE_FILE_MARBLESL_NVF) ? 1 : 2));

		handle = load_archive_file(l_si);
		v1 = v2 = 0;

		/* clear palette */
		buf = (HugePt)(ds_readd(BUFFER9_PTR3));
		memset((Bit8u*)(buf), 0, 0xc0);
		wait_for_vsync();
		set_palette((Bit8u*)(buf), 0x80, 0x40);

		do {
			v1 = read_archive_file(handle, (Bit8u*)(buf), 65000);
#if !defined(__BORLANDC__)
			buf = F_PADD(buf, v1);
#else
			buf += v1;
#endif
			v2 += v1;
		} while (v1);

		close(handle);

		ds_writed(BUFFER11_PTR, (Bit32u)F_PADD(F_PADD((HugePt)ds_readd(BUFFER9_PTR3), v2), -0xc0));

		ds_writew(AREA_PREPARED, !ds_readbs(DUNGEON_INDEX));
	}

	ds_writeb(DNG_AREA_LOADED, ds_readbs(DUNGEON_INDEX));
	ds_writeb(CITY_AREA_LOADED, -1);
	set_automap_tiles(ds_readws(X_TARGET), ds_readws(Y_TARGET));
}

void load_dungeon_ddt(void)
{
	signed short index;
	signed short low;
	signed short high;
	signed short handle;

	index = ds_readbs(DUNGEON_INDEX) + ARCHIVE_FILE_DNGS_DDT;
	handle = load_archive_file(index);
	read_archive_file(handle, (Bit8u*)&low, 2);
	read_archive_file(handle, (Bit8u*)&high, 2);

#if !defined(__BORLANDC__)
	/* BE-Fix: */
	low = host_readws((Bit8u*)&low);
	high = host_readws((Bit8u*)&high);
#endif

	read_archive_file(handle, (Bit8u*)ds_readd(DUNGEON_FIGHTS_BUF), low);
	read_archive_file(handle, (Bit8u*)ds_readd(DUNGEON_DOORS_BUF), high - low);
	read_archive_file(handle, (Bit8u*)ds_readd(DUNGEON_STAIRS_BUF), 0x7d0);

	close(handle);

}

void seg028_0224(void)
{
	signed short l_si;
	signed short l1;
	RealPt arr[4];

	l1 = ds_readbs(CURRENT_TOWN) + 77;

	if (ds_readbs(CITY_AREA_LOADED) != ds_readbs(CURRENT_TOWN)) {
		load_area_description(1);
		ds_writeb(DNG_AREA_LOADED, -1);
	}

	load_tx(l1);

	if ((ds_readws(AREA_PREPARED) == -1) || (ds_readws(AREA_PREPARED) == 0)) {

		set_var_to_zero();

		ds_writew(CURRENT_ANI, -1);

		seg066_172b();

		ds_writed(BUFFER9_PTR4, ds_readd(BUFFER9_PTR3));

		for (l_si = 0; l_si < 4; l_si++) {

			if (ds_readb(CITY_HOUSE_COUNT + l_si) != 0) {

				arr[l_si] = seg028_0444(!l_si ? ARCHIVE_FILE_HOUSE1_NVF :
				    (l_si == 1 ? ARCHIVE_FILE_HOUSE2_NVF :
				        (l_si == 2 ? ARCHIVE_FILE_HOUSE3_NVF :
				            ARCHIVE_FILE_HOUSE4_NVF)), 0, 0, 0);


			}
		}

		for (l_si = 0; l_si < 4; l_si++) {
			if (!ds_readb(CITY_HOUSE_COUNT + l_si)) {

				arr[l_si] = (!l_si ? arr[l_si + 1] : arr[l_si - 1]);
			}
		}

		ds_writed(TEX_HOUSE1, (Bit32u)arr[0]);
		ds_writed(TEX_HOUSE2, (Bit32u)arr[1]);
		ds_writed(TEX_HOUSE3, (Bit32u)arr[2]);
		ds_writed(TEX_HOUSE4, (Bit32u)arr[3]);

		if ((ds_readds(DAY_TIMER) >= HOURS(7)) && (ds_readds(DAY_TIMER) <= HOURS(20)))
		{
			ds_writed(TEX_SKY, (Bit32u) seg028_0444(ARCHIVE_FILE_TDIVERSE_NVF, 0x80, 0x40, 0));

			memcpy(p_datseg + PALETTE_BUILDINGS, (Bit8u*)ds_readd(BUFFER11_PTR), 0xc0);
		} else {
			ds_writed(TEX_SKY, (Bit32u) seg028_0444(ARCHIVE_FILE_TDIVERSE_NVF, 0x80, 0x40, 0));
		}

		ds_writed(TEX_FLOOR, (Bit32u) seg028_0444(!ds_readbs(LARGE_BUF)? ARCHIVE_FILE_TFLOOR1_NVF : ARCHIVE_FILE_TFLOOR2_NVF, 0, 0x20, 0));

		if ((ds_readds(DAY_TIMER) >= HOURS(7)) && (ds_readds(DAY_TIMER) <= HOURS(20)))
		{
			memcpy(p_datseg + PALETTE_FLOOR, (Bit8u*)ds_readd(BUFFER11_PTR), 0x60);
		}

		ds_writew(AREA_PREPARED, 1);
	}

	ds_writeb(CITY_AREA_LOADED, ds_readbs(CURRENT_TOWN));
	ds_writeb(DNG_AREA_LOADED, -1);

	set_automap_tiles(ds_readw(X_TARGET), ds_readw(Y_TARGET));
}

RealPt seg028_0444(signed short index, signed short firstcol, signed short colors, signed short ref)
{
	signed short fd;
	Bit32s v1;
	Bit32s v2;
	RealPt ptr;

	ptr = (Bit8u*)ds_readd(BUFFER9_PTR4);

	fd = load_archive_file(index);

	v1 = v2 = 0L;

	do {
		v1 = read_archive_file(fd, (Bit8u*)ds_readd(BUFFER9_PTR4), 65000);

#if !defined(__BORLANDC__)
		F_PADA((p_datseg + BUFFER9_PTR4), v1);
#else
		*(HugePt*)(p_datseg + BUFFER9_PTR4) += v1;
#endif

		v2 += v1;

	} while (v1);

	close(fd);

	if (colors) {

		ds_writed(BUFFER11_PTR, (Bit32u)(ptr + v2 - 3 * colors));

		if ((ref != 0) && (!ds_readb(FADING_STATE))) {

			wait_for_vsync();

			set_palette((Bit8u*)ds_readd(BUFFER11_PTR), firstcol, colors);
		}
	}

	return ptr;

}

void load_special_textures(signed short arg)
{
	signed short fd;

	fd = load_archive_file(arg == 9 ? ARCHIVE_FILE_FINGER_NVF : ARCHIVE_FILE_LTURM_NVF);
	read_archive_file(fd, (Bit8u*)ds_readd(BUFFER7_PTR), 64000);
	close(fd);

}

void call_load_buffer(void)
{
	load_tx(ds_readws(TX_FILE_INDEX));
}

void seg028_0555(signed short town)
{
	if (town == 1) {
		seg028_0224();
	} else {
		prepare_dungeon_area();
	}
}

/**
 * \brief   writes the old area and reads a new one
 *
 * \param   type        either 0,1,2
 *
 *	0 = only write old area
 *	1 = do both
 *	2 = only read new area (loading a savegame)
 */
void load_area_description(signed short type)
{
	signed short f_index;
	signed short fd;

	if (ds_readw(AREADESCR_FILEID) != 0) {
		if (type != 2) {
			fd = load_archive_file(ds_readw(AREADESCR_FILEID) + 0x8000);

			if ((ds_readw(AREADESCR_DNG_FLAG) == 0) && (ds_readb(DNG_MAP_SIZE) == 0x20)) {
				_write(fd, (void*)MK_FP(datseg, DNG_MAP), 0x200);
			} else {
				lseek(fd, ds_readws(AREADESCR_DNG_LEVEL) * 0x140, 0);
				_write(fd, (void*)MK_FP(datseg, DNG_MAP), 0x100);
			}
			/* write automap tiles */
			_write(fd, (void*)MK_FP(datseg, AUTOMAP_BUF), 64);
			/* write location information */
			_write(fd, (void*)MK_FP(datseg, LOCATIONS_LIST), ds_readw(LOCATIONS_LIST_SIZE));

			close(fd);

			ds_writew(AREADESCR_FILEID, ds_writew(AREADESCR_DNG_LEVEL, ds_writew(LOCATIONS_LIST_SIZE, ds_writew(AREADESCR_DNG_FLAG, 0))));
		}
	}

	if (type != 0) {

		/* calc archive file index */
		if (ds_readbs(DUNGEON_INDEX) != 0) {
			/* dungeon */
			ds_writew(AREADESCR_FILEID, f_index = ds_readbs(DUNGEON_INDEX) + (ARCHIVE_FILE_DNGS-1));
		} else {
			/* city */
			ds_writew(AREADESCR_FILEID, f_index = ds_readbs(CURRENT_TOWN) + (ARCHIVE_FILE_CITY_DAT-1));
		}

		/* save dungeon level */
		ds_writew(AREADESCR_DNG_LEVEL, ds_readbs(DUNGEON_LEVEL));

		/* save if we are in a dungeon */
		ds_writew(AREADESCR_DNG_FLAG, ds_readbs(DUNGEON_INDEX) != 0 ? 1 : 0);

		/* load DAT or DNG file */
		fd = load_archive_file(f_index + 0x8000);

		if (!ds_readbs(DUNGEON_INDEX) &&
			(ds_readb(CURRENT_TOWN) == TOWNS_THORWAL
				|| ds_readb(CURRENT_TOWN) == TOWNS_PREM
				|| ds_readb(CURRENT_TOWN) == TOWNS_PHEXCAER))
		{
			/* path taken in THORWAL PREM and PHEXCAER */
			_read(fd, p_datseg + DNG_MAP, 0x200);
			/* read automap tiles */
			_read(fd, p_datseg + AUTOMAP_BUF, 0x40);

			/* TODO: is that neccessary ? */
			memset(p_datseg + LOCATIONS_LIST, -1, 900);

			ds_writew(LOCATIONS_LIST_SIZE, _read(fd, p_datseg + LOCATIONS_LIST, 1000));

			ds_writeb(DNG_MAP_SIZE, 0x20);
		} else {
			/* Seek to Dungeon Level * 320 */
			lseek(fd, ds_readbs(DUNGEON_LEVEL) * 320, 0);
			_read(fd, p_datseg + DNG_MAP, 0x100);

			/* read automap tiles */
			_read(fd, p_datseg + AUTOMAP_BUF, 0x40);
			ds_writew(LOCATIONS_LIST_SIZE, 0);

			if (!ds_readbs(DUNGEON_INDEX)) {
				/* TODO: is that neccessary ? */
				memset(p_datseg + LOCATIONS_LIST, -1, 900);
				ds_writew(LOCATIONS_LIST_SIZE, _read(fd, p_datseg + LOCATIONS_LIST, 1000));
			}

			ds_writeb(DNG_MAP_SIZE, 0x10);
		}
		close(fd);
	}
}

void call_load_area(signed short type)
{
	load_area_description(type);
}

void unused_store(signed short no)
{
	signed short width;
	signed short height;
	Bit8u *ptr;
	struct nvf_desc nvf;
	signed short size;

	nvf.dst = (Bit8u*)ds_readd(RENDERBUF_PTR) + 30000;
	nvf.src = (Bit8u*)ds_readd(BUFFER9_PTR3);
	nvf.no = no;
	nvf.type = 0;
	nvf.width = (Bit8u*)&width;
	nvf.height = (Bit8u*)&height;
	process_nvf(&nvf);

#if !defined(__BORLANDC__)
	/* BE-fix */
	width = host_readws((Bit8u*)&width);
	height = host_readws((Bit8u*)&height);
#endif

	EMS_map_memory(ds_readws(EMS_UNUSED_HANDLE), ds_readws(EMS_UNUSED_LPAGE), 0);
	EMS_map_memory(ds_readws(EMS_UNUSED_HANDLE), ds_readws(EMS_UNUSED_LPAGE) + 1, 1);
	EMS_map_memory(ds_readws(EMS_UNUSED_HANDLE), ds_readws(EMS_UNUSED_LPAGE) + 2, 2);
	EMS_map_memory(ds_readws(EMS_UNUSED_HANDLE), 0, 3);

	size = width * height;
	memmove((Bit8u*)((Bit8u*)ds_readd(EMS_FRAME_PTR) + ds_readws(EMS_UNUSED_OFFSET)),
			(Bit8u*)((Bit8u*)ds_readd(RENDERBUF_PTR) + 0x7530),
			size);

	ptr = no * 5 + (Bit8u*)ds_readd(EMS_UNUSED_TAB);

	host_writeb(ptr, (signed char)ds_readws(EMS_UNUSED_LPAGE));
	host_writeb(ptr + 1, ds_readws(EMS_UNUSED_OFFSET) >> 8);
	host_writew(ptr + 2, width);
	host_writeb(ptr + 4, (signed char)height);

	ds_writew(EMS_UNUSED_LPAGE, (ds_readw(0x5ec0) + ((ds_readw(EMS_UNUSED_OFFSET) + size) >> 14)));
	ds_writew(EMS_UNUSED_OFFSET, ((((ds_readw(0x5ec2) + size) & 0x3fff) + 0x100) & 0xff00));
}

RealPt unused_load(signed short no)
{
	signed short l_si;

	EMS_map_memory(ds_readws(EMS_UNUSED_HANDLE), 0, 3);

	l_si = host_readb((Bit8u*)ds_readd(EMS_UNUSED_TAB) + 5 * no);

	EMS_map_memory(ds_readws(EMS_UNUSED_HANDLE), l_si, 0);
	EMS_map_memory(ds_readws(EMS_UNUSED_HANDLE), l_si + 1, 1);
	EMS_map_memory(ds_readws(EMS_UNUSED_HANDLE), l_si + 2, 2);

	return (Bit8u*)ds_readd(EMS_FRAME_PTR) + 256 * host_readb((Bit8u*)ds_readd(EMS_UNUSED_TAB) + 5 * no + 1);
}

void load_map(void)
{
	signed short fd;
	signed short wallclock_update_bak;
	struct nvf_desc nvf;

	wallclock_update_bak = ds_readw(WALLCLOCK_UPDATE);
	ds_writew(WALLCLOCK_UPDATE, 0);

	ds_writew(AREA_PREPARED, 0xffff);
	/* set current_ani to -1 */
	ds_writew(CURRENT_ANI, 0xffff);

	/* open OBJECTS.NVF */
	fd = load_archive_file(ARCHIVE_FILE_OBJECTS_NVF);
	read_archive_file(fd, (Bit8u*)ds_readd(RENDERBUF_PTR), 2000);
	close(fd);

    /* load the grey border for the wallclock overlay */
	nvf.src = (Bit8u*)ds_readd(RENDERBUF_PTR);
	nvf.type = 0;
	nvf.width = (Bit8u*)&fd;
	nvf.height = (Bit8u*)&fd;
	nvf.dst = (Bit8u*)(F_PADD((Bit8u*)ds_readd(BUFFER9_PTR), 18000));
	nvf.no = 16;

	process_nvf(&nvf);

	array_add((Bit8u*)(F_PADD((Bit8u*)ds_readd(BUFFER9_PTR), 18000)), 3003, 0xe0, 2);

	ds_writeb(PP20_INDEX, ARCHIVE_FILE_KARTE_DAT);

	/* if the ems_map_hanlder exists */
	if (ds_readw(EMS_TRAVEL_MAP) != 0) {
		/* get data from EMS */
		EMS_map_memory(ds_readw(EMS_TRAVEL_MAP), 0, 0);
		EMS_map_memory(ds_readw(EMS_TRAVEL_MAP), 1, 1);
		EMS_map_memory(ds_readw(EMS_TRAVEL_MAP), 2, 2);
		EMS_map_memory(ds_readw(EMS_TRAVEL_MAP), 3, 3);
		/* set map pointer to EMS */
		ds_writed(TRAVEL_MAP_PTR, ds_readd(EMS_FRAME_PTR));
	} else {
		/* or read KARTE.DAT from file */
		fd = load_archive_file(ARCHIVE_FILE_KARTE_DAT);

		read_archive_file(fd, (Bit8u*)(ds_writed(TRAVEL_MAP_PTR, ds_readd(RENDERBUF_PTR))), 64098);
		close(fd);

		if (ds_readb(EMS_ENABLED) != 0) {

			if ((ds_writew(EMS_TRAVEL_MAP, alloc_EMS(64100)))) {
				/* map the map into EMS */
				EMS_map_memory(ds_readw(EMS_TRAVEL_MAP), 0, 0);
				EMS_map_memory(ds_readw(EMS_TRAVEL_MAP), 1, 1);
				EMS_map_memory(ds_readw(EMS_TRAVEL_MAP), 2, 2);
				EMS_map_memory(ds_readw(EMS_TRAVEL_MAP), 3, 3);

				/* TODO: update window */
				memmove((void*)((Bit8u*)ds_readd(EMS_FRAME_PTR)),
					(void*)((Bit8u*)ds_readd(RENDERBUF_PTR)), 320 * 200 + 98);
			}
		}
	}

	/* load LROUT.DAT */
	fd = load_archive_file(ARCHIVE_FILE_LROUT_DAT);
	read_archive_file(fd, (Bit8u*)ds_readd(BUFFER9_PTR), 7600);
	close(fd);

	/* load HSROUT.DAT */
	fd = load_archive_file(ARCHIVE_FILE_HSROUT_DAT);
	read_archive_file(fd, (Bit8u*)(F_PADD((Bit8u*)ds_readd(BUFFER9_PTR), 7600)), 3800);
	close(fd);

	/* load SROUT.DAT */
	fd = load_archive_file(ARCHIVE_FILE_SROUT_DAT);
	read_archive_file(fd, (Bit8u*)(F_PADD((Bit8u*)ds_readd(BUFFER9_PTR), 11400)), 5900);
	close(fd);

	load_tx(ARCHIVE_FILE_MAPTEXT_LTX);

	ds_writew(WALLCLOCK_UPDATE, wallclock_update_bak);
}

void load_npc(signed short index)
{
	Bit8u *npc_dst;
	signed short fd;

	npc_dst = get_hero(6);

	/* load from temp directory */
	fd = load_archive_file(index | 0x8000);
	_read(fd, (void*)npc_dst, SIZEOF_HERO);
	close(fd);

	if (host_readb(npc_dst + HERO_SEX) == 1) {
		/* female */
		host_writeb(npc_dst + HERO_SPRITE_NO, host_readb(npc_dst + HERO_TYPE) + 11);
		if (host_readbs(npc_dst + HERO_SPRITE_NO) > 21)
			host_writeb(npc_dst + HERO_SPRITE_NO, 21);
	} else {
		/* male */
		host_writeb(npc_dst + HERO_SPRITE_NO, host_readb(npc_dst + HERO_TYPE));
		if (host_readbs(npc_dst + HERO_SPRITE_NO) > 10)
			host_writeb(npc_dst + HERO_SPRITE_NO, 10);
	}
}

void save_npc(signed short index)
{
	signed short fd;

	fd = load_archive_file(index | 0x8000);

	_write(fd, (Bit8u*)ds_readd(HEROES) + 6 * SIZEOF_HERO, SIZEOF_HERO);

	close(fd);
}

void load_splashes(void)
{
	signed short fd;
	signed short width;
	signed short height;
	struct nvf_desc nvf;

	/* read SPLASHES.DAT */
	fd = load_archive_file(ARCHIVE_FILE_SPLASHES_DAT);
	read_archive_file(fd, (Bit8u*)ds_readd(RENDERBUF_PTR), 3000);
	close(fd);

	/* nvf.dst = splash_le = ds_readd() */
	nvf.dst = (Bit8u*)(ds_writed(SPLASH_LE, ds_readd(SPLASH_BUFFER)));
	nvf.src = (Bit8u*)ds_readd(RENDERBUF_PTR);
	nvf.no = 0;
	nvf.type = 1;
	nvf.width = (Bit8u*)&width;
	nvf.height = (Bit8u*)&height;
	fd = (signed short)process_nvf(&nvf);

	/* nvf.dst = splash_ae = ds_readd() */
	nvf.dst = (Bit8u*)(ds_writed(SPLASH_AE, (Bit32u)((Bit8u*)ds_readd(SPLASH_BUFFER) + fd)));
	nvf.src = (Bit8u*)ds_readd(RENDERBUF_PTR);
	nvf.no = 1;
	nvf.type = 1;
	nvf.width = (Bit8u*)&width;
	nvf.height = (Bit8u*)&height;
	process_nvf(&nvf);
}

void load_informer_tlk(signed short index)
{
	signed short i;
	signed short fd;

	Bit32s text_len;
	Bit32s off;
	signed short partners;
	Bit8u *ptr;


	ds_writew(TEXT_FILE_INDEX, index);

	fd = load_archive_file(index);

	/* read the header */
	read_archive_file(fd, (Bit8u*)&off, 4);
	read_archive_file(fd, (Bit8u*)&partners, 2);

#if !defined(__BORLANDC__)
	/* BE-Fix */
	off = host_readd((Bit8u*)&off);
	partners = host_readw((Bit8u*)&partners);
#endif

	/* read the partner structures */
	read_archive_file(fd, ptr = (p_datseg + DIALOG_PARTNERS), partners * 0x26);

	/* read the dialog layouts */
	read_archive_file(fd, p_datseg + DIALOG_STATES, (Bit16u)(off - partners * 0x26));

	/* read the text */
	text_len = (signed short)read_archive_file(fd, (Bit8u*)ds_readd(BUFFER8_PTR), 10000);

	close(fd);

	split_textbuffer((Bit8u*)ds_readd(TX2_INDEX),
		(Bit8u*)ds_readd(BUFFER8_PTR), text_len);

	/* adjust the pointers to the layouts */
	for (i = 0; i < partners; i++, ptr += 0x26) {
		host_writed(ptr, (Bit32u)(p_datseg + host_readw(ptr) + DIALOG_STATES));
	}
}

void load_tlk(signed short index)
{
	signed short i;
	signed short fd;
	Bit32s text_len;
	Bit32s off;
	signed short partners;
	Bit8u *ptr;

	ds_writew(TEXT_FILE_INDEX, index);

	fd = load_archive_file(index);

	/* read the header */
	read_archive_file(fd, (Bit8u*)&off, 4);
	read_archive_file(fd, (Bit8u*)&partners, 2);

#if !defined(__BORLANDC__)
	/* BE-Fix */
	off = host_readd((Bit8u*)&off);
	partners = host_readw((Bit8u*)&partners);
#endif

	/* read the partner structures */
	read_archive_file(fd, ptr = p_datseg + DIALOG_PARTNERS, partners * 0x26);

	/* read the dialog layouts */
	read_archive_file(fd,
		(Bit8u*)((p_datseg + DIALOG_STATES)), off - partners * 0x26);

	/* read the text */
	text_len = (signed short)read_archive_file(fd, (Bit8u*)ds_readd(BUFFER7_PTR), 64000);

	close(fd);

	split_textbuffer((Bit8u*)ds_readd(TX_INDEX),
		(Bit8u*)ds_readd(BUFFER7_PTR), text_len);

	/* adjust the pointers to the layouts */
	for (i = 0; i < partners; i++, ptr += 0x26) {
		host_writed(ptr, (Bit32u)(p_datseg + host_readw(ptr) + DIALOG_STATES));
	}
}

void unused_load_archive_file(signed short index, signed short a2, Bit32u seg)
{
	signed short fd;

	fd = load_archive_file(index);
	read_archive_file(fd, (Bit8u*)(RealMake(seg, a2)), 64000);
	close(fd);
}


void load_fightbg(signed short index)
{
	signed short fd;

	fd = load_archive_file(index);
	read_archive_file(fd, (Bit8u*)ds_readd(RENDERBUF_PTR), 30000);
	decomp_pp20((Bit8u*)ds_readd(RENDERBUF_PTR),
			(Bit8u*)ds_readd(BUFFER8_PTR),
#if !defined(__BORLANDC__)
			(Bit8u*)ds_readd(RENDERBUF_PTR) + 4,
#else
			FP_OFF(ds_readd(RENDERBUF_PTR)) + 4,
			FP_SEG(ds_readd(RENDERBUF_PTR)),
#endif
			get_readlength2(fd));
	close(fd);
}

#if !defined(__BORLANDC__)
}
#endif
