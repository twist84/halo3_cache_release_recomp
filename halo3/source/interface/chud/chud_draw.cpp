/* ---------- headers */

#include "interface/chud/chud_definitions.h"
#include "interface/chud/chud_draw.h"
#include "text/font_list.h"

#include "rex_macros.h"

/* ---------- constants */

/* ---------- definitions */

/* ---------- prototypes */

static e_font_id convert_font_type(short font_type);

/* ---------- globals */

/* ---------- private variables */

/* ---------- ppc */

// exports

// hooks

// midasm_hooks

void midasm_hook__chud_text_widget_compute_geometry(PPCRegister& r5, PPCRegister& r11)
{
    // inline call to `convert_font_type`
    r5.s64 = convert_font_type(r11.u32);
}

/* ---------- public code */

/* ---------- private code */

static e_font_id convert_font_type(short font_type)
{
    e_font_id result;
    switch (font_type)
    {
    case _chud_widget_text_font_full_screen_hud_message:
        result = _full_screen_hud_message_font;
        break;

    case _chud_widget_text_font_split_screen_hud_message:
        result = _split_screen_hud_message_font;
        break;

    case _chud_widget_text_font_terminal:
        result = _terminal_font;
        break;

    case _chud_widget_text_font_body_text:
        result = _body_text_font;
        break;

    case _chud_widget_text_font_title:
        result = _title_font;
        break;

    case _chud_widget_text_font_super_large:
        result = _super_large_font;
        break;

    case _chud_widget_text_font_large_body_text:
        result = _large_body_text_font;
        break;

    case _chud_widget_text_font_english_body_text:
        result = _english_body_text_font;
        break;

    case _chud_widget_text_font_hud_number:
        result = _hud_number_font;
        break;

    case _chud_widget_text_font_subtitle:
        result = _subtitle_font;
        break;

    case _chud_widget_text_font_main_menu:
        result = _main_menu_font;
        break;

    default:
        throw "unreachable";
        result = _invalid_font;
        break;
    }
    return result;
}
