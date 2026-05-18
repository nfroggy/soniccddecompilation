#include <SDL3/SDL.h>
#include <string.h>
#include "constants.h"
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
	SDL_Surface *surfaces[4];
} BitmapData;

static int numSpriteBitmaps;
static BitmapData *spriteBitmaps;
static SDL_Palette *sharedPalette;
static SDL_Surface *framebuffer;
static SDL_Surface *presentSurface;
static SDL_Texture *presentTexture;

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

static SDL_Surface *CreateIndexedSurface(Sint32 width, Sint32 height, Uint8 *pixels, SDL_FlipMode flip) {
	SDL_Surface *surface;
	surface = SDL_CreateSurface(width, height, SDL_PIXELFORMAT_INDEX8);
	if (!surface) {
		SDL_Log("Couldn't create sprite surface: %s", SDL_GetError());
		return NULL;
	}
	SDL_SetSurfacePalette(surface, sharedPalette);
	SDL_SetSurfaceColorKey(surface, true, 0);
	SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_NONE);
	SDL_SetSurfaceRLE(surface, true);
	SDL_LockSurface(surface);
	for (int y = 0; y < height; y++) {
		memcpy((Uint8 *)surface->pixels + (y * surface->pitch), pixels + (y * width), width);
	}
	SDL_UnlockSurface(surface);
	if (flip != SDL_FLIP_NONE) {
		SDL_FlipSurface(surface, flip);
	}
	return surface;
}

static void DestroySpriteBitmaps(void) {
	if (!spriteBitmaps) {
		return;
	}
	for (int i = 0; i < numSpriteBitmaps; i++) {
		for (int j = 0; j < SDL_arraysize(spriteBitmaps[i].surfaces); j++) {
			SDL_DestroySurface(spriteBitmaps[i].surfaces[j]);
		}
	}
	free(spriteBitmaps);
	spriteBitmaps = NULL;
	numSpriteBitmaps = 0;
}

Sint32 SetGrid(Sint32 base, Sint32 x, Sint32 y, Sint32 block, Sint32 frip) {
	return 0;
}

