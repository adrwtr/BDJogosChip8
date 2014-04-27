#include <iostream>
#include "cppTest/src/cpptest.h"
#include "ExampleTestSuite.h"


int main()
{
    Test::TextOutput output(Test::TextOutput::Verbose);
    ExampleTestSuite ets;
    
    ets.run(output);

  return 0;
}

