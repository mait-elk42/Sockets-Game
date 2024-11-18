# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mait-elk <mait-elk@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/16 18:52:27 by mait-elk          #+#    #+#              #
#    Updated: 2024/11/17 17:20:00 by mait-elk         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

COMPILER= cc
LIBS= -Llib -lmlx -lft -framework OpenGL -framework AppKit
CFLAGS= -Wall -Wextra -Werror -I include -fsanitize=address -g
SRCS= src/system.c src/events.c src/init.c src/utils.c
SRCS_O= $(SRCS:.c=.o)
NAME= game


all: $(NAME)

$(NAME): $(SRCS_O) main.c
	$(COMPILER) $(CFLAGS) main.c $(SRCS_O) $(LIBS) -o $(NAME)

%.o: %.c %.h
	$(COMPILER) -c $< $(CFLAGS) -o $@

clean:
	rm -f $(SRCS_O)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: clean
