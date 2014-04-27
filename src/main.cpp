/**
 * Este é o arquivo principal do projeto bdjogos_chip8. 
 * O programa come?a a ser executado por aqui
 */

#include <iostream>
#include <stdio.h>
#include <windows.h>
#include "GL/glut.h"


#include "CLerRom.h"
#include "CChip8Maquina.h"

// objeto responsavel pela emulacao da maquina chip8
CChip8Maquina objCChip8Maquina;

// tamanho da tela que pode ser modificada pela opengl
int SCREEN_W = MODIFICA_TAMANHO_TELA * 64; 
int SCREEN_H = MODIFICA_TAMANHO_TELA * 32;


// funcoes padroes da opengl
void opengl_display();
void opengl_updateQuads();
void opengl_drawPixel(int x, int y);
void opengl_reshape_window( GLsizei w, GLsizei h );
void opengl_keyboardUp( unsigned char key, int x, int y );
void opengl_keyboardDown( unsigned char key, int x, int y );



/**
 * Função principal. A primeira a ser executada
 * func1
 * 
 * @return int
 */
int main( int argc, char **argv )
{
   // indica se o jogo foi carregado na memoria
   bool isJogoCarregadoMemoria = false;  

   // objeto responsavel por fazer a leitura da rom
   CLerRom objCLerRom;   
   objCLerRom.iniciar(); 

   
   objCChip8Maquina.iniciar();

   // nome do arquivo enviado por argumento
   const char * nome_arquivo = argv[1];

   printf("Iniciando o programa...\n\n");

   // verifica se o parametro com a rom do jogo foi enviada por linha de comando
   if ( argc < 2 )
   {
      printf("Programa iniciado de forma incorreta\n");
      printf("Tente: bdjogos_chip8.exe chip8_jogo\n\n");

      objCChip8Maquina.finalizar();
      objCLerRom.finalizar();

      return 1;
   }

   // faz a leitura da rom
   isJogoCarregadoMemoria = objCLerRom.lerRom( nome_arquivo );

   // a rom pode ser lida?
   if ( isJogoCarregadoMemoria == false )
   {
      printf("Ocorreu algum problema ao tentar carregar o jogo para a memoria. \n\n");         

      objCChip8Maquina.finalizar();
      objCLerRom.finalizar();
      return 1;
   }

   // inicia a maquina
   objCChip8Maquina.iniciarMaquina();
   
   // coloca a rom na posicao correta na maquina
   objCChip8Maquina.setMemoria( 
      objCLerRom.getConteudoArquivo(), 
      objCLerRom.getTamanhoArquivo() 
   );


   // iniciando a opengl
   // Setup OpenGL
   glutInit(&argc, argv);          
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
   glutInitWindowSize( SCREEN_W, SCREEN_H );
   glutInitWindowPosition(320, 320);
   glutCreateWindow("BDJOGOS CHIP8");
   
   glutDisplayFunc(opengl_display);
   glutIdleFunc(opengl_display);
   glutReshapeFunc(opengl_reshape_window);        
   glutKeyboardFunc(opengl_keyboardDown);
   glutKeyboardUpFunc(opengl_keyboardUp); 

   glutMainLoop(); 

   // para testes
   // objCChip8Maquina.emular(); 

   return 0;
}

void opengl_display()
{
   objCChip8Maquina.emular();
      
   if(objCChip8Maquina.getSnImprimirTela() == true )
   {
      // Clear framebuffer
      glClear(GL_COLOR_BUFFER_BIT);
      opengl_updateQuads();      
   
      // Swap buffers!
      glutSwapBuffers();    

      // Processed frame
      objCChip8Maquina.setNaoImprimirTela();
   }
}

void opengl_updateQuads()
{
   // Draw
   for(int y = 0; y < 32; ++y)      
      for(int x = 0; x < 64; ++x)
      {
         if(objCChip8Maquina.arrTela[(y*64) + x] == 0) 
            glColor3f(0.0f,0.0f,0.0f);       
         else 
            glColor3f(1.0f,1.0f,1.0f);

         opengl_drawPixel(x, y);
      }
}

