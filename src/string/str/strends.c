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
#include <string.h>



/**
 * Check whether a string ends with a specific string.
 * This check is case sensitive.
 * 
 * This is a slibc extension.
 * 
 * @param   string   The string to inspect.
 * @param   desired  The desired ending of the string.
 * @return           The `string`, where `desired` beings if
 *                   `string` ends with `desired`, `NULL` otherwise.
 */
char* (strends)(const char* string, const char* desired)
{
  size_t n = strlen(string);
  size_t m = strlen(desired);
  if (n < m)
    return NULL;
  return (memcmp)(string + (n - m), desired, m) ? NULL : (string + n);
}
