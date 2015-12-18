CC				=	gcc
CFLAGS			=	-g -static
AR     			=	ar rc
OBJS 			=	list.o iterator.o daemon.o ssdp_socket.o ssdp_server.o 
OBJS_PATH		=	
LIB_STATIC  	=	libssdp.a
LIB_STATIC_PATH	=	./
TARGET			=	ssdp

all: $(OBJS)
	$(AR) $(LIB_STATIC) $(OBJS) 
	$(CC) $(OBJS) -o $(TARGET) $(CFLAGS) $(LIB_STATIC_PATH)$(LIB_STATIC)
	$(RM) $(OBJS)
	@echo "make finished."
$(OBJS):%.o : %.c
	$(CC) -c $^ -o $@

clean:
	rm  $(TARGET) $(LIB_STATIC)
