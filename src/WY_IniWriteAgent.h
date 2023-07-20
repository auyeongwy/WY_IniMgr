/**
 * @file WY_IniWriteAgent.h
 * Declares functions for performing write operations.
*/

#ifndef _WY_INIWRITEAGENT_H_
#define _WY_INIWRITEAGENT_H_

#include "WY_IniDefs.h"

/**
 * Removes trailing whitespace from the variable value within a start and end index within the provided S_wyini_buffer.
 * @param p_start_offset The starting index in S_wyini_buffer pointing to a variable.
 * @param p_end_offset The ending index in S_wyini_buffer that points to a variable.
 * @param p_wyini_buffer The S_wyini_buffer to operate on.
 * @return A new p_end_offset that excludes any trailing whitespace originally found before p_end_offset.
 */
unsigned int wyini_remove_ending_whitespace(const unsigned int p_start_offset, unsigned int p_end_offset, struct S_wyini_buffer *restrict p_wyini_buffer);


/**
 * Writes a value into a specified offset into a line within the internal buffer S_wyini_buffer.
 * @param p_start_offset Index position in S_wyini_buffer pointing at the position before the 'var=' pattern.
 * @param p_end_offset Index position in S_wyini_buffer pointing at the byte before nextline or terminating indicator after the 'var=val' pattern.
 * @param p_val_len Length of the value to write.
 * @param p_val The value to write.
 * @param p_wyini_buffer The S_wyini_buffer to operate on.
 * @return WYINI_OK if success. Else a negative value defined in WY_IniDefs.h.
*/
int wyini_write_val_inline(const unsigned int p_start_offset, const unsigned int p_end_offset, const unsigned int p_val_len, const char *restrict const p_val, struct S_wyini_buffer *restrict p_wyini_buffer);

#endif
