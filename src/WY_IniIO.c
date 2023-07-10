#include <stdio.h>
#include <stdlib.h>
#include "WY_IniIO.h"


int wyini_read_file(const char *restrict const p_file, const unsigned int p_max_size, unsigned int *restrict p_buffer_len, char ** p_buffer)
{
    FILE *fp = NULL;
    if((fp = fopen(p_file, "r")) == NULL) /* Try to open file for reading. */     
        return -1;

    if(fseek(fp, 0, SEEK_END) != 0) /* Goto end of file. */
        goto bad_exit;

    long tmp = ftell(fp); /* Get size of file. */
    if((tmp<1) || (tmp>p_max_size)) /* Exit if too small or larger than the defined maximum. */
        goto bad_exit;
    else
        *p_buffer_len = tmp;
    rewind(fp); /* Return to start of file. */

    if((*p_buffer = (char*)malloc(*p_buffer_len)) == NULL) /* Create buffer to read the data. */
        goto bad_exit;

    fread(*p_buffer, *p_buffer_len, 1, fp); /* Read all data into m_buffer. */
    if(ferror(fp) != 0)
        goto bad_exit;

    fclose(fp);
    return 0;

bad_exit:
    fclose(fp);
    if(*p_buffer != NULL) {
        free(*p_buffer);
        *p_buffer = NULL;
    }
    return -1;
}
