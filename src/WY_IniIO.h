/**
 * @file WY_IniIO.h
 * Declares functions for performing IO operations.
*/
#ifndef _WY_INIIO_H_
#define _WY_INIIO_H_

/**
 * Opens a file and reads the content into a buffer. Then closes the file. Note that the buffer is dynamically allocated by this function, so it is necessary for the caller to deallocate the buffer.
 * Example Usage: <br>
 * @code
 * unsigned int buffer_len;
 * unsigned char *buffer;
 * 
 * if(wyini_read_file("filename", 1024, &buffer_len, &buffer) == 0) 
 *  printf("Read %u chars.\nContent: %s\n", buffer_len, buffer);
 * // process buffer here
 * // .....
 * free(buffer); // Done processing, remember to deallocate memory.
 *  
 * @endcode
 * @param p_file The file to open.
 * @param p_max_size Maximum size of the file to process. The max size that is read is p_max_size-1. E.g. 1024*1024 will limit the file to 1MB and the total size that is read is (1024*1024)-1. If the file's content exceeds this size then this function will return exit and return failure.
 * @param p_buffer_len Returns the length of the content read from the file.
 * @param p_buffer Returns a buffer of size p_buffer_len with content read from the file. Pass in an uninitialised pointer address here and the buffer will be dynamically allocated with the value of p_max_size. Hence it is necessary to deallocate this buffer once all parsing operations are completed. The max size is determined by p_max_size and only p_max_size-1 will be read into the buffer. p_buffer[p_max_size-1] will be set to 0.
 * @return WYINI_OK if success. Else negative value defined in WY_IniDefs.h if error encountered. 
 */
int wyini_read_file(const char *restrict const p_file, const unsigned int p_max_size, unsigned int *restrict p_buffer_len, char *restrict *restrict p_buffer);


/**
 * Saves data into a file, overwriting it if it exists. 
 * @param p_file The file name.
 * @param p_buffer_len Size of the content to write to file.
 * @param p_buffer THe data to write.
 * @return WYINI_OK if success. Else negative value defined in WY_IniDefs.h if error encountered.
 */
int wyini_save_file(const char *restrict const p_file, const unsigned int p_buffer_len, const char *restrict const p_buffer);

#endif