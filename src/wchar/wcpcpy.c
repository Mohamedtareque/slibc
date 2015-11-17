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
#include <wchar.h>



/**
 * Copy a memory segment to another, non-overlapping, segment,
 * stop when a NUL wide character is encountered.
 * 
 * This is a GNU-compliant slibc extension.
 * 
 * @param   whither  The destination memory segment.
 * @param   whence   The source memory segment.
 * @return           `whither + wcslen(whence)` is returned.
 */
wchar_t* wcpcpy(wchar_t* restrict whither, const wchar_t* restrict whence)
{
  return wmempcpy(whither, whence, wcslen(whence) + 1) - 1;
}

