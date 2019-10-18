#include "global.h"

/*******************************************************************************
* Description
* This function inputs a user ID string and will validate that it is
* valid as per the group outline such as first two chars being letters and
* having to be capital letters, followed by 1 to 4 intergers. this function
* will not change the user ID string
* inputs:
* - user_ID
* outputs:
* - 1 is input is valid, -1 is the input isn't valid
*******************************************************************************/
int validate_user_ID(const char user_ID[])
{
	/*initlise incrementing variable*/
	int i;
	/*test if userID is longer then the maximum determined size if it is
	return an error*/
	if ((int)strlen(user_ID) > USER_MAX_NUM_LEN)
	{
		return -1;
	}
	/*iterate through the letters of the userID checking they are capiital
	letters*/
	for (i = 0; i < USER_NUM_LETTER_LEN; i++)
	{
		/*test if the userID letters are between A - Z or 65 - 90*/
		if (user_ID[i] < 'A' || user_ID[i] > 'Z')
		{
			return -1;
		}
	}
	for (i = USER_NUM_LETTER_LEN; i < USER_MAX_NUM_LEN; i++)
	{
		/*test if the userID is at the end of the string as userID can
		be less then 6 chars*/
		if (user_ID[i] == '\0' && i > USER_NUM_LETTER_LEN)
		{
			return 1;
		}
		/*test if the userID numbers are between '0' - '9' or 48 - 57*/
		else if (user_ID[i] < '0' || user_ID[i] > '9')
		{
			return -1;
		}
	}
	return 1;
}

/*******************************************************************************
* Description
* INPUTS:
* Takes user_pw[], which is an assumed string.
* OUTPUTS:
* Returns the number of characters in that string, -1 if the password is
* invalid.
* POST:
* what happens to pointers and data after the function, I dunno.
*******************************************************************************/
int validate_user_pw(const char user_pw[])
{
	int i, upper = 0, lower = 0, digit = 0, special = 0;
	if (user_pw == NULL || strlen(user_pw) == 0)
	{
		return -1;
	}
	for (i = 0; i < strlen(user_pw); i++) {
		/*printf("%d\n", user_pw[i]);*/
		if (user_pw[i] >= 'a' && user_pw[i] <= 'z') {
			lower++;
		}
		if (user_pw[i] >= 'A' && user_pw[i] <= 'Z') {
			upper++;
		}
		if (user_pw[i] >= '0' && user_pw[i] <= '9') {
			digit++;
		}
		if (user_pw[i] >= 33 && user_pw[i] <= 47) {
			special++;
		}
		if (user_pw[i] >= 58 && user_pw[i] <= 64) {
			special++;
		}

	}
	if ((lower < 1) || (upper < 1) || (digit < 1) || (special < 1))
	{
		return -1;
	}
	return (lower + upper + digit + special);
}

/*******************************************************************************
* Description
* this function validates if the date time function is logical and valid with
* the real world representation of time
* INPUTS:
* date_time_t
* OUTPUTS:
* -1 if invalid, 1 if valid
* POST:
* const array no change
*******************************************************************************/
int validate_date_time(const struct date_time time)
{
	/*test all the variable within time date to validate they are logical based
	on the real world representation of time*/
	if (time.month < MONTHS_LOWER ||
		time.month > MONTHS_UPPER ||
		time.day < DAYS_LOWER ||
		time.day >= DAYS_UPPER ||
		time.hour < HOURS_LOWER ||
		time.hour > HOURS_UPPER ||
		time.minute < MINUTES_LOWER ||
		time.minute >= MINUTES_UPPER)
	{
		return -1;
	}
	return 1;
}

/*******************************************************************************
* Description
* This function asks the security questions to the user and verify if it
* matches with the one from the files
* INPUTS:
* char user_ID
* OUTPUTS:
* returns -1 if wasn't able to open the file, returns 1 otherwise
* POST:
* const array no change
*******************************************************************************/
int validate_sq(const char user_ID[USER_MAX_NUM_LEN]) {
	user_security_questions_t userSq;
	char answer1[ANSWER_MAX_LEN];
	char answer2[ANSWER_MAX_LEN];
	char answer3[ANSWER_MAX_LEN];
	char file_name[USER_MAX_NUM_LEN + 5];

	printf("You have failed logging in\n"
		"Please answer the 3 security questions to reset your password\n");

	FILE *fptr = NULL;
	strcpy(file_name, user_ID);
	strcat(file_name, "sq.txt");
	fptr = fopen(file_name, "rb");

	if (fptr == NULL)
	{
		printf("mem error file location doesnt exist");
	}

	while (fptr != NULL) {
		fread(&userSq, sizeof(user_security_questions_t), 1, fptr);
		printf("Question 1 : %s", userSq.sq1);
		fgets(answer1, ANSWER_MAX_LEN, stdin);;
		printf("Question 2 : %s", userSq.sq2);
		fgets(answer2, ANSWER_MAX_LEN, stdin);
		printf("Question 3 : %s", userSq.sq3);
		fgets(answer3, ANSWER_MAX_LEN, stdin);
		if (strcmp(userSq.ans1, answer1) == 0 &&
			strcmp(userSq.ans2, answer2) == 0 &&
			strcmp(userSq.ans3, answer3) == 0)
		{
			fclose(fptr);
			return 1;
		}
		if (strcmp(userSq.ans1, answer1) != 0) {
			printf("Answer 1 is incorrect\n");
		}
		if (strcmp(userSq.ans2, answer2) != 0) {
			printf("Answer 2 is incorrect\n");
		}
		if (strcmp(userSq.ans3, answer3) != 0) {
			printf("Answer 3 is incorrect\n");
		}
		printf("Please try again\n");

	}
	change_password(user_ID);
	return -1;
}

/*******************************************************************************
* Description
* This function inputs a transaction type and adds spaces at the end to make
* sure it is ready to print in printf format
* inputs:
* - type[]
* outputs:
* none
* post:
* transaction type will be six chars longs with a null charater at the end
*******************************************************************************/
void format_trans_type(char type[])
{
	/*initlise incrementing variables*/
	int i, j = 0;
	/*interate through the flightcode untiil the end of the string is found*/
	for (i = 0; i < MAX_TYPE_LEN; i++)
	{
		if (type[i] == '\0')
		{
			/*fill the rest of the chars within the string as spaces to format
			the flight code to be printed the same no matter the size*/
			for (j = i; j < MAX_TYPE_LEN; j++)
			{
				type[j] = ' ';
			}
			type[MAX_TYPE_LEN] = '\0';
			return;
		}
	}
	type[MAX_TYPE_LEN] = '\0';
	return;
}

/*******************************************************************************
* Descritpion
* This function inputs a user level string and checks its valid
* inputs:
* - user_lvl[]
* outputs:
* 1 is valid, -1 if invalid
* post:
* array is const
*******************************************************************************/
int validate_user_lvl(const char user_lvl[])
{
	if (strcmp(user_lvl, USER) == 0)
	{
		return 1;
	}
	if (strcmp(user_lvl, ADMIN) == 0)
	{
		return 1;
	}
	return -1;
}