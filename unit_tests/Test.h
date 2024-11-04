#pragma once

#include <iostream>
#include <xstring>
#include <vector>
#include <functional>
#include <Windows.h>

class Test
{
public:
    Test() = default;
    Test(const std::string& section);
    void add(const std::string& testName, int line, std::function<bool()> testFunc);
    void run();

    template <typename _T>
    void assertEquals(const _T& expected, const _T& actual)
    {
        if (expected != actual)
        {
            std::cout << "\n- Expected: " << expected << ", but got: " << actual;
            this->failedCount++;
        }
        else
        {
            this->passedCount++;
        }
    }
     
    template <typename _T>
    void assertNotEquals(const _T& expected, const _T& actual)
    {
        if (expected == actual)
        {
            std::cout << "\n- Expected: " << expected << ", but got: " << actual;
            this->failedCount++;
        }
        else
        {
            this->passedCount++;
        }
    }

    template <typename _T>
    void assertNotNull(const _T* ptr)
    {
        if (ptr == nullptr)
        {
            std::cout << "\n- Pointer is null." << std::endl;
            this->failedCount++;
        }
        else
        {
            this->passedCount++;
        }
    }

    template <typename _T>
    void assertNull(const _T* ptr)
    {
        if (ptr != nullptr)
        {
            std::cout << "\n- Pointer is not null." << std::endl;
            this->failedCount++;
        }
        else
        {
            this->passedCount++;
        }
    }

    void assertTrue(bool condition);
    void assertFalse(bool condition);

    bool getCaseResult();

private:
    struct TestCase
    {
        std::string name;
        int line;
        std::function<bool()> func;
    };

    uint32_t count = 0;
    std::vector<TestCase> tests;
    bool allPassed = true;

    uint32_t passedCount = 0;
    uint32_t failedCount = 0;
};

#define TEST_CASE_ADD(section, testName, function) section.add(testName, __LINE__, function)
