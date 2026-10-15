#ifndef ENCODE_H
#define ENCODE_H

#include "types.h" // Contains user defined types

// Structure to store information required for the encoding process
#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4 // Maximum characters allowed for file extension tracking

typedef struct _EncodeInfo
{
    /* Source Image info */
    char *src_image_fname; // Pointer to input source image filename
    FILE *fptr_src_image;  // File pointer for input source image

    /* Secret File Info */
    char *secret_fname;                    // Pointer to input secret filename
    FILE *fptr_secret;                     // File pointer for input secret file
    char extn_secret_file[MAX_FILE_SUFFIX]; // Buffer to store secret file extension
    char secret_data[MAX_SECRET_BUF_SIZE];  // Buffer for raw secret data reads
    long size_secret_file;                 // Size of target secret file in bytes

    /* Stego Image Info */
    char *stego_image_fname; // Pointer to output stego image filename
    FILE *fptr_stego_image;  // File pointer for output stego image

} EncodeInfo;


/* Encoding function prototype */

// Checks the command-line argument to determine operation mode
OperationType check_operation_type(char *argv[]);

// Reads and validates encoding options provided via arguments
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo);

// Encoding process
Status do_encoding(EncodeInfo *encInfo);

// Opens input and output files in binary mode
Status open_files(EncodeInfo *encInfo);

// Verifies if source image has enough capacity to hide secret data
Status check_capacity(EncodeInfo *encInfo);

// Computes size of the image file
uint get_image_size_for_bmp(FILE *fptr_image);

//copies the bmp header
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image);

// Encodes magic string to image
Status encode_magic_string(char *magic_string, EncodeInfo *encInfo);

// Encodes string to image
Status encode_string_to_image(char *str, int size, EncodeInfo *encInfo);

// Encodes integer to image
Status encode_int_to_image(int data,EncodeInfo *encInfo);

// Encodes the size of the extension
Status encode_size_of_extn(EncodeInfo *encInfo);

// Encodes the secret file extension
Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo);

// Encodes the secret file size
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo);

// Encodes the secret data into image pixels
Status encode_secret_file_data(EncodeInfo *encInfo);

// Encodes one byte of raw data across 8 target pixel LSBs
void encode_byte_to_lsb(char data, char *image_buffer);

// Copies remaining image data
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest);

#endif
