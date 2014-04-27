#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <bitset>
#include <string>

// projeto
#include "constantes.h"
#include "CChip8Uteis.h"


/**
 * Converte de hexadecimal para binario e joga em uma std::string
 *
 * uso:
 CChip8Uteis objTeste;
   std::string aaa = objTeste.GetBinaryStringFromHexString("0xA2");
   printf("aaa: %s\n", aaa.c_str() );
 */
std::string CChip8Uteis::GetBinaryStringFromHexString (std::string sHex)
{
   std::string sReturn = "";

   for (int i = 0; i < sHex.length (); ++i)
   {
      switch (sHex [i])
      {
         case '0': sReturn.append ("0000"); break;
         case '1': sReturn.append ("0001"); break;
         case '2': sReturn.append ("0010"); break;
         case '3': sReturn.append ("0011"); break;
         case '4': sReturn.append ("0100"); break;
         case '5': sReturn.append ("0101"); break;
         case '6': sReturn.append ("0110"); break;
         case '7': sReturn.append ("0111"); break;
         case '8': sReturn.append ("1000"); break;
         case '9': sReturn.append ("1001"); break;
         case 'a': sReturn.append ("1010"); break;
         case 'b': sReturn.append ("1011"); break;
         case 'c': sReturn.append ("1100"); break;
         case 'd': sReturn.append ("1101"); break;
         case 'e': sReturn.append ("1110"); break;
         case 'f': sReturn.append ("1111"); break;
      }
   }

   return sReturn;
}


/**
 *
 unsigned long long coisa = 234889762;
   
   std::string number;
   std::stringstream strstream;
   strstream << coisa;
   strstream >> number;
   */