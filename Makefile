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

# Test files
TEST_SOURCES = $(TESTDIR)/olympics_tests.cpp $(SRCDIR)/Olympicsa1.cpp $(SRCDIR)/contestant.cpp
TEST_OBJECTS = $(TEST_SOURCES:%.cpp=$(BUILDDIR)/%.o)
TEST_TARGET = $(BUILDDIR)/olympics_tests

# Default target
all: $(TARGET)

# Create build directory if it doesn't exist
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# Compile object files
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile test object files
$(BUILDDIR)/%.o: $(TESTDIR)/%.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link main executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@

# Link test executable
$(TEST_TARGET): $(BUILDDIR)/olympics_tests.o $(BUILDDIR)/Olympicsa1.o $(BUILDDIR)/contestant.o
	$(CXX) $(CXXFLAGS) $^ -o $@

# Clean build artifacts
clean:
	rm -rf $(BUILDDIR)/*.o $(BUILDDIR)/olympics $(BUILDDIR)/olympics_tests

# Clean everything
distclean: clean
	rm -rf $(BUILDDIR)

# Run the program
run: $(TARGET)
	$(TARGET)

# Build and run unit tests
test: $(TEST_TARGET)
	@echo "Running unit tests..."
	$(TEST_TARGET)

# Run integration test with sample data
integration-test: $(TARGET)
	@echo "Running integration test..."
	@printf "add_country 1 10\nadd_country 2 5\nadd_team 1 1 0\nadd_contestant 1 1 0 100\nadd_contestant_to_team 1 1\nget_team_strength 1\nget_medals 1\nprint\n" | $(TARGET)

# Run tests with data files
test-basic: $(TARGET)
	@echo "Running basic data test..."
	$(TARGET) < $(TESTDIR)/test_data_basic.txt

test-comprehensive: $(TARGET)
	@echo "Running comprehensive data test..."
	$(TARGET) < $(TESTDIR)/test_data_comprehensive.txt

test-edge-cases: $(TARGET)
	@echo "Running edge cases test..."
	$(TARGET) < $(TESTDIR)/test_data_edge_cases.txt

# Run all tests (unit tests + data tests)
test-all: test test-basic test-comprehensive test-edge-cases
	@echo "All tests completed!"

# Build tests without running
build-tests: $(TEST_TARGET)

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
