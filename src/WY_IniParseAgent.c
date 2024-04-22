/**
 * @file WY_IniParseAgent.c
*/

#include <string.h>
#include "WY_IniParseAgent.h"


unsigned int wyini_get_nextline(const unsigned int p_start_offset, unsigned int *restrict p_end_offset, struct S_wyini_buffer *restrict p_wyini_buffer)
{
    const unsigned int max_len = p_wyini_buffer->m_buffer_len;
    char *restrict buffer = p_wyini_buffer->m_buffer;
    unsigned int end_offset = p_start_offset;
    unsigned int nextline_len = 0;

    do {
        if((buffer[end_offset]=='\n') || (buffer[end_offset]=='\0')) { /* Found '\n' or a terminating char. */ 
            if(buffer[end_offset-1]=='\r') {/* If Windows style formatting, need to exclude the '\r' as well. */
                --end_offset;
                nextline_len = 2; /* Found nextline is '\r\n' */
            } else
                nextline_len = 1; /* Found nextline is '\n' */
            break;
        }
    } while ((end_offset += 1) < max_len);

    *p_end_offset = --end_offset; /* Retrace to pinpoint the offset index before nextline indicator or end of buffer. */
    return nextline_len;
}



int wyini_find_var_val_inline(const bool p_var_only, const unsigned int p_start_offset, const unsigned int p_end_offset, const unsigned int p_var_len, const char *restrict const p_var, unsigned int *restrict p_return_offset, struct S_wyini_buffer *restrict p_wyini_buffer)
{
    char *restrict buffer = p_wyini_buffer->m_buffer;
    int return_val = WYINI_NOT_FOUND;

    if(strncmp(buffer + p_start_offset, p_var, p_var_len) != 0) /* Cannot match the var so exit. */
        return WYINI_NOT_FOUND;

    unsigned int i = p_start_offset + p_var_len; /* Move index to right after the 'var' pattern found. */
    while(i<=p_end_offset) { /* Try to match the " =" pattern after the var. */
        if(buffer[i]==' ') /* Skip any whitespace. */
            ++i;
        else if(buffer[i]=='=') { /* Found '='. Break the loop. */
            return_val = WYINI_VAL_NOT_FOUND; /* Indicates that we at least found the 'var=' pattern. */
            ++i;
            break;
        }
        else /* Found something that isn't a whitespace or '=' after the 'var' pattern. Exit. */
            return WYINI_NOT_FOUND;
    }

    if(i > p_end_offset) /* Make sure we do not exceed the nextline bounds. */
        return return_val;

    if(p_var_only) { /* Only searching for the pattern 'var='. The 'val' that comes after is not required. */
        *p_return_offset = i;
        return WYINI_OK;
    }

    while(i<=p_end_offset) { /* Skip any whitespace after the '=' pattern. */
        if(buffer[i]==' ')
            ++i;
        else
            break;
    }

    if(i<=p_end_offset) { /* Get the value assigned to the var. */
        *p_return_offset = i;
        return WYINI_OK;
    } else
        return WYINI_VAL_NOT_FOUND;
}