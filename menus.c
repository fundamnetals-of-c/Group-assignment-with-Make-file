
#include "global.h"

/*******************************************************************************
 * Description
 * this function takes a user struct and inputs user data into it.
 * this function will return a 1 is the input matched the stored data in the 
 * users.txt file and will return a -1 if it couldn't find the user name or the
 * password is incorrect for a valid user name
 * INPUTS:
 * logged_user_t
 * OUTPUTS:
 * 1 if valid, -1 if invalid
 * POST:
 * this function will end with the logged user being filled with local user 
 * data based on inputs
*******************************************************************************/
int login_menu(logged_user_t * logged_user)
{
    /*setup user inputs*/
    char userID[USER_MAX_NUM_LEN + 1];
    char userPW[USER_MAX_PW_LEN + 1];

    /*validate user ID*/
    while(validate_user_ID(userID) == -1)
    {
        printf("Please enter a user name: \n");
        scanf("%s", userID);
        if(validate_user_ID(userID) == -1)
        {
            printf("invalid user ID");
        }
    }
    
    
    printf("Please enter your password: \n");
    scanf("%s", userPW);

    /*place into struct*/
    #ifdef DEBUG
    printf("USER NUM: %s\n"
        "USER PASSWORD: %s\n",
        userID,
        userPW);
    #endif
    strcpy(logged_user->user_num,userID);
    strcpy(logged_user->user_pw,userPW);
    
    /*check against database*/
    logged_user_t logger;
    FILE *fptr = NULL;
    fptr = fopen(USER_DB,"r");

    if(fptr == NULL)
    {
        printf("mem error file location doesnt exsist\n");
        return -1;
    }

    /*sort through file until found*/
    while(fptr != NULL)
    {
        /*fill the logger struct with temp data*/
        if(fread(&logger, sizeof(logged_user_t), 1, fptr) == 0)
        {
            printf("incorrect ID or password\n");
            return -1;
	    /*validate_sq(userID);*/
        }
        /*check the logger information against user input*/
        #ifdef DEBUG
        printf("compare passwords>%s : %s\ncompare user ID>%s : %s\n",
            logger.user_pw, userPW,
            logger.user_num, userID);
        #endif
        if(strcmp(logger.user_pw, userPW) == 0 && 
            strcmp(logger.user_num, userID) == 0)
        {
            strcpy(logged_user->user_lvl, logger.user_lvl);
            logged_user->acc_balance = logger.acc_balance;
            printf("Welcome\n");
            fclose(fptr);
            return 1;
        }
    }
    return -1;
}

/*******************************************************************************
 * Description
 * this function will change the command line display based on the user level 
 * that belongs to the loggered user function. the logged_user_t will return 
 * with the lastest user data. cannot be const as it is pass through this 
 * function into further functions
 * INPUTS:
 * logged_user_t
 * OUTPUTS:
 * - nothing
 * POST:
 * will be filled with the lasted user data after the user has choosen to log 
 * out
*******************************************************************************/
void print_menu(logged_user_t * user)
{ 
    if(strcmp(user->user_lvl,"test") == 0)
    {
        #ifdef DEBUG
        printf("Entering dev menu");
        #endif
        dev_menu(user);
    }
    else if(strcmp(user->user_lvl,"admin") == 0)
    {
        #ifdef DEBUG
        printf("Entering admin menu");
        #endif
        admin_menu(user);
    }
    else if(strcmp(user->user_lvl,"user") == 0)
    {
        #ifdef DEBUG
        printf("Entering user menu");
        #endif
        user_menu(user);
    }
    else
    {
        printf("Unable to match user level with a valid user level\n");
    }        
}

