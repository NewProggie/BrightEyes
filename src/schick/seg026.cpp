/**
 *	Rewrite of DSA1 v3.02_de functions of seg026 (texts savegames)
 *	Functions rewritten: 15/15 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg026.cpp
 */
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

#if defined(__BORLANDC__)
#include <DIR.H>
#include <DOS.H>
#include <IO.H>
#else
#include <unistd.h>
#endif

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg026.h"
#include "seg028.h"
#include "seg097.h"
#include "seg106.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

void init_text(void)
{
	Bit32s len;
	signed short handle;

	handle = load_archive_file(ARCHIVE_FILE_FONT6);
	read_archive_file(handle, (Bit8u*)ds_readd(BUF_FONT6), 1000);
	close(handle);

	handle = load_archive_file(ARCHIVE_FILE_TEXT_LTX);
	len = (signed short)read_archive_file(handle, (Bit8u*)ds_readd(TEXT_LTX_BUFFER), 64000);
	close(handle);

	split_textbuffer((Bit8u*)ds_readd(TEXT_LTX_INDEX), (Bit8u*)ds_readd(TEXT_LTX_BUFFER), len);

	handle = load_archive_file(ARCHIVE_FILE_ITEMNAME);
	len = (signed short)read_archive_file(handle, (Bit8u*)ds_readd(BUFFER5_PTR), 5000);
	close(handle);

	split_textbuffer((Bit8u*)ds_readd(ITEMSNAME), (Bit8u*)ds_readd(BUFFER5_PTR), len);

	handle = load_archive_file(ARCHIVE_FILE_MONNAMES);
	len = (signed short)read_archive_file(handle, (Bit8u*)ds_readd(MONNAMES_BUFFER), 5000);
	close(handle);

	split_textbuffer((Bit8u*)ds_readd(MONNAMES_INDEX), (Bit8u*)ds_readd(MONNAMES_BUFFER), len);

}

void load_tx(signed short index)
{
	Bit32s archive_file_len;
	signed short archive_file_handle;

	if (index == -1)
		return;

	archive_file_handle = load_archive_file(index);

	archive_file_len = (signed short)read_archive_file(archive_file_handle, (Bit8u*)ds_readd(BUFFER7_PTR), 64000);

	close(archive_file_handle);

	split_textbuffer((Bit8u*)ds_readd(TX_INDEX), (Bit8u*)ds_readd(BUFFER7_PTR), archive_file_len);

	ds_writew(TX_FILE_INDEX, index);
}

void load_tx2(signed short index)
{
	Bit32s len;
	register signed short fd;

	if (index == -1)
		return;

	ds_writew(TEXT_FILE_INDEX, index);
	fd = load_archive_file(index);
	len = (signed short)read_archive_file(fd, (Bit8u*)ds_readd(BUFFER8_PTR), 12000);
	close(fd);

	split_textbuffer((Bit8u*)ds_readd(TX2_INDEX), (Bit8u*)ds_readd(BUFFER8_PTR), len);
}

void load_ltx(unsigned short index)
{
	Bit32s len;
	signed short fd;

	fd = load_archive_file(index);
	ds_writew(AREA_PREPARED, 0xffff);
	len = (signed short)read_archive_file(fd, (Bit8u*)ds_readd(BUFFER9_PTR3) + 1000, 64000);
	close(fd);

	split_textbuffer((Bit8u*)ds_readd(BUFFER9_PTR3),
		F_PADD((Bit8u*)ds_readd(BUFFER9_PTR3), 1000L), len);
}

void split_textbuffer(Bit8u *dst, RealPt src, Bit32u len)
{
	Bit32u i = 0;

	host_writed(dst, (Bit32u)src);
	dst += 4;

	for (; i != len; src++, i++) {
		/* continue if not the end of the string */
		if (!host_readbs((Bit8u*)(src))) {

			/* write the adress of the next string */
			host_writed(dst, (Bit32u)(src + 1));
			dst += 4;
		}
	}
}

void load_ggsts_nvf(void)
{
	Bit16u fd;

	/* seek to GGSTS.NVF */
	fd = load_archive_file(ARCHIVE_FILE_GGSTS_NVF);
	/* read it */
	read_archive_file(fd, (Bit8u*)ds_readd(BUFFER10_PTR), 16771);
	/* close it */
	close(fd);

	ds_writew(AREA_PREPARED, 0xffff);
}

