################################
#    level | 4 | 3 | 2 | 1 | 0 #
################################
#    LOGVV | x |   |   |   |   # Very verbose
#    LOGV  | x | x |   |   |   # Verbose
#    LOGD  | x | x | x |   |   # Debug
#    LOGE  | x | x | x | x |   # Error
#    LOGI  | x | x | x | x |   # Information
################################
LOG_LEVEL=0

# Window parameters
WINDOW_WIDTH=800
WINDOW_HEIGHT=600
WINDOW_RESIZABLE=1
ENABLE_FULLSCREEN=1
DEFAULT_FULLSCREEN=0

# Simulator parameters
BLACK_HOLE_ON_CREATE=0
ENABLE_COLLISIONS=1

# Field view subsample
FIELD_VIEW_SUBSAMPLE_X = 4
FIELD_VIEW_SUBSAMPLE_Y = 4


#####################################################
#####################################################
TARGET=gravity

# Window
LDFLAGS += \
	-DWINDOW_WIDTH=$(WINDOW_WIDTH) \
	-DWINDOW_HEIGHT=$(WINDOW_HEIGHT) \
	-DENABLE_FULLSCREEN=$(ENABLE_FULLSCREEN) \
	-DDEFAULT_FULLSCREEN=$(DEFAULT_FULLSCREEN)

# Convert bool to SDL_Bool for WINDOW_RESIZABLE
ifeq ($(WINDOW_RESIZABLE), true)
LDFLAGS += -DWINDOW_RESIZABLE=SDL_TRUE
else
LDFLAGS += -DWINDOW_RESIZABLE=SDL_FALSE
endif

# Params
LDFLAGS += \
	-DBLACK_HOLE_ON_CREATE=$(BLACK_HOLE_ON_CREATE) \
	-DENABLE_COLLISIONS=$(ENABLE_COLLISIONS)

# Field subsampling
LDFLAGS += \
	-DFIELD_VIEW_SUBSAMPLE_Y=$(FIELD_VIEW_SUBSAMPLE_Y) \
	-DFIELD_VIEW_SUBSAMPLE_X=$(FIELD_VIEW_SUBSAMPLE_X)

CC=g++ -std=c++17

SRC=src
BUILD=build

COMPILER_FLAGS=-w -O3
LINKER_FLAGS=-lm -lSDL2 -lSDL -lSDL2_gfx
LDFLAGS += \
	-DLOG_LEVEL=$(LOG_LEVEL) \

OBJS=$(SRC)/gravity.cpp $(SRC)/Game.cpp $(SRC)/Universe.cpp $(SRC)/Timer.cpp $(SRC)/Launcher.cpp

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
