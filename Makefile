THIS := $(lastword $(MAKEFILE_LIST))
MAKEFLAGS += --no-print-directory

rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

EXENAME = VOD
SRCDIR = src/
BUILDDIR = build/
BINDIR = $(BUILDDIR)bin/
OBJDIR = $(BUILDDIR)obj/
DEPDIR = $(BUILDDIR)dep/
EXE = $(BINDIR)$(EXENAME)

LIBFOLDER = NetFlux/
LIBBUILDFOLDER = $(LIBFOLDER)build/
LIBBINFOLDER = $(LIBBUILDFOLDER)bin/
LIBINCFOLDER = $(LIBBUILDFOLDER)include/
LIBFILE = $(LIBBINFOLDER)libnetflux.a
LIB = $(patsubst lib%,-l%, $(notdir $(basename $(LIBFILE))))

CC = @clang++

CFLAGS = -std=c++11 -I$(LIBINCFOLDER) -Weverything -Wno-padded -Wno-disabled-macro-expansion -Wno-c++98-compat
LNKFLAGS = -L$(LIBBINFOLDER) $(LIB)

ifneq ($(MODE),release)
CFLAGS += -DDEBUG -g -O0
else
CFLAGS += -Werror -g0 -O3
endif

SRC = $(call rwildcard,$(SRCDIR),*.cpp)
OBJ = $(addprefix $(OBJDIR), $(notdir $(SRC:%.cpp=%.o)))
DEP = $(wildcard $(DEPDIR)*.d)

.PHONY: clean mrproper all
.SECONDEXPANSION:

ifneq ($(words $(OBJ)),0)
$(EXE): $(LIBFILE) $(OBJ)
	@make -C $(LIBFOLDER) $(MAKECMDGOALS)
	@mkdir -p $(BINDIR)
	@printf "%-13s <$@>...\n" "Linking"
	$(CC) -o $@ $(OBJ) $(LNKFLAGS)
	@ln -sf $(EXE) $(EXENAME)
else
$(EXE):
	$(error No source code found)
endif


$(OBJDIR)%.o: $$(call rwildcard,$(SRCDIR),%.cpp) $(THIS)
	@mkdir -p $(DEPDIR)
	@mkdir -p $(OBJDIR)
	@printf "%-13s <$<>...\n" "Compiling"
	$(CC) $(CFLAGS) -o $@ -c -MMD -MF $(addprefix $(DEPDIR), $(notdir $(<:.cpp=.d))) $<

-include $(DEP)

all: mrproper $(EXE)

clean:
	@echo "Cleaning..."
	@rm -rf $(OBJDIR) $(DEPDIR)

mrproper:
	@echo "Cleaning everything..."
	@rm -rf $(BUILDDIR) $(EXENAME)
