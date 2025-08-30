# Olympics Manager

A C++ data structures project implementing an Olympics competition management system using AVL trees.

## Project Structure

```
Olympics-Manager-1/
├── src/                    # Source files
│   ├── maina1.cpp         # Main program entry point
│   ├── Olympicsa1.cpp     # Olympics class implementation
│   └── contestant.cpp     # Contestant, Team, Country implementations
├── include/               # Header files
│   ├── Olympicsa1.h       # Olympics class declaration
│   ├── contestant.h       # Data structures declarations
│   ├── AvlTree.h         # AVL tree implementation
│   ├── vertice.h         # Tree node implementation
│   └── wet1util.h        # Utility types and enums
├── build/                 # Build artifacts and executables
├── docs/                  # Documentation
│   └── dry.pdf           # Project specification
├── tests/                 # Test files and framework
│   ├── test_framework.h   # Custom testing framework
│   ├── olympics_tests.cpp # Comprehensive unit tests
│   ├── test_data_basic.txt # Basic integration test data
│   ├── test_data_comprehensive.txt # Complex test scenarios
│   └── test_data_edge_cases.txt # Boundary condition tests
├── Makefile              # Build configuration
└── README.md             # This file
```

## Building the Project

### Using Make (Recommended)

```bash
# Build the project
make

# Build and run
make run

# Run unit tests
make test

# Build debug version
make debug

# Build optimized release version
make release

# Clean build artifacts
make clean

# Show all available targets
make help
```

### Manual Compilation

```bash
g++ -std=c++11 -Wall -Wextra -I./include src/maina1.cpp src/Olympicsa1.cpp src/contestant.cpp -o build/olympics
```

## Running the Program

```bash
# Interactive mode
./build/olympics

# With input file
./build/olympics < input.txt

# With piped commands
echo "add_country 1 10" | ./build/olympics
```

## Available Commands

- `add_country <id> <medals>` - Add a country with medal count
- `remove_country <id>` - Remove a country
- `add_team <teamId> <countryId> <sport>` - Add a team to a country
- `remove_team <teamId>` - Remove a team
- `add_contestant <id> <countryId> <sport> <strength>` - Add a contestant
- `remove_contestant <id>` - Remove a contestant
- `add_contestant_to_team <teamId> <contestantId>` - Add contestant to team
- `remove_contestant_from_team <teamId> <contestantId>` - Remove contestant from team
- `update_contestant_strength <contestantId> <change>` - Update contestant strength
- `get_strength <contestantId>` - Get contestant strength
- `get_medals <countryId>` - Get country medal count
- `get_team_strength <teamId>` - Get team total strength
- `unite_teams <teamId1> <teamId2>` - Merge two teams
- `play_match <teamId1> <teamId2>` - Simulate a match between teams
- `austerity_measures <teamId>` - Calculate optimal team strength after removing 3 players
- `print` - Display current data structure statistics

## Sports

- 0: BOULDERING
- 1: ACROBATICS
- 2: FOOTBALL
- 3: SWIMMING
- 4: SPINNING

## Features

- **AVL Tree Data Structures**: Efficient storage and retrieval
- **Team Organization**: Players organized in 3 balanced groups
- **Dynamic Strength Updates**: Real-time contestant strength modifications
- **Team Merging**: Combine teams from same country and sport
- **Match Simulation**: Team competitions with medal rewards
- **Optimization Algorithms**: Complex strength calculations
- **Data Structure Visualization**: Print command for debugging and analysis

## Testing

The project includes comprehensive test coverage with multiple testing approaches:

### Unit Tests
```bash
# Run unit tests (30 comprehensive tests)
make test

# Build tests without running
make build-tests
```

### Integration Tests
```bash
# Run basic integration test
make integration-test

# Run tests with data files
make test-basic          # Basic functionality tests
make test-comprehensive  # Complex scenarios
make test-edge-cases     # Boundary conditions and error cases

# Run all tests (unit + integration)
make test-all
```

### Test Coverage
- **Country Operations**: Add, remove, get medals, validation
- **Contestant Operations**: Add, remove, get strength, validation  
- **Team Operations**: Add, remove, add contestants, get strength
- **Complex Operations**: Play matches, austerity measures
- **Print Functionality**: Data structure visualization
- **Edge Cases**: Invalid inputs, boundary conditions, error handling
- **Integration Tests**: Real-world scenarios with comprehensive data

### Test Results
- **30 Unit Tests**: 100% pass rate ✅
- **Custom Test Framework**: Built-in assertion and reporting system
- **Data-Driven Tests**: Multiple test scenarios with different data sets

## Course Information

- Course: 234218 Data Structures 1
- Semester: 2024A (Winter)
- Assignment: Wet Exercise #1