void prepare_chr_name(char *dst, char *src)
{
	char tmp_str[40];
	signed short i;

	strcpy(tmp_str, src);

	for (i = 0; i < 8; i++) {

		if (!tmp_str[i])
			break;

		/* maybe !isalnum(tmp_str[i]) */
		if (!(ds_readbs(CHAR_TYPE_TABLE + tmp_str[i]) & 0x0e)) {
			tmp_str[i] = 0x5f;
		}
	}

	strncpy(dst, tmp_str, 8);
	dst[8] = '\0';
	strcat(dst, (char*)p_datseg + CHR_FILE_SUFFIX);
}

void prepare_sg_name(char *dst, char *src)
{
	char tmp_str[40];
	signed short i;

	strcpy(tmp_str, src);

	i = 0;
	while (i < 8) {

		if (!tmp_str[i]) {
			while (i < 8) {
				/* fill up with underscores */
				tmp_str[i] = 0x5f;
				i++;
			}
			break;
		}

		/* maybe !isalnum(tmp_str[i]) */
		if (!(ds_readbs(CHAR_TYPE_TABLE + tmp_str[i]) & 0x0e)) {
			tmp_str[i] = 0x5f;
		}
		i++;
	}

	strncpy(dst, tmp_str, 8);
	dst[8] = '\0';
}

