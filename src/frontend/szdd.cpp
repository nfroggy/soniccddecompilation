#include <SDL3/SDL.h>
#include <cstdlib>
#include <cstring>
#include <vector>
#include "file.h"
#include "szdd.hpp"

// LZSS impl by Haruhiko Okumura

#define N		 4096	/* size of ring buffer */
#define F		   16	/* upper limit for match_length */
#define THRESHOLD	2   /* encode string into position and length */

static void unlzss(FILE *infile, std::vector<Uint8> &outVec) {
	int  i, j, k, r, c;
	unsigned int  flags;
	static Uint8 text_buf[N + F - 1];

	for (i = 0; i < N - F; i++) text_buf[i] = ' ';
	r = N - F;  flags = 0;
	for (; ; ) {
		if (((flags >>= 1) & 256) == 0) {
			if ((c = getc(infile)) == EOF) break;
			flags = c | 0xff00;		/* uses higher byte cleverly */
		}							/* to count eight */
		if (flags & 1) {
			if ((c = getc(infile)) == EOF) break;
			outVec.push_back(static_cast<Uint8>(c));
			text_buf[r++] = c;
			r &= (N - 1);
		}
		else {
			if ((i = getc(infile)) == EOF) break;
			if ((j = getc(infile)) == EOF) break;
			i |= ((j & 0xf0) << 4);  j = (j & 0x0f) + THRESHOLD;
			for (k = 0; k <= j; k++) {
				c = text_buf[(i + k) & (N - 1)];
				outVec.push_back(static_cast<Uint8>(c));
				text_buf[r++] = c;
				r &= (N - 1);
			}
		}
	}
}

const Uint8 szddSig[] = {
	0x53, 0x5A, 0x44, 0x44, 0x88, 0xF0, 0x27, 0x33, 0x41,
};

std::vector<Uint8> SZDD_Decompress(const char *path) {
	std::vector<Uint8> out;
	FILE *fp = File_Open(path, "rb");
	if (!fp) {
		SDL_Log("couldn't open %s", path);
		return out;
	}

	Uint8 fileSig[sizeof(szddSig)];
	fread(fileSig, 1, sizeof(fileSig), fp);
	if (memcmp(szddSig, fileSig, sizeof(szddSig)) != 0) {
		SDL_Log("Invalid file %s", path);
		return out;
	}
	fgetc(fp); // skip past last character of filename

	Uint32 uncompressedSize = 0;
	uncompressedSize |= fgetc(fp);
	uncompressedSize |= (fgetc(fp) << 8);
	uncompressedSize |= (fgetc(fp) << 16);
	uncompressedSize |= (fgetc(fp) << 24);
	out.reserve(uncompressedSize);
	unlzss(fp, out);
	return out;
}
