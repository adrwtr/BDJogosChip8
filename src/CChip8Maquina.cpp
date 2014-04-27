#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <windows.h>



// projeto
#include "constantes.h"
#include "CChip8Maquina.h"
#include "CChip8Uteis.h"

/**
 * inicia as principais variaveis da classe - construtor
 */
void CChip8Maquina::iniciar()
{
   this->limparTela();
   
   this->pcContador   = 0;
   this->opcode_atual = 0;
   this->Iregister    = 0;                
   this->spPilha      = 0;   

   this->delay_timer  = char(0);              
   this->sound_timer  = char(0);     
   this->sn_imprimir  = true;         

   int i = 0;
   for ( i = 0 ; i<16; i++ )
   {
      this->Vregistradores[i] = char(0);
      this->arrPilha[i]       = 0;
   }
}

/**
 * Finaliza e zera as variaveis da classe - destrutor
 */
void CChip8Maquina::finalizar()
{
   this->iniciar();
   this->limparMemoria();
}

/**
 * Funcao responsavel por converter o conteudo 
 * de uma rom para a memoria do chip8
 * 
 * @var const char * nome_arquivo | rom a ser lida
 * @var long tamanho_arquivo | o tamanho total do arquivo
 * @return bool | indica se a rom foi convertida corretamente
 */
bool CChip8Maquina::setMemoria( const char * bfConteudoArquivo, long tamanho_arquivo )
{
   // copia o conteudo que estah no buffer para a memoria 
   // da maquina que inicia em 512
   for( int i = 0; i < tamanho_arquivo; ++i )
   {
      this->arrMemoria[INICIO_MEMORIA + i] = bfConteudoArquivo[i];
   }

   return true;   
}

/**
 * limpa a tela
 */
void CChip8Maquina::limparTela()
{
   int i=0;

   for ( i=0; i<TOTAL_PIXELS; i++ )
   {
      this->arrTela[i] = char( 0 );
   }   
}

/**
 * limpa a memoria
 */
void CChip8Maquina::limparMemoria()
{
   int i=0;

   for ( i=0; i<TOTAL_MEMORIA; i++ )
   {
      this->arrMemoria[i] = char( 0 );
   }   
}

/**
 * Inicia a maquina chip8 de acordo com os seus
 * parametros de emulacao
 */
void CChip8Maquina::iniciarMaquina()
{
   // aonde inicia o contedor eh em 0x200 ou 512 em decimal
   this->pcContador   = 0x200;

   // inicia sequencia de fontes
   this->iniciarFontSet();

   // inicia randomizador
   srand (time(NULL));
}

/**
 * reinicia a maquina em caso de erro
 */
void CChip8Maquina::reiniciarMaquina()
{
   this->iniciar();
   this->iniciarMaquina();
}

/**
 * Inicia o conjunto de fontes na memoria da maquina
 * conforme constantes de emulacao
 */
void CChip8Maquina::iniciarFontSet()
{
   for( int i = 0; i < TOTAL_FONTESET; ++i )
   {
      this->arrMemoria[i] = arrChip8_fontset[i];    
   }
}

/**
 * Indica para o emulador que pode ser imprimido a tela
 */
void CChip8Maquina::setImprimirTela()
{
   this->sn_imprimir = true;
}

/**
 * Indica para o emulador que nao pode ser imprimido a tela
 */
void CChip8Maquina::setNaoImprimirTela()
{
   this->sn_imprimir = false;
}

/**
 * Recupera a informacao indicando se pode ou nao imprimir a tela
 *
 * @return bool
 */
bool CChip8Maquina::getSnImprimirTela()
{
   return this->sn_imprimir;
}


/**
 * Funcao bola de fogo
 */
