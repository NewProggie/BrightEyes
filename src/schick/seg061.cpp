/**
 *	Rewrite of DSA1 v3.02_de functions of seg061 (temple, chr-management, miracles)
 *	Functions rewritten: 8/8 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg061.cpp
 */
#include <stdio.h>
#include <string.h>

#if defined(__BORLANDC__)
#include <DOS.H>
#else
#include <unistd.h>
#endif

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg004.h"
#include "seg007.h"
#include "seg025.h"
#include "seg026.h"
#include "seg027.h"
#include "seg029.h"
#include "seg047.h"
#include "seg061.h"
#include "seg062.h"
#include "seg095.h"
#include "seg096.h"
#include "seg097.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

void do_temple(void)
{
	signed short l_si;
	signed short l_di;
	signed short input;
	signed char done = 0;
	Bit32s money;
	Bit32s donation;
	signed short game_state;

	ds_writew(INTEMPLE, ds_writew(INTEMPLE2, 0));
	ds_writew(REQUEST_REFRESH, 1);

	draw_loc_icons(9, MENU_ICON_HIRE_HERO, MENU_ICON_DISMISS_HERO, MENU_ICON_DELETE_HERO, MENU_ICON_LOAD_GAME, MENU_ICON_SAVE_GAME, MENU_ICON_QUIT_GAME, MENU_ICON_PRAY, MENU_ICON_SACRIFICE, MENU_ICON_LEAVE);

	while (!done) {

		if (ds_readws(REQUEST_REFRESH) != 0) {

			/* search which god owns this temple */
			ds_writew(TEMPLE_GOD, 1);
			for (l_si = 1; l_si < 15; l_si++) {
				if (is_in_byte_array((signed char)ds_readws(CURRENT_TYPEINDEX), (Bit8u*)ds_readd(GOD_TEMPLES_INDEX + 4 * l_si)))
				{
					ds_writew(TEMPLE_GOD, l_si);
					break;
				}
			}

			draw_main_screen();
			load_ani(3);
			load_tempicon(ds_readws(TEMPLE_GOD) - 1);
			init_ani(0);
			set_audio_track(ARCHIVE_FILE_TEMPLE_XMI);

			/* draw temple icon */
			ds_writew(PIC_COPY_X1, ds_writew(PIC_COPY_Y1, 0));
			ds_writew(PIC_COPY_X2, 40);
			ds_writew(PIC_COPY_Y2, 22);
			ds_writed(PIC_COPY_DST, (Bit32u)((Bit8u*)ds_readd(FRAMEBUF_PTR) + 28259));
			ds_writed(PIC_COPY_SRC, (Bit32u)((Bit8u*)ds_readd(BUFFER8_PTR) + 7000));

			update_mouse_cursor();
			do_pic_copy(0);
			refresh_screen_size();
			ds_writed(PIC_COPY_DST, ds_readd(FRAMEBUF_PTR));

			/* location string */
			sprintf((char*)ds_readd(DTP2),
				get_ttx(235),
				get_ttx(ds_readws(TEMPLE_GOD) + 21),	/* name of the god */
				(char*)(ds_readws(CURRENT_TYPEINDEX) != 58 ? get_ttx(ds_readbs(CURRENT_TOWN) + 235): get_ttx(622)));

			GUI_print_loc_line((char*)ds_readd(DTP2));

			ds_writew(REQUEST_REFRESH, 0);
		}

		handle_gui_input();

		/* input window */
		if (ds_readws(MOUSE2_EVENT) != 0 || ds_readws(ACTION) == ACTION_ID_PAGE_UP) {

			l_di = GUI_radio(get_ttx(225), 9,
						get_ttx(226),
						get_ttx(227),
						get_ttx(293),
						get_ttx(228),
						get_ttx(229),
						get_ttx(230),
						get_ttx(620),
						get_ttx(296),
						get_ttx(231)) - 1;

			if (l_di != -2) {
				ds_writew(ACTION, l_di + ACTION_ID_ICON_1);
			}
		}

		if (ds_readws(ACTION) == ACTION_ID_ICON_9) {
			/* leave temple */
			if (!ds_readbs(GROUP_MEMBER_COUNTS + ds_readbs(CURRENT_GROUP))) {
				GUI_output(get_ttx(232));
			} else {
				done = 1;
			}
		} else if (ds_readws(ACTION) == ACTION_ID_ICON_1) {
			/* add character */
			char_add(ds_readws(CURRENT_TYPEINDEX));
			draw_status_line();
		} else if (ds_readws(ACTION) == ACTION_ID_ICON_2) {
			/* let go character */
			char_letgo(ds_readws(CURRENT_TYPEINDEX));
			draw_status_line();
		} else if (ds_readws(ACTION) == ACTION_ID_ICON_3) {
			/* erase character */
			char_erase();
		} else if (ds_readws(ACTION) == ACTION_ID_ICON_4) {
			/* load game */
			if (ds_readws(CURRENT_TYPEINDEX) != 58) {

				do {
					game_state = load_game_state();
				} while (game_state == -1);

				/* location string */
				sprintf((char*)ds_readd(DTP2),
					get_ttx(235),
					get_ttx(ds_readws(TEMPLE_GOD) + 21),	/* name of the god */
					get_ttx(ds_readbs(CURRENT_TOWN) + 235));
				GUI_print_loc_line((char*)ds_readd(DTP2));

				draw_status_line();

				if (ds_readbs(CURRENT_LOCTYPE) != LOCTYPE_TEMPLE) {
					done = 1;
				}
			} else {
				GUI_output(get_ttx(817));
			}
		} else if (ds_readws(ACTION) == ACTION_ID_ICON_5) {
			/* save game */
			if (ds_readws(CURRENT_TYPEINDEX) != 58) {
				if (!ds_readbs(GROUP_MEMBER_COUNTS + ds_readbs(CURRENT_GROUP))) {
					GUI_output(get_ttx(232));
				} else {
					save_game_state();
				}
			} else {
				GUI_output(p_datseg + STR_NO_SAVE_IN_TEMPLE);
			}
		}

		if (ds_readws(ACTION) == ACTION_ID_ICON_6) {
			/* quit game */
			if (GUI_bool(get_ttx(299))) {

				done = 1;
				ds_writews(GAME_STATE, GAME_STATE_QUIT);
			}
		}

		if (ds_readws(ACTION) == ACTION_ID_ICON_7) {
			/* ask for a miracle */
			if (!ds_readbs(GROUP_MEMBER_COUNTS + ds_readbs(CURRENT_GROUP))) {
				GUI_output(get_ttx(232));
			} else {
				ask_miracle();
			}
		}

		if (ds_readws(ACTION) == ACTION_ID_ICON_8) {
			/* make a donation */
			if (!ds_readbs(GROUP_MEMBER_COUNTS + ds_readbs(CURRENT_GROUP))) {
				GUI_output(get_ttx(232));
			} else {

				money = get_party_money();

				if (!money) {
					GUI_output(get_ttx(401));
				} else {

					make_valuta_str((char*)ds_readd(TEXT_OUTPUT_BUF), money);

					sprintf((char*)ds_readd(DTP2),
						get_ttx(297),
						(char*)ds_readd(TEXT_OUTPUT_BUF));

					input = GUI_input((char*)ds_readd(DTP2), 3);

					donation = input;

					if (donation > 0) {

						if (10 * donation >= money) {
							/* donate all money */
							add_ds_ds(GODS_ESTIMATION + 4 * ds_readws(TEMPLE_GOD), money / 10);
							money = 0;
						} else {
							add_ds_ds(GODS_ESTIMATION + 4 * ds_readws(TEMPLE_GOD), donation);
							money -= 10 * donation;
						}

						set_party_money(money);
						GUI_output(get_ttx(298));
					}
				}
			}
		}
	}

	copy_palette();
	leave_location();

	ds_writew(INTEMPLE, ds_writew(INTEMPLE2, 1));
}

