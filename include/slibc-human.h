/**
 * slibc — Yet another C library
 * Copyright © 2015  Mattias Andrée (maandree@member.fsf.org)
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _SLIBC_HUMAN_H
#define _SLIBC_HUMAN_H
#include <slibc/version.h>
#include <slibc/features.h>
#ifndef __PORTABLE



#define __NEED_mode_t
#ifdef __C99__
# define __NEED_intmax_t
# define __NEED_uintmax_t
#endif

#include <bits/types.h>



/**
 * Representation settings for file permissions.
 */
enum humanmode_mode
  {
    /**
     * Return in the format where 0750
     * resolves to 'rwxr-x---'.
     * 
     * If used in combination with `HUMANMODE_MASK`,
     * 0750 resolves to 'u=rwx,g=r-x,o=---'.
     */
    HUMANMODE_STAT = 1,
    
    /**
     * Return in the format where 0750
     * resolves to 'u=rwx,g=rx,o='.
     * 
     * If used in combination with `HUMANMODE_STAT`,
     * 0750 resolves to 'u=rwx,g=r-x,o=---'.
     */
    HUMANMODE_MASK = 2,
  };


/**
 * Representation settings for converting
 * sizes to human-readable format.
 */
enum humansize_mode
  {
    /**
     * 'k' is 1000.
     * 
     * Cannot be combined with `HUMANSIZE_IEC`
     * or `HUMANSIZE_IEC_EXPLICIT`.
     */
    HUMANSIZE_SI = 1,
    
    /**
     * 'K' is 1024.
     * 
     * Cannot be combined with `HUMANSIZE_SI`
     * or `HUMANSIZE_IEC_EXPLICIT`.
     */
    HUMANSIZE_IEC = 2,
    
    /**
     * 'Ki' is 1024.
     * 
     * Cannot be combined with `HUMANSIZE_SI`
     * or `HUMANSIZE_IEC`.
     */
    HUMANSIZE_IEC_EXPLICIT = 4,
    
    
    /**
     * 'B' is only included if there is no prefix.
     */
    HUMANSIZE_PREFIX_ONLY = 8,
    
    
    /**
     * Print size exactly if `detail` is 0,
     * otherwise use the highest `detail` prefixes.
     * 
     * For example `detail == 0` may yeild '3TB 2MB 1KB',
     * and `detail == 3` may yeild '3TB 2MB' for the same size.
     * 
     * Cannot be combined with `HUMANSIZE_ROUND`.
     */
    HUMANSIZE_EXACT = 16,
    
    /**
     * Similar to `HUMANSIZE_EXACT` with `detail == 1`,
     * but the value will include `detail` digits.
     * `detail` < 0 is allowed, 
     * 
     * Cannot be combined with `HUMANSIZE_EXACT`.
     */
    HUMANSIZE_ROUND = 32,
  };


/**
 * Settings for treating ambiguous file size
 * and file offset representations when parsing.
 */
enum machinesize_mode
  {
    /**
     * 'k' and 'K' is 1000.
     * 
     * If `MACHINESIZE_IEC` is also used,
     * 1000-base is used if 'B' is explicitly
     * included, otherwise 1024-base is used.
     */
    MACHINESIZE_SI = 1,
    
    /**
     * 'k' and 'K' is 1024.
     * 
     * If `MACHINESIZE_SI` is also used,
     * 1000-base is used if 'B' is explicitly
     * included, otherwise 1024-base is used.
     */
    MACHINESIZE_IEC = 2,
  };


/**
 * Ways to handled unrecognised escapes,
 * and other configurations.
 */
enum unescape_mode
  {
    /**
     * For any unrecognised character '#',
     * '\#' results in an EINVAL-error.
     * 
     * Cannot be used together with
     * `UNESCAPE_VERBATIM` or `UNESCAPE_IGNORE`.
     */
    UNESCAPE_EINVAL = 1,
    
    /**
     * For any unrecognised character '#',
     * '\#' results in '#'.
     * 
     * Cannot be used together with
     * `UNESCAPE_EINVAL` or `UNESCAPE_IGNORE`.
     */
    UNESCAPE_VERBATIM = 2,
    
    /**
     * For any unrecognised character '#',
     * '\#' results in '\#'.
     * 
     * Cannot be used together with
     * `UNESCAPE_EINVAL` or `UNESCAPE_VERBATIM`.
     */
    UNESCAPE_IGNORE = 4,
    
    
    /**
     * '\&' resolves to the byte 255 (0xFF).
     * 
     * If not used, '\&' is handled as an
     * unsupported escape.
     */
    UNESCAPE_AMPERSAND = 8,
    
    /**
     * '\0' resolves to the byte sequence
     * 192 128 (0xC0 0x80).
     * 
     * If not used, '\0' resolves to a
     * 0 byte (termination).
     */
    UNESCAPE_MOD_UTF8 = 16,
  };



/**
 * Convert file permission from machine representation to human representation.
 * 
 * @param   buffer  Sufficiently large buffer for the output, or `NULL`.
 *                  18 characters is always sufficient, regardless of `mode`.
 * @param   perm    Machine representation of the permissions, will be masked with 07777.
 * @param   mode    Representation style, 0 for default.
 * @return          Human representation of the file permissions, `NULL` on error.
 *                  On success, the caller is responsible for deallocating the
 *                  returned pointer, if and only if `buffer` is `NULL`.
 * 
 * @throws  EINVAL  If `mode` is invalid.
 * @throws  ENOMEM  The process cannot allocate more memory.
 */
char* humanmode(char* restrict, mode_t, enum humanmode_mode);

