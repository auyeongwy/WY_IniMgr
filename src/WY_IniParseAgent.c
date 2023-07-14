/**
 * @file WY_IniParseAgent.c
*/

#include <string.h>
#include "WY_IniParseAgent.h"


void wyini_get_nextline(const unsigned int p_start_offset, unsigned int *restrict p_end_offset, struct S_wyini_buffer *restrict p_wyini_buffer)
{
    const unsigned int max_len = p_wyini_buffer->m_buffer_len;
    char *restrict buffer = p_wyini_buffer->m_buffer;

    *p_end_offset = p_start_offset;
    do {
        if((buffer[*p_end_offset]=='\n') || (buffer[*p_end_offset]==0))  /* Found the "\n". or terminating char. */ 
            return;
    } while ((*p_end_offset += 1) < max_len);

    *p_end_offset -= 1; /* Exceeded end of the buffer. Retrace. */
}



int wyini_find_var_val_inline(const bool p_var_only, const unsigned int p_start_offset, const unsigned int p_end_offset, const unsigned int p_var_len, const char *restrict const p_var, unsigned int *restrict p_return_offset, struct S_wyini_buffer *restrict p_wyini_buffer)
{
    char *restrict buffer = p_wyini_buffer->m_buffer;

    if(strncmp(buffer + p_start_offset, p_var, p_var_len) != 0) /* Cannot match the var so exit. */
        return WYINI_NOT_FOUND;

    int i = p_start_offset + p_var_len; /* Move index to right after the 'var' pattern found. */
    while(i<p_end_offset) { /* Try to match the " =" pattern after the var. */
        if(buffer[i]==' ') /* Skip any whitespace. */
            ++i;
        else if(buffer[i]=='=') { /* Found '='. Break the loop. */
            ++i;
            break;
        }
        else /* Found something that isn't a whitespace or '=' after the 'var' pattern. Exit. */
            return WYINI_NOT_FOUND;
    }

    if(p_var_only) { /* Only searching for the pattern 'var='. The 'val' that comes after is not required. */
        *p_return_offset = i;
        return WYINI_OK;
    }

    while(i<p_end_offset) { /* Skip any whitespace after the ' =' pattern. */
        if(buffer[i]==' ')
            ++i;
        else
            break;
    }

    if(i<p_end_offset) { /* Get the value assigned to the var. */
        *p_return_offset = i;
        return WYINI_OK;
    }

    return WYINI_NOT_FOUND;
}