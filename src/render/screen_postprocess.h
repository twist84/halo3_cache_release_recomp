#ifndef __SCREEN_POSTPROCESS_H__
#define __SCREEN_POSTPROCESS_H__
#pragma once

/* ---------- headers */

/* ---------- constants */

/* ---------- definitions */

class c_screen_postprocess
{
public:
    struct s_settings
    {
        bool m_postprocess; // 0x0
        rex::be<long> m_display_alpha; // 0x4
        rex::be<long> m_accum; // 0x8
        rex::be<long> m_bloom_source; // 0xC
        rex::be<long> m_accum_filter; // 0x10
        rex::be<long> m_bloom; // 0x14
        rex::be<long> m_bling; // 0x18
        bool m_persist; // 0x1C
        rex::be<long> m_downsample; // 0x20
        rex::be<float> __unknown24; // 0x24
        rex::be<float> m_postprocess_exposure; // 0x28
        bool m_allow_debug_display; // 0x2C
        bool m_tone_curve; // 0x2D
        rex::be<float> m_tone_curve_white_point; // 0x30
        bool m_auto_exposure_lock; // 0x34
        rex::be<long> m_debug_mode; // 0x38
    };
    static_assert(sizeof(s_settings) == 0x3C);
};
static_assert(sizeof(c_screen_postprocess) == 1);

/* ---------- prototypes */

/* ---------- globals */

/* ---------- public code */

/* ---------- private code */

#endif // __SCREEN_POSTPROCESS_H__
