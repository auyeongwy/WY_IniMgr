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
 * @param p_max_size Maximum sie of the file to process. E.g. 1024*1024 will limit the file to 1MB.
 * @param p_buffer_len Returns the length of the content read from the file.
 * @param p_buffer Returns a buffer of size p_buffer_len with content read from the file. Pass in an uninitialised pointer address here and the buffer will be dynamically allocated with the value of p_max_size. Hence it is necessary to deallocate this buffer once all parsing operations are completed.  
 * @return 0 if success. -1 if error encountered.
 */
int wyini_read_file(const char *restrict const p_file, const unsigned int p_max_size, unsigned int *restrict p_buffer_len, char ** p_buffer);


/**
 * Saves data into a file, overwriting it if it exists. 
 * @param p_file The file name.
 * @param p_buffer_len Size of the content to write to file.
 * @param p_buffer THe data to write.
 * @return 0 if success, else -1.
 */
int wyini_save_file(const char *restrict const p_file, const unsigned int p_buffer_len, const char *restrict const p_buffer);

#endif