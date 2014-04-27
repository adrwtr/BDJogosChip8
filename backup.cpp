// #include <stdio.h>

#include <iostream>

// #include "classe_teste.h"

// #include "cppTest/src/cpptest.h"

// #include "ExampleTestSuite.h"

#include "teste_lib/calc_mean.h"

int main()
{

    // Rectangle * a = new Rectangle();


    // std::cout << "teste adriano" << std::endl;

    // std::cout << a->set_values( 2, 2 ) << std::endl;
 double v1, v2, m;
  v1 = 5.2;
  v2 = 7.9;
   m  = mean(v1, v2);

  // printf("The mean of %3.2f and %3.2f is %3.2f\n", v1, v2, m);

   std::cout << m << std::endl;

   // Test::TextOutput output(Test::TextOutput::Verbose);
   // ExampleTestSuite ets;
   // ets.first_test();

   // return ets.run(output) ? 1 : 0;

  return 0;
}

