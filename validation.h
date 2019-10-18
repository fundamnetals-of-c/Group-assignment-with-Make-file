#include "structs.h"

int validate_user_ID(const char user_ID[]); /*james*/
int validate_user_pw(const char user_pw[]); /*walter*/
int create_sq(const char user_ID[USER_MAX_NUM_LEN]); /*seb*/
int trans_cmp(const struct date_time trans_dt, const struct date_time date_dt);
int validate_sq(const char user_ID[USER_MAX_NUM_LEN]); /*seb*/
int validate_date_time(const struct date_time time); /*james*/
int validate_user_lvl(const char user_lvl[]); /*james*/
