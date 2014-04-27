#include <stdio.h>
#include <stdlib.h>

// projeto
#include "constantes.h"
#include "CLerRom.h"

/**
 * inicia as principais variaveis da classe - construtor
 */
void CLerRom::iniciar()
{
   this->pArquivo          = NULL;
   this->tamanho_arquivo   = 0;
   this->bfConteudoArquivo = NULL;
}

/**
 * Finaliza e zera as variaveis da classe - destrutor
 */
void CLerRom::finalizar()
{
   fclose(this->pArquivo);
   free( this->bfConteudoArquivo );

   this->pArquivo          = NULL;
   this->tamanho_arquivo   = 0;
   this->bfConteudoArquivo = NULL;
}

/**
 * Funcao responsavel por ler a rom
 * de um arquivo informado
 * 
 * @var const char * nome_arquivo | arquivo a ser lido
 * @return bool | indica se o arquivo foi lido com sucesso
 */
bool CLerRom::lerRom( const char * nome_arquivo )
{  
   bool leu_arquivo = false;

   printf( "Tentando abrir o arquivo: %s\n", nome_arquivo );
   
   leu_arquivo = this->abrirArquivo( nome_arquivo );

   if ( leu_arquivo == false )
   {
      return false;
   }
      
   fseek( this->pArquivo, 0 , SEEK_END );
   this->setTamanhoArquivo();
   rewind( this->pArquivo );

   printf("Filesize: %d\n", (int)this->getTamanhoArquivo() );
   
   // aloca a memoria para conter todo o conteudo do arquivo
   this->bfConteudoArquivo = (char*)malloc(sizeof(char) * this->getTamanhoArquivo() );

   if ( this->bfConteudoArquivo == NULL) 
   {
      printf("Memory error - N?o foi possivel alocar o arquivo na memoria: ", stderr); 
      return false;
   }

   // copia todo o conteudo do arquivo para o buffer bfConteudoArquivo
   size_t verifica_leitura = fread ( 
      this->bfConteudoArquivo, 
      1, 
      this->getTamanhoArquivo(), 
      this->pArquivo
   );


   if ( verifica_leitura != this->getTamanhoArquivo() ) 
   {
      printf("Reading error - N?o foi possivel ler o arquivo: ",stderr ); 
      return false;
   }

   
   if ( this->getTamanhoArquivo() > TOTAL_MEMORIA )
   {
      printf( "O arquivo de rom a ser lido ? maior que a memoria do CHIP8" ); 
      return false;
   }

   // limpa os ponteiros
   fclose(this->pArquivo);
   // free( this->bfConteudoArquivo );

   return true;
};

/**
 * Retorna o conteudo do arquivo
 * 
 * @return char * | conteudo do arquivo lido
 */
const char * CLerRom::getConteudoArquivo()
{
   return this->bfConteudoArquivo;
}


/**
 * Tenta realizar a leitura do arquivo
 * 
 * @var const char * nome_arquivo | arquivo a ser lido
 * @return bool | indica se o arquivo foi lido com sucesso
 */
bool CLerRom::abrirArquivo( const char * nome_arquivo )
{
   // Open file
   this->pArquivo = fopen( nome_arquivo, "rb" );

   if ( this->pArquivo == NULL )
   {
      printf( "Ocorreu um erro na tentativa de leitura do arquivo:", stderr ); 
      printf("\n\n");
      return false;
   }

   return true;
}

/**
 * Recupera o tamanho do arquivo
 */
void CLerRom::setTamanhoArquivo()
{
   this->tamanho_arquivo = ftell( this->pArquivo );
}

/**
 * Retorna o tamanho do arquivo
 * 
 * @return long int | indica o tamanho total do arquivo
 */
long CLerRom::getTamanhoArquivo()
{
   return this->tamanho_arquivo;
}