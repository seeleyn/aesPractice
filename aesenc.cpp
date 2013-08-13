/*
 * aesencdec.c - encrypt and decrypt data using AES CBC with a 256 bit key
 *
 * Dave Stoddard - 12/11/2008
 * dgs at accelix.com
 *
 * This software is placed the public domain and free for any use.
 * No warrantees.  Use at your own risk.
 *
 * Description
 * -----------
 * This is a demonstration program for illustrating the encryption and  
 * decryption of input data in C using AES CBC encryption that is  
 * interoperable with Perl language programming using Crypt::CBC.
 * Refer to aesencdec.pl for an equivalent of this program in Perl.
 *
 * The program functions as a console application that prompts for user
input,  
 * encrypts and displays the hex encoded result, then decrypts the result
and
 * displays the original input.  To exit the input loop, enter "exit" or  
 * "quit" at the prompt. 
 */

# include <stdio.h>
# include <ctype.h>
# include <string.h>


# include <openssl/aes.h>
# include <openssl/md5.h>
# include <iostream>
# include "base64.h"

# define BLOCK_LEN	16
# define MAXBUF		65536
# define MAXHEX		(MAXBUF * 2) + 1
# define KEY		"0123456789012345"
# define IV		"RandomIVRandomIV"

using namespace std;

// function prototypes
unsigned int bin2hex (unsigned char *in, unsigned char *out, unsigned int
len);
unsigned int hex2bin (unsigned char *in, unsigned char *out, unsigned int
len);
unsigned int PadData (unsigned char *ibuf, unsigned int ilen, int blksize);
unsigned int NoPadLen (unsigned char *ibuf, unsigned int ilen);

void calculateIvFromKey(const unsigned char* key, unsigned char* iv);


int
main(int argc, char *argv[])
{
  unsigned long ilen;
  unsigned char ibuf[MAXBUF];	// padded input to encrypt
  unsigned char obuf[MAXHEX];	// encrypt output
  unsigned char xbuf[MAXHEX];	// hex encrypt output
  unsigned char ybuf[MAXBUF];	// hex decrypt output
  unsigned char dbuf[MAXBUF];	// decrypt output
  unsigned char key[] = KEY;
  unsigned char iv[17];

  calculateIvFromKey(key,iv);
  iv[16] = '\0';
  cout << "iv (from md5 of key) is " << iv << endl;

  AES_KEY aeskeyEnc, aeskeyDec;


  // read input and convert \n to \0
  string input;
  cout << "input> " << endl;
  getline (cin, input);
    
  // prepare the input data with padding
  memset (ibuf, 0x00, sizeof (ibuf));
  memcpy (ibuf, input.c_str(), input.length());

  // pad and calc length of block to encode
  ilen = PadData (ibuf, input.length(), BLOCK_LEN);
 
  // init cipher keys 
  AES_set_encrypt_key (key, 128, &aeskeyEnc);
  AES_set_decrypt_key (key, 128, &aeskeyDec);

  // encrypt string
  //memcpy (iv, IV, sizeof (IV));
  AES_cbc_encrypt (ibuf, obuf, ilen, &aeskeyEnc, iv, AES_ENCRYPT);

  // convert encoded string to hex and display
  bin2hex (obuf,xbuf,ilen);
  printf ("hex encoded: %s (len = %d)\n", (char*)xbuf, (int) strlen ((char*)xbuf));

  // convert hex string to binary
  hex2bin (xbuf, ybuf, (int) strlen((char*)xbuf));

  // compare binary values
  if (memcmp (obuf, ybuf, ilen) != 0) {
    printf ("HEX DECODE FAILED!\n");
    return (1);
  }

  // convert encoded string to base64 and display
  string base64encoded = base64_encode(obuf,ilen);
  cout << "base64 encoded: " << base64encoded << endl;

  // decrypt string
  calculateIvFromKey(key,iv);
  //cout << "iv is now " << iv << endl;

  AES_cbc_encrypt (ybuf, dbuf, ilen, &aeskeyDec, iv, AES_DECRYPT);
  dbuf [NoPadLen (dbuf, ilen)] = 0x00;
  printf ("decode: %s (len = %d)\n", dbuf, (int)strlen((char*)dbuf));

  // compare binary values
  if (memcmp (input.c_str(), dbuf, input.length()) != 0) {
    printf ("DECRYPT FAILED!\n");
    return (1);
  }

  return (0);
}