void char_add(signed short temple_id)
{
	signed short l_si;
	signed short l_di;
	signed short i;
	Bit8u *ptr;
	Bit8u *hero;

	ptr = (Bit8u*)ds_readd(RENDERBUF_PTR) + 50000;
	l_di = copy_chr_names(ptr, temple_id);

	if (ds_readbs(TOTAL_HERO_COUNTER) == 7 ||
		(ds_readbs(TOTAL_HERO_COUNTER) == 6 && !host_readbs(get_hero(6) + HERO_TYPE)))
	{
		GUI_output(get_ttx(288));
	} else {

		do {

			if (!l_di) {
				GUI_output(get_ttx(290));
				l_si = -1;
			} else {

				l_si = menu_enter_delete(ptr, l_di, 1);

				if (l_si != -1) {

					hero = get_hero(0);

					for (i = 0; i < 6; i++, hero += SIZEOF_HERO) {

						if (!host_readbs(hero + HERO_TYPE)) {

							prepare_chr_name((char*)ds_readd(DTP2),	(char*)(ptr + 32 * l_si));

							if (read_chr_temp((char*)ds_readd(DTP2), i, ds_readbs(CURRENT_GROUP))) {
								inc_ds_bs_post(TOTAL_HERO_COUNTER);
								inc_ds_bs_post(GROUP_MEMBER_COUNTS + ds_readbs(CURRENT_GROUP));
								host_writebs(hero + HERO_GROUP_POS, i + 1);
								write_chr_temp(i);
							}
							break;
						}
					}

					draw_main_screen();
					init_ani(2);

					/* location string */
					sprintf((char*)ds_readd(DTP2),
						get_ttx(235),
						get_ttx(ds_readws(TEMPLE_GOD) + 21),	/* name of the god */
						get_ttx(ds_readbs(CURRENT_TOWN) + 235));

					GUI_print_loc_line((char*)ds_readd(DTP2));
				}

				l_di = copy_chr_names(ptr, temple_id);
			}
		} while (l_si != -1 && ds_readbs(TOTAL_HERO_COUNTER) < (host_readbs(get_hero(6) + HERO_TYPE) ? 7 : 6));
	}
}

