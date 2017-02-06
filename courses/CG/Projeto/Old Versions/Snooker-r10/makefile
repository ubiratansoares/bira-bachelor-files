CC				= gcc
CFLAGS			= -O2 -Wall -c
LIBS_GL			= -L/lib -lglu -lgl -lglut
LIBS			= $(LIBS_GL)
LDFLAGS			=
LD				= ld

OBJ_OUT			= .
BIN_OUT			= .
OBJS			= SnookerUtils.o Line.o Table.o Ball.o Snooker.o main.o

all				: Snooker.exe

Snooker.exe		: $(OBJS)
				$(CC) $(OBJ_OUT)/*.o -o $(BIN_OUT)/Snooker.exe $(LDFLAGS) $(LIBS)

SnookerUtils.o	:
				$(CC) SnookerUtils.c -o $(OBJ_OUT)/SnookerUtils.o $(CFLAGS)
				
Line.o			:
				$(CC) Line.c -o $(OBJ_OUT)/Line.o $(CFLAGS)
			
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
				
run				:
				./Snooker.exe