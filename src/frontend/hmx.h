#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include <SDL3/SDL.h>
#include "../types.h"
#include "../title/common/hmx_types.h"

typedef void (*HmxUpdatePaletteFn)(void);
typedef void (*HmxPresentFrameFn)(SDL_Renderer *renderer);

void Hmx_Init(SDL_Surface *framebuffer, SDL_Renderer *renderer, HmxUpdatePaletteFn updatePalette, HmxPresentFrameFn presentFrame);
void Hmx_Shutdown(void);
void Hmx_SetActive(int active);
int Hmx_IsActive(void);

void *hmx_get_environment(void);
void *hmx_get_loader_environment(void);
draw_context *get_draw_context(void);
void FlipToScreen(void);
void ld_bitmap_4to8(void *dst, void *src, Sint32 wx, Sint32 wy, Sint32 palet, Sint32 zero_chg, Sint32 flag4);
void *ld_load_cmpfile(hmx_environment *env, char *path);
void hmx_background_set_background(hmx_background *background, Sint32 color);
hmx_bitmap *hmx_bitmap_create(hmx_environment *env, Sint32 width, Sint32 height);
void hmx_bitmap_release(hmx_environment *env, hmx_bitmap *bitmap);
void *hmx_bitmap_get_scan0(hmx_bitmap *bitmap);
void hmx_bitmap_set_transparency(hmx_bitmap *bitmap, Sint32 color);
void hmx_free(hmx_environment *env, void *ptr);
hmx_grid *hmx_grid_create(hmx_environment *env, Sint32 tileWidth, Sint32 tileHeight, Sint32 width, Sint32 height);
void hmx_grid_release(hmx_environment *env, hmx_grid *grid);
hmx_renderer_base *hmx_grid_base(hmx_grid *grid);
void hmx_grid_set_position(hmx_grid *grid, Sint32 x, Sint32 y);
void hmx_grid_set_view(hmx_grid *grid, Sint32 x, Sint32 y, Sint32 width, Sint32 height);
void hmx_grid_set_tile(hmx_grid *grid, Sint32 x, Sint32 y, hmx_bitmap *bitmap, Sint32 flags);
void hmx_grid_set_horz_offsets(hmx_grid *grid, Sint32 *offsets);
void hmx_renderer_context_add(hmx_renderer_context *context, Sint32 priority, hmx_renderer_base *base);
void hmx_renderer_context_clear(hmx_renderer_context *context);
void hmx_renderer_context_draw(hmx_renderer_context *context, hmx_surface *surface);
hmx_renderer_base *hmx_sprite_base(hmx_sprite *sprite);
void hmx_sprite_set_position(hmx_sprite *sprite, Sint32 x, Sint32 y);
void hmx_sprite_set_bitmap(hmx_sprite *sprite, hmx_bitmap *bitmap);
void hmx_sprite_set_flags(hmx_sprite *sprite, Sint32 flags);
hmx_ddagrid *hmx_ddagrid_create(hmx_environment *env, Sint32 tileWidth, Sint32 tileHeight, Sint32 columns, Sint32 rows, Sint32 width, Sint32 height);
void hmx_ddagrid_release(hmx_environment *env, hmx_ddagrid *grid);
void hmx_ddagrid_set_scan(hmx_ddagrid *grid, Sint32 line, Sint32 startX, Sint32 deltaX, Sint32 startY, Sint32 deltaY);
void hmx_ddagrid_set_tile(hmx_ddagrid *grid, Sint32 x, Sint32 y, hmx_bitmap *bitmap, Sint32 flags);

#ifdef __cplusplus
}
#endif
