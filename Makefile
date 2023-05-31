NAME = miniRT
B_NAME = miniRT_bonus
SRCS =	$(wildcard *.c)
B_SRCS = $(wildcard ./bonus/*.c)
OBJS = $(SRCS:.c=.o)
B_OBJS = $(B_SRCS:.c=.o)
CC = cc

ifdef W_BO
	OBJECTS = $(B_OBJS)
	TARGET = $(B_NAME)
else
	OBJECTS = $(OBJS)
	TARGET = $(NAME) 
endif

all : $(TARGET)

%.o: %.c
	$(CC) $(CCOP) -c $< -o $@

$(NAME) : $(OBJECTS)
	cc -lmlx -framework OpenGL -framework Appkit $(OBJECTS) -o $(NAME)

$(B_NAME) : $(OBJECTS)
	cc -lmlx -framework OpenGL -framework Appkit $(OBJECTS) -o $(B_NAME)

bonus : 
	make W_BO=1 all

clean :
	rm -rf $(OBJS) $(B_OBJS)
.PHONY : clean

fclean : clean
	rm -rf $(NAME) $(B_NAME)
.PHONY : fclean

re : fclean all

.PHONY : bonus clean fclean re