#include <SDL3/SDL.h>
#include <string.h>
#include "graphics.h"
#include "szdd.h"

Uint16 mapwk[32768];
PALETTEENTRY colorwk[64];
PALETTEENTRY colorwk2[64];
PALETTEENTRY colorwk3[64];
PALETTEENTRY colorwk4[64];
int_union hscrollbuff[256];
Sint32 fade_flag;

#define ASSET_HEADER_ENCODED_LEN 16
typedef struct {
	char type[4];
	Sint32 endOffset;
	Sint32 count;
	Sint32 dataOffset;
} AssetHeader;

#define SPRITE_HEADER_ENCODED_LEN 12
typedef struct {
	Sint16 x;
	Sint16 y;
	Sint16 width;
	Sint16 paddedWidth;
	Sint16 height;
	Sint16 palette;
	Sint16 type;
} SpriteHeader;

typedef struct {
	Sint16 width;
	Sint16 height;
	Uint8 *data;
	SDL_Texture *texture;
} BitmapData;

static int numSpriteBitmaps;
static BitmapData *spriteBitmaps;

typedef struct {
	Sint16 x;
	Sint16 y;
	Uint16 index;
	Uint16 reverse;
} Sprite;

#define MAX_SPRITES 256
static Sprite sprites[MAX_SPRITES];

static Sint16 LoadSint16LE(Uint8 *data) {
	Sint16 out = data[0];
	out |= (data[1] << 8);
	return out;
}

static Sint32 LoadSint32LE(Uint8 *data) {
	Sint32 out = data[0];
	out |= (data[1] << 8);
	out |= (data[2] << 16);
	out |= (data[3] << 24);
	return out;
}

static void LoadAssetHeader(Uint8 *data, AssetHeader *out) {
	memcpy(out->type, data, sizeof(out->type)); data += sizeof(out->type);
	out->endOffset = LoadSint32LE(data); data += sizeof(Sint32);
	out->count = LoadSint32LE(data); data += sizeof(Sint32);
	out->dataOffset = LoadSint32LE(data);
}

static void LoadSpriteHeader(Uint8 *data, SpriteHeader *out) {
	out->x       = LoadSint16LE(data); data += sizeof(Sint16);
	out->y       = LoadSint16LE(data); data += sizeof(Sint16);
	out->width   = LoadSint16LE(data); data += sizeof(Sint16);
	out->paddedWidth = (out->width + 7) & ~7U;
	out->height  = LoadSint16LE(data); data += sizeof(Sint16);
	out->palette = LoadSint16LE(data) - 16; data += sizeof(Sint16);
	out->type    = LoadSint16LE(data); data += sizeof(Sint16);
}

static Uint8 *Convert4bpp(Uint8 *data, Sint32 width, Sint32 paddedWidth, Sint32 height, Sint32 palette) {
	Uint8 *out = malloc(width * height);
	int outCursor = 0;
	Uint8 pixel, pixels;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < paddedWidth; x++) {
			if ((x & 1) == 0) {
				pixels = *data++;
				pixel = pixels >> 4;
			}
			else {
				pixel = pixels & 0xf;
			}
			if (x < width) {
				out[outCursor++] = pixel ? pixel + palette : 0;
			}
		}
	}
	return out;
}

Sint32 SetGrid(Sint32 base, Sint32 x, Sint32 y, Sint32 block, Sint32 frip) {
	return 0;
}

void EAsprset(Sint16 x, Sint16 y, Uint16 index, Uint16 linkdata, Uint16 reverse) {
	sprites[linkdata].x = x;
	sprites[linkdata].y = y;
	sprites[linkdata].index = index;
	sprites[linkdata].reverse = reverse;
}

void ClrSpriteDebug(void) {
	memset(sprites, 0, sizeof(sprites));
}

void ChangeTileBmp(Sint32 tile_start, Sint32 bmp_no) {

}

