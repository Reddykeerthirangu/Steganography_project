#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

// Structure to store information required for the decoding process
typedef struct _DecodeInfo
{
    /* Secret File Info */
    char secret_file[200]; // output secret file name
    FILE *fptr_secret;     // File pointer for output secret file
    long size_extn;        // Size of secret file extension
    char extension[100];   // decoded file extension
    long size_data;        // Size secret data
    char magicstring[3];

    /* Stego Image Info */
    char *stego_file;      // Pointer to input stego image file name
    FILE *fp_stego;        // File pointer for input stego image

} DecodeInfo;

Status decode_size(long int *data,FILE *fp_stego);

// Reads and validates command-line arguments for decoding
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

// Decoding process
Status do_decoding(DecodeInfo *decInfo);

// Opens the input stego image file 
Status open_output_image_file(DecodeInfo *decInfo);

// Opens the scret file
Status open_secret_file(DecodeInfo *decInfo);

// Skips the BMP image header
Status skip_header(FILE *fp_stego);

// Decodes one byte of data from 8 bytes of image LSBs
char decode_byte_from_lsb(unsigned char *buffer);

// Decodes string from the image
Status decode_string_from_image(int size,DecodeInfo *decInfo,char *arr);

// Decodes and compare magic string
Status decode_magic_string(DecodeInfo *decInfo);

// Integer from 32 bytes of image LSBs
int decode_int_from_lsb(unsigned char *arr);

// Decodes the size of the secret file extension
Status decode_size_of_extn(DecodeInfo *decInfo);

// Decodes the secret file extension
Status decode_extension(DecodeInfo *decInfo);

// Decodes the size of the secret data 
Status decode_size_of_data(DecodeInfo *decInfo);

// Decodes secret data and writes it into the output file
Status decode_secret_data(DecodeInfo *decInfo);

#endif
