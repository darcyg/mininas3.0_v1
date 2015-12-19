CC				=	gcc
CFLAGS			=	-g
OBJS 			=	../../src/systemInfo.o ../../src/database_operate.o ../../src/system_operate.o
OBJS_PATH		=	
TARGET			=	../../res/systemInfo

all: $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(CFLAGS)  -lmysqlclient
	$(RM) $(OBJS)
	@echo "make finished."
$(OBJS):%.o : %.c
	$(CC) -c $^ -o $@

clean:
	rm  $(TARGET) $(LIB_STATIC)