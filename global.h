#include "structs.h"

/*******************************************************************************
 * Function prototypes - define your own functions if required.
*******************************************************************************/
/*menus*/
int login_menu(logged_user_t * logged_user); /*james*/
void print_menu(logged_user_t * user); /*james*/
void dev_menu(logged_user_t * user); /*james*/
void admin_menu(logged_user_t * user); /*james*/
void user_menu(logged_user_t * user); /*james*/

void print_users(users_t * user); /*james*/

/*user functions*/
double get_balance(const logged_user_t * user); /*james*/
int deposit(logged_user_t * user, double value); /*tam*/
int withdraw(logged_user_t * user, double value); /*tam*/
int transfer(logged_user_t * user, 
                const char target_acc[USER_MAX_NUM_LEN],
                double value); /*james*/
int print_statement(const char user_ID[USER_MAX_NUM_LEN]); /*terry*/
int change_password(const char user_ID[USER_MAX_NUM_LEN]); /*james*/

/*admin functions*/
int add_user(users_t * user); /*james*/
int delete_user(users_t * user); /*james*/
int store_users(users_t * user); /*james*/
int read_users(users_t * users); /*james*/
int write_users(users_t * users); /*james*/

int validate_user_ID(const char user_ID[]); /*james*/
int validate_user_pw(const char user_pw[]); /*walter*/
int create_sq(const char user_ID[USER_MAX_NUM_LEN]); /*seb*/
int trans_cmp(const struct date_time trans_dt, const struct date_time date_dt);
int validate_sq(const char user_ID[USER_MAX_NUM_LEN]); /*seb*/
int validate_date_time(const struct date_time time); /*james*/
int validate_user_lvl(const char user_lvl[]); /*james*/

const char* encryption(char key[], char string[]); /*walter*/
const char* decryption(char key[], char string[]); /*walter*/
void format_trans_type(char type[]); /*terry*/




