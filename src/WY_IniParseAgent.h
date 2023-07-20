/**
 * @file WY_IniParseAgent.h
 * Declares functions for performing parsing and read operations.
*/

#ifndef _WY_INIPARSEAGENT_H_
#define _WY_INIPARSEAGENT_H_

#include <stdbool.h>
#include "WY_IniDefs.h"


/**
 * Get the next line in the internal buffer. If the buffer ends without a next line this returns the index of the byte before the terminating char or simply the last char in the provided buffer. Currently this function looks for '\n' or '\r\n'.
 * .
 * @param p_start_offset Position in S_wyini_buffer to start parsing.
 * @param p_end_offset Returns position of the byte before the next line or terminating character found.
 * @param p_wyini_buffer The internal buffer to search.
 * @return Size of the nextline character. Possible values are: 1 for '\n'. 2 for Windows-style '\r\n'. 0 if we hit the end of the provided S_wyini_buffer without any nextline or terminating indicator.
 */
unsigned int wyini_get_nextline(const unsigned int p_start_offset, unsigned int *restrict p_end_offset, struct S_wyini_buffer *restrict p_wyini_buffer);


/**
 * Find the value assigned to a variable in a line of characters in the pattern 'var=val' within the internal buffer provided by S_wyini_buffer.
 * @param p_var_only If set to true, this function only searches for the 'var=' pattern and returns p_return_offset as the index after 'var='. If set to false, this function searches for the pattern 'var=val' and returns p_return_offset as the index after 'var=val'.  
 * @param p_start_offset The starting offset in the line of characters to parse in S_wyini_buffer. 
 * @param p_end_offset The ending offset in the line of characters to parse in S_wyini_buffer. 
 * @param p_var_len The length of the variable to match.
 * @param p_var The variable to match.
 * @param p_return_offset Returns the offset pointing to the start of the value as selected by the value in p_var_only.
 * \return WYINI_OK if success. Else a negative value defined in WY_IniDefs.h.
 */
int wyini_find_var_val_inline(const bool p_var_only, const unsigned int p_start_offset, const unsigned int p_end_offset, const unsigned int p_var_len, const char *restrict const p_var, unsigned int *restrict p_return_offset, struct S_wyini_buffer *restrict p_wyini_buffer);

#endif