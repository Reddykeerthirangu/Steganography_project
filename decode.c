#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "decode.h"
#include "types.h"
#include "common.h"

// Reads and validates input stego image file and output secret filename
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    // Check if the input file extension is a valid BMP format
    if(strstr(argv[2],".bmp")!=NULL)
    {
        decInfo->stego_file=argv[2]; // Save to stego image path
    }
    else
    {
        printf(RED"\nERROR: Output image must be a .bmp file\n"RESET);
        return e_failure; // Return failure if file format is incorrect
    }
    // Check if user passed optional argument for the output secret file
    if (argv[3] != NULL)
    {
        strtok(argv[3],".");
        strcpy(decInfo->secret_file,argv[3]); // Safely copy user's filename to buffer
    }
    else
    {
        strcpy(decInfo->secret_file,"decoded"); // Fallback to default name if omitted
    }
    return e_success; // Return success status on valid inputs
}

// Opens the input stego BMP image in binary read mode
Status open_output_image_file(DecodeInfo *decInfo)
{
    decInfo->fp_stego=fopen(decInfo->stego_file,"rb"); // Open stego image file
    if (decInfo->fp_stego== NULL)
    {
    	perror("fopen"); // Print system error description
    	fprintf(stderr, "\nERROR: Unable to open file %s\n",decInfo->stego_file);
    	return e_failure; // Return failure if open fails
    }
    return e_success; // Return success if file opened correctly
}

// Skips 54-byte BMP header to reach pixel data
Status skip_header(FILE *fp_stego)
{
    fseek(fp_stego,54,SEEK_SET); // Position file cursor past BMP header
    return e_success; // Return success status
}

// Reconstructs one byte of message data from the LSBs of 8 image bytes
char decode_byte_from_lsb(unsigned char *arr)
{
    char ch = 0,ch1; // Initialize loop storage 
    for (int i = 0; i < 8; i++)
    {
        ch1 = arr[i]&1;
        ch = ch|ch1<<i; 
    }
    return ch; 
}
Status decode_string_from_image(int size,DecodeInfo *decInfo,char *arr)
{
    int i;
    char buffer[8]; // Buffer to capture raw pixel data
    for (i=0;i<size;i++)
    {
        /* Read 8 bytes from stego image */
        if (fread(buffer, 1, 8, decInfo->fp_stego) != 8)
        {
            printf(RED"\nERROR: Unable to read magic string\n"RESET);
            return e_failure;
        }
        /* Decode one character from 8 LSBs */
        arr[i] = decode_byte_from_lsb(buffer); 
    }
    arr[i] = '\0'; // Properly terminate the magic string array
    return e_success;
}
// Extracts and verifies the magic string from stego file
Status decode_magic_string(DecodeInfo *decInfo)
{
    if(decode_string_from_image(strlen(MAGIC_STRING),decInfo,decInfo->magicstring)==e_failure)
    {
        return e_failure;
    }
    /* Validate magic string */
    if (strcmp(decInfo->magicstring, "#*") == 0)
    {
        printf(GREEN"\nMagic string matched successfully\n"RESET);
        return e_success; 
    }
    else
    {
        printf(RED"\nERROR: Magic string mismatch\n"RESET);
        return e_failure; // Failure
    }
}

// Reconstructs a full 32-bit integer from the LSBs of 32 image bytes
int decode_int_from_lsb(unsigned char *arr)
{
    int ch = 0,ch1; // Initialize loop calculation stores
    for (int i = 0; i < 32; i++)
    {
        ch1 = arr[i]&1; 
        ch = ch|ch1<<i; 
    }
    return ch; // Return reconstituted 32-bit integer value
}

Status decode_size(long int *data,FILE *fp_stego)
{
    int size; // Variable to store integer value 
    char arr[32]; // Read track for 32 image byte matrix
    if(fread(arr,1,32,fp_stego)!=32)
    {
        return e_failure; 
    }
    size=decode_int_from_lsb(arr); 
    *data=size; // store data in structure
}

// Decodes the size of extension 
Status decode_size_of_extn(DecodeInfo *decInfo)
{
    if(decode_size(&(decInfo->size_extn),decInfo->fp_stego)==e_failure)
    {
        return e_failure;
    }
    return e_success; // Return success status
}

