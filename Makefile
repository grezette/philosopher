NAME 	= philo

SRCS 	=	srcs/philo.c \
			srcs/philo_utils.c \
			srcs/philo_parsing.c

OBJS 	= $(SRCS:.c=.o)

RM		= 	rm -rf

CC 		= gcc

FLAGS 	= -Wall -Wextra -Werror

all:		$(NAME)

$(NAME):	$(OBJS)
				$(CC) $(FLAGS) $^ -o $@

%.o:		%.c
				$(CC) $(FLAGS) -c $< -o $@

clean:
				$(RM) $(OBJS)

fclean:		clean
				$(RM) $(NAME)

re:			fclean all

.PHONY: 	clean fclean re
