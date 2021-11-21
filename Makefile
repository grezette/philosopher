NAME 	= philo

SRCS 	=	srcs/philo.c \
			srcs/ph_threads.c \
			srcs/philo_parsing.c

OBJS 	= $(SRCS:.c=.o)

RM		= 	rm -rf

CC 		= gcc

FLAGS 	= -Wall -Wextra -Werror -pthread

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