/*******************************************************************************
 * Description
 * this is the menu that will be given to the devs, with special functions 
 * such as choosing the user rather then having it forced into args of some 
 * functions
 * this is where devs can choose what function they require
 * INPUTS:
 * logged_user_t
 * OUTPUTS:
 * -nothing
 * POST:
 * will be filled with the lasted user data after the user has choosen to log 
 * out
*******************************************************************************/
void dev_menu(logged_user_t * user)
{

    users_t * start = NULL;
    start = malloc(sizeof(users_t));
    char str_temp[8] = "string";
    char user_ID[USER_MAX_NUM_LEN + 1];

    if(start == NULL)
    {
        printf("mem error");
        return;
    }
    strcpy(start->user_lvl,"start");
    start->next = NULL;

    double val = 0;
    int user_input = -1;

    read_users(start);

    while(1)
    {
        printf("\n"
        "Admin options:\n"
        "1. Add user\n"
        "2. Remove user\n"
        "3. View user statement\n"
        "User options:\n"
        "4. View balance\n"
        "5. withdraw funds\n"
        "6. Deposit funds\n"
        "7. Transfer funds\n"
        "8. Change password\n"
        "Dev options:\n"
        "9. View all user account numbers\n"
        "10. Logout\n"
	"11. Encrypt\n"
	"12. Decrypt\n"
        "13. Validate PW\n");
    
        scanf("%d", &user_input);
        
        switch(user_input)
        {
            case 1 :
                    #ifdef DEBUG
                    printf("Add user\n");
                    #endif
                    add_user(start);
                    printf("\nPrinting total users\n");
                    print_users(start);
                    break;
            case 2 :
                    #ifdef DEBUG
                    printf("Remove user\n");
                    #endif
                    delete_user(start);
                    break;
            case 3 :
                    #ifdef DEBUG
                    printf("View a users statement\n");
                    #endif
                    printf("Enter target users account number");
                    scanf("%s", user_ID);
                    print_statement(user_ID);
                    break;
            case 4 :
                    #ifdef DEBUG
                    printf("View balance\n");
                    #endif
                    printf("Your balance is: $%.2lf\n", get_balance(user));
                    break;
            case 5 :
                    #ifdef DEBUG
                    printf("Withdraw funds\n");
                    #endif
                    printf("Please enter how much you wish to withdraw:>");
                    scanf("%lf", &val);
                    withdraw(user, val);
                    break;
            case 6 :
                    #ifdef DEBUG
                    printf("Depoit funds\n");
                    #endif
                    printf("Please enter how much you wish to deposit:>");
                    scanf("%lf", &val);
                    deposit(user, val);
                    break;
            case 7 :
                    #ifdef DEBUG
                    printf("Transfer funds\n");
                    #endif
                    printf("What account ID would you like to transfer to>");
                    scanf("%s", user_ID);
                    printf("How much funds would like to transfer>");
                    scanf("%lf", &val);
                    transfer(user, user_ID, val);
                    break;
            case 8 :
                    #ifdef DEBUG
                    printf("Change user password\n");
                    #endif
                    printf("Enter target users account number");
                    scanf("%s", user_ID);
                    change_password(user_ID);
                    break;
            case 9 :
                    #ifdef DEBUG
                    printf("View all account numbers\n");
                    #endif
                    print_users(start);
                    break;
            case 10:
                    printf("Logging out\n");
                    return;
	    case 11: 
                    /*printf("Encrypt; Key, String:\n");*/
		    strcpy(str_temp,encryption("crypt", str_temp));
                    printf("%s",str_temp);
                    break;
	    case 12:
                    /*printf("Decrypt; Key, String:\n");*/
		    strcpy(str_temp,decryption("crypt", str_temp));
                    printf("%s <%d>", str_temp, strcmp(str_temp,"string"));
                    break;
            case 13:
                    printf("Validate PW, String:\n");
                    scanf("%s", str_temp);
		    validate_user_pw(str_temp);
                    break;
            default:
                    printf("Invalid input\n");
                    break;
        }
    }
}

