/*
 * Copyright (C) 1999-2002, 2004-2011, 2016 Free Software Foundation, Inc.
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
 * If not, see <http://www.gnu.org/licenses/>.
 */

/* This file defines all the converters. */


/* Our own notion of wide character, as UCS-4, according to ISO-10646-1. */
typedef unsigned int ucs4_t;

/* State used by a conversion. 0 denotes the initial state. */
typedef unsigned int state_t;

/* iconv_t is an opaque type. This is the real iconv_t type. */
typedef struct conv_struct * conv_t;

/*
 * Data type for conversion multibyte -> unicode
 */
struct mbtowc_funcs {
  int (*xxx_mbtowc) (conv_t conv, ucs4_t *pwc, unsigned char const *s, size_t n);
  /*
   * int xxx_mbtowc (conv_t conv, ucs4_t *pwc, unsigned char const *s, size_t n)
   * converts the byte sequence starting at s to a wide character. Up to n bytes
   * are available at s. n is >= 1.
   * Result is number of bytes consumed (if a wide character was read),
   * or -1 if invalid, or -2 if n too small,
   * or RET_SHIFT_ILSEQ(number of bytes consumed) if invalid input after a shift
   * sequence was read,
   * or RET_TOOFEW(number of bytes consumed) if only a shift sequence was read.
   */
  int (*xxx_flushwc) (conv_t conv, ucs4_t *pwc);
  /*
   * int xxx_flushwc (conv_t conv, ucs4_t *pwc)
   * returns to the initial state and stores the pending wide character, if any.
   * Result is 1 (if a wide character was read) or 0 if none was pending.
   */
};

/* Return code if invalid input after a shift sequence of n bytes was read.
   (xxx_mbtowc) */
#define RET_SHIFT_ILSEQ(n)  (-1-2*(n))
/* Return code if invalid. (xxx_mbtowc) */
#define RET_ILSEQ           RET_SHIFT_ILSEQ(0)
/* Return code if only a shift sequence of n bytes was read. (xxx_mbtowc) */
#define RET_TOOFEW(n)       (-2-2*(n))
/* Retrieve the n from the encoded RET_... value. */
#define DECODE_SHIFT_ILSEQ(r)  ((unsigned int)(RET_SHIFT_ILSEQ(0) - (r)) / 2)
#define DECODE_TOOFEW(r)       ((unsigned int)(RET_TOOFEW(0) - (r)) / 2)
/* Maximum value of n that may be used as argument to RET_SHIFT_ILSEQ or RET_TOOFEW. */
#define RET_COUNT_MAX       ((INT_MAX / 2) - 1)

/*
 * Data type for conversion unicode -> multibyte
 */
struct wctomb_funcs {
  int (*xxx_wctomb) (conv_t conv, unsigned char *r, ucs4_t wc, size_t n);
  /*
   * int xxx_wctomb (conv_t conv, unsigned char *r, ucs4_t wc, size_t n)
   * converts the wide character wc to the character set xxx, and stores the
   * result beginning at r. Up to n bytes may be written at r. n is >= 1.
   * Result is number of bytes written, or -1 if invalid, or -2 if n too small.
   */
  int (*xxx_reset) (conv_t conv, unsigned char *r, size_t n);
  /*
   * int xxx_reset (conv_t conv, unsigned char *r, size_t n)
   * stores a shift sequences returning to the initial state beginning at r.
   * Up to n bytes may be written at r. n is >= 0.
   * Result is number of bytes written, or -2 if n too small.
   */
};

/* Return code if invalid. (xxx_wctomb) */
#define RET_ILUNI      -1
/* Return code if output buffer is too small. (xxx_wctomb, xxx_reset) */
#define RET_TOOSMALL   -2

/*
 * Contents of a conversion descriptor.
 */
struct conv_struct {
  struct loop_funcs lfuncs;
  /* Input (conversion multibyte -> unicode) */
  int iindex;
  struct mbtowc_funcs ifuncs;
  state_t istate;
  /* Output (conversion unicode -> multibyte) */
  int oindex;
  struct wctomb_funcs ofuncs;
  int oflags;
  state_t ostate;
  /* Operation flags */
  int transliterate;
  int discard_ilseq;
  #ifndef LIBICONV_PLUG
  struct iconv_fallbacks_t fallbacks;
  struct iconv_hooks_t hooks;
  #endif
};

/*
 * Include all the converters.
 */

#include "code/ascii.h"

/* General multi-byte encodings */
#include "code/utf8.h"
#include "code/ucs2.h"
#include "code/ucs2be.h"
#include "code/ucs2le.h"
#include "code/ucs4.h"
#include "code/ucs4be.h"
#include "code/ucs4le.h"
#include "code/utf16.h"
#include "code/utf16be.h"
#include "code/utf16le.h"
#include "code/utf32.h"
#include "code/utf32be.h"
#include "code/utf32le.h"
#include "code/utf7.h"
#include "code/ucs2internal.h"
#include "code/ucs2swapped.h"
#include "code/ucs4internal.h"
#include "code/ucs4swapped.h"
#include "code/c99.h"
#include "code/java.h"

