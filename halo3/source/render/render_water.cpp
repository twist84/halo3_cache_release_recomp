/* ---------- headers */

#include "render/render_water.h"

#include "rex_macros.h"

/* ---------- constants */

/* ---------- definitions */

/* ---------- prototypes */

static void c_water_renderer_render_shading(void);

/* ---------- globals */

REXCVAR_DEFINE_BOOL(render_water_wireframe_enabled, false, "Blam/Render/Water", "");
REX_DATA_REFERENCE_DECLARE(0x828818B4, bool, render_water_wireframe_enabled);

/* ---------- private variables */

/* ---------- ppc */

// exports

REX_PPC_EXTERN_IMPORT(c_water_renderer_render_shading);

// hooks

REX_PPC_HOOK(c_water_renderer_render_shading);

/* ---------- public code */

void c_water_renderer::render_shading(void)
{
	c_water_renderer_render_shading();
}

/* ---------- private code */

static void c_water_renderer_render_shading(void)
{
	render_water_wireframe_enabled = REXCVAR_GET(render_water_wireframe_enabled);

	REX_PPC_INVOKE(c_water_renderer_render_shading);
}