/*******************************************************************************
 * Description
 * this is the menu that will be given to the admins, with special functions 
 * such as choosing the user rather then having it forced into args of some 
 * functions
 * this is where admins can choose what function they require
 * INPUTS:
 * logged_user_t
 * OUTPUTS:
 * -nothing
 * POST:
 * will be filled with the lasted user data after the user has choosen to log 
 * out
*******************************************************************************/
void admin_menu(logged_user_t * user)
{
    users_t * start = NULL;
    start = malloc(sizeof(users_t));

    if(start == NULL)
    {
        printf("mem error");
        return ;
    }
    strcpy(start->user_lvl,"start");
    start->next = NULL;

    char user_ID[10];
  
    read_users(start);

    int user_input = -1;
    while(1)
    {
        printf("\n"
        "1. Add user\n"
        "2. Remove user\n"
        "3. View user statement\n"
        "4. Change users password\n"
        "4. Logout\n");
    
        scanf("%d", &user_input);
        switch(user_input)
        {
            case 1 :
                    #ifdef DEBUG
                    printf("Add user\n");
                    #endif
                    add_user(start);
                    printf("\nPrinting total users\n");
                    print_users(start);
                    break;
            case 2 :
                    #ifdef DEBUG
                    printf("Remove user\n");
                    #endif
                    delete_user(start);
                    break;
            case 3 :
                    #ifdef DEBUG
                    printf("View a users statement\n");
                    #endif
                    printf("Enter target users account number");
                    scanf("%s", user_ID);
                    print_statement(user_ID);
                    break;
            case 4 :
                    #ifdef DEBUG
                    printf("Change user password\n");
                    #endif
                    printf("Enter target users account number");
                    scanf("%s", user_ID);
                    change_password(user_ID);
                    break;
            case 5 :
                    printf("Logging out\n");
                    return;
            default:
                    printf("Invalid input\n");
                    break;
        }
    }
}

/*******************************************************************************
 * Description
 * this is the menu that will be given to the users, with all functions having 
 * args forced into the functions 
 * functions
 * this is where users can choose banking actions they need
 * INPUTS:
 * logged_user_t
 * OUTPUTS:
 * -nothing
 * POST:
 * will be filled with the lasted user data after the user has choosen to log 
 * out
*******************************************************************************/
void user_menu(logged_user_t * user)
{
    int user_input = -1;
    double val = 0;
    char user_ID[USER_MAX_NUM_LEN + 1];
    while(1)
    {
        printf("\n"
        "1. View balance\n"
        "2. Withdraw funds\n"
        "3. Deposit funds\n"
        "4. Transfer funds\n"
        "5. View user statement\n"
        "6. Change password\n"
        "7. Logout\n");
    
        scanf("%d", &user_input);
        switch(user_input)
        {
            case 1 :
                    #ifdef DEBUG
                    printf("View balance\n");
                    #endif
                    printf("%.2lf",get_balance(user));
                    break;
            case 2 :
                    #ifdef DEBUG
                    printf("Withdraw funds\n");
                    #endif
                    printf("How much funds would like to withdraw>");
                    scanf("%lf", &val);
                    withdraw(user, val);
                    break;
            case 3 :
                    #ifdef DEBUG
                    printf("Deposit funds\n");
                    #endif
                    printf("How much funds would like to deposit>");
                    scanf("%lf", &val);
                    deposit(user, val);
                    break;
            case 4 :
                    #ifdef DEBUG
                    printf("Transfer funds\n");
                    #endif
                    printf("What account ID would you like to transfer to>");
                    scanf("%s", user_ID);
                    printf("How much funds would like to transfer>");
                    scanf("%lf", &val);
                    transfer(user, user_ID, val);
                    break;
            case 5 :
                    #ifdef DEBUG
                    printf("View user statement");
                    #endif
                    print_statement(user->user_num);
                    break;
            case 6 :
                    #ifdef DEBUG
                    printf("Change password");
                    #endif
                    change_password(user->user_num);
                    break;
            case 7 :
                    printf("Logging out\n");
                    return;
            default:
                    printf("Invalid input\n");
                    break;
        }
    }
}

/*******************************************************************************
 * Description
 * this function will print the group of new users
 * INPUTS:
 * users_t
 * OUTPUTS:
 * -nothing
 * POST:
 * what happens to pointers and data after the function
*******************************************************************************/
void print_users(users_t * user)
{
    users_t * it = user;

    while(it != NULL)
    {
        printf("%s\n", it->user_num);
        it = it->next;
    }
}
