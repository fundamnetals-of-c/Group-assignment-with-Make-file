#include "global.h"

/*******************************************************************************
 * Description
 * this function returns the logged in users balance
 * INPUTS:
 * logged_user_t
 * OUTPUTS:
 * double (user balance)
 * POST:
 * pointers stay constant
*******************************************************************************/
double get_balance(const logged_user_t * user)
{
    return user->acc_balance;
}

/*******************************************************************************
 * Description
 * this function will allow the user to print the statement for the user account
 * between two dates
 * INPUTS:
 * char[USER_MAX_NUM_LEN]
 * OUTPUTS:
 * -1 if invalid, no of statements
 * POST:
 * array is constant no change
*******************************************************************************/
int print_statement(const char user_ID[USER_MAX_NUM_LEN])
{
    /*ask for date*/
    date_time_t start_dt;
    date_time_t end_dt;
    date_time_t dt;
    int flag = 0;
    transaction_details_t transaction;   

    /*while dates arent valid*/ 
    while(flag == 0)
    {
        printf("Please enter that date you should like to see from:\n");
        start_dt = dt;
        end_dt = dt;
        /*while start date isnt valid*/
        while(validate_date_time(start_dt) == -1)
        {
            printf("Enter year, month, date, hour and minute ");
            printf("separated by spaces>\n");
            scanf("%d %d %d %d %d", 
                &start_dt.year,
                &start_dt.month, 
                &start_dt.day, 
                &start_dt.hour, 
                &start_dt.minute);
        }
        printf("Please enter that date you should like to see to:\n");
        /*while end date isnt valid*/
        while(validate_date_time(end_dt) == -1)
        {
            printf("Enter year, month, date, hour and minute ");
            printf("separated by spaces>\n");
            scanf("%d %d %d %d %d", 
                &end_dt.year,
                &end_dt.month, 
                &end_dt.day, 
                &end_dt.hour, 
                &end_dt.minute);
        }
        /*if valid dates change the flag to break the while*/
        if(trans_cmp(start_dt, end_dt) == -1)
        {
            flag = 1;
        }
        else
            printf("Invalid start and/or end date\n");
    }
    
    /*set up file data base pointer*/
    FILE *fptr = NULL;
    char file_name[USER_MAX_NUM_LEN + 5];
    strcpy(file_name, user_ID);
    strcat(file_name, ".txt");
    fptr = fopen(file_name, "r");

    #ifdef DEBUG
    printf("The file name is>%s",file_name);
    #endif	
	
    /*check if file is read*/
    if(fptr == NULL)
    {
        printf("Error when openning data base");
        return -1;
    }

    /*while file isnt at the end keep reading data*/
    /*break on first valid transaction*/
    while(fptr != NULL)
    {
        /*check if there is anything left to read in file*/
        if(fread(&transaction, sizeof(transaction_details_t), 1, fptr) == 0)
        {
            printf("No transactions found\n");
            fclose(fptr);
            return -1;
        }
        if(trans_cmp(transaction.trans_dt, start_dt) == 1)
        {
            break;
        }
	    
        #ifdef DEBUG
        format_trans_type(transaction.type);
        printf("%02d/%02d/%04d  %02d:%02d %s $%.2lf $%.2lf\n", 
            transaction.trans_dt.day,
            transaction.trans_dt.month,
            transaction.trans_dt.year,
            transaction.trans_dt.hour,
            transaction.trans_dt.minute,
            transaction.type,
            transaction.trans_val,
            transaction.acc_balance);
        #endif
	    
    }
    /*print transaction history header*/
    printf("Transaction history:\n");
    printf("Transaction       transaction\n");
    printf("Date        time  type            ammount balance\n");
    /*print all the valid transactions until end date reached*/
    while(trans_cmp(end_dt,transaction.trans_dt) == 1)
    {
        if(fread(&transaction, sizeof(transaction_details_t), 1, fptr) == 0)
        {
            printf("End of statement\n");
            fclose(fptr);
            break;
        }
        format_trans_type(transaction.type);
        printf("%02d/%02d/%04d  %02d:%02d %s $%.2lf $%.2lf\n", 
            transaction.trans_dt.day,
            transaction.trans_dt.month,
            transaction.trans_dt.year,
            transaction.trans_dt.hour,
            transaction.trans_dt.minute,
            transaction.type,
            transaction.trans_val,
            transaction.acc_balance);
    }

return 0;
}

/*******************************************************************************
 * Description
 * this function comapre two dates with the early date being on the first 
 * argument and the later date being on the second argument
 * INPUTS:
 * date_time_t, date_time_t
 * OUTPUTS:
 * -1 if invalid, 1 if valid
 * POST:
 * arrays is constant no change
*******************************************************************************/
int trans_cmp(const struct date_time trans_dt, const struct date_time date_dt)
{
    if(trans_dt.year < date_dt.year)
        return -1;
    if(trans_dt.year == date_dt.year &&
        trans_dt.month < date_dt.month)
        return -1;
    if(trans_dt.year == date_dt.year &&
        trans_dt.month == date_dt.month &&
        trans_dt.day < date_dt.day)
        return -1;
    if(trans_dt.year == date_dt.year &&
        trans_dt.month == date_dt.month &&
        trans_dt.day == date_dt.day &&
        trans_dt.hour < date_dt.hour)
        return -1;
    if(trans_dt.year == date_dt.year &&
        trans_dt.month == date_dt.month &&
        trans_dt.day == date_dt.day &&
        trans_dt.hour == date_dt.hour &&
        trans_dt.minute < date_dt.minute)
        return -1;
    
    return 1;
}


