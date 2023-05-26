NAME = minirt
SRCS =	minirt.c get_next_line.c get_next_line_utils.c \
		mrt_hook.c vec_cal.c vec_cal2.c vec_cal3.c	\
		mrt_cam_init.c mrt_hit_obj.c mrt_input.c mrt_input2.c mrt_ratio.c \
		mrt_ray_color.c mrt_set.c mrt_set_obj.c mrt_shadow.c mrt_utils.c mrt_set_ratio.c
OBJS = $(SRCS:.c=.o)


all : $(NAME)

%.o: %.c
	$(CC) $(CCOP) -c $< -o $@

$(NAME) : $(OBJS)
	cc -lmlx -framework OpenGL -framework Appkit $(OBJS) -o $(NAME) -fsanitize=address

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