int Graphics_LoadSprites(const char *path) {
	AssetHeader ah;
	SpriteHeader sh;
	Uint8 *data = SZDD_Decompress(path);
	if (!data) {
		return 0;
	}
	LoadAssetHeader(data, &ah);
	if (spriteBitmaps) {
		free(spriteBitmaps);
	}
	spriteBitmaps = malloc(ah.count * sizeof(BitmapData));
	Uint8 *spriteHeaders = data + ASSET_HEADER_ENCODED_LEN;
	Uint8 *spriteGraphics = data + ah.dataOffset;
	for (int i = 0; i < ah.count; i++) {
		LoadSpriteHeader(spriteHeaders, &sh);
		spriteBitmaps[i].width = sh.width;
		spriteBitmaps[i].height = sh.height;
		spriteBitmaps[i].data = Convert4bpp(spriteGraphics, sh.width, sh.paddedWidth, sh.height, sh.palette);
		spriteBitmaps[i].texture = NULL;
		spriteHeaders += SPRITE_HEADER_ENCODED_LEN;
		spriteGraphics += ((sh.paddedWidth / 2) * sh.height);
	}
	numSpriteBitmaps = ah.count;
	return 0;
}

static void UpdatePalettes(SDL_Renderer *renderer) {
	static PALETTEENTRY lastColorwk[64];
	if (memcmp(colorwk, lastColorwk, sizeof(colorwk)) == 0) {
		return;
	}
	memcpy(lastColorwk, colorwk, sizeof(colorwk));
	static int count = 0;
	SDL_Log("reload %d", count++);
	for (int i = 0; i < numSpriteBitmaps; i++) {
		BitmapData *sb = &spriteBitmaps[i];
		if (!sb->texture) {
			sb->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, sb->width, sb->height);
			SDL_SetTextureScaleMode(sb->texture, SDL_SCALEMODE_PIXELART);
		}
		Uint8 *indices = sb->data;
		Uint32 *pixels;
		int pitch;
		SDL_LockTexture(sb->texture, NULL, &pixels, &pitch);
		for (int y = 0; y < sb->height; y++) {
			for (int x = 0; x < sb->width; x++) {
				Uint8 index = *indices++;
				if (!index) {
					pixels[x] = 0;
				}
				else {
					PALETTEENTRY *color = &colorwk[index];
					pixels[x] = (0xff << 24) | (color->peRed << 16) | (color->peGreen << 8) | (color->peBlue);
				}
			}
			pixels += (pitch / sizeof(Uint32));
		}
		SDL_UnlockTexture(sb->texture);
	}
}

static void DrawSprites(SDL_Renderer *renderer, int priority) {
	for (int i = MAX_SPRITES - 1; i >= 0; --i) {
		Sprite *sprite = &sprites[i];
		BitmapData *bitmap;
		SDL_FRect rect;
		if (!sprite->index) {
			continue;
		}
		if (((sprite->reverse & 0x8000) != 0) != priority) {
			continue;
		}
		bitmap = &spriteBitmaps[sprite->index];
		if (!bitmap->texture) {
			continue;
		}
		rect.x = sprite->x - 128;
		rect.y = sprite->y - 128;
		rect.w = bitmap->width;
		rect.h = bitmap->height;
		SDL_RenderTextureRotated(renderer, bitmap->texture, NULL, &rect, 0, 0, (SDL_FlipMode)(sprite->reverse & 3));
	}
}

void Graphics_Draw(SDL_Renderer *renderer) {
	UpdatePalettes(renderer);
	PALETTEENTRY *bgColor = &colorwk[0];
	SDL_SetRenderDrawColor(renderer, bgColor->peRed, bgColor->peGreen, bgColor->peBlue, 0xff);
	SDL_RenderClear(renderer);
	DrawSprites(renderer, 0);
	DrawSprites(renderer, 1);
	SDL_RenderPresent(renderer);
}
