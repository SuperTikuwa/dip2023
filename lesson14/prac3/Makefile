CC  = g++
CFLAGS = -std=c++11 `pkg-config --cflags --libs opencv4`
TARGET = a.out
SRCS = main.cpp particlefilter.cpp
OBJS = $(SRCS:.cpp=.o)

.PHONY: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: $(OBJS)
$(OBJS): $(SRCS)
	$(CC) -c $(SRCS) $(CFLAGS)

.PHONY: run
run:
	./$(TARGET)

.PHONY: all
all: clean $(OBJS) $(TARGET) run

.PHONY: clean
clean:
	-rm -f $(OBJS) $(TARGET) *.d