#include "global.h"

/*******************************************************************************
 * Description
 * This function asks the user to create the security questions and the answers
 * corresponding with the questions, are stored in a file
 * INPUTS:
 * char user_ID
 * OUTPUTS:
 * returns -1 if wasn't able to open the file, returns 1 otherwise
 * POST:
 * const array no change
*******************************************************************************/
int create_sq(const char user_ID[USER_MAX_NUM_LEN]) { 
    user_security_questions_t userSq;

    char sq1[SQ_MAX_LEN];
    char ans1[ANSWER_MAX_LEN];
    char sq2[SQ_MAX_LEN];
    char ans2[ANSWER_MAX_LEN];
    char sq3[SQ_MAX_LEN];
    char ans3[ANSWER_MAX_LEN];
    char file_name[USER_MAX_NUM_LEN + 5];

    printf("Choose 3 security questions:\n");

    printf("Question 1 : ");
    fgets(sq1, SQ_MAX_LEN, stdin); 
    printf("Answer 1 : ");
    fgets(ans1, ANSWER_MAX_LEN, stdin); 

    printf("Question 2 : ");
    fgets(sq2, SQ_MAX_LEN, stdin); 
    printf("Answer 2 : ");
    fgets(ans2, ANSWER_MAX_LEN, stdin); 

    printf("Question 3 : ");
    fgets(sq3, SQ_MAX_LEN, stdin); 
    printf("Answer 3 : ");
    fgets(ans3, ANSWER_MAX_LEN, stdin); 

    strcpy(userSq.sq1, sq1);
    strcpy(userSq.ans1, ans1);
    strcpy(userSq.sq2, sq2);
    strcpy(userSq.ans2, ans2);
    strcpy(userSq.sq3, sq3);
    strcpy(userSq.ans3, ans3);

    FILE *fptr= NULL;
    strcpy(file_name, user_ID);
    strcat(file_name, "sq.txt");
    fptr=fopen(file_name,"wb");
    if(fptr == NULL) {
        printf("error when opening data base");
        return -1;
    }
    fwrite(&userSq, sizeof(user_security_questions_t), 1, fptr);
    fclose(fptr);
    return 1;
}

