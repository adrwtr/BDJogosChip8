#ifndef CCHIP8MAQUINA_H
#define CCHIP8MAQUINA_H

#include <stdio.h>
#include <stdlib.h>

// projeto
#include "constantes.h"

/**
 * Esta classe eh responsavel pela maquina chip8 
 * ela representa todas as regras do chip8 
 */
class CChip8Maquina
{
   public:
      void iniciar();
      void finalizar();      
      bool setMemoria( const char * bfConteudoArquivo, long tamanho_arquivo );
      void limparTela();
      void limparMemoria();
      void iniciarMaquina();
      void reiniciarMaquina();
      void setImprimirTela();
      void setNaoImprimirTela();
      bool getSnImprimirTela();

      void emular();

      unsigned char  arrTela[TOTAL_PIXELS];     // Total de pixels: 2048
      unsigned char  arrKeys[TOTAL_TECLAS];     // teclado
      unsigned char  arrMemoria[TOTAL_MEMORIA]; // Memory (size = 4k)      

   private: 
      unsigned short pcContador;                // Program counter
      unsigned short opcode_atual;              // opcode que eh o atual a ser processador
      unsigned short Iregister;                 // Este eh o registro I
      unsigned short spPilha;                   // Este eh o ponteiro para a pilha
      
      unsigned char  Vregistradores[16];        // Os registradores V-regs (V0-VF)
      unsigned short arrPilha[16];              // a pilha Stack (16 levels)
                  
      unsigned char  delay_timer;               // Delay timer
      unsigned char  sound_timer;               // Sound timer 
      bool sn_imprimir;   

      void iniciarFontSet();
      void mostraMemoria();
};

#endif // #ifndef CCHIP8MAQUINA_H