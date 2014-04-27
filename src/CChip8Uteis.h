#ifndef CCHIP8UTEIS_H
#define CCHIP8UTEIS_H


#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <bitset>
#include <string>


// projeto
#include "constantes.h"

/**
 * Esta classe tem funcoes uteis para teste do emulador
 */
class CChip8Uteis
{
   public:
      std::string GetBinaryStringFromHexString (std::string sHex);
      

};

#endif // #ifndef CCHIP8UTEIS_H
