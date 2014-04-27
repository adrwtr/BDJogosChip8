#include <iostream>
#include <string.h>
#include <stdio.h>

// do projeto
#include "CLerRomTest.h"
#include "../src/CLerRom.h"


void CLerRomTest::lerRomTest()
{
    CLerRom objCLerRom;
    objCLerRom.iniciar();

    bool ler_arquivo = false;
    ler_arquivo = objCLerRom.lerRom( "arquivo.txt" );
    objCLerRom.finalizar();

    // conseguiu ler o arquivo?
    TEST_ASSERT( ler_arquivo == true );
}

void CLerRomTest::getConteudoArquivoTest()
{
   CLerRom objCLerRom;
   objCLerRom.iniciar();

   objCLerRom.lerRom( "arquivo.txt" );
   const char * conteudo = objCLerRom.getConteudoArquivo();

   // o conteudo do arquivo foi lido corretamente?
   TEST_ASSERT( strcmp(conteudo, "arquivo") == 0 );


   objCLerRom.finalizar();
}