signed short load_game_state(void)
{
	register signed short handle_gs;
	signed short i;
	signed short handle;
	signed short answer;
	signed short l1;
	HugePt p_status_start;
	HugePt p_status_end;
	signed short status_length;
	signed short l2;
	signed short l3;
	signed short retval;
	signed short l4;
	RealPt hero_i;
	signed char version[4];
	struct ffblk blk;
	char name[20];

	retval = 0;

	/* select a game state */
	answer = GUI_radio(get_ttx(0), 6,
			p_datseg + (SAVEGAME_NAMES + 0),
			p_datseg + (SAVEGAME_NAMES + 9),
			p_datseg + (SAVEGAME_NAMES + 18),
			p_datseg + (SAVEGAME_NAMES + 27),
			p_datseg + (SAVEGAME_NAMES + 36),
			get_ttx(737)) -1;

	/* sanity check if answer is in range */
	if (answer != -2 && answer != 5) {

		prepare_sg_name((char*)ds_readd(TEXT_OUTPUT_BUF), (char*)p_datseg + SAVEGAME_NAMES + 9 * answer);
		/* concat with ".gam" */
		strcat((char*)ds_readd(TEXT_OUTPUT_BUF), (char*)p_datseg + SAVEGAME_SUFFIX);

		/* open the game state file */
		if ((handle_gs = open((char*)ds_readd(TEXT_OUTPUT_BUF), O_BINARY | O_RDONLY)) == -1)
		{
			GUI_output(get_ttx(635));
			retval = -1;
			return retval;
		}

		update_mouse_cursor();

		/* something ani related */
		l1 = ds_readws(UPDATE_STATUSLINE);
		ds_writew(UPDATE_STATUSLINE, 0);

		l4 = ds_readws(ANI_ENABLED);
		ds_writew(ANI_ENABLED, 0);

		/* delete every file in TEMP */
		sprintf((char*)ds_readd(TEXT_OUTPUT_BUF),
			/* "TEMP\\%s" */
			(char*)ds_readd(STR_TEMP_XX_PTR2),
			/* "*.*" */
			(char*)p_datseg + ALL_FILES_WILDCARD);

#if defined(__BORLANDC__)
		l2 = findfirst((char*)ds_readd(TEXT_OUTPUT_BUF), &blk, 0);

		if (l2 == 0) {

			do {
				sprintf((char*)ds_readd(TEXT_OUTPUT_BUF),
					(char*)ds_readd(STR_TEMP_XX_PTR2),
					((char*)(&blk))+ 30);

				unlink((char*)ds_readd(TEXT_OUTPUT_BUF));

				l2 = findnext(&blk);

			} while (l2 == 0);
		}
#endif

		/* init */
		ds_writed(SAVED_FILES_BUF, ds_readd(DTP2));
		memset((Bit8u*)ds_readd(SAVED_FILES_BUF), 0, 286 * 4);

		/* read version info */
		_read(handle_gs, (Bit8u*)ds_readd(TEXT_OUTPUT_BUF), 12);
		_read(handle_gs, (Bit8u*)&version[3], 1);
		_read(handle_gs, (Bit8u*)&version[2], 1);
		_read(handle_gs, (Bit8u*)&version[0], 1);
		_read(handle_gs, (Bit8u*)&version[1], 1);

		_read(handle_gs, p_datseg + DATSEG_STATUS_START, 4);

		/* read game status */
		p_status_start = (HugePt)(p_datseg + DATSEG_STATUS_START);
		p_status_end = (HugePt)(p_datseg + DATSEG_STATUS_END);
#if !defined(__BORLANDC__)
		status_length = (signed short)F_PSUB(p_status_end, p_status_start);
#else
		status_length = (signed short)(p_status_end - p_status_start);
#endif

		_read(handle_gs, p_datseg + DATSEG_STATUS_START, status_length);

		ds_writeb(SPECIAL_SCREEN, 1);

		/* read file table */
		_read(handle_gs, (Bit8u*)ds_readd(SAVED_FILES_BUF), 286 * 4);

		/* create for each saved file in gam a file in TEMP */
		for (i = 0; i < 286; i++) {

			if (host_readd((Bit8u*)ds_readd(SAVED_FILES_BUF) + 4 * i)) {

				/* write file content to TEMP */
				sprintf((char*)ds_readd(TEXT_OUTPUT_BUF),
					(char*)ds_readd(STR_TEMP_XX_PTR2),
					(char*)(Bit8u*)(ds_readd(FNAMES + 4 * i)));

				/* TODO: should be O_BINARY | O_WRONLY */
				handle = _creat((char*)ds_readd(TEXT_OUTPUT_BUF), 0);

				_read(handle_gs, (Bit8u*)ds_readd(RENDERBUF_PTR), (unsigned short)host_readd((Bit8u*)ds_readd(SAVED_FILES_BUF) + 4 * i));
				_write(handle,   (Bit8u*)ds_readd(RENDERBUF_PTR), (unsigned short)host_readd((Bit8u*)ds_readd(SAVED_FILES_BUF) + 4 * i));
				close(handle);
			}
		}

		/* clear the heroes */
		hero_i = (Bit8u*)ds_readd(HEROES);
		for (i = 0; i <= 6; i++, hero_i += SIZEOF_HERO) {
			memset(hero_i, 0, SIZEOF_HERO);
		}

		hero_i = (Bit8u*)ds_readd(RENDERBUF_PTR);

		do {
			l3 = _read(handle_gs, (Bit8u*)hero_i, SIZEOF_HERO);

			if (l3 != 0) {

				prepare_chr_name(name, (char*)hero_i);

				/* write file content to TEMP */
				sprintf((char*)ds_readd(TEXT_OUTPUT_BUF),
					(char*)ds_readd(STR_TEMP_XX_PTR2),
					name);

				/* TODO: should be O_BINARY | O_WRONLY */
				handle = _creat((char*)ds_readd(TEXT_OUTPUT_BUF), 0);

				_write(handle, (Bit8u*)hero_i, SIZEOF_HERO);
				close(handle);

				if (host_readbs(hero_i + HERO_GROUP_POS) != 0) {

					prepare_chr_name(name, (char*)hero_i);
#if !defined(__BORLANDC__)
					{
						/* create a char[20] on the stack */
						Bit16u sp_bak = reg_sp;
						reg_sp -= 20;

						RealPt r_name = RealMake(SegValue(ss), reg_sp);
						strncpy((char*)(Bit8u*)(r_name), name, 20);
						host_writeb((Bit8u*)(r_name) + 20, 0);
						read_chr_temp(r_name, host_readbs(hero_i + HERO_GROUP_POS) - 1, host_readbs(hero_i + HERO_GROUP_NO));
						reg_sp = sp_bak;
					}
#else
					read_chr_temp(name, host_readbs(hero_i + HERO_GROUP_POS) - 1, host_readbs(hero_i + HERO_GROUP_NO));
#endif
				}
			}
		} while (l3 != 0);

		close(handle_gs);

#if defined(__BORLANDC__)
		/* search for "*.CHR" */
		l2 = findfirst((char*)(p_datseg + ALL_CHR_WILDCARD), &blk, 0);

		while (l2 == 0) {

			sprintf((char*)ds_readd(TEXT_OUTPUT_BUF),
				(char*)ds_readd(STR_TEMP_XX_PTR2),
				((char*)(&blk)) + 30);

			if ((handle_gs = open((char*)ds_readd(TEXT_OUTPUT_BUF), O_BINARY | O_RDWR)) == -1) {
				handle = open((char*)(&blk) + 30, O_BINARY | O_RDWR);
				_read(handle, (Bit8u*)ds_readd(RENDERBUF_PTR), SIZEOF_HERO);
				close(handle);

				/* TODO: should be O_BINARY | O_WRONLY */
				handle_gs = _creat((char*)ds_readd(TEXT_OUTPUT_BUF), 0);
				_write(handle_gs, (Bit8u*)ds_readd(RENDERBUF_PTR), SIZEOF_HERO);
			} else {
				/* Yes, indeed! */
			}

			close(handle_gs);

			l2 = findnext(&blk);
		}
#endif

		for (i = ARCHIVE_FILE_NPCS; i <= (ARCHIVE_FILE_NPCS+5); i++) {
			load_npc(i);

			if (host_readbs(get_hero(6) + HERO_GROUP_POS) != 7) {
				memset(get_hero(6), 0, SIZEOF_HERO);
			} else {
				break;
			}
		}

		ds_writew(AREA_PREPARED, -1);
		ds_writew(REQUEST_REFRESH, retval = 1);
		ds_writew(CHECK_DISEASE, 0);
		ds_writew(CHECK_POISON, 0);
		ds_writeb(FADING_STATE, 3);

		if (ds_readbs(CURRENT_LOCTYPE) != LOCTYPE_TEMPLE) {
			ds_writebs((NEW_MENU_ICONS + 6), ds_writebs((NEW_MENU_ICONS + 7), ds_writebs((NEW_MENU_ICONS + 8), MENU_ICON_NONE)));
		}

		load_area_description(2);

		ds_writews(UPDATE_STATUSLINE, l1);
		ds_writews(ANI_ENABLED, l4);

		refresh_screen_size();
	}

	return retval;
}

