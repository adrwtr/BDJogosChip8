#ifndef CLERROM_H
#define CLERROM_H

#include <stdio.h>
#include <stdlib.h>


/**
 * Esta classe eh? responsavel pela leitura da ROM 
 * que contem o jogo em chip8. normalmente o arquivo eh um ch8 ou c8
 */
class CLerRom
{
   public:
      void iniciar();
      void finalizar();      
      bool lerRom( const char * nome_arquivo );
      const char * getConteudoArquivo();
      void setTamanhoArquivo();
      long getTamanhoArquivo();

   private:
      FILE * pArquivo;
      long tamanho_arquivo;
      char * bfConteudoArquivo;

      bool abrirArquivo( const char * nome_arquivo );
      
};

#endif // #ifndef CLERROM_H