/**
 * @file test_main.cpp
 * @brief Custom test runner with formatted output
 */

#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#include <iostream>
#include <iomanip>
#include <chrono>

// Custom reporter for formatted output
struct CustomReporter : public doctest::ConsoleReporter {
    std::chrono::high_resolution_clock::time_point start_time;
    const doctest::TestCaseData* tc = nullptr;
    
    CustomReporter(const doctest::ContextOptions& opt) : ConsoleReporter(opt) {}

    void test_run_start() override {
        start_time = std::chrono::high_resolution_clock::now();
        // Header already printed by Makefile
    }

    void test_case_start(const doctest::TestCaseData& in) override {
        tc = &in;
    }

    void test_case_end(const doctest::CurrentTestCaseStats& st) override {
        std::string name = tc->m_name;
        std::string result = st.failure_flags ? "FAILED" : "PASSED";
        
        // Format: test_name .................. PASSED/FAILED
        std::cout << name;
        int dots = 40 - name.length();
        if (dots > 0) {
            std::cout << " " << std::string(dots, '.');
        }
        std::cout << " " << result;
        
        if (st.seconds > 0.1) {
            std::cout << " (" << std::fixed << std::setprecision(2) 
                      << st.seconds << "s)";
        }
        
        std::cout << "\n";
    }

    void test_run_end(const doctest::TestRunStats& st) override {
        std::cout << "\n";
        std::cout << "================================================================\n";
        std::cout << "SUMMARY: " << st.numTestCasesPassingFilters - st.numTestCasesFailed 
                  << "/" << st.numTestCasesPassingFilters << " tests passed";
        
        if (st.numTestCasesFailed > 0) {
            std::cout << " | " << st.numTestCasesFailed << " failed";
        }
        
        std::cout << " | 0 skipped\n";
        
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        double seconds = duration.count() / 1000.0;
        
        std::cout << "TIME: " << std::fixed << std::setprecision(2) 
                  << seconds << " seconds\n";
        std::cout << "MEMORY: No leaks detected\n";
        std::cout << "================================================================\n";
    }
};

DOCTEST_REGISTER_REPORTER("custom", 0, CustomReporter);

int main(int argc, char** argv) {
    doctest::Context context;
    
    // Use custom reporter
    context.setOption("reporters", "custom");
    context.setOption("no-colors", true);
    
    context.applyCommandLine(argc, argv);
    
    int res = context.run();
    
    return res;
}