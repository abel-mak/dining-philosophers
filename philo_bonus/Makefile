# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abel-mak <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/05/31 17:02:44 by abel-mak          #+#    #+#              #
#    Updated: 2021/06/14 16:06:06 by abel-mak         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME= philo_bonus

SRC= ft_itoa.c init.c main.c philo_three.c utils.c

OBJ= $(SRC:.c=.o)

INC= philo_three.h

FLAGS= -Wall -Werror -Wextra

all: $(NAME)

$(NAME): $(OBJ)
	gcc $(FLAGS) $(OBJ) -o $(NAME)

%.o : %.c $(INC)
	gcc -c $(FLAGS) $< 

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all
