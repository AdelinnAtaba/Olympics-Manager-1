# Olympics Manager Makefile
# Compiler settings
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -I./include
SRCDIR = src
INCDIR = include
BUILDDIR = build
TESTDIR = tests

# Source files
SOURCES = $(SRCDIR)/maina1.cpp $(SRCDIR)/Olympicsa1.cpp $(SRCDIR)/contestant.cpp
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o)
TARGET = $(BUILDDIR)/olympics

# Default target
all: $(TARGET)

# Create build directory if it doesn't exist
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# Compile object files
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@

# Clean build artifacts
clean:
	rm -rf $(BUILDDIR)/*.o $(BUILDDIR)/olympics

# Clean everything
distclean: clean
	rm -rf $(BUILDDIR)

# Run the program
run: $(TARGET)
	$(TARGET)

# Run with test input
test: $(TARGET)
	@echo "Running basic test..."
	@printf "add_country 1 10\nadd_country 2 5\nadd_team 1 1 0\nadd_contestant 1 1 0 100\nadd_contestant_to_team 1 1\nget_team_strength 1\nget_medals 1\n" | $(TARGET)

# Debug build
debug: CXXFLAGS += -g -DDEBUG
debug: $(TARGET)

# Release build (optimized)
release: CXXFLAGS += -O2 -DNDEBUG
release: clean $(TARGET)

# Help
help:
	@echo "Available targets:"
	@echo "  all      - Build the project (default)"
	@echo "  clean    - Remove object files and executable"
	@echo "  distclean- Remove all build artifacts"
	@echo "  run      - Build and run the program"
	@echo "  test     - Build and run with test input"
	@echo "  debug    - Build with debug symbols"
	@echo "  release  - Build optimized release version"
	@echo "  help     - Show this help message"

.PHONY: all clean distclean run test debug release help
