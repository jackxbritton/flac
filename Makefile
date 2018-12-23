OBJS = main.o
CFLAGS = $(shell pkg-config --cflags flac)
LIBS = $(shell pkg-config --libs flac) -lm
TARGET = flac

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LIBS)

clean:
	rm $(TARGET) $(OBJS)