/**
 * \brief   writes a game state file
 *
 * \return              1 = OK, 0 = error
 */
signed short save_game_state(void)
{
	signed short l_di;
	HugePt p_status_start;
	HugePt p_status_end;
	unsigned short status_len;
	signed short handle;
	signed short tw_bak;
	signed short l1;
	signed short slot;
	Bit32u filepos;
	Bit32u filepos2;
	signed short flag;
	Bit32u len;
	struct ffblk blk;

	tw_bak = ds_readws(TEXTBOX_WIDTH);
	ds_writew(TEXTBOX_WIDTH, 5);

	/* prepare the header for the radio box */
	if (ds_readws(GAME_STATE) == GAME_STATE_VICTORY) {

		/* game won. creating savegame for import in DSA2 */
		strcpy((char*)ds_readd(TEXT_OUTPUT_BUF), get_ttx(810)); /* "Welcher Spielstand soll fuer die Fortsetzung abgespeichert werden?" */

	} else {

#ifndef M302de_FEATURE_MOD
		/* Feature mod 4: In the original game, when creating a savegame while not being in a temple, the AP of all heroes is decreased by 1. This feature mod stops the AP decrease.
		 * Here, the warning message "Dabei verliert jeder Held in der Gruppe einen Abenteuerpunkt" is displayed. */
		if (ds_readbs(CURRENT_LOCTYPE) != LOCTYPE_TEMPLE && ds_readws(GAME_STATE) != GAME_STATE_VICTORY) {

			/* create savegame not in a temple */

			sprintf((char*)ds_readd(DTP2),
				get_ttx(813), /* "Dabei verliert jeder Held in der Gruppe einen Abenteuerpunkt" */
				1,
				get_ttx(392),
				p_datseg + EMPTY_STRING1);

			sprintf((char*)ds_readd(TEXT_OUTPUT_BUF),
				get_ttx(1), /* "Welchen Spielstand wollen Sie abspeichern ?" */
				(char*)ds_readd(DTP2));
		} else {
#endif

			/* create savegame inside a temple */
			sprintf((char*)ds_readd(TEXT_OUTPUT_BUF),
				get_ttx(1), /* "Welchen Spielstand wollen Sie abspeichern ?" */
				(char*)p_datseg + EMPTY_STRING2);
#ifndef M302de_FEATURE_MOD
		/* Feature mod 4: In the original game, when creating a savegame while not being in a temple, the AP of all heroes is decreased by 1. This mod stops the AP decrease. */
		}
#endif
	}

	/* get the slot number */
	slot = GUI_radio((char*)ds_readd(TEXT_OUTPUT_BUF), 6,
			p_datseg + (SAVEGAME_NAMES + 9 * 0),
			p_datseg + (SAVEGAME_NAMES + 9 * 1),
			p_datseg + (SAVEGAME_NAMES + 9 * 2),
			p_datseg + (SAVEGAME_NAMES + 9 * 3),
			p_datseg + (SAVEGAME_NAMES + 9 * 4),
			get_ttx(737)) - 1;

	ds_writew(TEXTBOX_WIDTH, tw_bak);

	ds_writed(SAVED_FILES_BUF, ds_readd(DTP2));
	memset((Bit8u*)ds_readd(SAVED_FILES_BUF), 0, 4 * 286);

	if (slot != -2 && slot != 5) {

		do {
			/* ask for filename */
			ds_writew(GUI_ENTERING_SAVEGAME, 1);
			strcpy((char*)ds_readd(TEXT_INPUT_BUF), (char*)p_datseg + SAVEGAME_NAMES + 9 * slot);
			GUI_input(get_ttx(634), 8);
			ds_writew(GUI_ENTERING_SAVEGAME, 0);

			if (host_readbs((char*)ds_readd(TEXT_INPUT_BUF)) == 0) {
				return 0;
			}

			flag = 0;

			prepare_sg_name((char*)ds_readd(TEXT_OUTPUT_BUF), (char*)ds_readd(TEXT_INPUT_BUF));

			for (tw_bak = 0; tw_bak < 5; tw_bak++) {

				prepare_sg_name((char*)ds_readd(TEXT_OUTPUT_BUF) + 50, (char*)p_datseg + SAVEGAME_NAMES + 9 * tw_bak);

				if (slot != tw_bak && !strcmp((char*)ds_readd(TEXT_OUTPUT_BUF), (char*)ds_readd(TEXT_OUTPUT_BUF) + 50)) {

					GUI_output(get_ttx(806));
					flag = 1;
				}
			}
		} while (flag != 0);

		/* delete the previous file of that slot */
		prepare_sg_name((char*)ds_readd(TEXT_OUTPUT_BUF), (char*)p_datseg + SAVEGAME_NAMES + 9 * slot);
		strcat((char*)ds_readd(TEXT_OUTPUT_BUF), (char*)p_datseg + SAVEGAME_SUFFIX2);
		unlink((char*)ds_readd(TEXT_OUTPUT_BUF));
		strcpy((char*)p_datseg + SAVEGAME_NAMES + 9 * slot, (char*)ds_readd(TEXT_INPUT_BUF));

		/* create a CHR-file for each hero in TEMP */
		for (tw_bak = 0; tw_bak < 6; tw_bak++) {

			if (host_readbs(get_hero(tw_bak) + HERO_TYPE) != HERO_TYPE_NONE) {

				/* save position on the playmask */
				host_writebs(get_hero(tw_bak) + HERO_GROUP_POS, tw_bak + 1);

#ifndef M302de_FEATURE_MOD
				/* Feature mod 4: In the original game, when creating a savegame while not being in a temple, the AP of all heroes is decrease by 1. This feature mod stops the AP decrease.
				 * Here, the actual decrease is executed */
				if (ds_readws(GAME_STATE) != GAME_STATE_VICTORY &&
					ds_readbs(CURRENT_LOCTYPE) != LOCTYPE_TEMPLE &&
					host_readds(get_hero(tw_bak) + HERO_AP) > 0)
				{
					add_hero_ap(get_hero(tw_bak), -1L);
				}
#endif

				write_chr_temp(tw_bak);
			}
		}

		/* save the current NPC in TEMP */
		if (host_readbs(get_hero(6) + HERO_TYPE) != HERO_TYPE_NONE) {

			host_writeb(get_hero(6) + HERO_GROUP_POS, 7);
			save_npc(host_readbs(get_hero(6) + HERO_NPC_ID) + 225);
		}

		load_area_description(1);

		p_status_start = (HugePt)(p_datseg + DATSEG_STATUS_START);
		p_status_end = (HugePt)(p_datseg + DATSEG_STATUS_END);
#if !defined(__BORLANDC__)
		status_len = (signed short)F_PSUB(p_status_end, p_status_start);
#else
		status_len = (signed short)(p_status_end - p_status_start);
#endif

		prepare_sg_name((char*)ds_readd(TEXT_OUTPUT_BUF), (char*)p_datseg + SAVEGAME_NAMES + 9 * slot);
		strcat((char*)ds_readd(TEXT_OUTPUT_BUF), (char*)p_datseg + SAVEGAME_SUFFIX3);

		/* TODO: should be O_BINARY | O_RWONLY */
		while ((l_di = _creat((char*)ds_readd(TEXT_OUTPUT_BUF), 0)) == -1) {
			GUI_output(get_ttx(348));
			return 0;
		}

		time((Bit32s*)(p_datseg + LAST_SAVE_TIME));

		filepos = 0;

		/* write version identifier 16 bytes */
		filepos += _write(l_di, p_datseg + DSA_VERSION_STRING, 12);
		filepos += _write(l_di, p_datseg + VERSION_TOKEN4, 1);
		filepos += _write(l_di, p_datseg + VERSION_TOKEN3, 1);
		filepos += _write(l_di, p_datseg + VERSION_TOKEN1, 1);
		filepos += _write(l_di, p_datseg + VERSION_TOKEN2, 1);

		/* write fileposition 4 bytes */
		/* this will be updated later to find the data of the CHR files */
		filepos += _write(l_di, &filepos, 4);

		/* save the status section 5952 bytes */
		filepos += _write(l_di, p_status_start, status_len);

		/* check if enough bytes were written */
		if (status_len + 16 + 4L != filepos) {
			GUI_output(get_ttx(348));
			close(l_di);
			return 0;
		}

		filepos2 = filepos;
		len = (Bit16u)_write(l_di, (Bit8u*)ds_readd(SAVED_FILES_BUF), 4 * 286);
		filepos += len;

		if (len != 4 * 286) {
			GUI_output(get_ttx(348));
			close(l_di);
			return 0;
		}

#if defined(__BORLANDC__)
		/* save all changed files from SCHICK.DAT */
		for (tw_bak = 0; tw_bak < 286; tw_bak++) {

			sprintf((char*)ds_readd(TEXT_OUTPUT_BUF),
				(char*)ds_readd(STR_TEMP_XX_PTR2),
				(char*)(Bit8u*)(ds_readd(FNAMES + 4 * tw_bak)));

			l1 = findfirst((char*)ds_readd(TEXT_OUTPUT_BUF), &blk, 0);


			if (l1 == 0) {

				handle = load_archive_file(tw_bak + 0x8000);
				host_writed((Bit8u*)ds_readd(SAVED_FILES_BUF) + 4 * tw_bak, get_readlength2(handle));
				_read(handle, (Bit8u*)ds_readd(RENDERBUF_PTR), (unsigned short)host_readd((Bit8u*)ds_readd(SAVED_FILES_BUF) + 4 * tw_bak));
				close(handle);

				len = (Bit16u)_write(l_di, (Bit8u*)ds_readd(RENDERBUF_PTR), (unsigned short)host_readd((Bit8u*)ds_readd(SAVED_FILES_BUF) + 4 * tw_bak));
				filepos += len;

				if ((Bit16u)host_readd((Bit8u*)ds_readd(SAVED_FILES_BUF) + 4 * tw_bak) != len) {
					GUI_output(get_ttx(348));
					close(l_di);
					return 0;
				}
			}
		}
#endif

		/* skip back to the start of the offset of the CHR data */
		lseek(l_di, 16, 0);
		_write(l_di, &filepos, 4);

		/* write the file table */
		lseek(l_di, filepos2, 0);
		_write(l_di, (Bit8u*)ds_readd(SAVED_FILES_BUF), 4 * 286);

		/* append all CHR files */
		lseek(l_di, filepos, 0);
		sprintf((char*)ds_readd(TEXT_OUTPUT_BUF),
			(char*)ds_readd(STR_TEMP_XX_PTR2),
			(char*)p_datseg + ALL_CHR_WILDCARD2);
#if defined(__BORLANDC__)
		l1 = findfirst((char*)ds_readd(TEXT_OUTPUT_BUF), &blk, 0);
		do {
			/* create the CHR filename */
			sprintf((char*)ds_readd(TEXT_OUTPUT_BUF),
				(char*)ds_readd(STR_TEMP_XX_PTR2),
				((char*)(&blk)) + 30);

			/* read the CHR file from temp */
			handle = open((char*)ds_readd(TEXT_OUTPUT_BUF), O_BINARY | O_RDWR);
			_read(handle, (Bit8u*)ds_readd(RENDERBUF_PTR), SIZEOF_HERO);
			close(handle);

			/* append it */
			len = _write(l_di, (Bit8u*)ds_readd(RENDERBUF_PTR), SIZEOF_HERO);

			if (len != SIZEOF_HERO) {
				GUI_output(get_ttx(348));
				close(l_di);
				return 0;
			}

			l1 = findnext(&blk);

		} while (l1 == 0);

		close(l_di);
#endif

		/* rewrite GAMES.NAM */
		l_di = _creat((char*)ds_readd(FNAMES + 0x33c), 0);
		_write(l_di, p_datseg + SAVEGAME_NAMES, 45);
		close(l_di);

		return 1;
	}

	return 0;
}

