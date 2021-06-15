# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abel-mak <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/06/01 14:07:03 by abel-mak          #+#    #+#              #
#    Updated: 2021/06/14 15:41:13 by abel-mak         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME= philo

SRC= philo_one.c utils.c main.c

OBJ= $(SRC:.c=.o)

FLAGS= -Wall -Werror -Wextra

INC= philo_one.h 

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
