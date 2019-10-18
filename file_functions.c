#include "global.h"

/*******************************************************************************
 * Description
 * this function will ask for user information and store it into a linked list
 * it will then write the user data into a file when finished
 * this allows the program to store as many users with no end
 * the function will return the number of members in temp storage
 * INPUTS:
 * users_t
 * OUTPUTS:
 * int 
 * POST:
 * users_t will be filled with the temp new members
*******************************************************************************/
int add_user(users_t * user)
{
    /*set up temp variables to store user entry*/
    char user_num[USER_MAX_NUM_LEN + 1];
    char user_pw[USER_MAX_PW_LEN + 1];
    char user_lvl[USER_MAX_LVL_LEN + 1];
    double acc_balance;

    int user_choice;

    /*entering user data*/
    printf("Enter user information\n");
    
    /*enter valid user ID*/
    while(validate_user_ID(user_num) == -1)
    {
        printf("Enter the new user number: \n");
        scanf("%s", user_num);
        if(validate_user_ID(user_num) == -1)
        {
            printf("Invalid user ID\n");
        }
    }
    
    /*validate user password*/
    while(validate_user_pw(user_pw) == -1)
    {
        printf("Enter the new user password: \n");
        scanf("%s", user_pw);
        if(validate_user_pw(user_pw) == -1)
        {
            printf("Invalid user password\n");
            printf("Please use at least 1 of Lower & Upper Case,"
                "Number and special characters\n");
        }
    }

    while(validate_user_lvl(user_lvl) != 1)
    {
        printf("Enter the user level:\n");
        printf("Enter 1 for admin, Enter 2 for user\n");
        /*validate*/
        scanf("%d", &user_choice);
        if(user_choice == 1)
            strcpy(user_lvl,ADMIN);
        if(user_choice == 2)
            strcpy(user_lvl,USER);
    }

    printf("Enter user initial account balance: \n");
    scanf("%lf", &acc_balance);

    #ifdef DEBUG
    printf("users ID:%s\n"
        "users PW:%s\n"
        "users level:%s\n"
        "users balance:%.2lf\n",
        user_num, user_pw, user_lvl, acc_balance);
    #endif
	
    users_t * it = user;
    
    /*check if this is the first user*/
    if(strcmp(it->user_lvl, "test") == 0)
    {
        strcpy(it->user_num, user_num);
        strcpy(it->user_pw, user_pw);
        strcpy(it->user_lvl, user_lvl);
        it->acc_balance = acc_balance;
        store_users(it);
    }
    else
    {
        while(it->next != NULL)
        {
            it = it->next;
        }
        it->next = malloc(sizeof(users_t));
        /*entering user data into struct*/
        strcpy(it->next->user_num, user_num);
        strcpy(it->next->user_pw, user_pw);
        strcpy(it->next->user_lvl, user_lvl);
        it->next->acc_balance = acc_balance;
        it->next->next = NULL;
        /*create_sq(it->next->user_num);*/
        store_users(it->next);
    }    
    return 1;
}

