CC				=	gcc
RM 				=	rm -rf
CFLAGS			=	-g
AR 				= 	ar rc
OBJS 			=	../../src/systemInfo.o ../../src/database_operate.o ../../src/system_operate.o
OBJS_PATH		=	
LIB_STATIC  	=	../../lib/libssdp.a
TARGET			=	../../res/systemInfo

all: $(OBJS)
	$(AR) $(LIB_STATIC) $(OBJS) 
	$(CC) $(OBJS) -o $(TARGET) $(CFLAGS)  $(LIB_STATIC) -lmysqlclient
	$(RM) $(OBJS)
	@echo "make finished."
$(OBJS):%.o : %.c
	$(CC) -c $^ -o $@

clean:
	$(RM)  $(TARGET) $(LIB_STATIC)