void char_letgo(signed short temple_id)
{
	signed short hero_pos;
	Bit8u *hero;

	if (!ds_readbs(TOTAL_HERO_COUNTER) || !ds_readbs(GROUP_MEMBER_COUNTS + ds_readbs(CURRENT_GROUP))) {
		GUI_output(get_ttx(232));
	} else {

		do {

			hero_pos = select_hero_from_group(get_ttx(618));

			if (hero_pos != -1) {

				if (hero_pos == 6) {
					/* let go an NPC */
					ds_writew(NPC_MONTHS, 99);
					npc_farewell();
				} else {
					/* let go a hero */
					hero = get_hero(hero_pos);
					dec_ds_bs_post(TOTAL_HERO_COUNTER);
					dec_ds_bs_post(GROUP_MEMBER_COUNTS + ds_readbs(CURRENT_GROUP));

					host_writeb(hero + HERO_TEMPLE_ID, (signed char)temple_id);
					host_writeb(hero + HERO_GROUP_POS, 0);

					write_chr_temp(hero_pos);

					memset(hero, 0, SIZEOF_HERO);

					draw_main_screen();
					init_ani(2);

					/* location string */
					sprintf((char*)ds_readd(DTP2),
						get_ttx(235),
						get_ttx(ds_readws(TEMPLE_GOD) + 21),	/* name of the god */
						get_ttx(ds_readbs(CURRENT_TOWN) + 235));
					GUI_print_loc_line((char*)ds_readd(DTP2));
				}
			}

		} while (hero_pos != -1 && ds_readbs(GROUP_MEMBER_COUNTS + ds_readbs(CURRENT_GROUP)) > (host_readbs(get_hero(6) + HERO_TYPE) ? 1 : 0));
	}
}

signed short char_erase(void)
{
	signed short l_si;
	signed short l_di;
	signed short unlink_ret;
	Bit8u *ptr;

	if (ds_readbs(RENDERBUF_IN_USE_FLAG)) {
		ptr = (HugePt)ds_readd(BUFFER9_PTR) + 30000L;
	} else {
		ptr = (Bit8u*)ds_readd(RENDERBUF_PTR) + 50000;
	}

	l_di = copy_chr_names(ptr, -1);

	do {
		if (!l_di) {
			l_si = -1;
		} else {
			l_si = menu_enter_delete(ptr, l_di, -1);

			if (l_si != -1) {

				strcpy((char*)ds_readd(DTP2), (char*)ptr + 32 * l_si);

				sprintf((char*)ds_readd(TEXT_OUTPUT_BUF), get_ttx(295),	(char*)ds_readd(DTP2));

				if (GUI_bool((char*)ds_readd(TEXT_OUTPUT_BUF))) {

					prepare_chr_name((char*)ds_readd(TEXT_OUTPUT_BUF), (char*)ds_readd(DTP2));

					unlink_ret = unlink((char*)ds_readd(TEXT_OUTPUT_BUF));

					if (unlink_ret != 0) {
						GUI_output(get_ttx(294));
						return 0;
					}

					sprintf((char*)ds_readd(DTP2), (char*)p_datseg + STR_TEMP_FILE_WILDCARD, (char*)ds_readd(TEXT_OUTPUT_BUF));

					unlink((char*)ds_readd(DTP2));
				}

				l_di = copy_chr_names(ptr, -1);

			} else {
				return 0;
			}
		}

	} while (l_si != -1);

	return 1;
}

/**
 * \brief   MIRACLE heal one hero
 *
 * \param   le_in       healable LE maximum
 * \param   str         a format-string for the output
 */