void opengl_drawPixel(int x, int y)
{
   glBegin(GL_QUADS);
      glVertex3f((x * MODIFICA_TAMANHO_TELA ) + 0.0f,     (y * MODIFICA_TAMANHO_TELA ) + 0.0f,    0.0f);
      glVertex3f((x * MODIFICA_TAMANHO_TELA ) + 0.0f,     (y * MODIFICA_TAMANHO_TELA ) + MODIFICA_TAMANHO_TELA , 0.0f);
      glVertex3f((x * MODIFICA_TAMANHO_TELA ) + MODIFICA_TAMANHO_TELA , (y * MODIFICA_TAMANHO_TELA ) + MODIFICA_TAMANHO_TELA , 0.0f);
      glVertex3f((x * MODIFICA_TAMANHO_TELA ) + MODIFICA_TAMANHO_TELA , (y * MODIFICA_TAMANHO_TELA ) + 0.0f,    0.0f);
   glEnd();
}

void opengl_reshape_window(GLsizei w, GLsizei h)
{
   glClearColor(0.0f, 0.0f, 0.5f, 0.0f);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0, w, h, 0);        
   glMatrixMode(GL_MODELVIEW);
   glViewport(0, 0, w, h);

   SCREEN_W = w;
   SCREEN_H = h;
}


void opengl_keyboardDown(unsigned char key, int x, int y)
{
   if(key == 27)    // esc
      exit(0);

   if (key == '1')      objCChip8Maquina.arrKeys[0x1] = 1;
   else if(key == '2')  objCChip8Maquina.arrKeys[0x2] = 1;
   else if(key == '3')  objCChip8Maquina.arrKeys[0x3] = 1;
   else if(key == '4')  objCChip8Maquina.arrKeys[0xC] = 1;

   else if(key == 'q')  objCChip8Maquina.arrKeys[0x4] = 1;
   else if(key == 'w')  objCChip8Maquina.arrKeys[0x5] = 1;
   else if(key == 'e')  objCChip8Maquina.arrKeys[0x6] = 1;
   else if(key == 'r')  objCChip8Maquina.arrKeys[0xD] = 1;

   else if(key == 'a')  objCChip8Maquina.arrKeys[0x7] = 1;
   else if(key == 's')  objCChip8Maquina.arrKeys[0x8] = 1;
   else if(key == 'd')  objCChip8Maquina.arrKeys[0x9] = 1;
   else if(key == 'f')  objCChip8Maquina.arrKeys[0xE] = 1;

   else if(key == 'z')  objCChip8Maquina.arrKeys[0xA] = 1;
   else if(key == 'x')  objCChip8Maquina.arrKeys[0x0] = 1;
   else if(key == 'c')  objCChip8Maquina.arrKeys[0xB] = 1;
   else if(key == 'v')  objCChip8Maquina.arrKeys[0xF] = 1;

   //printf("Press key %c\n", key);
}

void opengl_keyboardUp(unsigned char key, int x, int y)
{
   if(key == '1')       objCChip8Maquina.arrKeys[0x1] = 0;
   else if(key == '2')  objCChip8Maquina.arrKeys[0x2] = 0;
   else if(key == '3')  objCChip8Maquina.arrKeys[0x3] = 0;
   else if(key == '4')  objCChip8Maquina.arrKeys[0xC] = 0;

   else if(key == 'q')  objCChip8Maquina.arrKeys[0x4] = 0;
   else if(key == 'w')  objCChip8Maquina.arrKeys[0x5] = 0;
   else if(key == 'e')  objCChip8Maquina.arrKeys[0x6] = 0;
   else if(key == 'r')  objCChip8Maquina.arrKeys[0xD] = 0;

   else if(key == 'a')  objCChip8Maquina.arrKeys[0x7] = 0;
   else if(key == 's')  objCChip8Maquina.arrKeys[0x8] = 0;
   else if(key == 'd')  objCChip8Maquina.arrKeys[0x9] = 0;
   else if(key == 'f')  objCChip8Maquina.arrKeys[0xE] = 0;

   else if(key == 'z')  objCChip8Maquina.arrKeys[0xA] = 0;
   else if(key == 'x')  objCChip8Maquina.arrKeys[0x0] = 0;
   else if(key == 'c')  objCChip8Maquina.arrKeys[0xB] = 0;
   else if(key == 'v')  objCChip8Maquina.arrKeys[0xF] = 0;
}
