OBJS= video.o main.o AY-3-8910.o

TARGET=madam_y

all: ${OBJS}
	$(CC) $(OBJS) -o ${TARGET}
