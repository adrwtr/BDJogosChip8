#include <iostream>
#include <stdio.h>
#include "../cppTest/src/cpptest.h"

#include "CLerRomTest.h"
#include "CChip8MaquinaTest.h"

int main()
{

   printf("\nIniciando sequencia de testes:\n\n");

   Test::TextOutput output(Test::TextOutput::Verbose);
   
   CLerRomTest objCLerRomTest;    
   objCLerRomTest.run(output);

   CChip8MaquinaTest objCChip8MaquinaTest;    
   objCChip8MaquinaTest.run(output);

  return 0;
}
