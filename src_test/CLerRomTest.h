#include "../cppTest/src/cpptest.h"

class CLerRomTest : public Test::Suite
{
public:
    CLerRomTest()
    {
        TEST_ADD(CLerRomTest::lerRomTest);    
        TEST_ADD(CLerRomTest::getConteudoArquivoTest);    
    }
    

    void lerRomTest();
    void getConteudoArquivoTest();
};