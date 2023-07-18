/**
 * @file WY_IniWriteAgent.c
*/

#include <string.h>
#include "WY_IniWriteAgent.h"


unsigned int wyini_remove_ending_whitespace(const unsigned int p_start_offset, unsigned int p_end_offset, struct S_wyini_buffer *restrict p_wyini_buffer)
{
    char *restrict buffer = p_wyini_buffer->m_buffer;
    while((buffer[p_end_offset] == ' ') && (p_end_offset > p_start_offset))
        --p_end_offset;
    return p_end_offset;
}



int wyini_write_val_inline(const unsigned int p_start_offset, const unsigned int p_end_offset, const unsigned int p_val_len, const char *restrict const p_val, struct S_wyini_buffer *restrict p_wyini_buffer)
{
    const unsigned int current_space = p_end_offset - p_start_offset + 1; /* How much space between '=' and '\n' or '\0'. */
    char *restrict buffer = p_wyini_buffer->m_buffer;
    
    if(current_space >= p_val_len) { /* Enough space to fit in the new variable. */ 
        if(current_space > p_val_len) {
            memmove(buffer + p_start_offset + p_val_len, buffer + p_end_offset + 1, p_wyini_buffer->m_buffer_len - p_end_offset); /* Move existing content in m_buffer to fill in the space. No need for this step if current_space == p_val_len. */
            p_wyini_buffer->m_buffer_len -= (current_space - p_val_len); /* Current buffer size decreased. Update it. */ 
        }
        memcpy(buffer + p_start_offset, p_val, p_val_len); /* Write the new variable. */
    }

    else { /* Not enough space to fit in the new variable. Additional checks to conduct. */
        const unsigned int additional_space = p_val_len - current_space;
        if(additional_space + p_wyini_buffer->m_buffer_len >= p_wyini_buffer->m_max_file_size) /* Writing the new var exceeds max allowed file content size. Return failure. */
            return WYINI_MEMORY_ERR;

        memmove(buffer + p_start_offset + p_val_len, buffer + p_end_offset + 1, p_wyini_buffer->m_buffer_len - p_end_offset); /* Move existing content in m_buffer to make space. */
        memcpy(buffer + p_start_offset, p_val, p_val_len); /* Write the new variable. */
    }

    return WYINI_OK;
}