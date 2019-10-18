/* encodeuP.c - Attribute encoding (userParameters) in "Active Directory"
   Copyright (C) 2008 Harun "Harry" Basalamah

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.*/

/* This code is based on research done by Christian Herzog and Elmar
   Heeb. Find details here: <daduke.org/linux/userparameters.html> */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define VERSION "v0.5"
#define MYSTERIOUSOFFSET 2 /* no idea why this can differ */

void usage(); /* Prototypes */
unsigned long EncodeuParamChar(char);

int main(int argc, char *argv[]) {
 /* variables */
 unsigned char uParamPart1[] = {0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
  0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
  0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
  0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x50,0x07,0x1A,0x08,0x01,0x43,
  0x74,0x78,0x43,0x66,0x67,0x50,0x72,0x65,0x73,0x65,0x6E,0x74,0xE3,0x94,0xB5,
  0xE6,0x94,0xB1,0xE6,0x88,0xB0,0xE3,0x81,0xA2,0x18,0x08,0x01,0x43,0x74,0x78,
  0x43,0x66,0x67,0x46,0x6C,0x61,0x67,0x73,0x31,0xE3,0x80,0xB0,0xE3,0x80,0xB1,
  0xE3,0x80,0xB2,0xE3,0x80,0xB9,0x12,0x08,0x01,0x43,0x74,0x78,0x53,0x68,0x61,
  0x64,0x6F,0x77,0xE3,0x84,0xB0,0xE3,0x80,0xB0,0xE3,0x80,0xB0,0xE3,0x80,0xB0,
  0x2A,0x02,0x01,0x43,0x74,0x78,0x4D,0x69,0x6E,0x45,0x6E,0x63,0x72,0x79,0x70,
  0x74,0x69,0x6F,0x6E,0x4C,0x65,0x76,0x65,0x6C,0xE3,0x84,0xB0,0x18,0xFF,0x01,
  0x43,0x74,0x78,0x57,0x46,0x48,0x6F,0x6D,0x65,0x44,0x69,0x72},   /*---*/
 uParamPart2[] = {0x22,0x06,0x01,0x43,0x74,0x78,0x57,0x46,0x48,0x6F,0x6D,0x65,
  0x44,0x69,0x72,0x44,0x72,0x69,0x76,0x65,0xE3,0x9C,0xB5,0xE6,0x84,0xB3,0xE3,
  0x80,0xB0,0x20,0xFF,0x01,0x43,0x74,0x78,0x57,0x46,0x50,0x72,0x6F,0x66,0x69,
                /*---*/
  0x6C,0x65,0x50,0x61,0x74,0x68};
/* uParamPart3[] = {0x44,0x69,0x72,0x44,0x72,0x69,0x76,0x65,0xE3,0x9C,0xB5,0xE6,
  0x84,0xB3,0xE3,0x80,0xB0}; */

 unsigned int count = 0;
 unsigned long uParamSEnd = 0xE380B0; /*encoded string end marker */

 if (argc != 3)
  usage();

 /* determine length of 'CtxWFHomeDir' plus offset, replace the $FF value */
 uParamPart1[157] = 2 * strlen(argv[1]) + MYSTERIOUSOFFSET;

 /* spit out part one */
 for (; count < sizeof(uParamPart1); count++)
  printf("%02x", uParamPart1[count]);

 /* encode 'CtxWFHomeDir', eject it */
 for (count = 0; count < strlen(argv[1]); count++)
  printf("%06x", EncodeuParamChar(argv[1][count]));

 /* print blob-conform string-end */
 printf("%06x", uParamSEnd);

 /* determine length of 'CtxWFProfilePath' plus offset and set it in part 2 */
 uParamPart2[30] = 2 * strlen(argv[2]) + MYSTERIOUSOFFSET;

 /* throw out part two */
 for (count = 0; count < sizeof(uParamPart2); count++)
  printf("%02x", uParamPart2[count]);

 /* encode 'CtxWFProfilePath' value and disgorge it */
 for (count = 0; count < strlen(argv[2]); count++)
  printf("%06x", EncodeuParamChar(argv[2][count]));

 printf("%06x", uParamSEnd);

 exit(EXIT_SUCCESS);
}

unsigned long EncodeuParamChar(char uParamChar) {
 unsigned long header =       0x00e00000, clnibblele9 =  0x00038000,
               clnibblegt9 =  0x00068000, chnibblele9 =  0x000000B0,
               chnibblegt9 =  0x000001A0, result =       0x00000000,
               temp =         0x00000000;

 unsigned short hnibble = uParamChar >> 4; /* isolate nibbles */
 unsigned short lnibble = uParamChar & 0x0F;

 result |= header; /* insert header into result */

 if (hnibble <= 9) /* insert "control value" */
  result |= chnibblele9;
 else
  result |= chnibblegt9;

 if (lnibble <= 9) /* here the CV for the lo nibble */
  result |= clnibblele9;
 else
  result |= clnibblegt9;

 if (hnibble > 9) /* obfuscate the nibble if > 9 */
  hnibble -= 9;

 if (lnibble > 9) /* same here *sigh* */
  lnibble -= 9;

 temp = (long)lnibble; /* prepare lo nibble for injection */
 temp <<= 10; /* shift it to its destination */
 result |= temp; /* insert into result */

 temp &= 0x00000000; /* i must have read to much assembler code... ;) */

 temp = (long)hnibble;
 result |= temp; /* insert hi nibble */

 return result;
}

void usage(void) {
 printf("encodeuP %s copyright (C) 2008 Harun \"Harry\" Basalamah\n", VERSION);
 printf("Encodes CtxWFHomeDir and CtxWFProfilePath binary data\nfor the userParameters attribute in \"Active Directory\"\n\n");
 printf("Usage: encodeuP <TS HomeDir> <TS Profile Path>\n");
 printf("This program returns the data as ASCII code for use in Joe Richards'\n");
 printf("excellent tool 'AdMod' and uses drive letter 'W' for CtxWFHomeDirDrive\n");
 exit(EXIT_FAILURE);
}
