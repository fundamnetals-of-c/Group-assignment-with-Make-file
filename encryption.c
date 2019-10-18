#include "global.h"

/*******************************************************************************
 * Description
 * INPUTS:
 * what is required to input into this function
 * OUTPUTS:
 * what gets returned
 * POST:
 * what happens to pointers and data after the function
*******************************************************************************/
const char* encryption(char key[], char string[])
{
    int i, j;
    char strKey[strlen(key)], encString[strlen(string)];
    
    for (i = 0, j = 0; i < strlen(string); i++, j++)
    {
        /*strKey will become key repeated until the same length as string*/ 
        if (j == strlen(key))
        {
            j = 0;
        }
        strKey[i] = key[j];
    }
    strKey[i] = '\0';
    
    /*For each character until the end, add the ASCII value to string*/
    for (i = 0; (i < 100 && string[i] != '\0'); i++)
    {
        encString[i] = string[i] + (int)strKey[i];
    }
    encString[i] = '\0';
    /*Note to self: Can't return local pointers*/
    string = encString;
    /*Returns encrypted string to be saved*/
    return string;
}
/*******************************************************************************
 * Description
 * INPUTS:
 * what is required to input into this function
 * OUTPUTS:
 * what gets returned
 * POST:
 * what happens to pointers and data after the function
*******************************************************************************/
const char* decryption(char key[], char string[])
{
    int i, j;
    char strKey[strlen(key)], decString[strlen(string)];
    
    for (i = 0, j = 0; i < strlen(string); i++, j++)
    {
        /*strKey will become key repeated until the same length as string*/ 
        if (j == strlen(key))
        {
            j = 0;
        }
        strKey[i] = key[j];
    }
    strKey[i] = '\0';
    
    /*For each character until the end, remove the ASCII value from encString*/
    for (i = 0; (i < 100 && string[i] != '\0'); i++)
    {
        decString[i] = string[i] - (int)strKey[i];
    }
    decString[i] = '\0';
    /*Note to self: Can't return local pointers*/
    string = decString;
    /*Returns decrypted string to be saved*/
    return string;
}

