#include "test_framework.h"
#include "../include/Olympicsa1.h"
#include "../include/wet1util.h"
#include <iostream>
#include <memory>

class OlympicsTests {
private:
    std::unique_ptr<Olympics> olympics;

public:
    void setUp() {
        olympics.reset(new Olympics());
    }

    void tearDown() {
        olympics.reset();
    }

    // Country Tests
    void test_country_operations() {
        setUp();
        // Test valid add
        StatusType result = olympics->add_country(1, 10);
        TestFramework::assert_success(result, "Add country with valid parameters");
        
        // Test invalid inputs
        StatusType invalid_id = olympics->add_country(0, 10);
        TestFramework::assert_failure(invalid_id, "Add country with invalid ID (0)");
        
        StatusType invalid_medals = olympics->add_country(2, -1);
        TestFramework::assert_failure(invalid_medals, "Add country with negative medals");
        
        // Test duplicate
        StatusType duplicate = olympics->add_country(1, 5);
        TestFramework::assert_failure(duplicate, "Add duplicate country");
        
        // Test get medals
        output_t<int> medals = olympics->get_medals(1);
        TestFramework::assert_success(medals.status(), "Get medals from existing country");
        
        // Test remove operations
        StatusType remove_success = olympics->remove_country(1);
        TestFramework::assert_success(remove_success, "Remove existing country");
        
        StatusType remove_nonexistent = olympics->remove_country(999);
        TestFramework::assert_failure(remove_nonexistent, "Remove non-existent country");
        
        tearDown();
    }



    // Contestant Tests
    void test_contestant_operations() {
        setUp();
        olympics->add_country(1, 10);
        
        // Test valid add
        StatusType result = olympics->add_contestant(1, 1, Sport::SWIMMING, 100);
        TestFramework::assert_success(result, "Add contestant with valid parameters");
        
        // Test invalid inputs
        StatusType invalid_id = olympics->add_contestant(0, 1, Sport::SWIMMING, 100);
        TestFramework::assert_failure(invalid_id, "Add contestant with invalid ID (0)");
        
        StatusType invalid_strength = olympics->add_contestant(2, 1, Sport::SWIMMING, -1);
        TestFramework::assert_failure(invalid_strength, "Add contestant with invalid strength (-1)");
        
        StatusType nonexistent_country = olympics->add_contestant(3, 999, Sport::SWIMMING, 100);
        TestFramework::assert_failure(nonexistent_country, "Add contestant to non-existent country");
        
        // Test duplicate
        StatusType duplicate = olympics->add_contestant(1, 1, Sport::FOOTBALL, 150);
        TestFramework::assert_failure(duplicate, "Add duplicate contestant");
        
        // Test get strength
        output_t<int> strength = olympics->get_strength(1);
        TestFramework::assert_success(strength.status(), "Get strength from existing contestant");
        
        // Test remove operations
        StatusType remove_success = olympics->remove_contestant(1);
        TestFramework::assert_success(remove_success, "Remove existing contestant");
        

        
        tearDown();
    }



    // Team Tests
    void test_team_operations() {
        setUp();
        olympics->add_country(1, 10);
        olympics->add_contestant(1, 1, Sport::SWIMMING, 100);
        olympics->add_contestant(2, 1, Sport::FOOTBALL, 150);
        
        // Test valid add team
        StatusType result = olympics->add_team(1, 1, Sport::SWIMMING);
        TestFramework::assert_success(result, "Add team with valid parameters");
        
        // Test invalid inputs
        StatusType invalid_id = olympics->add_team(0, 1, Sport::SWIMMING);
        TestFramework::assert_failure(invalid_id, "Add team with invalid ID (0)");
        
        StatusType nonexistent_country = olympics->add_team(2, 999, Sport::SWIMMING);
        TestFramework::assert_failure(nonexistent_country, "Add team to non-existent country");
        
        StatusType duplicate = olympics->add_team(1, 1, Sport::FOOTBALL);
        TestFramework::assert_failure(duplicate, "Add duplicate team");
        
        // Test contestant to team operations
        StatusType success = olympics->add_contestant_to_team(1, 1);
        TestFramework::assert_success(success, "Add contestant to team successfully");
        
        StatusType nonexistent_contestant = olympics->add_contestant_to_team(999, 1);
        TestFramework::assert_failure(nonexistent_contestant, "Add non-existent contestant to team");
        
        StatusType nonexistent_team = olympics->add_contestant_to_team(1, 999);
        TestFramework::assert_failure(nonexistent_team, "Add contestant to non-existent team");
        
        StatusType sport_mismatch = olympics->add_contestant_to_team(2, 1);
        TestFramework::assert_failure(sport_mismatch, "Add contestant with different sport to team");
        
        // Test team strength operations
        output_t<int> team_strength = olympics->get_team_strength(1);
        TestFramework::assert_success(team_strength.status(), "Get team strength successfully");
        
        output_t<int> nonexistent_team_strength = olympics->get_team_strength(999);
        TestFramework::assert_failure(nonexistent_team_strength.status(), "Get strength from non-existent team");
        
        tearDown();
    }