void miracle_heal_hero(signed short le_in, Bit8u *str)
{
	signed short i;
	signed short le;
	signed short hero_pos;
	signed short le_diff;
	Bit8u *hero;

	le = 0;
	hero_pos = -1;

	/* search for the hero with the largest LE-difference */
	for (i = 0; i <= 6; i++) {
		hero = get_hero(i);

		if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
			host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
			!hero_dead(hero) &&
			!hero_gods_pissed(hero) &&
			!hero_dead(hero) &&
			((le_diff = host_readws(hero + HERO_LE_ORIG) - host_readws(hero + HERO_LE)) > le))
		{
			le = le_diff;
			hero_pos = i;
		}
	}

	if (hero_pos != -1) {

		/* adjust le_in if the maximum is not reached */
		if (le_in > le) {
			le_in = le;
		}

		add_hero_le(get_hero(hero_pos), le_in);

		/* prepare a message */
		strcpy((char*)ds_readd(TEXT_OUTPUT_BUF), get_ttx(392));

		if (le_in > 1) {
			strcat((char*)ds_readd(TEXT_OUTPUT_BUF), get_ttx(393));
		}

		sprintf((char*)ds_readd(DTP2), (char*)str, (char*)get_hero(hero_pos) + HERO_NAME2, le_in, (char*)ds_readd(TEXT_OUTPUT_BUF));
	}
}

void miracle_resurrect(Bit8u *str)
{
	signed short i;

	for (i = 0; i <= 6; i++) {

		Bit8u *hero = get_hero(i);

		if (hero_dead(hero) && host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) && !hero_gods_pissed(hero))
		{
			/* resurrect from the dead */
			and_ptr_bs(hero + HERO_FLAGS1, 0xfe); /* unset 'dead' flag */

			/* add 7 LE */
			add_hero_le(hero, 7);

			/* update_ the status line */
			draw_status_line();

			/* prepare a message */
			sprintf((char*)ds_readd(DTP2), (char*)str, (char*)hero + HERO_NAME2);

			break;
		}
	}
}

/**
 * \brief   MIRACLE modify all living group members
 *
 * \param   offset      offset in the datasheet
 * \param   timer_value how long should the modification last
 * \param   mod         modification value
 */
void miracle_modify(unsigned short offset, Bit32s timer_value, signed short mod)
{
	int i;
	int slot;
	HugePt ptr;
	Bit8u *hero = (Bit8u*)ds_readd(HEROES);

	for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

		if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
			host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
			!hero_dead(hero) &&
			!hero_gods_pissed(hero))
		{
			slot = get_free_mod_slot();
			ptr = hero;
			ptr += offset;

			set_mod_slot(slot, timer_value, (Bit8u*)ptr, (signed char)mod, (signed char)i);
		}
	}
}

/**
 * \brief   MIRACLE repair or magicise weapon
 *
 * \param   str         format string for output
 * \param   mode        0 = magic, != 0 repair
 */
void miracle_weapon(Bit8u *str, signed short mode)
{
	int i;
	int j;
	int done;
	int item_id;

	for (j = done = 0; (j <= 6) && (!done); j++) {

		Bit8u *hero = get_hero(j);

		if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
			host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
			!hero_dead(hero) &&
			!hero_gods_pissed(hero))
		{
			for (i = 0; i < NR_HERO_INVENTORY_SLOTS; i++)
			{

				if ((item_id = host_readws(hero + HERO_INVENTORY + INVENTORY_ITEM_ID + SIZEOF_INVENTORY * i)) &&
					item_weapon(get_itemsdat(item_id)))
				{

					if (mode == 0) {
						/* if weapon is neither broken nor magic magic, make it magic and magic_revealed */

						if (!inventory_broken(hero + HERO_INVENTORY + SIZEOF_INVENTORY * i) &&
							!inventory_magic(hero + HERO_INVENTORY + SIZEOF_INVENTORY * i))
						{
							or_ptr_bs(hero + HERO_INVENTORY + INVENTORY_FLAGS + SIZEOF_INVENTORY * i, 0x08); /* set 'magic' flag */
							or_ptr_bs(hero + HERO_INVENTORY + INVENTORY_FLAGS + SIZEOF_INVENTORY * i, 0x80); /* set 'magic_revealed' flag */

							sprintf((char*)ds_readd(DTP2), (char*)str,
								(char*)GUI_names_grammar((signed short)0x8000, item_id, 0),
								(char*)hero + HERO_NAME2);

							done = 1;
							break;
						}
					} else {
						/* repair a broken weapon */
						if (inventory_broken(hero + HERO_INVENTORY + SIZEOF_INVENTORY * i))
						{
							and_ptr_bs(hero + HERO_INVENTORY + INVENTORY_FLAGS + SIZEOF_INVENTORY * i, 0xfe); /* unset 'broken' flag */

							sprintf((char*)ds_readd(DTP2), (char*)str,
								(char*)GUI_names_grammar((signed short)0x8000, item_id, 0),
								(char*)hero + HERO_NAME2);

							done = 1;
							break;
						}
					}
				}
			}
		}
	}
}

#if !defined(__BORLANDC__)
}
#endif