/**
 * \brief   reads a CHR file from TEMP-dir
 *
 * \param   fname       filename
 * \param   hero_pos    position of the hero
 * \param   a2          ???
 * \return              1 = OK, 0 = Error
 */
signed short read_chr_temp(RealPt fname, signed short hero_pos, signed short a2)
{
	signed short handle;
	signed short hero_size = SIZEOF_HERO;
	Bit8u *hero;

	sprintf((char*)ds_readd(TEXT_OUTPUT_BUF),
		(char*)ds_readd(STR_TEMP_XX_PTR2),
		(char*)(Bit8u*)(fname));

	if ((handle = open((char*)ds_readd(TEXT_OUTPUT_BUF), O_BINARY | O_RDWR)) == -1) {
		copy_file_to_temp(fname, (char*)ds_readd(TEXT_OUTPUT_BUF));
		handle = open((char*)ds_readd(TEXT_OUTPUT_BUF), O_BINARY | O_RDWR);
	}

	if (handle != -1) {

		hero = get_hero(hero_pos);
		_read(handle, hero, hero_size);
		close(handle);

		host_writeb(hero + HERO_GROUP_NO, (signed char)a2);

		if (host_readbs(hero + HERO_SEX) == 1) {
			host_writeb(hero + HERO_SPRITE_NO, host_readbs(hero + HERO_TYPE) + 11);

			if (host_readbs(hero + HERO_SPRITE_NO) > 21) {
				host_writeb(hero + HERO_SPRITE_NO, 21);
			}
		} else {
			host_writeb(hero + HERO_SPRITE_NO, host_readbs(hero + HERO_TYPE));

			if (host_readbs(hero + HERO_SPRITE_NO) > 10) {
				host_writeb(hero + HERO_SPRITE_NO, 10);
			}
		}

		/* In the following line it would be more consistent to check only bit 0.
		 * bit 1 is a flag which is set if the hero got the IN attribute bonus at the black eye at the Monolith (Einsiedlersee <-> Einsiedlersee, tevent135).
		 * However, this should still be ok, as it should never happen that bit 0 is unset and bit 1 is set. */

		if (!host_readbs(hero + HERO_START_GEAR)) {

			startup_equipment(hero);
			host_writeb(get_hero(hero_pos) + HERO_START_GEAR, 1); /* it would be more consistent to set only bit 0 and leave the others untouched, see above. */

			write_chr_temp(hero_pos);
		}

	} else {
		GUI_output(get_ttx(4));
		return 0;
	}

	return 1;

}