// Decodes the extension string name from the stego image
Status decode_extension(DecodeInfo *decInfo)
{
    if (decode_string_from_image(decInfo->size_extn, decInfo, decInfo->extension) == e_failure)
    {
        printf(RED "\nERROR: Unable to read extension string\n" RESET);
        return e_failure;
    }
    strcat(decInfo->secret_file,decInfo->extension);
    return e_success; // Return success status
}

// Decodes the size of the data
Status decode_size_of_data(DecodeInfo *decInfo)
{
    if(decode_size(&(decInfo->size_data),decInfo->fp_stego)==e_failure)
    {
        return e_failure;
    }
    return e_success; // Return success status
}
Status open_secret_file(DecodeInfo *decInfo)
{
    decInfo->fptr_secret=fopen(decInfo->secret_file,"wb");   // open the secret file in write mode
    if(decInfo->fptr_secret == NULL)
    {
        printf(RED"\nERROR: Unable to create secret file\n"RESET);
        return e_failure; 
    }
    return e_success;
}
// Decodes the secret data into secret file
Status decode_secret_data(DecodeInfo *decInfo)
{
    if(open_secret_file(decInfo)==e_failure)
    {
        printf(RED"\nERROR: unable to open the file to store secret data\n"RESET);
        return e_failure;
    }
    printf(GREEN"\nSuccessfully opened the file to store secret data\n"RESET);
    char *data=malloc(decInfo->size_data+1);
    if(data==NULL)
    {
        printf(RED "\nERROR: memory is not allocated\n" RESET);
        fclose(decInfo->fptr_secret);
        return e_failure;
    }
    if(decode_string_from_image(decInfo->size_data, decInfo, data)==e_failure)
    {
        printf(RED "\nERROR: Unable to decode secret data from image\n" RESET);
        free(data);
        fclose(decInfo->fptr_secret);
        return e_failure;
    }
    if(fwrite(data, 1, decInfo->size_data, decInfo->fptr_secret)!=decInfo->size_data) 
    {
        printf(RED "\nERROR: Unable to write data to secret file\n" RESET);
        free(data);
        fclose(decInfo->fptr_secret);
        return e_failure; 
    }
    free(data);
    return e_success; // Return operation execution success
}

// Decoding process
Status do_decoding(DecodeInfo *decInfo)
{
    if(open_output_image_file(decInfo)==e_failure)
    {
        printf(RED"\nERROR: Unable to open the input stego image file\n"RESET);
        return e_failure; // failure
    }
    else
    {
        printf(GREEN"\nSuccessfully opened the input stego image file\n"RESET);
    }
    if(skip_header(decInfo->fp_stego)==e_failure)
    {
        printf(RED"\nERROR: Unable to read and skip the header\n"RESET);
        return e_failure; // failure
    }
    else
    {
        printf(GREEN"\nSuccessfully skip the header\n"RESET);
    }
    if(decode_magic_string(decInfo)==e_failure)
    {
        printf(RED"\nERROR: Unable to decode the magic string\n"RESET);
        return e_failure; // failure
    }
    else
    {
        printf(GREEN"\nSuccessfully decoded the magic stirng \n"RESET);
    }
    if(decode_size_of_extn(decInfo)==e_failure)
    {
        printf(RED"\nERROR: Unable to decode the size of the extension\n"RESET);
        return e_failure; // failure
    }
    else
    {
        printf(GREEN"\nSuccessfully decoded the size of the extension\n"RESET);
    }
    if(decode_extension(decInfo)==e_failure)
    {
        printf(RED"\nERROR: Unable to decode the extension\n"RESET);
        return e_failure; // failure
    }
    else
    {
        printf(GREEN"\nSuccessfully decoded the extension\n"RESET);
    }
    if(decode_size_of_data(decInfo)==e_failure)
    {
        printf(RED"\nERROR: Unable to decode the size of secret data\n"RESET);
        return e_failure; // failure
    }
    else
    {
        printf(GREEN"\nSuccessfully decoded the size of secret data\n"RESET);   
    }
    if(decode_secret_data(decInfo)==e_failure)
    {
        printf(RED"\nERROR: Unable to decode the secret data\n"RESET);
        return e_failure; // failure
    }
    else
    {
        printf(GREEN"\nSuccessfully decoded the secret data\n"RESET);
    }
    fclose(decInfo->fp_stego);
    fclose(decInfo->fptr_secret); // Closes the secret file
    return e_success; 
}