##
## EPITECH PROJECT, 2024
## Makefile
## File description:
## Makefile for the my_ls program
##

SRC 		=	src/manage_errors.c				\
				src/manage_initialization.c		\
				src/manage_header.c				\
				src/manage_display.c			\
				src/manage_updates.c			\
				src/manage_data.c				\
				src/my_top.c

MAIN_SRC 	=	src/main.c

TEST_SRC 	=	tests/tests_manage_error.c

OBJ			=	$(SRC:.c=.o)
MAIN_OBJ	=	$(MAIN_SRC:.c=.o)

LIB_MAKE	=	make -C ./lib/my
CTESTS		=	-lcriterion --coverage
CGCOVR		=	gcovr --exclude tests ; gcovr --branch --exclude tests
CFLAGS		=   -Wall -Wextra -Werror -iquote include
LIBFLAGS	=	-lncurses
RM			=	rm -f
CC			=	gcc

TEST_NAME	=	unit_tests
NAME		=	my_top

all:			$(NAME)

$(NAME):		$(OBJ) $(MAIN_OBJ)
		$(CC) -o $(NAME) $(OBJ) $(MAIN_OBJ) $(CFLAGS) $(LIBFLAGS)

$(TEST_NAME):	clean all
		$(CC) -o $(TEST_NAME) $(TEST_SRC) $(SRC) $(CTESTS)		\
		$(CFLAGS) $(LIBFLAGS)

tests_run:      $(TEST_NAME)
		./$(TEST_NAME) ; $(CGCOVR)

clean:
		$(RM) $(OBJ)
		$(RM) $(MAIN_OBJ)
		$(RM) $(TEST_OBJ)
		$(RM) *.gcno
		$(RM) *.gcda

fclean:			clean
		$(RM) $(NAME)
		$(RM) $(TEST_NAME)

re:				fclean all

.PHONY:			all clean fclean all tests_run
