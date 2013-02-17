/*
 * Copyright (C) 1999-2001 Free Software Foundation, Inc.
 * This file is part of the GNU LIBICONV Library.
 *
 * The GNU LIBICONV Library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * The GNU LIBICONV Library is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with the GNU LIBICONV Library; see the file COPYING.LIB.
 * If not, write to the Free Software Foundation, Inc., 51 Franklin Street,
 * Fifth Floor, Boston, MA 02110-1301, USA.
 */

/*
 * CP775
 */

static const unsigned short cp775_2uni[128] = {
  /* 0x80 */
  0x0106, 0x00fc, 0x00e9, 0x0101, 0x00e4, 0x0123, 0x00e5, 0x0107,
  0x0142, 0x0113, 0x0156, 0x0157, 0x012b, 0x0179, 0x00c4, 0x00c5,
  /* 0x90 */
  0x00c9, 0x00e6, 0x00c6, 0x014d, 0x00f6, 0x0122, 0x00a2, 0x015a,
  0x015b, 0x00d6, 0x00dc, 0x00f8, 0x00a3, 0x00d8, 0x00d7, 0x00a4,
  /* 0xa0 */
  0x0100, 0x012a, 0x00f3, 0x017b, 0x017c, 0x017a, 0x201d, 0x00a6,
  0x00a9, 0x00ae, 0x00ac, 0x00bd, 0x00bc, 0x0141, 0x00ab, 0x00bb,
  /* 0xb0 */
  0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x0104, 0x010c, 0x0118,
  0x0116, 0x2563, 0x2551, 0x2557, 0x255d, 0x012e, 0x0160, 0x2510,
  /* 0xc0 */
  0x2514, 0x2534, 0x252c, 0x251c, 0x2500, 0x253c, 0x0172, 0x016a,
  0x255a, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256c, 0x017d,
  /* 0xd0 */
  0x0105, 0x010d, 0x0119, 0x0117, 0x012f, 0x0161, 0x0173, 0x016b,
  0x017e, 0x2518, 0x250c, 0x2588, 0x2584, 0x258c, 0x2590, 0x2580,
  /* 0xe0 */
  0x00d3, 0x00df, 0x014c, 0x0143, 0x00f5, 0x00d5, 0x00b5, 0x0144,
  0x0136, 0x0137, 0x013b, 0x013c, 0x0146, 0x0112, 0x0145, 0x2019,
  /* 0xf0 */
  0x00ad, 0x00b1, 0x201c, 0x00be, 0x00b6, 0x00a7, 0x00f7, 0x201e,
  0x00b0, 0x2219, 0x00b7, 0x00b9, 0x00b3, 0x00b2, 0x25a0, 0x00a0,
};

int
cp775_mbtowc (conv_t conv, ucs4_t *pwc, const unsigned char *s, int n)
{
  unsigned char c = *s;
  if (c < 0x80)
    *pwc = (ucs4_t) c;
  else
    *pwc = (ucs4_t) cp775_2uni[c-0x80];
  return 1;
}

