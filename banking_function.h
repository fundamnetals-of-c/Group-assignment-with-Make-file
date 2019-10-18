#include "structs.h"

/*user functions*/
double get_balance(const logged_user_t * user); /*james*/
int deposit(logged_user_t * user, double value); /*tam*/
int withdraw(logged_user_t * user, double value); /*tam*/
int transfer(logged_user_t * user, 
                const char target_acc[USER_MAX_NUM_LEN],
                double value); /*james*/
int print_statement(const char user_ID[USER_MAX_NUM_LEN]); /*terry*/
int change_password(const char user_ID[USER_MAX_NUM_LEN]); /*james*/