/*******************************************************************************
 * Description
 * this function allows a user to deposit an ammount of money into their account
 * and adjust the users account balance
 * INPUTS:
 * logged_user_t, double
 * OUTPUTS:
 * -1 is invalid, 1 is valid
 * POST:
 * logged_user_t is filled with the most up to date account balance
*******************************************************************************/
int deposit(logged_user_t * user, double value)
{
    /*set temp variables*/
    date_time_t dt;
    transaction_details_t transaction_details;    

    /*inport local computer time for transaction history*/
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    dt.year = timeinfo->tm_year + 1900;
    dt.month = timeinfo->tm_mon + 1;
    dt.day = timeinfo->tm_mday;
    dt.hour = timeinfo->tm_hour;
    dt.minute = timeinfo->tm_min;

    #ifdef DEBUG
    printf("The local time is> %02d/%02d/%04d %02d:%02d\n",
        dt.day,dt.month,dt.year,dt.hour,dt.minute);
    #endif
	    
    /*fill transaction information*/
    transaction_details.trans_dt = dt;
    strcpy(transaction_details.type, "deposit");
    transaction_details.principal = user->acc_balance;
    transaction_details.trans_val = value;
    transaction_details.acc_balance = 
        transaction_details.principal + transaction_details.trans_val;

    /*update account balance*/
    user->acc_balance = transaction_details.acc_balance;

    #ifdef DEBUG
    printf("Principal: %.2lf\n"
        "Transaction value: %.2lf\n"
        "Account balance: %.2lf\n",
        transaction_details.principal, 
        transaction_details.trans_val,
        transaction_details.acc_balance);
    #endif
	
    /*find file to fill transaction data into*/
    FILE *fptr = NULL;
    char file_name[USER_MAX_NUM_LEN + 5];
    strcpy(file_name, user->user_num);
    strcat(file_name, ".txt");
    fptr = fopen(file_name, "a");

    /*check the data base has no errors*/
    if(fptr == NULL)
    {
        printf("Rrror when openning data base");
        return -1;
    }

    /*write to database*/
    fwrite(&transaction_details, sizeof(transaction_details_t), 1, fptr);
    fclose(fptr);

    /*create linked lists to rewrite*/
    users_t * start = NULL;
    start = malloc(sizeof(users_t));

    if(start == NULL)
    {
        printf("mem error");
        return -1;
    }
    strcpy(start->user_lvl,"start");
    start->next = NULL;

    read_users(start);
    
    /*obtain and write data*/ 
    users_t * it = start;
    while(it != NULL)
    {
        if(strcmp(it->user_num, user->user_num) == 0)
        {
            break;
        }
        it = it->next;
    }

    it->acc_balance = transaction_details.acc_balance;

    write_users(start);

    return 1;
}

/*******************************************************************************
 * Description
 * this is a user function use to withdraw money from their account and write 
 * it to their transaction history file
 * INPUTS:
 * logged_user_t, double
 * OUTPUTS:
 * -1 if invalid, 1 is valid
 * POST:
 * logged_user_t with new balance added
*******************************************************************************/
int withdraw(logged_user_t * user, double value)
{
    /*set temp variables*/
    date_time_t dt;
    transaction_details_t transaction_details;    

    /*import local computer time for transaction*/
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    dt.year = timeinfo->tm_year + 1900;
    dt.month = timeinfo->tm_mon + 1;
    dt.day = timeinfo->tm_mday;
    dt.hour = timeinfo->tm_hour;
    dt.minute = timeinfo->tm_min;

    #ifdef DEBUG
    printf("The local time is> %02d/%02d/%04d %02d:%02d\n",
        dt.day,dt.month,dt.year,dt.hour,dt.minute);
    #endif
	
    /*check if user has enough balance to withdraw*/
    if(user->acc_balance < value)
    {
        #ifdef DEBUG
        printf("Not enough funds to withdraw");
        #endif

        printf("Error: not enough funds within account");
        transaction_details.trans_dt = dt;
        strcpy(transaction_details.type, "low fund");
        transaction_details.principal = user->acc_balance;
        transaction_details.trans_val = -1 * value;
        transaction_details.acc_balance = user->acc_balance;
    }
    else
    {
        transaction_details.trans_dt = dt;
        strcpy(transaction_details.type, "withdraw");
        transaction_details.principal = user->acc_balance;
        transaction_details.trans_val = value;
        transaction_details.acc_balance = 
            transaction_details.principal - transaction_details.trans_val;

    }
    /*update user account balance*/
    user->acc_balance = transaction_details.acc_balance;

    #ifdef DEBUG
    printf("Principal: %.2lf\n"
        "Transaction value: %.2lf\n"
        "Account balance: %.2lf\n",
        transaction_details.principal, 
        transaction_details.trans_val,
        transaction_details.acc_balance);
    #endif
	
    /*open the file to fill the data base*/
    FILE *fptr = NULL;
    char file_name[USER_MAX_NUM_LEN + 5];
    strcpy(file_name, user->user_num);
    strcat(file_name, ".txt");
	
    #ifdef DEBUG
    printf("The file name is>%s",file_name);
    #endif
	
    fptr = fopen(file_name, "a");

    /*check the database has no errors opening*/
    if(fptr == NULL)
    {
        printf("Error when openning data base");
        return -1;
    }

    /*write transaction to file*/
    fwrite(&transaction_details, sizeof(transaction_details_t), 1, fptr);
    fclose(fptr);

    /*create linked lists to rewrite*/
    users_t * start = NULL;
    start = malloc(sizeof(users_t));

    if(start == NULL)
    {
        printf("mem error");
        return -1;
    }
    strcpy(start->user_lvl,"start");
    start->next = NULL;

    read_users(start);
    
    /*obtain and write data*/ 
    users_t * it = start;
    while(it != NULL)
    {
        if(strcmp(it->user_num, user->user_num) == 0)
        {
            break;
        }
        it = it->next;
    }

    it->acc_balance = transaction_details.acc_balance;

    write_users(start);

    return 1;
}

