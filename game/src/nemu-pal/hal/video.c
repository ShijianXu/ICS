#include "hal.h"
#include "device/video.h"
#include "device/palette.h"

#include <string.h>
#include <stdlib.h>


#define MAKE_CLIP(line, row, line_limit, row_limit, r, l, t, d) \
line_limit = (line_limit > d)?d:line_limit; line = (line < t)?t:line;\
row_limit = (row_limit > r)?r:row_limit; row = (row < l)?l:row;

#define MAKE_CLIP_SD(srcline, srcrow, srclinelimit, srcrowlimit,dstline, dstrow, dstlinelimit, dstrowlimit, r, l, t, d) \
if (dstlinelimit > d) {\
		srclinelimit -= dstlinelimit - d;\
		dstlinelimit = d;\
}\
if (dstline < t) {\
		srcline += t - dstline;\
		dstline = t;\
}\
if (dstrowlimit > r) {\
		srcrowlimit -= dstrowlimit - r;\
		dstrowlimit = r;\
}\
if (dstrow < l) {\
		srcrow += l - dstrow;\
		dstrow = l;\
}

int get_fps();

void SDL_BlitSurface(SDL_Surface *src, SDL_Rect *scrrect, 
		SDL_Surface *dst, SDL_Rect *dstrect) {
	assert(dst && src);

	/* TODO: Performs a fast blit from the source surface to the 
	 * destination surface. Only the position is used in the
	 * ``dstrect'' (the width and height are ignored). If either
	 * ``srcrect'' or ``dstrect'' are NULL, the entire surface 
	 * (``src'' or ``dst'') is copied. The final blit rectangle 
	 * is saved in ``dstrect'' after all clipping is performed
	 * (``srcrect'' is not modified).
	 */

	//assert(0);
	int src_x = (scrrect == NULL || dstrect == NULL)?0:scrrect->x;
	int src_y = (scrrect == NULL || dstrect == NULL)?0:scrrect->y;
	int src_w = (scrrect == NULL || dstrect == NULL)?src->w:scrrect->w;
	int src_h = (scrrect == NULL || dstrect == NULL)?src->h:scrrect->h;

	int dst_x = (scrrect && dstrect)?dstrect->x:0;
	int dst_y = (scrrect && dstrect)?dstrect->y:0;

	int pitch = src->pitch;

	int src_line = src_y, src_row = src_x;
	int dst_line = dst_y, dst_row = dst_x;

    int src_line_limit = src_y + src_h - 1;
    int src_row_limit = src_x + src_w - 1;

    int dst_line_limit = dst_y + src_h - 1;
	int dst_row_limit = dst_x + src_w - 1;

    if (dstrect != NULL) {
		dstrect->x = dst_row;
		dstrect->y = dst_line;
		dstrect->w = dst_row_limit - dst_row + 1;
		dstrect->h = dst_line_limit - dst_line + 1;
	}

	//SDL_PixelFormat *src_fm = src->format;

	int temp_src_row = src_row;
	int temp_dst_row = dst_row;

    for (; src_line <= src_line_limit; ++src_line, ++dst_line) {
				uint8_t* src_pixel = (uint8_t*)&(src->pixels[src_line * pitch + temp_src_row]);
				uint8_t* dst_pixel = (uint8_t*)&(dst->pixels[dst_line * pitch + temp_dst_row]);
		for (src_row = temp_src_row; src_row <= src_row_limit; src_row++, dst_pixel++, src_pixel++) {
				*dst_pixel = *src_pixel;
		}
	}

}


void SDL_FillRect(SDL_Surface *dst, SDL_Rect *dstrect, uint32_t color) {
	assert(dst);
	assert(color <= 0xff);

	/* TODO: Fill the rectangle area described by ``dstrect''
	 * in surface ``dst'' with color ``color''. If dstrect is
	 * NULL, fill the whole surface.
	 */

	//assert(0);
	int x, y, w, h;
	x = (dstrect != NULL)?dstrect->x:0;
	y = (dstrect != NULL)?dstrect->y:0;
	w = (dstrect != NULL)?dstrect->w:dst->w;
	h = (dstrect != NULL)?dstrect->h:dst->h;

		    int i = 0;
			int limit = dst->format->palette->ncolors; 
			SDL_Color *colors = dst->format->palette->colors;
			for (; i < limit; ++i) 
				if (color == *(uint32_t*)&colors[i])
				    break;
		    
			assert(i != limit) ;
			assert(dst->format->BitsPerPixel == 8);
			uint8_t *pixel = dst->pixels;

		    int line = y;
			int row = x;
			int row_limit = x + w - 1;
			int line_limit = y + h - 1;

			int pitch = dst->pitch;
		    
			SDL_Rect *clip = &(dst->clip_rect);
			int l = clip->x;
			int t = clip->y;
			int r = clip->x + clip->w - 1;
			int d = clip->y + clip->h - 1;

			if (line > d || row > r) {
					dstrect->w = 0;
					dstrect->h = 0;
					assert(0);
					return;
			}

			MAKE_CLIP(line, row, line_limit, row_limit, l, r, t, d);
			if (dstrect != NULL) {
				dstrect->x = row;
				dstrect->y = line;
				dstrect->h = line_limit - line + 1;
				dstrect->w = row_limit - row + 1;
			}

			l = row;

			for (; line <= line_limit; ++line) {
					for (row = l; row <= row_limit; ++row) {
							pixel[line * pitch + row] = i;
					}
			}
}

