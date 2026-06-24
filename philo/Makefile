# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abroslav <abroslav@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/04/18 20:51:19 by abroslav          #+#    #+#              #
#    Updated: 2026/06/24 21:50:00 by abroslav         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

#compiler
CC = cc

#flags
CFLAGS = -Wall -Wextra -Werror -pthread

#directories
SRC_DIR = src
OBJ_DIR = obj
INCLUDES_DIR = includes

#includes
INCLUDES = -I$(INCLUDES_DIR)

#sources
SRCS = 	$(SRC_DIR)/main.c \
	  	$(SRC_DIR)/setup.c \
	  	$(SRC_DIR)/philosopher.c \
	 	$(SRC_DIR)/monitor.c \
	 	$(SRC_DIR)/actions.c \
	 	$(SRC_DIR)/utils.c \

#objects
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

#targets
all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(INCLUDES) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