/*******************************************************************************
 * Description
 * this function takes the logged in user and will transfer an ammount of money
 * from their account into the target account and write this transfer on both
 * users transaction statements
 * INPUTS:
 * logged_user_t, char[], double
 * OUTPUTS:
 * -1 if invalid, 1 if valid
 * POST:
 * what happens to pointers and data after the function
*******************************************************************************/
int transfer(logged_user_t * user, const char target_acc[], double value)
{
    /*set up temp variables*/
    date_time_t dt;
    transaction_details_t transaction_details; 
    logged_user_t logger;   

    /*import local time from computer*/
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    dt.year = timeinfo->tm_year + 1900;
    dt.month = timeinfo->tm_mon + 1;
    dt.day = timeinfo->tm_mday;
    dt.hour = timeinfo->tm_hour;
    dt.minute = timeinfo->tm_min;

    #ifdef DEBUG
    printf("The local time is> %02d/%02d/%04d %02d:%02d\n",
        dt.day,dt.month,dt.year,dt.hour,dt.minute);
    #endif	
	
    /*open file database to check users*/
    FILE *fptr = NULL;
    fptr = fopen(USER_DB,"r");

    if(fptr == NULL)
    {
        printf("mem error file location doesnt exsist");
    }

    /*test if account exists*/
    while(fptr != NULL)
    {
        if(fread(&logger, sizeof(logged_user_t), 1, fptr) == 0)
        {
            printf("Invalid account ID\n");
            fclose(fptr);
            return -1;
        }
        if(strcmp(logger.user_num, target_acc) == 0)
        {
            fclose(fptr);
            break;
        }
    }
    /*check if the account has enough to transfer*/
    if(user->acc_balance < value)
    {
        #ifdef DEBUG
        printf("Not enough funds to transfer");
        #endif

        transaction_details.trans_dt = dt;
        strcpy(transaction_details.type, "low funds");
        transaction_details.principal = user->acc_balance;
        transaction_details.trans_val = -1 * value;
        transaction_details.acc_balance = user->acc_balance;
        return user->acc_balance;
    }
    else
    {
        transaction_details.trans_dt = dt;
        strcpy(transaction_details.type, "transaction out");
        transaction_details.principal = user->acc_balance;
        transaction_details.trans_val = -1 * value;
        transaction_details.acc_balance = 
            transaction_details.principal - transaction_details.trans_val;
    }
    /*update user account balance*/
    user->acc_balance = transaction_details.acc_balance;
    
    #ifdef DEBUG
    printf("Account out account amounts>\n"
        "Principal: %.2lf\n"
        "Transaction value: %.2lf\n"
        "Account balance: %.2lf\n",
        transaction_details.principal, 
        transaction_details.trans_val,
        transaction_details.acc_balance);
    #endif	
	
    /*write to user transaction database*/
    fptr = NULL;
    char file_name[USER_MAX_NUM_LEN + 5];
    strcpy(file_name, user->user_num);
    strcat(file_name, ".txt");\
	    
    #ifdef DEBUG
    printf("The file name is>%s",file_name);
    #endif	    
	    
    fptr = fopen(file_name, "a");

    if(fptr == NULL)
    {
        printf("Error when openning data base");
        return -1;
    }

    fwrite(&transaction_details, sizeof(transaction_details_t), 1, fptr);
    fclose(fptr);

    /*create linked lists to rewrite*/
    users_t * head = NULL;
    head = malloc(sizeof(users_t));

    if(head == NULL)
    {
        printf("mem error");
        return -1;
    }
    strcpy(head->user_lvl,"start");
    head->next = NULL;

    read_users(head);
    
    /*obtain and write data*/ 
    users_t * it = head;
    while(it != NULL)
    {
        if(strcmp(it->user_num, user->user_num) == 0)
        {
            break;
        }
        it = it->next;
    }

    it->acc_balance = transaction_details.acc_balance;

    write_users(head);


    /*create linked lists to rewrite*/
    users_t * start = NULL;
    start = malloc(sizeof(users_t));

    if(start == NULL)
    {
        printf("mem error");
        return -1;
    }
    strcpy(start->user_lvl,"start");
    start->next = NULL;

    read_users(start);
    
    /*obtain and write data*/ 
    it = start;
    while(it != NULL)
    {
        if(strcmp(it->user_num, target_acc) == 0)
        {
            break;
        }
        it = it->next;
    }
	
    transaction_details.trans_dt = dt;
    strcpy(transaction_details.type, "transaction in");
    transaction_details.principal = it->acc_balance;
    transaction_details.trans_val = 1 * value;
    transaction_details.acc_balance = 
        transaction_details.principal + transaction_details.trans_val;       

    /*update target user account balance*/
    it->acc_balance = transaction_details.acc_balance;

    #ifdef DEBUG
    printf("Account in account amounts>\n"
        "Principal: %.2lf\n"
        "Transaction value: %.2lf\n"
        "Account balance: %.2lf\n",
        transaction_details.principal, 
        transaction_details.trans_val,
        transaction_details.acc_balance);
    #endif	
	
    /*write transaction history on reciever*/
    fptr = NULL;
    strcpy(file_name, it->user_num);
    strcat(file_name, ".txt");
    fptr = fopen(file_name, "a");

    #ifdef DEBUG
    printf("The file name is>%s",file_name);
    #endif	
	
    if(fptr == NULL)
    {
        printf("Error when openning data base");
        return -1;
    }

    /*write transactio details to file*/
    fwrite(&transaction_details, sizeof(transaction_details_t), 1, fptr);
    fclose(fptr);

    /*update all users in the linked list*/
    write_users(start);
    return 1;
}

