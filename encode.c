#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "encode.h"
#include "types.h"
#include "common.h"

// Extracts the pixel capacity from the input BMP file
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height; 
    rewind(fptr_image); 
    fseek(fptr_image, 18, SEEK_SET); 

    fread(&width, sizeof(int), 1, fptr_image); // Read width from file
    printf("\nwidth = %u\n", width);

    fread(&height, sizeof(int), 1, fptr_image); // Read height from file
    printf("height = %u\n", height);

    rewind(fptr_image); 
    return width * height * 3; // Calculates the size of pixel data
}

// opens the files for read and write operations
Status open_files(EncodeInfo *encInfo)
{
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "rb"); // Opens source image file
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen"); 
    	fprintf(stderr, "\nERROR: Unable to open file %s\n", encInfo->src_image_fname);
    	return e_failure; 
    }

    encInfo->fptr_secret = fopen(encInfo->secret_fname, "rb");
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "\nERROR: Unable to open file %s\n", encInfo->secret_fname);
    	return e_failure; 
    }

    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "wb"); // Open production container file handle
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen"); 
    	fprintf(stderr, "\nERROR: Unable to open file %s\n", encInfo->stego_image_fname);
    	return e_failure; 
    }

    return e_success; 
}

// Validates command-line formats and stores in structure members
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    if (strstr(argv[2], ".bmp") != NULL)
    {
        encInfo->src_image_fname = argv[2]; // Storing valid argument of source image file in structure
    }
    else
    {
        printf(RED"\nERROR: Source image must be a .bmp file\n"RESET);
        return e_failure; //failure
    }
    
    char *dot = strchr(argv[3], '.'); // checking whether dot is present in secret argument or not
    if (dot != NULL && *(dot + 1) != '\0')
    {
        encInfo->secret_fname = argv[3]; // Storing valid argument of secre file in structure
        strcpy(encInfo->extn_secret_file,dot); // Storing extension in structure
    }
    else
    {
        printf(RED"\nERROR: Secret file must have an extension\n"RESET);
        return e_failure; 
    }
    
    if(argv[4]!=NULL)
    {
        //validating the optional argument as outuput image file with .BMP extension
        if (strstr(argv[4], ".bmp") != NULL)
        {
            encInfo->stego_image_fname = argv[4]; // Storing valid argument of output image file in structure
        }
        else
        {
            printf(RED"\nERROR: Output image must be a .bmp file\n"RESET);
            return e_failure;
        }
    }
    else
    {
        encInfo->stego_image_fname ="stego.bmp"; // giving default name for  output image file
    }
    return e_success; 
}

// Comparing the capacity of image file with secret file 
Status check_capacity(EncodeInfo *encInfo)
{
    fseek(encInfo->fptr_secret,0,SEEK_END);
    encInfo->size_secret_file=ftell(encInfo->fptr_secret); 
    rewind(encInfo->fptr_secret);
    
    //calculating the overall data which needs to be encoded in output image file
    uint size_of_info=strlen(MAGIC_STRING)+sizeof(int)+strlen(encInfo->extn_secret_file)+sizeof(int)+encInfo->size_secret_file;
    uint image_size=get_image_size_for_bmp(encInfo->fptr_src_image);
    rewind(encInfo->fptr_src_image); 
    //Comparing the capacity of image file with secret file 
    if(image_size > (size_of_info*8))
    {
        return e_success; 
    }
    
    return e_failure; 
}

// Copies the BMP header to ouput image file
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    char arr[54]; 
    rewind(fptr_src_image);
    if(fread(arr,54,1,fptr_src_image)!=1)
    {
        return e_failure; 
    }
    if(fwrite(arr,54,1,fptr_dest_image)!=1)
    {
        return e_failure; 
    }
    return e_success; 
}

// Encodes one byte of message data to LSBs of 8 image bytes
void encode_byte_to_lsb(char data, char *arr)
{
    for(int i=0;i<8;i++)
    {
        arr[i]=arr[i]&(~1); 
        arr[i]=arr[i] | ((data &(1<<i))>>i); 
    }
}

// Encodes string to image
Status encode_string_to_image(char *str, int size, EncodeInfo *encInfo)
{
    for(int i=0;i<size;i++)
    {
        unsigned char arr[8]; 
        if(fread(arr,8,1,encInfo->fptr_src_image)!=1)
        {
            return e_failure;
        }
        encode_byte_to_lsb(str[i],(char*)arr);//calling function for 1 byte encoding
        if(fwrite(arr,8,1,encInfo->fptr_stego_image)!=1)
        {
            return e_failure; 
        }
    }
    return e_success; 
}

//Encodes the magic string into ouput image file
Status encode_magic_string(char *magic_string, EncodeInfo *encInfo)
{
    if(encode_string_to_image(magic_string,strlen(magic_string),encInfo)==e_success)
    {
        return e_success; 
    }
    return e_failure; 
}