void EAsprset(Sint16 x, Sint16 y, Uint16 index, Uint16 linkdata, Uint16 reverse) {
	if (linkdata >= SDL_arraysize(sprites)) {
		return;
	}
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

int Graphics_Init(SDL_Window **window, SDL_Renderer **renderer) {
	if (!SDL_CreateWindowAndRenderer("Sonic CD", SCREEN_WIDTH * 3, SCREEN_HEIGHT * 3, 0, window, renderer)) {
		SDL_Log("Failed to create window and renderer: %s", SDL_GetError());
		return 0;
	}
	SDL_SetRenderLogicalPresentation(*renderer, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);
	SDL_SetRenderVSync(*renderer, 1);

	sharedPalette = SDL_CreatePalette(256);
	if (!sharedPalette) {
		SDL_Log("Couldn't create shared palette: %s", SDL_GetError());
		return 0;
	}

	framebuffer = SDL_CreateSurface(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_PIXELFORMAT_INDEX8);
	if (!framebuffer) {
		SDL_Log("Couldn't create framebuffer surface: %s", SDL_GetError());
		return 0;
	}
	SDL_SetSurfacePalette(framebuffer, sharedPalette);

	presentSurface = SDL_CreateSurface(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_PIXELFORMAT_ARGB8888);
	if (!presentSurface) {
		SDL_Log("Couldn't create presentation surface: %s", SDL_GetError());
		return 0;
	}
	SDL_SetSurfaceBlendMode(presentSurface, SDL_BLENDMODE_NONE);

	presentTexture = SDL_CreateTexture(*renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!presentTexture) {
		SDL_Log("Couldn't create presentation texture: %s", SDL_GetError());
		return;
	}
	SDL_SetTextureScaleMode(presentTexture, SDL_SCALEMODE_PIXELART);
	return 1;
}

int Graphics_LoadSprites(const char *path, bmp_info *spriteInfo, int spriteInfoCount) {
	AssetHeader ah;
	SpriteHeader sh;
	Uint8 *data = SZDD_Decompress(path);
	if (!data) {
		return 0;
	}
	LoadAssetHeader(data, &ah);
	DestroySpriteBitmaps();
	spriteBitmaps = malloc(ah.count * sizeof(BitmapData));
	if (spriteInfo) {
		memset(spriteInfo, 0, spriteInfoCount * sizeof(*spriteInfo));
	}
	Uint8 *spriteHeaders = data + ASSET_HEADER_ENCODED_LEN;
	Uint8 *spriteGraphics = data + ah.dataOffset;
	for (int i = 0; i < ah.count; i++) {
		Uint8 *pixels;
		LoadSpriteHeader(spriteHeaders, &sh);
		spriteBitmaps[i].width = sh.width;
		spriteBitmaps[i].height = sh.height;
		if (spriteInfo && i < spriteInfoCount) {
			spriteInfo[i].xs = (Uint8)sh.width;
			spriteInfo[i].ys = (Uint8)sh.height;
			spriteInfo[i].ofs = 0;
		}
		pixels = Convert4bpp(spriteGraphics, sh.width, sh.paddedWidth, sh.height, sh.palette);
		spriteBitmaps[i].surfaces[SDL_FLIP_NONE] = CreateIndexedSurface(sh.width, sh.height, pixels, SDL_FLIP_NONE);
		spriteBitmaps[i].surfaces[SDL_FLIP_HORIZONTAL] = CreateIndexedSurface(sh.width, sh.height, pixels, SDL_FLIP_HORIZONTAL);
		spriteBitmaps[i].surfaces[SDL_FLIP_VERTICAL] = CreateIndexedSurface(sh.width, sh.height, pixels, SDL_FLIP_VERTICAL);
		spriteBitmaps[i].surfaces[SDL_FLIP_HORIZONTAL_AND_VERTICAL] = CreateIndexedSurface(sh.width, sh.height, pixels, SDL_FLIP_HORIZONTAL_AND_VERTICAL);
		free(pixels);
		spriteHeaders += SPRITE_HEADER_ENCODED_LEN;
		spriteGraphics += ((sh.paddedWidth / 2) * sh.height);
	}
	free(data);
	numSpriteBitmaps = ah.count;
	return 0;
}

void Graphics_Shutdown(void) {
	DestroySpriteBitmaps();
	SDL_DestroySurface(framebuffer);
	framebuffer = NULL;
	SDL_DestroySurface(presentSurface);
	presentSurface = NULL;
	SDL_DestroyTexture(presentTexture);
	presentTexture = NULL;
	SDL_DestroyPalette(sharedPalette);
	sharedPalette = NULL;
}

static void UpdatePalette(void) {
	static PALETTEENTRY lastColorwk[64];
	static int paletteInitialized;
	SDL_Color colors[256];
	if (paletteInitialized && memcmp(colorwk, lastColorwk, sizeof(colorwk)) == 0) {
		return;
	}
	memcpy(lastColorwk, colorwk, sizeof(colorwk));
	for (int i = 0; i < SDL_arraysize(colors); i++) {
		colors[i].r = 0;
		colors[i].g = 0;
		colors[i].b = 0;
		colors[i].a = 255;
	}
	for (int i = 0; i < SDL_arraysize(colorwk); i++) {
		colors[i].r = colorwk[i].peRed;
		colors[i].g = colorwk[i].peGreen;
		colors[i].b = colorwk[i].peBlue;
		colors[i].a = 255;
	}
	SDL_SetPaletteColors(sharedPalette, colors, 0, SDL_arraysize(colors));
	paletteInitialized = 1;
}

static void DrawSprites(SDL_Surface *target, bool highPriority) {
	for (int i = MAX_SPRITES - 1; i >= 0; --i) {
		Sprite *sprite = &sprites[i];
		BitmapData *bitmap;
		SDL_Surface *surface;
		SDL_Rect rect;
		if (!sprite->index) {
			continue;
		}
		if ((!!(sprite->reverse & 0x8000)) != highPriority) {
			continue;
		}
		if (sprite->index >= numSpriteBitmaps) {
			continue;
		}
		bitmap = &spriteBitmaps[sprite->index];
		surface = bitmap->surfaces[sprite->reverse & 3];
		if (!surface) {
			continue;
		}
		rect.x = sprite->x - 128;
		rect.y = sprite->y - 128;
		rect.w = bitmap->width;
		rect.h = bitmap->height;
		SDL_BlitSurface(surface, NULL, target, &rect);
	}
}

void Graphics_Draw(SDL_Renderer *renderer) {
	UpdatePalette();
	SDL_FillSurfaceRect(framebuffer, NULL, 0);
	DrawSprites(framebuffer, false);
	DrawSprites(framebuffer, true);
	SDL_BlitSurface(framebuffer, NULL, presentSurface, NULL);
	SDL_UpdateTexture(presentTexture, NULL, presentSurface->pixels, presentSurface->pitch);
	SDL_RenderTexture(renderer, presentTexture, NULL, NULL);
	SDL_RenderPresent(renderer);
}