static const unsigned char cp775_page00[224] = {
  0xff, 0x00, 0x96, 0x9c, 0x9f, 0x00, 0xa7, 0xf5, /* 0xa0-0xa7 */
  0x00, 0xa8, 0x00, 0xae, 0xaa, 0xf0, 0xa9, 0x00, /* 0xa8-0xaf */
  0xf8, 0xf1, 0xfd, 0xfc, 0x00, 0xe6, 0xf4, 0xfa, /* 0xb0-0xb7 */
  0x00, 0xfb, 0x00, 0xaf, 0xac, 0xab, 0xf3, 0x00, /* 0xb8-0xbf */
  0x00, 0x00, 0x00, 0x00, 0x8e, 0x8f, 0x92, 0x00, /* 0xc0-0xc7 */
  0x00, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0xc8-0xcf */
  0x00, 0x00, 0x00, 0xe0, 0x00, 0xe5, 0x99, 0x9e, /* 0xd0-0xd7 */
  0x9d, 0x00, 0x00, 0x00, 0x9a, 0x00, 0x00, 0xe1, /* 0xd8-0xdf */
  0x00, 0x00, 0x00, 0x00, 0x84, 0x86, 0x91, 0x00, /* 0xe0-0xe7 */
  0x00, 0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0xe8-0xef */
  0x00, 0x00, 0x00, 0xa2, 0x00, 0xe4, 0x94, 0xf6, /* 0xf0-0xf7 */
  0x9b, 0x00, 0x00, 0x00, 0x81, 0x00, 0x00, 0x00, /* 0xf8-0xff */
  /* 0x0100 */
  0xa0, 0x83, 0x00, 0x00, 0xb5, 0xd0, 0x80, 0x87, /* 0x00-0x07 */
  0x00, 0x00, 0x00, 0x00, 0xb6, 0xd1, 0x00, 0x00, /* 0x08-0x0f */
  0x00, 0x00, 0xed, 0x89, 0x00, 0x00, 0xb8, 0xd3, /* 0x10-0x17 */
  0xb7, 0xd2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x18-0x1f */
  0x00, 0x00, 0x95, 0x85, 0x00, 0x00, 0x00, 0x00, /* 0x20-0x27 */
  0x00, 0x00, 0xa1, 0x8c, 0x00, 0x00, 0xbd, 0xd4, /* 0x28-0x2f */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe8, 0xe9, /* 0x30-0x37 */
  0x00, 0x00, 0x00, 0xea, 0xeb, 0x00, 0x00, 0x00, /* 0x38-0x3f */
  0x00, 0xad, 0x88, 0xe3, 0xe7, 0xee, 0xec, 0x00, /* 0x40-0x47 */
  0x00, 0x00, 0x00, 0x00, 0xe2, 0x93, 0x00, 0x00, /* 0x48-0x4f */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8a, 0x8b, /* 0x50-0x57 */
  0x00, 0x00, 0x97, 0x98, 0x00, 0x00, 0x00, 0x00, /* 0x58-0x5f */
  0xbe, 0xd5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x60-0x67 */
  0x00, 0x00, 0xc7, 0xd7, 0x00, 0x00, 0x00, 0x00, /* 0x68-0x6f */
  0x00, 0x00, 0xc6, 0xd6, 0x00, 0x00, 0x00, 0x00, /* 0x70-0x77 */
  0x00, 0x8d, 0xa5, 0xa3, 0xa4, 0xcf, 0xd8, 0x00, /* 0x78-0x7f */
};
static const unsigned char cp775_page20[8] = {
  0x00, 0xef, 0x00, 0x00, 0xf2, 0xa6, 0xf7, 0x00, /* 0x18-0x1f */
};
static const unsigned char cp775_page25[168] = {
  0xc4, 0x00, 0xb3, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x00-0x07 */
  0x00, 0x00, 0x00, 0x00, 0xda, 0x00, 0x00, 0x00, /* 0x08-0x0f */
  0xbf, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, /* 0x10-0x17 */
  0xd9, 0x00, 0x00, 0x00, 0xc3, 0x00, 0x00, 0x00, /* 0x18-0x1f */
  0x00, 0x00, 0x00, 0x00, 0xb4, 0x00, 0x00, 0x00, /* 0x20-0x27 */
  0x00, 0x00, 0x00, 0x00, 0xc2, 0x00, 0x00, 0x00, /* 0x28-0x2f */
  0x00, 0x00, 0x00, 0x00, 0xc1, 0x00, 0x00, 0x00, /* 0x30-0x37 */
  0x00, 0x00, 0x00, 0x00, 0xc5, 0x00, 0x00, 0x00, /* 0x38-0x3f */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x40-0x47 */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x48-0x4f */
  0xcd, 0xba, 0x00, 0x00, 0xc9, 0x00, 0x00, 0xbb, /* 0x50-0x57 */
  0x00, 0x00, 0xc8, 0x00, 0x00, 0xbc, 0x00, 0x00, /* 0x58-0x5f */
  0xcc, 0x00, 0x00, 0xb9, 0x00, 0x00, 0xcb, 0x00, /* 0x60-0x67 */
  0x00, 0xca, 0x00, 0x00, 0xce, 0x00, 0x00, 0x00, /* 0x68-0x6f */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x70-0x77 */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x78-0x7f */
  0xdf, 0x00, 0x00, 0x00, 0xdc, 0x00, 0x00, 0x00, /* 0x80-0x87 */
  0xdb, 0x00, 0x00, 0x00, 0xdd, 0x00, 0x00, 0x00, /* 0x88-0x8f */
  0xde, 0xb0, 0xb1, 0xb2, 0x00, 0x00, 0x00, 0x00, /* 0x90-0x97 */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x98-0x9f */
  0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0xa0-0xa7 */
};

int
cp775_wctomb (conv_t conv, unsigned char *r, ucs4_t wc, int n)
{
  unsigned char c = 0;
  if (wc < 0x0080) {
    *r = wc;
    return 1;
  }
  else if (wc >= 0x00a0 && wc < 0x0180)
    c = cp775_page00[wc-0x00a0];
  else if (wc >= 0x2018 && wc < 0x2020)
    c = cp775_page20[wc-0x2018];
  else if (wc == 0x2219)
    c = 0xf9;
  else if (wc >= 0x2500 && wc < 0x25a8)
    c = cp775_page25[wc-0x2500];
  if (c != 0) {
    *r = c;
    return 1;
  }
  return RET_ILUNI;
}
