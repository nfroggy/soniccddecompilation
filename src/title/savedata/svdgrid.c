#include "../../types.h"
#include "../common/hmx_types.h"
#include "../common/grid.h"

extern void (*hmx_grid_set_view_module)(hmx_grid *, Sint32, Sint32, Sint32,
                                        Sint32);
extern draw_context *s_ctx;
extern grid_bmp infoGridBmp[];
extern void (*hmx_grid_set_position_module)(hmx_grid *, Sint32, Sint32);
extern void (*hmx_grid_set_tile_module)(hmx_grid *, Sint32, Sint32,
                                        hmx_bitmap *, Sint32);
extern hmx_grid *(*hmx_grid_create_module)(hmx_environment *, Sint32, Sint32,
                                           Sint32, Sint32);
extern hmx_environment *g_env_module;
#ifdef SAVEDATA
#define IDX_GDBMP_BACK (*(volatile Uint16 *)0x01009160)
#else
extern Uint16 IDX_GDBMP_BACK;
#endif
extern void (*hmx_grid_release_module)(hmx_environment *, hmx_grid *);

Sint32 OEGridCreate(Uint16 indx) {
    tile_location loc;
    grid_rect rectwh;
    Uint16 x;
    Uint16 y;

    if (indx == IDX_GDBMP_BACK) {

        s_ctx->grids[indx] = hmx_grid_create_module(g_env_module, 8, 8, 40, 28);
        *infoGridBmp[indx].lphGrid = (Uint32)s_ctx->grids[indx];

        for (y = 0; y < 28; ++y) {
            loc.nTile = y;
            for (x = 0; x < 40; ++x) {
                loc.mTile = x;

                hmx_grid_set_tile_module(
                    s_ctx->grids[indx], x, y,
                    (hmx_bitmap *)*infoGridBmp[indx].lphBmp, 0);
            }
        }

        hmx_grid_set_position_module(s_ctx->grids[indx],
                                     infoGridBmp[indx].point.x,
                                     infoGridBmp[indx].point.y);

        rectwh.x = 0;
        rectwh.y = 0;
        rectwh.dx = infoGridBmp[indx].w * 40;
        rectwh.dy = infoGridBmp[indx].h * 28;

        hmx_grid_set_view_module(s_ctx->grids[indx], rectwh.x, rectwh.y,
                                 rectwh.dx, rectwh.dy);
    }
    return 0;
}

void OEGridDelete(Uint16 indx) {
    if (*infoGridBmp[indx].lphGrid != 0) {

        hmx_grid_release_module(g_env_module, s_ctx->grids[indx]);
        s_ctx->grids[indx] = 0;
        *infoGridBmp[indx].lphGrid = 0;
    }

    if (indx == IDX_GDBMP_BACK) {
        if (*infoGridBmp[indx].lphTile != 0) {

            *infoGridBmp[indx].lphTile = 0;
        }
    }
}

void OEAllGridDelete(void) { OEGridDelete(IDX_GDBMP_BACK); }