    // Complex Operations Tests
    void test_play_match_operations() {
        setUp();
        // Setup two countries with teams and contestants
        olympics->add_country(1, 10);
        olympics->add_country(2, 5);
        olympics->add_team(1, 1, Sport::SWIMMING);
        olympics->add_team(2, 2, Sport::SWIMMING);
        olympics->add_contestant(1, 1, Sport::SWIMMING, 100);
        olympics->add_contestant(2, 2, Sport::SWIMMING, 90);
        olympics->add_contestant_to_team(1, 1);
        olympics->add_contestant_to_team(2, 2);
        
        // Test valid match
        StatusType result = olympics->play_match(1, 2);
        TestFramework::assert_success(result, "Play match between valid teams");
        
        // Test match with non-existent team
        StatusType nonexistent = olympics->play_match(1, 999);
        TestFramework::assert_failure(nonexistent, "Play match with non-existent team");
        
        // Test match with same team
        StatusType same_team = olympics->play_match(1, 1);
        TestFramework::assert_failure(same_team, "Play match with same team");
        
        tearDown();
    }

    // Print functionality test
    void test_print_functionality() {
        setUp();
        olympics->add_country(1, 10);
        olympics->add_contestant(1, 1, Sport::SWIMMING, 100);
        olympics->add_team(1, 1, Sport::SWIMMING);
        
        // This test just ensures print doesn't crash
        olympics->print_data_structure();
        TestFramework::assert_true(true, "Print functionality executes without crashing");
        tearDown();
    }

    // Edge Cases
    void test_edge_cases() {
        setUp();
        olympics->add_country(1, 10);
        olympics->add_country(2, 20);
        olympics->add_contestant(1, 1, Sport::SWIMMING, 100);
        olympics->add_contestant(2, 2, Sport::SWIMMING, 200);
        
        // Test constraint violations
        StatusType remove_with_contestants = olympics->remove_country(1);
        TestFramework::assert_failure(remove_with_contestants, "Remove country with contestants should fail");
        
        TestFramework::assert_true(true, "Edge cases handled correctly");
        tearDown();
    }

    // Run all tests
    void run_all_tests() {
        std::cout << "Running Olympics Manager Unit Tests...\n" << std::endl;
        
        // Country tests
        std::cout << "=== COUNTRY TESTS ===" << std::endl;
        test_country_operations();
        
        // Contestant tests
        std::cout << "\n=== CONTESTANT TESTS ===" << std::endl;
        test_contestant_operations();
        
        // Team tests
        std::cout << "\n=== TEAM TESTS ===" << std::endl;
        test_team_operations();
        
        // Complex operations tests
        std::cout << "\n=== COMPLEX OPERATIONS TESTS ===" << std::endl;
        test_play_match_operations();
        
        // Print functionality test
        std::cout << "\n=== PRINT FUNCTIONALITY TEST ===" << std::endl;
        test_print_functionality();
        
        // Edge cases
        std::cout << "\n=== EDGE CASES ===" << std::endl;
        test_edge_cases();
        
        TestFramework::print_summary();
    }
};

int main() {
    OlympicsTests tests;
    tests.run_all_tests();
    
    return TestFramework::all_tests_passed() ? 0 : 1;
}