/**
 * \brief   writes a CHR file to temp
 *
 * \param   hero_pos    position of the hero
 */
void write_chr_temp(unsigned short hero_pos)
{
	char fname[20];
	unsigned short fd;

	prepare_chr_name(fname, (char*)get_hero(hero_pos));

	sprintf((char*)ds_readd(TEXT_OUTPUT_BUF),
		(char*)ds_readd(STR_TEMP_XX_PTR2),		/* "TEMP\\%s" */
		fname);

	/* TODO: should be O_BINARY | O_WRONLY */
	fd = _creat((char*)ds_readd(TEXT_OUTPUT_BUF), 0);
	_write(fd, (Bit8u*)ds_readd(HEROES) + SIZEOF_HERO * hero_pos, SIZEOF_HERO);
	close(fd);
}

/**
 * \brief   copy the names from CHR file
 *
 * \param   ptr         pointer
 * \param   temple_id   > 0 the id of the temple, -1 on delete mode
 * \return              # of CHR-files in TEMP-dir
 */
signed short copy_chr_names(Bit8u *ptr, signed short temple_id)
{
#if defined(__BORLANDC__)
	signed short count = 0;
	signed short l_di;
	signed short handle;
	Bit8u *buf;
	struct ffblk blk;

	buf = (Bit8u*)ds_readd(RENDERBUF_PTR) + 60000;
	sprintf((char*)ds_readd(TEXT_OUTPUT_BUF),
		(char*)ds_readd(STR_TEMP_XX_PTR2),
		(char*)p_datseg + ALL_CHR_WILDCARD3);

	l_di = findfirst((char*)ds_readd(TEXT_OUTPUT_BUF), &blk, 0);

	if (!l_di) {

		do {
			/* create the CHR filename */
			sprintf((char*)ds_readd(TEXT_OUTPUT_BUF),
				(char*)ds_readd(STR_TEMP_XX_PTR2),
				((char*)(&blk)) + 30);

			/* read the CHR file from temp */
			handle = open((char*)ds_readd(TEXT_OUTPUT_BUF), O_BINARY | O_RDWR);
			_read(handle, buf, SIZEOF_HERO);
			close(handle);

			if ((host_readbs(buf + 0x88) == temple_id && !host_readbs(buf + 0x8a)) ||
				(!host_readbs(buf + 0x8a) && temple_id == -1))
			{
				strcpy((char*)ptr + 32 * count, (char*)buf);
				strcpy((char*)ptr + 32 * count + 16, (char*)buf + 16);
				count++;
			}

			l_di = findnext(&blk);

		} while (!l_di);

		return count;
	} else {
		return 0;
	}
#else
	return 0;
#endif
}

