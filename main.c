#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

/*Checks the command-line argument to determine the requested operation.
Array of command-line argument strings.
OperationType Returns e_encode for '-e', e_decode for '-d', or e_unsupported.
*/
OperationType check_operation_type(char *argv[])
{
    // Compare the first operational argument (argv[1]) with supported flags
    if (strcmp(argv[1], "-e") == 0)
    {
        return e_encode;
    }
    else if (strcmp(argv[1], "-d") == 0)
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }
}

int main(int argc, char *argv[])
{
    // checks whether the user wants to encode, decode, or if the flag is invalid
    OperationType res = check_operation_type(argv);
    
    //ENCODING PROCESS
    if (res == e_encode)
    {
        EncodeInfo encodeinfo;
        
        // Read and validate command-line arguments for encoding
        if (read_and_validate_encode_args(argv, &encodeinfo) == e_success)
        {
            //Encoding procedure
            if (do_encoding(&encodeinfo) == e_failure)
            {
                printf(RED "ERROR: unable to do encoding process\n\n" RESET);
                exit(0);
            }
            printf(MAGENTA"\n=======================================================\n"RESET);
            printf(YELLOW "\nSuccessfully completed encoding process\n" RESET);
            printf(MAGENTA"\n=======================================================\n\n"RESET);
        }
        else
        {
            printf("\nERROR : please enter valid arguments\n\n");
            exit(0);
        }
    }
    //DECODING PROCESS
    else if (res == e_decode)
    {
        DecodeInfo decodeinfo;
        //Read and validate command-line arguments for decoding
        if (read_and_validate_decode_args(argv, &decodeinfo) == e_success)
        {
            //Decoding procedure
            if (do_decoding(&decodeinfo) == e_failure)
            {
                printf(RED "ERROR: unable to do decoding process\n\n" RESET);
                exit(0);
            }
            printf(MAGENTA"\n=======================================================\n"RESET);
            printf(YELLOW "\nSuccessfully completed decoding process\n" RESET);
            printf(MAGENTA"\n=======================================================\n\n"RESET);
        }
        else
        {
            printf(RED "\nERROR : please enter valid arguments\n\n" RESET);
            exit(0);
        }
    }
    //UNSUPPORTED FLAG
    else
    {
        printf(RED "\nERROR: Invalid argument\n\n" RESET);
        exit(0);
    }

    return 0;
}
