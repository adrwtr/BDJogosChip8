#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <bitset>
#include <string>


std::string GetBinaryStringFromHexString (std::string sHex)
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


int main()
{
   char a = 0xF;
   a = a << 3;
   a = a | 1;

   printf ("Opcode 1: 0x%X\n", a);

   std::string aaa = GetBinaryStringFromHexString( "0xFFFFFFF8" );
   printf("aaa: %s\n", aaa.c_str() );

   return 0;
}