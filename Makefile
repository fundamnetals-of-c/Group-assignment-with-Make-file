#this target and files and complie them, saving time writing it in the complier
#making sure it is complied the same way
all:
	gcc -Wall -Werror -ansi -o BMS.out menus.c file_functions.c banking_function.c validation.c serect_questions.c encryption.c main.c -lm
