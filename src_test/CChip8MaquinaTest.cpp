#include <iostream>
#include "CChip8MaquinaTest.h"

#include "../src/CChip8Maquina.h"
#include "../src/CLerRom.h"


void CChip8MaquinaTest::limparTelaTest()
{
   CChip8Maquina objCChip8Maquina;
   objCChip8Maquina.iniciar();

   objCChip8Maquina.limparTela();

   // a tela está limpa?
   TEST_ASSERT( objCChip8Maquina.arrTela[0] == char( 0 ) );
   TEST_ASSERT( objCChip8Maquina.arrTela[TOTAL_PIXELS-1] == char( 0 ) );

   objCChip8Maquina.finalizar();   
}


void CChip8MaquinaTest::limparMemoriaTest()
{
   CChip8Maquina objCChip8Maquina;
   objCChip8Maquina.iniciar();

   objCChip8Maquina.limparMemoria();

   // a memoria está limpa?
   TEST_ASSERT( objCChip8Maquina.arrMemoria[0] == char( 0 ) );
   TEST_ASSERT( objCChip8Maquina.arrMemoria[TOTAL_MEMORIA-1] == char( 0 ) );

   objCChip8Maquina.finalizar();   
}

void CChip8MaquinaTest::setMemoriaTest()
{
   CChip8Maquina objCChip8Maquina;
   objCChip8Maquina.iniciar();


   CLerRom objCLerRom;
   objCLerRom.iniciar();
   objCLerRom.lerRom( "arquivo_teste_memoria.txt" );
   const char * conteudo = objCLerRom.getConteudoArquivo();

   objCChip8Maquina.setMemoria( conteudo, objCLerRom.getTamanhoArquivo() );

   // a memoria foi setada corretamente de acordo com o arquivo lido?
   TEST_ASSERT( objCChip8Maquina.arrMemoria[0] == char( 0 ) );
   TEST_ASSERT( objCChip8Maquina.arrMemoria[512] == '1' );
   TEST_ASSERT( objCChip8Maquina.arrMemoria[513] == '2' );
   TEST_ASSERT( objCChip8Maquina.arrMemoria[516] == '5' );
   
   objCChip8Maquina.finalizar();   
   objCLerRom.finalizar(); 
}

