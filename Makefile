# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pblagoje <pblagoje@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/07 10:49:02 by pblagoje          #+#    #+#              #
#    Updated: 2023/01/16 14:10:55 by pblagoje         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= ircserv

CXX			= c++

CXXFLAGS	= -Wall -Wextra -Werror -std=c++98 -pedantic-errors

# Directories
SRCS_DIR	= srcs/
OBJS_DIR	= objs/
INCS_DIR	= incs/

# Source files
SRCS_FILES	= main.cpp
OBJS_FILES	= $(SRCS_FILES:.cpp=.o)

# Paths
SRCS		= $(addprefix $(SRCS_DIR), $(SRCS_FILES))
OBJS		= $(addprefix $(OBJS_DIR), $(OBJS_FILES))

# Colors / Actions
RESET		= \033[0m
GREEN		= \033[32m
YELLOW		= \033[33m
BLUE		= \033[34m
RED			= \033[31m
UP			= \033[A
CUT			= \033[K

# Main rule
all:		$(NAME)

# Executable compilation rule
$(NAME):	$(OBJS)
			@echo "$(YELLOW)Compiling executable file [$(NAME)]...$(RESET)"
			@$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
			@echo "$(GREEN)[$(NAME)] is compiled!$(RESET)"

# Object files compilation rule
$(OBJS_DIR)%.o: $(SRCS_DIR)%.cpp
			@mkdir -p $(OBJS_DIR)
			@echo "$(YELLOW)Compiling [$@]...$(RESET)"
			@$(CXX) $(CXXFLAGS) -I $(INCS_DIR) -o $@ -c $<
			@printf "$(UP)$(CUT)"
			@echo "$(GREEN)Finished compiling [$@]$(RESET)"
			@printf "$(UP)$(CUT)"

# Other rules
clean:
			@if [ -d "$(OBJS_DIR)" ]; then \
			echo "$(BLUE)Deleting object files...$(RESET)"; \
			rm -rf $(OBJS_DIR); \
			echo "$(GREEN)Object files deleted!$(RESET)"; else \
			echo "No object files to delete."; \
			fi;

fclean:		clean
			@if [ -f "$(NAME)" ]; then \
			echo "$(BLUE)Deleting executable file...$(RESET)"; \
			rm -rf $(NAME); \
			echo "$(GREEN)Executable file deleted!$(RESET)"; else \
			echo "No executable file to delete."; \
			fi;

re:			fclean all

.PHONY:		all clean fclean re