/*******************************************************************************
 * Description
 * this function will allow the user to change their password
 * INPUTS:
 * char[USER_MAX_PW_LEN]
 * OUTPUTS:
 * -1 if invalid, 1 if valid
 * POST:
 * array is constant no change
*******************************************************************************/
int change_password(const char user_ID[USER_MAX_NUM_LEN])
{
    char pw_holder[USER_MAX_PW_LEN];

    /*create linked lists*/
    users_t * start = NULL;
    start = malloc(sizeof(users_t));

    if(start == NULL)
    {
        printf("mem error cannot allocate memory");
        return -1;
    }
    strcpy(start->user_lvl,"start");
    start->next = NULL;

    read_users(start);

    /*obtain user position*/ 
    users_t * it = start;
    while(it != NULL)
    {
        if(strcmp(it->user_num, user_ID) == 0)
        {
            break;
        }
        it = it->next;
    }
    
    printf("What would you like to change your password to>");
    scanf("%s", pw_holder);

    /*validate pw*/
    
    strcpy(it->user_pw, pw_holder);

    write_users(start);
    printf("Password successfully changed");   
    
    return 1;
}

/*******************************************************************************
 * Description
 * this function will find and delete a user based on user ID
 * INPUTS:
 * users_t*
 * OUTPUTS:
 * int num of deleted users 
 * POST:
 * the pointer will contain the same data but without the user that was removed
*******************************************************************************/
int delete_user(users_t * user)
{
    char user_num[USER_MAX_NUM_LEN];
    printf("What is the user account number you would like to delete: ");
    scanf("%s", user_num);

    users_t * temp = NULL;
    users_t * it = user;

    #ifdef DEBUG	
    printf("%s : %s\n", it->user_num, user_num);
    #endif
	
    /*edge case*/
    /*check statement if first user*/
    if(strcmp(it->user_num, user_num) == 0)
    {
        user = it->next;
        user->next = NULL;
    }
    
    /*search through linked list*/
    while(it->next != NULL)
    {
        #ifdef DEBUG	
        printf("%s : %s\n", it->next->user_num, user_num);
        #endif
	    
        if(strcmp(it->next->user_num, user_num) == 0)
        {
            temp = it->next;
            /*second edge case testing for end of linked list*/
            if(temp->next != NULL)
            {
                it->next = temp->next;
            }
            /*if user is at end of list just delete the last entry by setting
            pointer being tested to NULL*/
            else
            {
                it->next = NULL;
                break;
            }
        }
        it = it->next;
    }
    return 1;
}

