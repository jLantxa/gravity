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
LOG_LEVEL=1

# Window parameters
WINDOW_WIDTH=1024
WINDOW_HEIGHT=768
WINDOW_RESIZABLE=true
ENABLE_FULLSCREEN=true
DEFAULT_FULLSCREEN=false

FIELD_VIEW_SUBSAMPLE_X = 4
FIELD_VIEW_SUBSAMPLE_Y = 4


# Enable fullscreen
ifeq ($(ENABLE_FULLSCREEN), true)
LDFLAGS += \
	-DENABLE_FULLSCREEN

	ifeq ($(DEFAULT_FULLSCREEN), true)
	LDFLAGS += -DDEFAULT_FULLSCREEN=1
	else
	LDFLAGS += -DDEFAULT_FULLSCREEN=0
	endif
endif

# Define window size
LDFLAGS += \
	-DWINDOW_WIDTH=$(WINDOW_WIDTH) \
	-DWINDOW_HEIGHT=$(WINDOW_HEIGHT)

# Convert bool to SDL_Bool for WINDOW_RESIZABLE
ifeq ($(WINDOW_RESIZABLE), true)
LDFLAGS += -DWINDOW_RESIZABLE=SDL_TRUE
else
LDFLAGS += -DWINDOW_RESIZABLE=SDL_FALSE
endif

LDFLAGS += \
	-DBLACK_HOLE_ON_CREATE \
	-DENABLE_COLLISIONS

LDFLAGS += \
	-DFIELD_VIEW_SUBSAMPLE_Y=$(FIELD_VIEW_SUBSAMPLE_Y) \
	-DFIELD_VIEW_SUBSAMPLE_X=$(FIELD_VIEW_SUBSAMPLE_X)

CC=g++

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