/**
 * \brief   loads a head icon from IN_HEADS.NVF
 *
 * \param   head        index of the desired head
 */
void load_in_head(signed short head)
{
	signed short handle;

	if (head >= 0) {

		handle = load_archive_file(ARCHIVE_FILE_IN_HEADS_NVF);

		seek_archive_file(handle, 1024L * head, 0);

		read_archive_file(handle, (char*)ds_readd(DTP2), 1024);

		close(handle);

		ds_writew(LOADED_HEAD_ID, head);
	}
}

/**
 * \brief   load a temple icon
 *
 * \param   no          the number of the icon
 */
void load_tempicon(signed short no)
{
	struct nvf_desc nvf;
	signed short handle;

	if (no == 14) {
		no = 7;
	}

	/* load TEMPICON */
	handle = load_archive_file(ARCHIVE_FILE_TEMPICON);
	read_archive_file(handle, (Bit8u*)ds_readd(BUFFER8_PTR), 7000);
	close(handle);

	nvf.dst = (Bit8u*)ds_readd(BUFFER8_PTR) + 7000;
	nvf.src = (Bit8u*)ds_readd(BUFFER8_PTR);
	nvf.no = no;
	nvf.type = 0;
	nvf.width = (Bit8u*)&handle;
	nvf.height = (Bit8u*)&handle;

	process_nvf(&nvf);
}

#if !defined(__BORLANDC__)
}
#endif
