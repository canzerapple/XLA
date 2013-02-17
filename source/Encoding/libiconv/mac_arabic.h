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
 * MacArabic
 */

static const unsigned short mac_arabic_2uni[128] = {
  /* 0x80 */
  0x00c4, 0x00a0, 0x00c7, 0x00c9, 0x00d1, 0x00d6, 0x00dc, 0x00e1,
  0x00e0, 0x00e2, 0x00e4, 0x06ba, 0x00ab, 0x00e7, 0x00e9, 0x00e8,
  /* 0x90 */
  0x00ea, 0x00eb, 0x00ed, 0x2026, 0x00ee, 0x00ef, 0x00f1, 0x00f3,
  0x00bb, 0x00f4, 0x00f6, 0x00f7, 0x00fa, 0x00f9, 0x00fb, 0x00fc,
  /* 0xa0 */
  0xfffd, 0xfffd, 0xfffd, 0xfffd, 0xfffd, 0x066a, 0xfffd, 0xfffd,
  0xfffd, 0xfffd, 0xfffd, 0xfffd, 0x060c, 0xfffd, 0xfffd, 0xfffd,
  /* 0xb0 */
  0x0660, 0x0661, 0x0662, 0x0663, 0x0664, 0x0665, 0x0666, 0x0667,
  0x0668, 0x0669, 0xfffd, 0x061b, 0xfffd, 0xfffd, 0xfffd, 0x061f,
  /* 0xc0 */
  0x066d, 0x0621, 0x0622, 0x0623, 0x0624, 0x0625, 0x0626, 0x0627,
  0x0628, 0x0629, 0x062a, 0x062b, 0x062c, 0x062d, 0x062e, 0x062f,
  /* 0xd0 */
  0x0630, 0x0631, 0x0632, 0x0633, 0x0634, 0x0635, 0x0636, 0x0637,
  0x0638, 0x0639, 0x063a, 0xfffd, 0xfffd, 0xfffd, 0xfffd, 0xfffd,
  /* 0xe0 */
  0x0640, 0x0641, 0x0642, 0x0643, 0x0644, 0x0645, 0x0646, 0x0647,
  0x0648, 0x0649, 0x064a, 0x064b, 0x064c, 0x064d, 0x064e, 0x064f,
  /* 0xf0 */
  0x0650, 0x0651, 0x0652, 0x067e, 0x0679, 0x0686, 0x06d5, 0x06a4,
  0x06af, 0x0688, 0x0691, 0xfffd, 0xfffd, 0xfffd, 0x0698, 0x06d2,
};

int
mac_arabic_mbtowc (conv_t conv, ucs4_t *pwc, const unsigned char *s, int n)
{
  unsigned char c = *s;
  if (c < 0x80) {
    *pwc = (ucs4_t) c;
    return 1;
  }
  else {
    unsigned short wc = mac_arabic_2uni[c-0x80];
    if (wc != 0xfffd) {
      *pwc = (ucs4_t) wc;
      return 1;
    }
  }
  return RET_ILSEQ;
}

static const unsigned char mac_arabic_page00[96] = {
  0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0xa0-0xa7 */
  0x00, 0x00, 0x00, 0x8c, 0x00, 0x00, 0x00, 0x00, /* 0xa8-0xaf */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0xb0-0xb7 */
  0x00, 0x00, 0x00, 0x98, 0x00, 0x00, 0x00, 0x00, /* 0xb8-0xbf */
  0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x82, /* 0xc0-0xc7 */
  0x00, 0x83, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0xc8-0xcf */
  0x00, 0x84, 0x00, 0x00, 0x00, 0x00, 0x85, 0x00, /* 0xd0-0xd7 */
  0x00, 0x00, 0x00, 0x00, 0x86, 0x00, 0x00, 0x00, /* 0xd8-0xdf */
  0x88, 0x87, 0x89, 0x00, 0x8a, 0x00, 0x00, 0x8d, /* 0xe0-0xe7 */
  0x8f, 0x8e, 0x90, 0x91, 0x00, 0x92, 0x94, 0x95, /* 0xe8-0xef */
  0x00, 0x96, 0x00, 0x97, 0x99, 0x00, 0x9a, 0x9b, /* 0xf0-0xf7 */
  0x00, 0x9d, 0x9c, 0x9e, 0x9f, 0x00, 0x00, 0x00, /* 0xf8-0xff */
};
static const unsigned char mac_arabic_page06[208] = {
  0x00, 0x00, 0x00, 0x00, 0xac, 0x00, 0x00, 0x00, /* 0x08-0x0f */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x10-0x17 */
  0x00, 0x00, 0x00, 0xbb, 0x00, 0x00, 0x00, 0xbf, /* 0x18-0x1f */
  0x00, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, /* 0x20-0x27 */
  0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, /* 0x28-0x2f */
  0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, /* 0x30-0x37 */
  0xd8, 0xd9, 0xda, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x38-0x3f */
  0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, /* 0x40-0x47 */
  0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef, /* 0x48-0x4f */
  0xf0, 0xf1, 0xf2, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x50-0x57 */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x58-0x5f */
  0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, /* 0x60-0x67 */
  0xb8, 0xb9, 0xa5, 0x00, 0x00, 0xc0, 0x00, 0x00, /* 0x68-0x6f */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x70-0x77 */
  0x00, 0xf4, 0x00, 0x00, 0x00, 0x00, 0xf3, 0x00, /* 0x78-0x7f */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf5, 0x00, /* 0x80-0x87 */
  0xf9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x88-0x8f */
  0x00, 0xfa, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x90-0x97 */
  0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x98-0x9f */
  0x00, 0x00, 0x00, 0x00, 0xf7, 0x00, 0x00, 0x00, /* 0xa0-0xa7 */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, /* 0xa8-0xaf */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0xb0-0xb7 */
  0x00, 0x00, 0x8b, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0xb8-0xbf */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0xc0-0xc7 */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0xc8-0xcf */
  0x00, 0x00, 0xff, 0x00, 0x00, 0xf6, 0x00, 0x00, /* 0xd0-0xd7 */
};

int
mac_arabic_wctomb (conv_t conv, unsigned char *r, ucs4_t wc, int n)
{
  unsigned char c = 0;
  if (wc < 0x0080) {
    *r = wc;
    return 1;
  }
  else if (wc >= 0x00a0 && wc < 0x0100)
    c = mac_arabic_page00[wc-0x00a0];
  else if (wc >= 0x0608 && wc < 0x06d8)
    c = mac_arabic_page06[wc-0x0608];
  else if (wc == 0x2026)
    c = 0x93;
  if (c != 0) {
    *r = c;
    return 1;
  }
  return RET_ILUNI;
}