/**
 * Parses a human representation of file permissions, and updates to file permissions.
 * 
 * Assuming the current file permissions is `value`, and neither
 * `mode` nor `mask` is `NULL`, the new permissions should be
 * `value & ~*mask | *mode`. The new mode (includes file type) should
 * be `value & ~*mask | *mode & 07777`.
 * 
 * @param   mode  Output parameter for the bits to set, may be `NULL`.
 * @param   mask  Output parameter for the bits to update, may be `NULL`.
 * @param   str   The file permission to parse, must not include file type or be `NULL`.
 * @return        Zero on success, -1 on error.
 * 
 * @throws  EINVAL  If `str` is not parseable.
 */
int machinemode(mode_t* restrict, mode_t* restrict, const char* restrict)
  __GCC_ONLY(__attributes__((__nonnull__(3))));


/**
 * Convert a file size of file offset from machine representation to human representation.
 * 
 * @param   buffer        A buffer than shall be used if it is sufficiently large.
 * @param   bufsize       The allocation size of `buffer`.
 *                        Must be 0 if and only if `buffer` is `NULL`.
 * @param   size          The value to convert.
 * @param   mode          Representation style, 0 for default.
 * @param   detail        See documentation for the select value on `mode`.
 * @param   point         The symbol to use for decimal points. `NULL` or empty for default.
 * @param   intraspacing  Spacing between values and units. `NULL` or empty for none.
 *                        This value should depend on language and context. For English
 *                        this value should be "" or "-", but in for example Swedish it
 *                        should always be " ". Hence this value is a string rather than
 *                        a booleanic integer.
 * @param   interspacing  Spacing between value–unit-pairs. `NULL` for default (" ").
 *                        This value should depend on language and context.
 * @return                Human representation of the file size/offset, `NULL` on error.
 *                        On success, the caller is responsible for deallocating the
 *                        returned pointer, if and only if it is not `buffer`.
 * 
 * @throws  EINVAL  If `mode` is invalid.
 * @throws  EINVAL  If `mode & HUMANSIZE_EXACT` and `detail < 0`.
 * @throws  ENOMEM  The process cannot allocate more memory.
 */
char* humansize(char*, size_t, size_t, enum humansize_mode, int, const char* restrict,
		const char* restrict, const char* restrict)
  __GCC_ONLY(__attribute__((__warn_unused_result__)));

/* TODO machinesize */
int machinesize(size_t* restrict size, const char* restrict str, enum machinesize_mode mode,
		const char* restrict space, const char* restrict point);


#ifdef __C99__
/* TODO humandur */
int humandur(intmax_t sec, long int nsec, const char* restrict point, const char* restrict format,
	     const char* restrict intraspacing, const char* restrict interspacing);

/* TODO machinedur */
int machinedur(intmax_t* restrict sec, long int* nsec, const char* restrict str,
	       const char* restrict space, const char* restrict point);


/* TODO machineint */
char* machineint(intmax_t* restrict r, const char* restrict str)
  __GCC_ONLY(__attribute__((__warn_unused_result__)));
# ifdef __CONST_CORRECT
#  define machineint(...)  (__const_correct_2p(machineint, __VA_ARGS__))
# endif

/* TODO machineuint */
char* machineuint(uintmax_t* restrict r, const char* restrict str)
  __GCC_ONLY(__attribute__((__warn_unused_result__)));
# ifdef __CONST_CORRECT
#  define machineuint(...)  (__const_correct_2p(machineuint, __VA_ARGS__))
# endif
#endif

/* TODO machinefloat */
int machinefloat(long double* restrict r, const char* restrict str,
		 const char* restrict space, const char* restrict comma);
#ifdef __CONST_CORRECT
# define machinefloat(...)  (__const_correct_2(machinefloat, __VA_ARGS__))
#endif


/**
 * Parse an escaped string.
 * 
 * Supported escapes:
 *   \' \" \$ \& \? \\ \/ \### \a \b \e \f \n
 *   \r \t \s \u#### \u{#…} \U######## \v \x##
 *   \^@…\^_
 *   \NUL \SOH \STX \ETX \EOT \ENQ \ACK \BEL \BS \HT
 *   \LF \VT \FF \CR \SO \SI \DLE \DC1 \DC2 \DC3 \DC4
 *   \NAK \SYN \ETB \CAN \EM \SUB \ESC \FS \GS \RS
 *   \US \SP \DEL
 * 
 * Unsupported escapes:
 *   \N{character name}
 * 
 * @param   str   The escaped string, may be edited, may be `NULL`.
 *                Must not be reused on error.
 * @param   mode  How unrecognised escapes should be handled,
 *                and other configurations, 0 for default.
 * @return        The new end of `str` is returned. `NULL` is returned
 *                on error or if `str` is `NULL`.
 * 
 * @throws  0       `str` is `NULL`.
 * @throws  EINVAL  If `mode` is invalid.
 * @throws  EINVAL  If `str` is invalid and `mode & UNESCAPE_EINVAL`.
 */
char* unescape(char*, enum unescape_mode);

/**
 * Escapes a string.
 * 
 * @param   str    The unescaped string, may be `NULL`.
 * @param   quote  The queue character, must be either ', "
 *                 or a NUL-character (for no surrounding quotes).
 *                 Note, these quotes are not added to output.
 * @return         Escaped variant of the string, `NULL`.
 *                 You are responsible for deallocating the
 *                 returned pointer.
 * 
 * @throws  0       `str` is `NULL`.
 * @throws  EINVAL  If `quote` is invalid.
 * @throws  ENOMEM  The process cannot allocate more memory.
 */
char* escape(const char* restrict)
  __GCC_ONLY(__attribute__((__malloc__, __warn_unused_result__)));



#endif
#endif

