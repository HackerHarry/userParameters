/* encodeuP.c - User attribute encoding (userParameters) in "Active Directory"
   Initially written somewhere around 2008, refurbished in 2020
   See LICENSE file for license details

   This code is based on research done by Christian Herzog and Elmar
   Heeb. Find details here: <daduke.org/linux/userparameters.html> */

#include <stdio.h>
#include <stdlib.h>

int main(void) {
 unsigned long header =       0x00e00000, clnibblele9 =  0x00038000,
               clnibblegt9 =  0x00068000, chnibblele9 =  0x000000B0,
               chnibblegt9 =  0x000001A0, result =       0x00000000,
               temp =         0x00000000;
 char c;
 unsigned short hnibble, lnibble;

 while ((c = getchar()) != EOF) {
  hnibble = c >> 4;
  lnibble = c & 0x0F;
  result |= header; /* insert header into result */
  if (hnibble <= 9) /* insert "control value" */
   result |= chnibblele9;
  else
   result |= chnibblegt9;
  if (lnibble <= 9) /* here the CV for the low nibble */
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
  temp &= 0x00000000; /* i must have read to much assembler code... */
  temp = (long)hnibble;
  result |= temp; /* insert hi nibble */
  printf("%000000x", result);
  result = temp = 0x00000000;
 }
 return EXIT_SUCCESS;
}
