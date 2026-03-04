#ifndef __FONT_LIST_H__
#define __FONT_LIST_H__
#pragma once

/* ---------- headers */

/* ---------- constants */

enum e_font_id
{
    _font_id_fallback = -2,
    _invalid_font = -1,

    _terminal_font = 0,
    _body_text_font,
    _title_font,
    _super_large_font,
    _large_body_text_font,
    _split_screen_hud_message_font,
    _full_screen_hud_message_font,
    _english_body_text_font,
    _hud_number_font,
    _subtitle_font,
    _main_menu_font,

    k_public_font_id_count,
};

/* ---------- definitions */

/* ---------- prototypes */

/* ---------- globals */

/* ---------- public code */

/* ---------- private code */

#endif // __FONT_LIST_H__
