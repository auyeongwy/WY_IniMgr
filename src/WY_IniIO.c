#include <stdio.h>
#include <stdlib.h>
#include "WY_IniIO.h"
#include "WY_IniDefs.h"


int wyini_read_file(const char *restrict const p_file, const unsigned int p_max_size, unsigned int *restrict p_buffer_len, char *restrict *restrict p_buffer)
{
    int return_val = WYINI_IO_ERR;

    FILE *fp = NULL;
    if((fp = fopen(p_file, "rb")) == NULL) /* Open file for reading. */     
        return return_val;
    if(fseek(fp, 0, SEEK_END) != 0) /* Goto end of file. */
        goto bad_exit;

    const long tmp = ftell(fp); /* Get size of content in file. */
    if((tmp<1) || (tmp>p_max_size)) /* Exit if too small or larger than the defined maximum. */
        goto bad_exit;
    else
        *p_buffer_len = tmp;
    rewind(fp); /* Return to start of file. */

    if((*p_buffer = (char*)malloc(p_max_size)) == NULL) { /* Create buffer to read the data. This can be larger than p_buffer_len. */
        return_val = WYINI_MEMORY_ERR;
        goto bad_exit;
    }

    if(fread(*p_buffer, 1, *p_buffer_len, fp) != *p_buffer_len) /* Read content into m_buffer. */
        goto bad_exit;

    fclose(fp);
    return WYINI_OK;

bad_exit:
    fclose(fp);
    if(*p_buffer != NULL) {
        free(*p_buffer);
        *p_buffer = NULL;
    }
    return return_val;
}



int wyini_save_file(const char *restrict const p_file, const unsigned int p_buffer_len, const char *restrict const p_buffer)
{
    int return_val = WYINI_IO_ERR;

    FILE *fp = NULL;
    if((fp = fopen(p_file, "wb")) == NULL) /* Create file for writing, overwriting where necessary. */
        return return_val;

    if(fwrite(p_buffer, 1, p_buffer_len, fp) > 0) /* Writes data to file. */
        return_val = WYINI_OK;

    fclose(fp);
    return return_val;
}