void SDL_UpdateRect(SDL_Surface *screen, int x, int y, int w, int h) {
	assert(screen);
	assert(screen->pitch == 320);
	if(screen->flags & SDL_HWSURFACE) {
		if(x == 0 && y == 0) {
			/* Draw FPS */
			vmem = VMEM_ADDR;
			char buf[80];
			sprintf(buf, "%dFPS", get_fps());
			draw_string(buf, 0, 0, 10);
		}
		return;
	}

	/* TODO: Copy the pixels in the rectangle area to the screen. */

	//assert(0);
    if (w == 0 && h == 0) {
			w = screen->w;
			h = screen->h;
	}

	int pitch = screen->pitch;

	int line, row;
	int line_limit = y + h - 1, row_limit = x + w - 1;
	SDL_Color *colors = screen->format->palette->colors;
	uint8_t* pixel = screen->pixels;

	for (line = y; line <= line_limit ; ++line)
			for (row = x; row <= row_limit; ++row) {
					draw_pixel(row, line, *(int*)&colors[(int)pixel[line * pitch + row]]);
			}
}

void SDL_SetPalette(SDL_Surface *s, int flags, SDL_Color *colors, 
		int firstcolor, int ncolors) {
	assert(s);
	assert(s->format);
	assert(s->format->palette);
	assert(firstcolor == 0);

	if(s->format->palette->colors == NULL || s->format->palette->ncolors != ncolors) {
		if(s->format->palette->ncolors != ncolors && s->format->palette->colors != NULL) {
			/* If the size of the new palette is different 
			 * from the old one, free the old one.
			 */
			free(s->format->palette->colors);
		}

		/* Get new memory space to store the new palette. */
		s->format->palette->colors = malloc(sizeof(SDL_Color) * ncolors);
		assert(s->format->palette->colors);
	}

	/* Set the new palette. */
	s->format->palette->ncolors = ncolors;
	memcpy(s->format->palette->colors, colors, sizeof(SDL_Color) * ncolors);

	if(s->flags & SDL_HWSURFACE) {
		/* TODO: Set the VGA palette by calling write_palette(). */
		//assert(0);
		write_palette(colors, ncolors);
	}
}

/* ======== The following functions are already implemented. ======== */

void SDL_SoftStretch(SDL_Surface *src, SDL_Rect *scrrect, 
		SDL_Surface *dst, SDL_Rect *dstrect) {
	assert(src && dst);
	int x = (scrrect == NULL ? 0 : scrrect->x);
	int y = (scrrect == NULL ? 0 : scrrect->y);
	int w = (scrrect == NULL ? src->w : scrrect->w);
	int h = (scrrect == NULL ? src->h : scrrect->h);

	assert(dstrect);
	if(w == dstrect->w && h == dstrect->h) {
		/* The source rectangle and the destination rectangle
		 * are of the same size. If that is the case, there
		 * is no need to stretch, just copy. */
		SDL_Rect rect;
		rect.x = x;
		rect.y = y;
		rect.w = w;
		rect.h = h;
		SDL_BlitSurface(src, &rect, dst, dstrect);
	}
	else {
		/* No other case occurs in NEMU-PAL. */
		assert(0);
	}
}

SDL_Surface* SDL_CreateRGBSurface(uint32_t flags, int width, int height, int depth,
		uint32_t Rmask, uint32_t Gmask, uint32_t Bmask, uint32_t Amask) {
	SDL_Surface *s = malloc(sizeof(SDL_Surface));
	assert(s);
	s->format = malloc(sizeof(SDL_PixelFormat));
	assert(s);
	s->format->palette = malloc(sizeof(SDL_Palette));
	assert(s->format->palette);
	s->format->palette->colors = NULL;

	s->format->BitsPerPixel = depth;

	s->flags = flags;
	s->w = width;
	s->h = height;
	s->pitch = (width * depth) >> 3;
	s->pixels = (flags & SDL_HWSURFACE ? (void *)VMEM_ADDR : malloc(s->pitch * height));
	assert(s->pixels);

	return s;
}

SDL_Surface* SDL_SetVideoMode(int width, int height, int bpp, uint32_t flags) {
	return SDL_CreateRGBSurface(flags,  width, height, bpp,
			0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
}

void SDL_FreeSurface(SDL_Surface *s) {
	if(s != NULL) {
		if(s->format != NULL) {
			if(s->format->palette != NULL) {
				if(s->format->palette->colors != NULL) {
					free(s->format->palette->colors);
				}
				free(s->format->palette);
			}

			free(s->format);
		}
		
		if(s->pixels != NULL) {
			free(s->pixels);
		}

		free(s);
	}
}

