# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/14 18:06:36 by rkhakimu          #+#    #+#              #
#    Updated: 2024/11/14 18:18:04 by rkhakimu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME				= philosophers
CC					= cc
CFLAGS				= -Wall -Wextra -Werror -I./inc

SRC_DIR				= philo
INC_DIR				= inc

SRC_FILES			= $(SRC_DIR)/main.c \
					  $(SRC_DIR)/ft_atoi.c \
					  $(SRC_DIR)/ft_atol.c \
					  $(SRC_DIR)/initialization.c \
					  $(SRC_DIR)/routine.c \
					  $(SRC_DIR)/utils.c \
					  $(SRC_DIR)/parsing_args.c \
					  $(SRC_DIR)/freeing.c

OBJ_FILES			= $(SRC_FILES:.c=.o)

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJ_FILES)
	$(CC) $(CFLAGS) $(OBJ_FILES) -o $(NAME)
	@echo "Compiled executable: $(NAME)"

%.o: $(SRC_DIR)/%.c $(INC_DIR)/philosophers.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_FILES)
	@echo "Object files removed"

fclean: clean
	rm -f $(NAME)
	@echo "Executable removed"

re: fclean all