void CChip8Maquina::emular()
{
   printf("CONTADOR =  %i\n", this->pcContador );


   // le o opcode 
   // faz shift ali..
   // se tiver 0x00e0 na memoria 1
   // e 0x2222 na memoria 1 + 1  ( conforme jogo.txt 2 primeiros)
   // ele vai fazer 0x00e0 << 8 = 0x00e0 + 00000000
   // 0x00e000000000 
   // e depois vai fazer um MERGE com 0x2222
   // no final vai ficar 00000000111000000010001000100010                             
   // convertendo isso para hexa denovo fica: 0x0e02222
   // e soma ao final com a proxima posicao na memoria 12288 + valor proxima memoria
   // vc pode fazer o teste usando CChip8Uteis com a funcao GetBinaryStringFromHexString( "0e02222" );
   printf ("Opcode 1: 0x%X\n", this->arrMemoria[ this->pcContador ] );  
   printf ("Opcode 2: 0x%X\n", this->arrMemoria[ this->pcContador + 1 ] );  
   this->opcode_atual = this->arrMemoria[ this->pcContador ] << 8 | this->arrMemoria[ this->pcContador + 1 ];
   printf ("Opcode merge: 0x%X\n", this->opcode_atual );  
    
   // a memoria armazena apenas 1 byte
   // como um opcode eh composto de 2 bytes
   // entao temos que concatenar a memoria de 2 em 2

   // o & 0xF000 discarta qualquer bit extra, trabalhando assim com 32 bits
   switch( this->opcode_atual & 0xF000 )
   {     

      // 
      case 0x0000:

         switch( this->opcode_atual & 0x000F)
         {
            // 
            // 0x00E0: Clears the screen
            // limpa a tela
            case 0x0000: 
               printf("0x00E0: Limpa a tela\n");
               this->limparTela();
               this->setImprimirTela();
               this->pcContador += 2;
            break;

            // 
            // 0x00EE: Returns from subroutine
            // retorna de uma subrotina
            case 0x000E:               
               printf("0x00EE: Retorna da subrotina\n");  
               --this->spPilha; 
               this->pcContador = this->arrPilha[this->spPilha];
               this->pcContador += 2;
            break;

            default:
               printf ("OPCODE nao encontrado [0x0000]: 0x%X\n", this->opcode_atual );               
         }
      break;

      // 0x1NNN: Jumps to address NNN
      // pula para o endereco NNN
      case 0x1000: 
         printf ("0x1NNN: pula para o endereco NNN\n\n");               
         printf ("Endereco %x\n", this->opcode_atual & 0x0FFF );               
         this->pcContador = this->opcode_atual & 0x0FFF;
      break;


      // 
      // 0x2NNN: Calls subroutine at NNN.
      // chama uma subrotina em NNN
      case 0x2000: 
         printf("0x2NNN: chama uma subrotina em NNN.\n");
         // Guarda a posicao atual na pilha
         this->arrPilha[ this->spPilha] = this->pcContador;         
         
         // Incrementa a pilha para nao perder a proxima variavel a ser armazenada
         ++this->spPilha;                                

         // Indica para o contador ir para o endereco NNN          
         this->pcContador = this->opcode_atual & 0x0FFF;   
         printf ("Correto: 0x%X\n", this->opcode_atual );  
         printf ("Merge: 0x%X\n", this->opcode_atual & 0x0FFF );  
      break;

      // 
      // 0x3XNN: Skips the next instruction if VX equals NN
      // pula a proxima instrucao se VX for igual a NN
      // exemplo>
      // hexa 3030
      // binario 11000000110000
      // V0 tem 30 eh se sim soma +4 e pula a leitura de uma instrucao
      case 0x3000: 
         printf(" 0x3000 pula a proxima instrucao se VX for igual a NN\n");
         if ( this->Vregistradores[( this->opcode_atual & 0x0F00) >> 8] == (this->opcode_atual & 0x00FF))
         {
            this->pcContador += 4;            
         }
         else
         {
            this->pcContador += 2;            
         }
      break;

      // 0x4XNN: Skips the next instruction if VX doesn't equal NN
      // 0x4XNN: Pula a proxima instrucao se VX for diferente de NN
      case 0x4000: 
         printf ("0x4XNN: Pula a proxima instrucao se VX for diferente de NN\n");               
         if(Vregistradores[(opcode_atual & 0x0F00) >> 8] != (opcode_atual & 0x00FF))
            pcContador += 4;
         else
            pcContador += 2;
      break;

      // 
      // 0x5XY0: Skips the next instruction if VX equals VY.
      // pula a proxima instrucao se vx for igual a vy
      case 0x5000: 
         printf("0x5000 pula a proxima instrucao se vx for igual a vy\n");
         if( this->Vregistradores[(this->opcode_atual & 0x0F00) >> 8] == this->Vregistradores[(this->opcode_atual & 0x00F0) >> 4])
            this->pcContador += 4;   
         else
            this->pcContador += 2;   
      break;

      // 0x6XNN: Sets VX to NN.
      // seta o valor de VX igual a NN
      case 0x6000: 
         printf("0x6000 seta o valor de VX igual a NN\n");
         this->Vregistradores[( this->opcode_atual & 0x0F00) >> 8] = this->opcode_atual & 0x00FF;
         this->pcContador += 2;
         printf ("V posicao : 0x%X\n", ( this->opcode_atual & 0x0F00) >> 8 );  
      break;

      // 0x7XNN: Adds NN to VX.
      // adiciona o valor de NN a Vx
      case 0x7000: 
         printf ("0x7XNN: adiciona o valor de NN a Vx\n.");
         Vregistradores[(opcode_atual & 0x0F00) >> 8] += opcode_atual & 0x00FF;
         pcContador += 2;
      break;



      // 
      case 0x8000:
         switch(this->opcode_atual & 0x000F)
         {
            // 
            // 0x8XY0: Sets VX to the value of VY
            // seta o Vx para o valor q estah em Vy 
            case 0x0000: 
               printf("0x8XY0 seta o Vx para o valor q estah em Vy \n");
               this->Vregistradores[(this->opcode_atual & 0x0F00) >> 8] = this->Vregistradores[(this->opcode_atual & 0x00F0) >> 4];
               this->pcContador += 2;    
               printf("Aqui A");
            break;

            // 0x8XY1: Sets VX to "VX OR VY"
            // seta VX igual a VX or (binario) VY
            case 0x0001: 
               printf("0x8XY1: Sets VX to VX OR VY\n" );
               this->Vregistradores[(this->opcode_atual & 0x0F00) >> 8] |= this->Vregistradores[(this->opcode_atual & 0x00F0) >> 4];
               this->pcContador += 2;
            break;

            //  0x8XY2: Sets VX to "VX AND VY"
            // and binario
            case 0x0002: 
               printf("0x8XY2: Sets VX to VX AND VY\n");
               this->Vregistradores[(this->opcode_atual & 0x0F00) >> 8] &= this->Vregistradores[(this->opcode_atual & 0x00F0) >> 4];
               this->pcContador += 2;
            break;

            // 0x8XY3: Sets VX to "VX XOR VY"
            // xor binario
            case 0x0003: 
               printf("0x8XY3: Sets VX to VX XOR VY\n");
               this->Vregistradores[(this->opcode_atual & 0x0F00) >> 8] ^= this->Vregistradores[(this->opcode_atual & 0x00F0) >> 4];
               this->pcContador += 2;
            break;

            // 0x8XY4: Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't             
            // Soma Vx com Vy. se VY for maior que VX entao adiciona 1 a "VF"
            case 0x0004: 
               printf("0x8XY4: Soma Vx com Vy. se VY for maior que VX entao adiciona 1 a VF\n");
               if(this->Vregistradores[(this->opcode_atual & 0x00F0) >> 4] > (0xFF - this->Vregistradores[(this->opcode_atual & 0x0F00) >> 8])) 
                  this->Vregistradores[0xF] = 1; //carry
               else 
                  this->Vregistradores[0xF] = 0;    

               this->Vregistradores[(this->opcode_atual & 0x0F00) >> 8] += this->Vregistradores[(this->opcode_atual & 0x00F0) >> 4];
               this->pcContador += 2;             
            break;

            // 0x8XY5: VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't
            // subtrai Vx com Vy. se VY for maior que VX entao adiciona 1 a "VF"
            case 0x0005: 
            printf("0x8XY5: subtrai Vx com Vy. se VY for maior que VX entao adiciona 1 a VF\n");
               if(this->Vregistradores[(this->opcode_atual & 0x00F0) >> 4] > this->Vregistradores[(this->opcode_atual & 0x0F00) >> 8]) 
                  this->Vregistradores[0xF] = 0; // there is a borrow
               else 
                  this->Vregistradores[0xF] = 1;             
               this->Vregistradores[(this->opcode_atual & 0x0F00) >> 8] -= this->Vregistradores[(this->opcode_atual & 0x00F0) >> 4];
               this->pcContador += 2;
            break;

            // 0x8XY6: Shifts VX right by one. VF is set to the value of the least significant bit of VX before the shift
            // shifts >> vx por 1. VF eh setado com o bit mais significante de vx antes do shift
            case 0x0006: 
               printf("0x8XY6: // shifts >> vx por 1. VF eh setado com o bit mais significante de vx antes do shift\n");

               this->Vregistradores[0xF] = this->Vregistradores[(this->opcode_atual & 0x0F00) >> 8] & 0x1;
               this->Vregistradores[(this->opcode_atual & 0x0F00) >> 8] >>= 1;
               this->pcContador += 2;
            break;

            // 0x8XY7: Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't
            // seta o valor de VX para VY-VX. se VY for maior que VX entao adiciona 1 a VF
            case 0x0007: 
               printf("0x8XY7: seta o valor de VX para VY-VX. se VY for maior que VX entao adiciona 1 a VF\n");
               if(this->Vregistradores[(this->opcode_atual & 0x0F00) >> 8] > this->Vregistradores[(this->opcode_atual & 0x00F0) >> 4])   // VY-VX
                  this->Vregistradores[0xF] = 0; // there is a borrow
               else
                  this->Vregistradores[0xF] = 1;
               this->Vregistradores[(this->opcode_atual & 0x0F00) >> 8] = this->Vregistradores[(this->opcode_atual & 0x00F0) >> 4] - this->Vregistradores[(this->opcode_atual & 0x0F00) >> 8];           
               this->pcContador += 2;
            break;

            
            // 0x8XYE: Shifts VX left by one. VF is set to the value of the most significant bit of VX before the shift
            // VF eh setado com o bit mais significativo de VX
            // apos isso, VX sofre shift 1 << 
            case 0x000E: 
               printf("0x8XYE: Shifts VX left by one. VF is set to the value of the most significant bit of VX before the shift\n");
               this->Vregistradores[0xF] = this->Vregistradores[(this->opcode_atual & 0x0F00) >> 8] >> 7;
               printf ("Antes : " );  
               this->Vregistradores[(this->opcode_atual & 0x0F00) >> 8] <<= 1;
               printf ("Depois : 0x%i\n", this->Vregistradores[(this->opcode_atual & 0x0F00) >> 8] );  

               this->pcContador += 2;
            break;

            default:
               printf ("OPCODE nao encontrado [0x8000] dentro de 0x80: 0x%X\n", this->opcode_atual);
         }
      break;


      // 
      // 0x9XY0: Skips the next instruction if VX doesn't equal VY
      // pula a proxima instrucao se vx for diferente de vy
      case 0x9000:
         printf("pula a proxima instrucao se vx for diferente de vy\n");
         printf ("V1 valor : 0x%X\n", ( Vregistradores[ (this->opcode_atual & 0x0F00) ]) >> 8 );  
         printf ("V2 valor : 0x%X\n", ( Vregistradores[ (this->opcode_atual & 0x00F0) ]) >> 4 );  
         if(Vregistradores[ (this->opcode_atual & 0x0F00) >> 8] != Vregistradores[ (this->opcode_atual & 0x00F0) >> 4])
            this->pcContador += 4;
         else
            this->pcContador += 2;
      break;


      // ANNN: Sets I to the address NNN
      // aponta o I para o endereco NNN
      case 0xA000: 
         printf("ANNN: aponta o I para o endereco NNN\n");
         Iregister = opcode_atual & 0x0FFF;
         pcContador += 2;
      break;
      
      case 0xB000: // BNNN: Jumps to the address NNN plus V0
      printf("BNNN: Jumps to the address NNN plus V0\n");
         pcContador = (opcode_atual & 0x0FFF) + Vregistradores[0];
      break;
      
      // CXNN: Sets VX to a random number and NN
      // seta em vX um numero randomico de 0 a 16
      case 0xC000: 
      printf("CXNN: Sets VX to a random number and NN\n");
         Vregistradores[(opcode_atual & 0x0F00) >> 8] = (rand() % 0xFF) & (opcode_atual & 0x00FF);
         printf("randomico - %i\n", rand() % 10 + 1 );
         printf("randomico - %i\n", rand() % 0xFF );
         printf("randomico - %i\n", (rand() % 0xFF) & (opcode_atual & 0x00FF) );
         pcContador += 2;
      break;
   
      case 0xD000: // DXYN: Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels. 
                // Each row of 8 pixels is read as bit-coded starting from memory location I; 
                // I value doesn't change after the execution of this instruction. 
                // VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn, 
                // and to 0 if that doesn't happen
      {
         printf("DXYN\n");

         // x eh o primeiro digito apos D
         unsigned short x = Vregistradores[(opcode_atual & 0x0F00) >> 8];

         // y eh o segundo digito apos D
         unsigned short y = Vregistradores[(opcode_atual & 0x00F0) >> 4];

         // quantas posicoes da memoria serao lidos eh a altura
         // que eh a ultima posicao apos o D
         unsigned short height = opcode_atual & 0x000F;


         unsigned short pixel;

         // este eh o registrador de colisao
         Vregistradores[0xF] = 0;

         // y ira representar as linhas.. ler ate a altura(height)
         for (int yline = 0; yline < height; yline++)
         {

            // aqui esta a representacao dos pixels na memoria
            pixel = arrMemoria[Iregister + yline];
            printf("posicao da memoria %X\n",  (Iregister + yline) );

            // sempre de 8 em 8 por linha
            for(int xline = 0; xline < 8; xline++)
            {
               // testa um bit de cada vez   
               if((pixel & (0x80 >> xline)) != 0)
               {
                  printf("Tela antes: %X\n",  arrTela[(x + xline + ((y + yline) * 64))] );

                  if( arrTela[(x + xline + ((y + yline) * 64))] == 1 )
                  {
                     // indica que esta colidindo com alguma coisa que sera impressa na tela
                     Vregistradores[0xF] = 1;                                    
                  }

                  arrTela[x + xline + ((y + yline) * 64)] ^= 1; // xor

                  printf("Tela depois: %X\n",  arrTela[(x + xline + ((y + yline) * 64))] );
               }
            }
         }
                  
         // drawFlag = true;        
         this->setImprimirTela();
         pcContador += 2;
      }
      break;
         

      case 0xE000:
         switch(opcode_atual & 0x00FF)
         {
            // EX9E: Skips the next instruction if the key stored in VX is pressed
            // EX9E: pula a proxima instrucao se o valor que esta em VX foi precionado
            case 0x009E: 
               printf("EX9E: pula a proxima instrucao se o valor que esta em VX foi precionado\n");
               if( arrKeys[Vregistradores[(opcode_atual & 0x0F00) >> 8]] != 0)
                  pcContador += 4;
               else
                  pcContador += 2;
            break;
            
            // EXA1: Skips the next instruction if the key stored in VX isn't pressed
            // pula a proxima instrucao se a tecla em VX nao esta precionada
            case 0x00A1: 
               printf("// EXA1: pula a proxima instrucao se a tecla em VX nao esta precionada\n");
               if(arrKeys[Vregistradores[(opcode_atual & 0x0F00) >> 8]] == 0)
                  pcContador += 4;
               else
                  pcContador += 2;
            break;

            default:
               printf ("Unknown opcode [0xE000]: 0x%X\n", opcode_atual);
         }
      break;


      // 
      case 0xF000:

         // verifica apenas os 2 ultimos hexas
         switch(this->opcode_atual & 0x00FF)
         {

            // FX07: Sets VX to the value of the delay timer
            // seta o VX com o valor do timer DELAY
            case 0x0007: 
            printf("FX07: seta o VX com o valor do timer DELAY\n");
               this->Vregistradores[(this->opcode_atual & 0x0F00) >> 8] = this->delay_timer;
               this->pcContador+= 2;
               
            break;
                        
            // FX0A: A key press is awaited, and then stored in VX     
            // aguarda uma tecla ser precionada.. se ela for, salva em VX
            case 0x000A: 
            {
               printf("FX0A: aguarda uma tecla ser precionada.. se ela for, salva em VX     \n");
               bool keyPress = false;

               for(int i = 0; i < TOTAL_TECLAS; ++i)
               {
                  if(this->arrKeys[i] != 0)
                  {
                     this->Vregistradores[(this->opcode_atual & 0x0F00) >> 8] = i;
                     keyPress = true;
                  }
               }

               // se nenhuma tecla foi precionada, espera uma nova tecla
               // observe q o contador nao eh incrementado
               if(!keyPress)                 
               {
                  return;
               }

               this->pcContador+= 2;      
               
            }
            break;
            
            // FX15: Sets the delay timer to VX
            // seta delay_timer com o valor que esta em VX
            case 0x0015: 
            printf("// FX15: seta delay_timer com o valor que esta em VX\n");
               this->delay_timer = this->Vregistradores[(this->opcode_atual & 0x0F00) >> 8];
               this->pcContador+= 2;
               
            break;

            // FX18: Sets the sound timer to VX
            // seta o valor de sound_timer com o que esta em VX
            case 0x0018: 
               printf("// FX18: seta o valor de sound_timer com o que esta em VX\n");
               this->sound_timer = this->Vregistradores[(this->opcode_atual & 0x0F00) >> 8];
               this->pcContador+= 2;
               
            break;

            // FX1E: Adds VX to I
            // FX1E: adiciona I + VX 
            case 0x001E: 
               printf("// FX1E: adiciona I + VX \n");

               // VF is set to 1 when range overflow (I+VX>0xFFF), and 0 when there isn't.
               if ( this->Iregister + this->Vregistradores[(this->opcode_atual & 0x0F00) >> 8] > 0xFFF) 
               {
                  this->Vregistradores[0xF] = 1;
               }
               else
               {
                  this->Vregistradores[0xF] = 0;
               }

               this->Iregister  += this->Vregistradores[ (this->opcode_atual & 0x0F00) >> 8 ];
               this->pcContador += 2;
            break;

            // FX29: Sets I to the location of the sprite for the character in VX. 
            // Characters 0-F (in hexadecimal) are represented by a 4x5 font
            // seta I para o local em VX. Provavelmente eh um sprite de caracter
            case 0x0029: 
               printf("// FX29: seta I para o local em VX. Provavelmente eh um sprite de caracter\n");
               this->Iregister = Vregistradores[(this->opcode_atual & 0x0F00) >> 8] * 0x5;
               this->pcContador+= 2;
            break;

            // FX33: Stores the Binary-coded decimal representation 
            // of VX at the addresses I, I plus 1, and I plus 2
            // Salva a representacao decimal na memoria em 3 posicoes comecando por I
            case 0x0033: 
               printf("// FX33: Salva a representacao decimal na memoria em 3 posicoes comecando por I\n");
               this->arrMemoria[ this->Iregister ]    = Vregistradores[(this->opcode_atual & 0x0F00) >> 8] / 100;
               this->arrMemoria[ this->Iregister + 1] = (Vregistradores[(this->opcode_atual & 0x0F00) >> 8] / 10) % 10;
               this->arrMemoria[ this->Iregister + 2] = (Vregistradores[(this->opcode_atual & 0x0F00) >> 8] % 100) % 10;              
               this->pcContador+= 2;
            break;

            // FX55: Stores V0 to VX in memory starting at address I               
            // salva na memoria a partir da posicao i, todos os valores de V0 ateh Vx          
            case 0x0055: 
               printf("// FX55: salva na memoria a partir da posicaoo i, todos os valores de V0 ateh Vx\n");
               
               for (int i = 0; i <= ((this->opcode_atual & 0x0F00) >> 8); ++i)
               {
                  arrMemoria[ this->Iregister + i] = Vregistradores[i];   
               }

               // On the original interpreter, when the operation is done, I = I + X + 1.
               this->Iregister += ((this->opcode_atual & 0x0F00) >> 8) + 1;
               this->pcContador+= 2;            
            break;

            // FX65: Fills V0 to VX with values from memory starting at address I              
            // preenche os registradores v0 ate Vx com os valores da memoria comecando em I
            case 0x0065: 
               printf("// FX65: preenche os registradores v0 ate Vx com os valores da memoria comecando em I\n");
               for (int i = 0; i <= ((this->opcode_atual & 0x0F00) >> 8); ++i)
               {
                  this->Vregistradores[i] = this->arrMemoria[this->Iregister + i];         
               }

               // On the original interpreter, when the operation is done, I = I + X + 1.
               this->Iregister += ((this->opcode_atual & 0x0F00) >> 8) + 1;
               this->pcContador+= 2;
            
            break;

            default:
               printf ("OPCODE nao encontrado [0xF000] dentro de 0xF: 0x%X\n", this->opcode_atual);
         }
      break;

      default:
         printf ("OPCODE nao encontrado: 0x%X\n", this->opcode_atual );
   }



   // Update timers
   if(this->delay_timer > 0)
   {
      --this->delay_timer;            
   }

   if(this->sound_timer > 0)
   {
      if(this->sound_timer == 1)
         printf("BEEP!\n");
      --this->sound_timer;
   }  

   printf("\n\n");
   Sleep(2);

}

/**
 * Mostra todos os valroes de memoria na tela
 */
void CChip8Maquina::mostraMemoria()
{
   for( int i=0; i<TOTAL_MEMORIA; i++ )
   {
      printf ("Memoria %i: 0x%X\n", i, this->arrMemoria[ i ] );  
   }
}