/*******************************************************************************
 * Description
 * appends new users to the users database file
 * INPUTS:
 * users_t
 * OUTPUTS:
 * -1 if invalid, 1 if successful
 * POST:
 * nothing chnages within pointer
*******************************************************************************/
int store_users(users_t * user)
{
    FILE *fptr = NULL;
    fptr = fopen("users.txt", "a");
    if(fptr == NULL)
    {
        printf("Error when openning data base");
        return -1;
    }
    logged_user_t write_user;
    strcpy(write_user.user_num, user->user_num);
    strcpy(write_user.user_pw, user->user_pw);
    strcpy(write_user.user_lvl, user->user_lvl);
    write_user.acc_balance = user->acc_balance;

    fwrite(&write_user, sizeof(logged_user_t), 1, fptr);
    fclose(fptr);
    return 1;
}
/*******************************************************************************
 * Description
 * This function stores all users from users.txt into the users_t linked list
 * INPUTS:
 * what is required to input into this function
 * OUTPUTS:
 * what gets returned
 * POST:
 * what happens to pointers and data after the function
*******************************************************************************/
int read_users(users_t * users)
{
    logged_user_t logged_user;
    users_t * it = users;
    FILE *fptr = NULL;
    fptr = fopen("users.txt","r");

    if(fptr == NULL)
    {
        printf("mem error file location doesnt exsist");
    }
    /*sort through file until found*/

    while(fptr != NULL)
    {
        if(fread(&logged_user, sizeof(logged_user_t), 1, fptr) == 0)
        {
            return -1;
        }
        if(strcmp(users->user_lvl,"start") == 0)
        {
            /*local store first user*/
            strcpy(users->user_num,logged_user.user_num);
            strcpy(users->user_pw,logged_user.user_pw);
            strcpy(users->user_lvl,logged_user.user_lvl);
            users->acc_balance = logged_user.acc_balance;
            users->next = NULL;
        }
        else
        {
            /*search for next empty list*/
            while(it->next != NULL)
            {
                it = it->next;
            }

            /*local store users*/
            it->next = malloc(sizeof(users_t));
            strcpy(it->next->user_num,logged_user.user_num);
            strcpy(it->next->user_pw,logged_user.user_pw);
            strcpy(it->next->user_lvl,logged_user.user_lvl);
            it->next->acc_balance = logged_user.acc_balance;
            it->next->next = NULL;
        }
    }
    return 1;
}

/*******************************************************************************
 * Description
 * this function writes a linked list of any size into the users file
 * INPUTS:
 * users_t
 * OUTPUTS:
 * -1 if invalid, 1 if completed task
 * POST:
 * no user data changes
*******************************************************************************/
int write_users(users_t * users)
{
    FILE *fptr = NULL;
    fptr = fopen("users.txt","w");
    logged_user_t holder;

    if(fptr == NULL)
    {
        printf("mem error: couldn't create user file");
        return -1;
    }

    users_t * it = users;
    while(it != NULL)
    {
        strcpy(holder.user_num,it->user_num);
        strcpy(holder.user_pw,it->user_pw);
        strcpy(holder.user_lvl,it->user_lvl);
        holder.acc_balance = it->acc_balance;
        fwrite(&holder, sizeof(logged_user_t), 1, fptr);
        it = it->next;
    }
    fclose(fptr);
    return 1;
}
