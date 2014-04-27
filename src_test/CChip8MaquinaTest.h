#include "../cppTest/src/cpptest.h"

class CChip8MaquinaTest : public Test::Suite
{
public:
    CChip8MaquinaTest()
    {
        TEST_ADD( CChip8MaquinaTest::limparTelaTest );    
        TEST_ADD( CChip8MaquinaTest::limparMemoriaTest );    
        TEST_ADD( CChip8MaquinaTest::setMemoriaTest );    
    }
    

    void limparTelaTest();
    void limparMemoriaTest();
    void setMemoriaTest();
};