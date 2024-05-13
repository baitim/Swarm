CFLAGS   = -O3

LFLAGS   = -lm -lpthread `sdl2-config --cflags --libs` -lSDL2 -lSDL2_ttf

VALGRIND_FLAGS = # valgrind --leak-check=full

CPP      = gcc
LINKER   = gcc
rm       = rm -rf

SRCDIR   = src
OBJDIR   = obj

CMD_FLAGS = 

TARGET   = swarm

SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

$(TARGET): $(IMAGEDIR) $(TXTDIR) $(OBJDIR) $(OBJECTS)
	@$(LINKER) $(OBJECTS) $(LFLAGS) -o $@
	@echo "Linking complete!"

$(IMAGEDIR):
	@mkdir $(IMAGEDIR)
	@echo "Created images directory!"

$(TXTDIR):
	@mkdir $(TXTDIR)
	@echo "Created txt directory!"

$(OBJDIR):
	@mkdir $(OBJDIR)
	@echo "Created obj directory!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@$(CPP) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

.PHONY: clean
clean:
	@$(rm) $(OBJECTS)
	@$(rm) $(TARGET)
	@echo "Cleanup complete!"

.PHONY: run
run:
	@$(VALGRIND_FLAGS) ./$(TARGET) $(CMD_FLAGS)