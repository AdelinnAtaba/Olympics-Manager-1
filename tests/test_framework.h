#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <iostream>
#include <string>
#include <vector>
#include "../include/wet1util.h"

class TestFramework {
private:
    static int total_tests;
    static int passed_tests;
    static int failed_tests;
    static std::vector<std::string> failed_test_names;

public:
    static void assert_true(bool condition, const std::string& test_name) {
        total_tests++;
        if (condition) {
            passed_tests++;
            std::cout << "✓ PASS: " << test_name << std::endl;
        } else {
            failed_tests++;
            failed_test_names.push_back(test_name);
            std::cout << "✗ FAIL: " << test_name << std::endl;
        }
    }

    static void assert_equals(int expected, int actual, const std::string& test_name) {
        total_tests++;
        if (expected == actual) {
            passed_tests++;
            std::cout << "✓ PASS: " << test_name << std::endl;
        } else {
            failed_tests++;
            failed_test_names.push_back(test_name);
            std::cout << "✗ FAIL: " << test_name << " (expected: " << expected << ", got: " << actual << ")" << std::endl;
        }
    }

    static void assert_success(StatusType status, const std::string& test_name) {
        assert_true(status == StatusType::SUCCESS, test_name);
    }

    static void assert_failure(StatusType status, const std::string& test_name) {
        assert_true(status == StatusType::FAILURE || status == StatusType::INVALID_INPUT, 
                   status == StatusType::INVALID_INPUT ? 
                   test_name + " (got INVALID_INPUT)" : test_name);
    }

    static void print_summary() {
        std::cout << "\n" << std::string(50, '=') << std::endl;
        std::cout << "TEST SUMMARY" << std::endl;
        std::cout << std::string(50, '=') << std::endl;
        std::cout << "Total Tests: " << total_tests << std::endl;
        std::cout << "Passed: " << passed_tests << std::endl;
        std::cout << "Failed: " << failed_tests << std::endl;
        
        if (failed_tests > 0) {
            std::cout << "\nFailed Tests:" << std::endl;
            for (const auto& test_name : failed_test_names) {
                std::cout << "  - " << test_name << std::endl;
            }
        }
        
        std::cout << "\nSuccess Rate: " << (total_tests > 0 ? (passed_tests * 100 / total_tests) : 0) << "%" << std::endl;
        std::cout << std::string(50, '=') << std::endl;
    }

    static bool all_tests_passed() {
        return failed_tests == 0;
    }
};

// Static member definitions
int TestFramework::total_tests = 0;
int TestFramework::passed_tests = 0;
int TestFramework::failed_tests = 0;
std::vector<std::string> TestFramework::failed_test_names;

#endif // TEST_FRAMEWORK_H
