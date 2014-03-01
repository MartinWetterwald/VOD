THIS := $(lastword $(MAKEFILE_LIST))
MAKEFLAGS += --no-print-directory

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

SRC = $(shell find $(SRCDIR) -iname "*.cpp")
OBJ = $(addprefix $(OBJDIR), $(notdir $(SRC:%.cpp=%.o)))
DEP = $(wildcard $(DEPDIR)*.d)

.PHONY: clean mrproper all

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


$(OBJDIR)%.o: $(SRCDIR)%.cpp $(THIS)
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
