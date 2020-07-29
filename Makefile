################################
#    level | 4 | 3 | 2 | 1 | 0 #
################################
#    LOGVV | x |   |   |   |   # Very verbose
#    LOGV  | x | x |   |   |   # Verbose
#    LOGD  | x | x | x |   |   # Debug
#    LOGI  | x | x | x | x |   # Information
#    LOGE  | x | x | x | x | x # Error
################################
LOG_LEVEL=1

# Window parameters
WINDOW_RESIZABLE=1
ENABLE_FULLSCREEN=1
DEFAULT_FULLSCREEN=1

# Simulator parameters
STAR_ON_CREATE=0
FIXED_STARS=1
ENABLE_FIELD=0

# Field view subsample
FIELD_VIEW_SUBSAMPLE_X = 4
FIELD_VIEW_SUBSAMPLE_Y = 4


#####################################################
#####################################################
TARGET=gravity

# Window
LDFLAGS += \
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
	-DSTAR_ON_CREATE=$(STAR_ON_CREATE) \
	-DENABLE_COLLISIONS=$(ENABLE_COLLISIONS) \
	-DENABLE_FIELD=$(ENABLE_FIELD) \
	-DFIXED_STARS=$(FIXED_STARS)

# Field subsampling
LDFLAGS += \
	-DFIELD_VIEW_SUBSAMPLE_Y=$(FIELD_VIEW_SUBSAMPLE_Y) \
	-DFIELD_VIEW_SUBSAMPLE_X=$(FIELD_VIEW_SUBSAMPLE_X)

CC=g++ -std=c++17

SRC=src
INCLUDE=include
BUILD=build

COMPILER_FLAGS=-w -O3
LINKER_FLAGS=-lm -lSDL2 -lSDL2_gfx
LDFLAGS += \
	-DLOG_LEVEL=$(LOG_LEVEL) \

OBJS=$(SRC)/gravity.cpp $(SRC)/Game.cpp $(SRC)/Universe.cpp $(SRC)/Timer.cpp $(SRC)/Launcher.cpp

all:
	$(info Building target...)
	@make init
	$(CC) -I $(INCLUDE)/ $(COMPILER_FLAGS) $(LDFLAGS) $(OBJS) -o $(BUILD)/$(TARGET) $(LINKER_FLAGS)

run:
	./$(BUILD)/$(TARGET)

build-run:
	$(MAKE) all
	$(MAKE) run

init:
	$(info Init build directory...)
	@mkdir -p $(BUILD)

clean:
	$(info Cleaning...)
	@rm -rf $(BUILD)
