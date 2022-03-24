FT			=	ft
STD			=	std

RAND		=	$$RANDOM

NAME		=	$(FT) $(STD)

CC			=	clang++
INCLUDES	=	includes
CPPFLAGS	=	-Wall -Werror -Wextra -I$(INCLUDES)
RM			=	rm -rf

SRCS		=	main.cpp

FT_OBJS		=	$(SRCS:%.cpp=%.ft.o)
STD_OBJS	=	$(SRCS:%.cpp=%.std.o)

all:		$(NAME)

$(FT):		$(FT_OBJS)
			@$(CC) $(CPPFLAGS) $(FT_OBJS) -o $(FT)
			@echo "Linked into executable \033[38;2;39;154;186m$(FT)\033[0m"

$(STD):		$(STD_OBJS)
			@$(CC) $(CPPFLAGS) $(STD_OBJS) -o $(STD)
			@echo "Linked into executable \033[38;2;39;154;186m$(STD)\033[0m"

diff:		$(NAME)	
			@$(RM) $(FT)_out $(STD)_out
			@./$(FT) RAND
			@./$(STD) RAND
			@diff -y --suppress-common-lines $(FT)_out $(STD)_out

%.ft.o:		%.cpp
			@$(CC) $(CPPFLAGS) -c $< -o $(<:.cpp=.ft.o)
			@echo "Compiling $<."

%.std.o:	CPPFLAGS += -DSTD
%.std.o:	%.cpp
			@$(CC) $(CPPFLAGS) -c $< -o $(<:.cpp=.std.o)
			@echo "Compiling $<."

clean:
			@$(RM) $(FT_OBJS) $(STD_OBJS)
			@echo "Removed object files." 

fclean:		clean
			@$(RM) $(NAME)
			@$(RM) $(FT)_out $(STD)_out
			@echo "Removed executable and output files."

re:			fclean all

.PHONY:		fclean, all, re, diff