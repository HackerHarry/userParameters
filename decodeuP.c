/* decodeuP.c - User attribute decoding (userParameters) in "Active Directory"
   Initially written somewhere around 2008, refurbished in 2020
   See LICENSE file for license details

   This code is based on research done by Christian Herzog and Elmar
   Heeb. Find details here: <daduke.org/linux/userparameters.html> */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 8192

void decodeuParamChar(char, char, char);

int main(void) {
 unsigned long convert, len, n, bufptr = 0;
 char temp[3] = { 0 }, c, c2, buffer[MAXLEN];

 while ((c = getchar()) != EOF && bufptr < MAXLEN) {
  temp[0] = c;
  temp[1] = (c2 = getchar());
  convert = strtoul(temp, NULL, 16);
  buffer[bufptr] = (char)convert;
  bufptr++;
 }
 buffer[bufptr] = '\0';

 len = strlen(buffer);
 if (len < 3 || len > MAXLEN)
  return EXIT_FAILURE;
 for (n = 0; n <= (len - 3); n += 3)
  decodeuParamChar(buffer[n], buffer[n + 1], buffer[n + 2]);
 return EXIT_SUCCESS;
}

void decodeuParamChar(char c1, char c2, char c3) {
 char result = 0x00;
 unsigned int hnibbleplus9 = 0, lnibbleplus9 = 0;
 lnibbleplus9 = c1 & 0x04; /* determine whether nibbles need to be +9 */
 hnibbleplus9 = c2 & 0x01;
 c2 >>= 2; /* shift to fit */
 c2 &= 0x0f; /* mask lo nibble */
 if (lnibbleplus9)
  c2 += 9;
 c3 <<= 4; /* shift to fit hi nibble */
 if (hnibbleplus9)
  c3 += 0x90;
 result = c2 | c3;
 putchar(result);
}
