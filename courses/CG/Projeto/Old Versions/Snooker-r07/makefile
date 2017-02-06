CC				= gcc
CFLAGS			= -O2 -Wall -c -g
LIBS_GL			= -lglut32 -lopengl32
LIBS			= $(LIBS_GL)
LDFLAGS			=
LD				= ld

OBJ_OUT			= obj/Debug
BIN_OUT			= bin/Debug
OBJS			= SnookerUtils.o Table.o Ball.o Snooker.o main.o

all				: Snooker.exe

Snooker.exe		: $(OBJS)
				$(CC) $(OBJS) -o $(BIN_OUT)/Snooker.exe $(LDFLAGS) $(LIBS)

SnookerUtils.o	:
				$(CC) SnookerUtils.c -o $(OBJ_OUT)/SnookerUtils.o $(CFLAGS)
			
Table.o			:
				$(CC) Table.c -o $(OBJ_OUT)/Table.o $(CFLAGS)
			
Ball.o			:
				$(CC) Ball.c -o $(OBJ_OUT)/Ball.o $(CFLAGS)
				
Snooker.o		:
				$(CC) Snooker.c -o $(OBJ_OUT)/Snooker.o $(CFLAGS)

main.o			:
				$(CC) main.c -o $(OBJ_OUT)/main.o $(CFLAGS)

rebuild			: clean all			
			
clean			:
				@rm -v $(OBJ_OUT)/*.o