/* 8-bit encodings */
#include "code/iso8859_1.h"
#include "code/iso8859_2.h"
#include "code/iso8859_3.h"
#include "code/iso8859_4.h"
#include "code/iso8859_5.h"
#include "code/iso8859_6.h"
#include "code/iso8859_7.h"
#include "code/iso8859_8.h"
#include "code/iso8859_9.h"
#include "code/iso8859_10.h"
#include "code/iso8859_11.h"
#include "code/iso8859_13.h"
#include "code/iso8859_14.h"
#include "code/iso8859_15.h"
#include "code/iso8859_16.h"
#include "code/koi8_r.h"
#include "code/koi8_u.h"
#include "code/koi8_ru.h"
#include "code/cp1250.h"
#include "code/cp1251.h"
#include "code/cp1252.h"
#include "code/cp1253.h"
#include "code/cp1254.h"
#include "code/cp1255.h"
#include "code/cp1256.h"
#include "code/cp1257.h"
#include "code/cp1258.h"
#include "code/cp850.h"
#include "code/cp862.h"
#include "code/cp866.h"
#include "code/cp1131.h"
#include "code/mac_roman.h"
#include "code/mac_centraleurope.h"
#include "code/mac_iceland.h"
#include "code/mac_croatian.h"
#include "code/mac_romania.h"
#include "code/mac_cyrillic.h"
#include "code/mac_ukraine.h"
#include "code/mac_greek.h"
#include "code/mac_turkish.h"
#include "code/mac_hebrew.h"
#include "code/mac_arabic.h"
#include "code/mac_thai.h"
#include "code/hp_roman8.h"
#include "code/nextstep.h"
#include "code/armscii_8.h"
#include "code/georgian_academy.h"
#include "code/georgian_ps.h"
#include "code/koi8_t.h"
#include "code/pt154.h"
#include "code/rk1048.h"
#include "code/mulelao.h"
#include "code/cp1133.h"
#include "code/tis620.h"
#include "code/cp874.h"
#include "code/viscii.h"
#include "code/tcvn.h"

/* CJK character sets [CCS = coded character set] [CJKV.INF chapter 3] */

typedef struct {
  unsigned short indx; /* index into big table */
  unsigned short used; /* bitmask of used entries */
} Summary16;

#include "code/iso646_jp.h"
#include "code/jisx0201.h"
#include "code/jisx0208.h"
#include "code/jisx0212.h"

#include "code/iso646_cn.h"
#include "code/gb2312.h"
#include "code/isoir165.h"
/*#include "gb12345.h"*/
#include "code/gbk.h"
#include "code/cns11643.h"
#include "code/big5.h"

#include "code/ksc5601.h"
#include "code/johab_hangul.h"

/* CJK encodings [CES = character encoding scheme] [CJKV.INF chapter 4] */

#include "code/euc_jp.h"
#include "code/sjis.h"
#include "code/cp932.h"
#include "code/iso2022_jp.h"
#include "code/iso2022_jp1.h"
#include "code/iso2022_jp2.h"
#include "code/iso2022_jpms.h"

#include "code/euc_cn.h"
#include "code/ces_gbk.h"
#include "code/cp936.h"
#include "code/gb18030.h"
#include "code/iso2022_cn.h"
#include "code/iso2022_cnext.h"
#include "code/hz.h"
#include "code/euc_tw.h"
#include "code/ces_big5.h"
#include "code/cp950.h"
#include "code/big5hkscs1999.h"
#include "code/big5hkscs2001.h"
#include "code/big5hkscs2004.h"
#include "code/big5hkscs2008.h"

#include "code/euc_kr.h"
#include "code/cp949.h"
#include "code/johab.h"
#include "code/iso2022_kr.h"

/* Encodings used by system dependent locales. */

#ifdef USE_AIX
#include "cp856.h"
#include "cp922.h"
#include "cp943.h"
#include "cp1046.h"
#include "cp1124.h"
#include "cp1129.h"
#include "cp1161.h"
#include "cp1162.h"
#include "cp1163.h"
#endif

#ifdef USE_OSF1
#include "dec_kanji.h"
#include "dec_hanyu.h"
#endif

#ifdef USE_DOS
#include "code/cp437.h"
#include "code/cp737.h"
#include "code/cp775.h"
#include "code/cp852.h"
#include "code/cp853.h"
#include "code/cp855.h"
#include "code/cp857.h"
#include "code/cp858.h"
#include "code/cp860.h"
#include "code/cp861.h"
#include "code/cp863.h"
#include "code/cp864.h"
#include "code/cp865.h"
#include "code/cp869.h"
#include "code/cp1125.h"
#endif

#ifdef USE_EXTRA
#include "euc_jisx0213.h"
#include "shift_jisx0213.h"
#include "iso2022_jp3.h"
#include "big5_2003.h"
#include "tds565.h"
#include "atarist.h"
#include "riscos1.h"
#endif

