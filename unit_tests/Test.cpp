#include "Test.h"

Test::Test(const std::string& section)
{
    std::cout << section << std::endl;
}

void Test::add(const std::string& testName, int line, std::function<bool()> testFunc)
{
    this->tests.push_back({ testName, line, testFunc });
}

void Test::run()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    if (hConsole == INVALID_HANDLE_VALUE)
    {
        std::cout << "Error: Invalid handle console handle, exit . . ." << std::endl;
        return;
    }

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    WORD oldtextColor;
    WORD oldbackgroundColor;

    if (GetConsoleScreenBufferInfo(hConsole, &csbi))
    {
        WORD attributes = csbi.wAttributes;
        //0000_0000 0000_0000
        oldtextColor = attributes & 0x0F; // left 4 bits
        oldbackgroundColor = (attributes & 0xF0) >> 4; // right 4 bits
    }
    else
    {
        std::cout << "Error: cant get information about of console buffer, exit . . ." << std::endl;
        return;
    }

    std::cout << "Running Tests:\n\n";

    std::vector<int> error_lines;

    for (auto test_case = this->tests.begin(); test_case != this->tests.end(); ++test_case)
    {
        SetConsoleTextAttribute(hConsole, oldtextColor | oldbackgroundColor);
        std::cout << "#" << ++this->count << " " << test_case->name;
        bool result = test_case->func();

        if (result)
        {
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | oldbackgroundColor);
            std::cout << " ... PASSED [" << this->passedCount << "]" << std::endl;
        }
        else
        {
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | oldbackgroundColor);
            std::cout << "\nFAILED [" << this->failedCount << "] ... Line: [" << test_case->line << "]" <<  std::endl;
            this->allPassed = false;
            error_lines.push_back(test_case->line);
        }

        this->passedCount = 0;
        this->failedCount = 0;

        std::cout << std::endl;
    }

    SetConsoleTextAttribute(hConsole, oldtextColor | oldbackgroundColor);
    if (this->allPassed)
    {
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | oldbackgroundColor);
        std::cout << "All tests were successfully passed!" << std::endl;
    }
    else
    {
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | oldbackgroundColor);
        std::cout << "Some tests have failed, check the lines:" << std::endl;
        
        for (auto line = error_lines.begin(); line != error_lines.end(); ++line)
        {
            std::cout << "[" << *line << "]";

            if (line + 1 != error_lines.end())
            {
                std::cout << ", ";
            }
        }
    }

    std::cout << std::endl;

    SetConsoleTextAttribute(hConsole, oldtextColor | oldbackgroundColor);
}

void Test::assertTrue(bool condition)
{
    if (!condition)
    {
        std::cout << "\n- Expected True, but got False.";
        this->failedCount++;
    }
    else
    {
        this->passedCount++;
    }
}

void Test::assertFalse(bool condition)
{
    if (condition)
    {
        std::cout << "\n- Expected False, but got True.";
        this->failedCount++;
    }
    else
    {
        this->passedCount++;
    }
}

bool Test::getCaseResult()
{
    return !this->failedCount;
}
