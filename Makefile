NAME = minirt
SRCS = minirt.c
OBJS = minirt.o
all : $(NAME)

%.o: %.c
	$(CC) -c $< -o $@

$(NAME) : $(OBJS)
	cc -lmlx -framework OpenGL -framework Appkit $(OBJS) -o $(NAME)

bonus : $(OBJS)
	cc -lmlx -framework OpenGL -framework Appkit $(OBJS) -o $(NAME)

clean :
	rm -rf $(OBJS)
.PHONY : clean

fclean : clean
	rm -rf $(NAME)
.PHONY : fclean

re : fclean
	make all
.PHONY : re
