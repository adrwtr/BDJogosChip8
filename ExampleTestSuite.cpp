#include <iostream>
#include "ExampleTestSuite.h"

void ExampleTestSuite::first_test()
{

    // std::cout << "aqui no primeiro teste" << std::endl;

    // Will succeed since the expression evaluates to true
    //
     TEST_ASSERT(1 + 1 == 1)
     
    
    // Will fail since the expression evaluates to false
    //
     TEST_ASSERT(1 == 1)
     TEST_ASSERT(1 == 2)
}

void ExampleTestSuite::second_test()
{
    // Will succeed since the expression evaluates to true
    //
    // TEST_ASSERT_DELTA(0.5, 0.7, 0.3);
    
    // Will fail since the expression evaluates to false
    //
    // TEST_ASSERT_DELTA(0.5, 0.7, 0.1);
}