TARGET=gravity

###############################
#	level | 4 | 3 | 2 | 1 | 0 #
###############################
#	LOGVV |	x |   |   |   |   # Very verbose
#	LOGV  |	x | x |   |   |   # Verbose
#	LOGD  |	x | x | x |   |   # Debug
#	LOGE  |	x | x | x | x |   # Error
#	LOGI  |	x | x | x | x |   #	Information
###############################
LOG_LEVEL=3

CC=g++

SRC=src
BUILD=build

COMPILER_FLAGS=-w -O3
LINKER_FLAGS=-lSDL2 -lm
LDFLAGS += -DLOG_LEVEL=$(LOG_LEVEL)

OBJS=$(SRC)/gravity.cpp $(SRC)/Game.cpp $(SRC)/Universe.cpp $(SRC)/Timer.cpp

all:
	$(info Building target...)
	$(CC) -I $(SRC) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(LDFLAGS) $(OBJS) -o $(BUILD)/$(TARGET)

run:
	./$(BUILD)/$(TARGET)

build-run:
	$(MAKE) all
	$(MAKE) run

init:
	mkdir $(BUILD)

clean:
	$(info Cleaning...)
	rm $(BUILD)/*