// Encodes the integer value to output image file
Status encode_int_to_image(int data,EncodeInfo *encInfo)
{
    unsigned char arr[32]; 
    if (fread(arr, 1, 32, encInfo->fptr_src_image) != 32)
    {
        return e_failure; 
    }
    for(int i = 0; i < 32; i++)
    {
        char get=(data&1<<i)>>i; 
        arr[i] = arr[i] & (~1); 
        arr[i] = arr[i] | get; 
    }
    if (fwrite(arr, 1, 32, encInfo->fptr_stego_image) != 32)
    {
        return e_failure; 
    }
    return e_success; 
}

// Encodes the size of extension
Status encode_size_of_extn(EncodeInfo *encInfo)
{
    if(encode_int_to_image(strlen(encInfo->extn_secret_file),encInfo)==e_failure)
    {
        return e_failure; 
    }
    return e_success; 
}

// Encodes secret file extension into output image file
Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo)
{
    if(encode_string_to_image(file_extn,strlen(file_extn),encInfo)==e_failure)
    {
        return e_failure; 
    }
    return e_success; 
}

// Encodes size of secret data from secret file into output image file
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    if(encode_int_to_image(file_size,encInfo)==e_failure)
    {
        return e_failure; 
    }
    return e_success; 
}

// Encodes secret data from secret file into output image file
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    unsigned char ch; 
    rewind(encInfo->fptr_secret); 
    for (int i = 0; i < encInfo->size_secret_file; i++)
    {
        if (fread(&ch, 1, 1, encInfo->fptr_secret) != 1)
        {
            return e_failure; 
        }
        unsigned char arr[8]; 
        if (fread(arr, 1, 8, encInfo->fptr_src_image) != 8)
        {
            return e_failure; 
        }
        encode_byte_to_lsb(ch, (char*)arr); 
        if (fwrite(arr, 1, 8, encInfo->fptr_stego_image) != 8)
        {
            return e_failure; 
        }
    }
    return e_success; 
}
// Copies the remaing pixel data into output image file to get proper image after emcoding
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    int ch;
    while ((ch = fgetc(fptr_src)) != EOF)
    {
        if (fputc(ch, fptr_dest) == EOF)
        {
            return e_failure; 
        }
    } 
    if (ferror(fptr_src))
    {
        return e_failure; 
    }
    return e_success; 
}
// Actual decoding procedure
Status do_encoding(EncodeInfo *encInfo)
{
    if(open_files(encInfo)==e_failure)
    {
        printf(RED"\nERROR: Files are not opened\n"RESET);
        return e_failure; 
    }
    else
    {
        printf(GREEN"\nFiles are opened successfully\n"RESET);
    }
    if(check_capacity(encInfo)==e_failure)
    {
        printf(RED"\nERROR:No capacity to encode the secret data in the image file\n"RESET);
        return e_failure; 
    }
    else
    {
        printf(GREEN"\nYES there is capacity to encode the secret data in the image file\n"RESET);
    }
    if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_failure)
    {
        printf(RED"\nERROR: Failed to copy BMP header\n"RESET);
        return e_failure; 
    }
    else
    {
        printf(GREEN"\nSuccessfully encoded the BMP header\n"RESET);
    }
    if(encode_magic_string(MAGIC_STRING, encInfo)==e_failure)
    {
        printf(RED"\nERROR: Failed to encode magic string\n"RESET);
        return e_failure; 
    }
    else
    {
        printf(GREEN"\nSuccessfully encoded the magic string\n"RESET);
    }
    if(encode_size_of_extn(encInfo)==e_failure)
    {
        printf(RED"\nERROR: Failed to encode size of extension\n"RESET);
        return e_failure;
    }
    else
    {
        printf(GREEN"\nSuccessfully encoded the size of extension\n"RESET);
    }
    if(encode_secret_file_extn(encInfo->extn_secret_file, encInfo)==e_failure)
    {
        printf(RED"\nERROR: Failed to encode secret file extension\n"RESET);
        return e_failure; 
    }
    else
    {
        printf(GREEN"\nSuccessfully encoded the extension\n"RESET);
    }
    if(encode_secret_file_size(encInfo->size_secret_file, encInfo)==e_failure)
    {
        printf(RED"\nERROR: Failed to encode secret file size\n"RESET);
        return e_failure; 
    }
    else
    {
        printf(GREEN"\nSuccessfully encoded the size of secret file \n"RESET);
    }
    if(encode_secret_file_data(encInfo)==e_failure)
    {
        printf(RED"\nERROR: Failed to encode secret file data\n"RESET);
        return e_failure; 
    }
    else
    {
        printf(GREEN"\nSuccessfully encoded the secret file data\n"RESET);
    }
    if(copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image)==e_failure)
    {
        printf(RED"\nERROR: Failed to encode the remaining image data\n"RESET);
        return e_failure; 
    }
    else
    {
        printf(GREEN"\nSuccessfully encoded the remaining image data \n"RESET);
    }
    fclose(encInfo->fptr_src_image);
    fclose(encInfo->fptr_secret);
    fclose(encInfo->fptr_stego_image);
    return e_success; // Complete the encoding successfully
}