/*
 * bin2hex() takes a block of data provided in ibuf with a length
 * of ilen and converts it into a zero-terminated ascii string of
 * hexadecimal characters.  It returns the length of the resulting
 * hexadecimal string (exclusive of the terminating zero).  NOTE: the
 * output buffer (obuf) must be able to accommodate twice the amount
 * of data as the input buffer + 1 byte for the zero terminator.
 */

unsigned int
bin2hex (unsigned char *ibuf, unsigned char *obuf, unsigned int ilen)
{
  unsigned int  i;			// loop iteration conuter
  unsigned int  j = (ilen * 2) + 1;	// output buffer length
  unsigned char *p;

  p = obuf;		// point to start of output buffer

  for (i = 0; i < ilen; i++) {
    sprintf ((char*)p,"%2.2x", (unsigned char) ibuf [i]);
    p += 2;
    j -= 2;
  }
  *p = '\0';
  return (strlen ((char*)obuf));
}

/*
 * hex2bin() converts an array of hex digits into an array of binary values.
 * Inputs are pointers to the input and output character arrays, and the
 * length on the input string.  The function returns the length of the
 * resulting output array, or 0 if there is an error.  If the input string
 * is zero terminated, you can call this as follows:
 *
 *   ilen = hex2bin (ibuf, obuf, strlen (ibuf));
 *
 * Note that the output is not terminated by a zero as the output
 * list is likely contain binary data.
 */

unsigned int
hex2bin (unsigned char *ibuf, unsigned char *obuf, unsigned int ilen)
{
  unsigned int   i;			// loop iteration variable
  unsigned int   j;			// current character
  unsigned int   by = 0;		// byte value for conversion
  unsigned char  ch;			// current character

  // process the list of characaters
  for (i = 0; i < ilen; i++) {
    ch = toupper(*ibuf++);		// get next uppercase character

    // do the conversion
    if(ch >= '0' && ch <= '9')
      by = (by << 4) + ch - '0';
    else if(ch >= 'A' && ch <= 'F')
      by = (by << 4) + ch - 'A' + 10;
    else {				// error if not hexadecimal
      memcpy (obuf,"ERROR",5);
      return 0;
    }

    // store a byte for each pair of hexadecimal digits
    if (i & 1) {
      j = ((i + 1) / 2) - 1;
      obuf [j] = by & 0xff;
    }
  }

  return (j+1);
}

/*
 * The PadData() function is used to pad input data for cipher block
 * chaining using standard padding as specified in PKCS#5.  Input data
 * is padded in place.  The ilen value is the length of the data before
 * padding.  The blksize parameter is the block size to use (16 for AES
CBC).
 * The function returns the new length of ibuf after padding.
 *
 * NOTE: The ibuf must be large enough to hold at least blksize additional
 * bytes of data to accommodate padding.
 */

unsigned int
PadData (unsigned char *ibuf, unsigned int ilen, int blksize)
{
  unsigned int   i;			// loop counter
  unsigned char  pad;			// pad character (calculated)
  unsigned char *p;			// pointer to end of data

  // calculate pad character
  pad = (unsigned char) (blksize - (ilen % blksize));

  // append pad to end of string
  p = ibuf + ilen;
  for (i = 0; i < (int) pad; i++) {
    *p = pad;
    ++p;
  }

  return (ilen + pad);
}

/*
 * NoPadLen() will recalculate the length of an array of data after the pad
 * characters have been removed and will return the new length to the
caller.
 * Note that nothing is altered by this routine -- it simply returns the
 * adjusted length after taking into account the removal of the padding.
 *
 * NOTE: Assuming "buf" is a character array that contains your data after
 * it has been decrypted (with padding still appended), you can remove the
 * padding and zero terminate the text string by using the following
 * construct:
 *
 *   buf [NoPadLen (buf, len)] = 0x00;
 */

unsigned int
NoPadLen (unsigned char *ibuf, unsigned int ilen)
{
  unsigned int   i;			// adjusted length
  unsigned char *p;			// pointer to last character

  p = ibuf + (ilen - 1);
  i = ilen - (unsigned int) *p;
  return (i);
}

/* Calculates the iv from the key using an md5 hash.
  @param key the AES key
  @param iv a pointer to the initialization vector memory that must be at 
  least 16 bytes. Whatever was in the iv will be overwritten by an md5 hash
*/
void calculateIvFromKey(const unsigned char* key, unsigned char* iv) {
  MD5_CTX ctx;
  MD5_Init(&ctx);
  MD5_Update(&ctx,(char*)key, strlen((char*)key));
  MD5_Final(iv, &ctx);
}


// end aesencdec.cpp

