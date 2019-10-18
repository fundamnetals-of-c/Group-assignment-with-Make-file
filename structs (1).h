#include <stdio.h> /*scanf printf fscanf fprintf fflush fopen fclose*/
#include <string.h> /*strlen strcmp strcpy strcat*/
#include <stdlib.h> 
#include <time.h> /*time localtime*/

/*#include "program_modes.h"*/

/*******************************************************************************
 * List preprocessing directives - DEFINES
*******************************************************************************/
#define DEFINES "defined"
#define USER_MAX_NUM_LEN 6
#define USER_NUM_LETTER_LEN 2
#define USER_MAX_PW_LEN 32
#define USER_MAX_LVL_LEN 5
#define SQ_MAX_LEN 100
#define ANSWER_MAX_LEN 15
#define MAX_TYPE_LEN 15
#define USER "user"
#define ADMIN "admin"
#define USER_DB "users.txt"

/*defines for testing valid date_time inputs*/
#define MONTHS_LOWER 1
#define MONTHS_UPPER 12
#define DAYS_LOWER 1
#define DAYS_UPPER 32
#define HOURS_LOWER 0
#define HOURS_UPPER 23
#define MINUTES_LOWER 0
#define MINUTES_UPPER 60

/*******************************************************************************
 * List structs - you may define struct date_time and struct flight only. Each
 * struct definition should have only the fields mentioned in the assignment
 * description.
*******************************************************************************/
typedef struct users
{
    char user_num[USER_MAX_NUM_LEN + 1];
    char user_pw[USER_MAX_PW_LEN + 1];
    char user_lvl[USER_MAX_LVL_LEN + 1];
    double acc_balance;
    struct users * next;
} users_t;

typedef struct logged_user
{ 
    char user_num[USER_MAX_NUM_LEN + 1];
    char user_pw[USER_MAX_PW_LEN + 1];
    char user_lvl[USER_MAX_LVL_LEN + 1];
    double acc_balance;
}logged_user_t;

typedef struct user_security_questions
{
    char sq1[SQ_MAX_LEN];
    char ans1[ANSWER_MAX_LEN];
    char sq2[SQ_MAX_LEN];
    char ans2[ANSWER_MAX_LEN];
    char sq3[SQ_MAX_LEN];
    char ans3[ANSWER_MAX_LEN];
}user_security_questions_t;

typedef struct date_time
{
    int year;
    int month;
    int day;
    int hour;
    int minute;
} date_time_t;

typedef struct transaction_details
{
    date_time_t trans_dt;
    char type[MAX_TYPE_LEN+1];
    double principal;
    double trans_val;
    double acc_balance;
} transaction_details_t;

typedef struct transactions
{
    transaction_details_t trans_detail;
    struct transactions * next ;
} transactions_t;
