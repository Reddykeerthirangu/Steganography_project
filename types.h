#ifndef TYPES_H
#define TYPES_H

// ANSI Escape sequences for terminal output text colour modifications
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define RESET   "\033[0m" // Resets terminal text colour back to default

/* User defined types */
typedef unsigned int uint;


typedef enum
{
    e_success, 
    e_failure  
}Status;


typedef enum
{
    e_encode,      
    e_decode,      
    e_unsupported  
}OperationType;

#endif
