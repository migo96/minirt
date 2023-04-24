NAME = minirt
SRCS = minirt.c get_next_line.c get_next_line_utils.c ver_cal.c
OBJS = minirt.o get_next_line.o get_next_line_utils.o ver_cal.o
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
