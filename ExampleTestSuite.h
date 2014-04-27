#include "cppTest/src/cpptest.h"

class ExampleTestSuite : public Test::Suite
{
public:
    ExampleTestSuite()
    {
        TEST_ADD(ExampleTestSuite::first_test)        
    }
    

    void first_test();
    void second_test();
};