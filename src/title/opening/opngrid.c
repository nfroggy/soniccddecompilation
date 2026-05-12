#include "../../types.h"
#include "../common/hmx_types.h"
#include "../common/grid.h"

extern void (*hmx_ddagrid_set_tile_module)(hmx_ddagrid *, Sint32, Sint32,
                                           hmx_bitmap *, Sint32);
extern grid_bmp infoGridBmp[];
extern draw_context *s_ctx;
extern hmx_ddagrid *(*hmx_ddagrid_create_module)(hmx_environment *, Sint32,
                                                 Sint32, Sint32, Sint32, Sint32,
                                                 Sint32);
extern hmx_environment *g_env_module;
extern void (*hmx_grid_set_view_module)(hmx_grid *, Sint32, Sint32, Sint32,
                                        Sint32);
extern void (*hmx_grid_set_tile_module)(hmx_grid *, Sint32, Sint32,
                                        hmx_bitmap *, Sint32);
extern hmx_grid *(*hmx_grid_create_module)(hmx_environment *, Sint32, Sint32,
                                           Sint32, Sint32);
extern void (*hmx_grid_set_position_module)(hmx_grid *, Sint32, Sint32);
extern void (*hmx_grid_release_module)(hmx_environment *, hmx_grid *);

Sint32 OEGridCreate(Uint16 indx) {
    tile_location loc;
    grid_rect rectwh;
    Uint16 x;
    Uint16 y;

    if (indx == 0) {

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
    } else if (indx == 1) {
        Sint32 tileh[320];
        Sint32 i;
        s_ctx->grids[indx] = hmx_grid_create_module(g_env_module, 8, 8, 80, 8);
        *infoGridBmp[indx].lphGrid = (Uint32)s_ctx->grids[indx];

        for (y = 0, i = 0; y < 8; y++) {
            for (x = 0; x < 40; ++x) {

                tileh[i++] = x + y * 40;
            }
        }
        for (y = 0; y < 8; ++y) {
            loc.nTile = y;
            for (x = 0; x < 80; ++x) {
                loc.mTile = x;
                hmx_grid_set_tile_module(
                    s_ctx->grids[indx], x, y,
                    (hmx_bitmap *)infoGridBmp[indx]
                        .lphBmp[tileh[x % 40 + (7 - y) * 40]],
                    0);
            }
        }

        rectwh.x = 0;
        rectwh.y = 0;
        rectwh.dx = infoGridBmp[indx].w * 41;
        rectwh.dy = infoGridBmp[indx].h * 8;

        hmx_grid_set_view_module(s_ctx->grids[indx], rectwh.x,
                                 infoGridBmp[indx].point.y, rectwh.dx,
                                 rectwh.dy);
    } else if (indx == 2) {
        Sint32 tileh[512];
        Sint32 i;

        s_ctx->grids[indx] = (hmx_grid *)hmx_ddagrid_create_module(
            g_env_module, 3, 3, 6, 4, 320, 92);
        *infoGridBmp[indx].lphGrid = (Uint32)s_ctx->grids[indx];
        for (y = 0, i = 0; y < 16; ++y) {
            for (x = 0; x < 32; ++x) {

                tileh[i++] = x + y * 32;
            }
        }
        for (y = 0; y < 16; ++y) {
            for (x = 0; x < 32; ++x) {

                hmx_ddagrid_set_tile_module(
                    (hmx_ddagrid *)s_ctx->grids[indx], x, y,
                    (hmx_bitmap *)infoGridBmp[indx]
                        .lphBmp[tileh[x + (15 - y) * 32]],
                    0);
            }
            for (; x < 40; ++x) {

                hmx_ddagrid_set_tile_module(
                    (hmx_ddagrid *)s_ctx->grids[indx], x, y,
                    (hmx_bitmap *)infoGridBmp[indx]
                        .lphBmp[tileh[x + (15 - y) * 32 - 32]],
                    0);
            }
        }
    }

    return 0;
}

void OEGridDelete(Uint16 indx) {
    Uint16 x, y;

    if (*infoGridBmp[indx].lphGrid != 0) {
        if (indx != 1) {

            (Uint32)(indx ^ 2) < 1;
        }

        hmx_grid_release_module(g_env_module, s_ctx->grids[indx]);
        *infoGridBmp[indx].lphGrid = 0;
        s_ctx->grids[indx] = 0;
    }

    if (indx == 0) {
        if (*infoGridBmp[indx].lphTile != 0) {

            *infoGridBmp[indx].lphTile = 0;
        }
    } else if (indx == 1) {
        for (y = 0; y < 8; ++y) {
            for (x = 0; x < 40; ++x) {
                if (infoGridBmp[indx].lphTile[x + y * 40] != 0) {

                    infoGridBmp[indx].lphTile[x + y * 40] = 0;
                }
            }
        }
    } else if (indx == 2) {
        for (y = 0; y < 16; ++y) {
            for (x = 0; x < 32; ++x) {
                if (infoGridBmp[indx].lphTile[x + y * 32] != 0) {

                    infoGridBmp[indx].lphTile[x + y * 32] = 0;
                }
            }
        }
    }
}

void OEAllGridDelete(void) {

    OEGridDelete(0);
    OEGridDelete(1);
    OEGridDelete(2);
}
