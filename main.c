/*******************************************************************************
 * 48430 Fundamentals of C Programming - Assignment 3
 * Name: James Walsh
 * Student ID: 99143152
 * Name: Sebastien Liao
 * Student ID: 13689043
 * Name: Walter Chan
 * Student ID: 12569582
 * Name: Tam-Hoang Nguyen
 * Student ID: 13589256
 * Name: Eleftherios Asiminaris
 * Student ID: 12889097
 * Date of submission: 18/10/2019
 * A brief statement on what you could achieve (less than 50 words):
 * linked lists are used to store data
 * searching algothrims are used to both search through user.txt data base an
 * can search by date for the transaction databases
 * add delete and transfer user all work completely
 * withdraw deposit and transfer all work and ammend the individual transfer
 * databases
 * make file that complies all c files by calling make in the command line
 * DEBUG and DEV mode made by changing defines
 * A brief statement on what you could NOT achieve:
 * SQ function was not completed in time for it to be placed at the start of the
 * indivdual user data bases
 * encryption was also meant to have many modes and but was not fully 
 * implemented within the code althrough the code will run a demo of the 
 * functions in DEV mode
 * not a large amount of testing on invalid inputs
*******************************************************************************/

/*******************************************************************************
 * List header files - do NOT use any other header files. 
*******************************************************************************/

#include "global.h"
#include "program_modes.h"

/*******************************************************************************
 * Main
*******************************************************************************/
int main(void)
{
    /*SETUP OF VARIABLES USED IN THE MAJORITY OF THE CODE*/
    logged_user_t * logged_user;
    logged_user = malloc(sizeof(logged_user_t));

    users_t * start = NULL;
    start = malloc(sizeof(users_t));

    if(start == NULL)
    {
        printf("mem error");
        return -1;
    }
    strcpy(start->user_lvl,"test");
    start->next = NULL;

    /*if dev mode is not active*/
    #ifndef DEV
        #ifdef DEBUG
        printf("USER/ADMIN MENU\n");
        #endif
        while(login_menu(logged_user) != 1);
    #endif

    /*if dev mode is active*/
    #ifdef DEV
        #ifdef DEBUG
        printf("DEV MENU\n");
        #endif
        strcpy(logged_user->user_lvl,"test");
    #endif
    print_menu(logged_user);
    
    return 0;
}
