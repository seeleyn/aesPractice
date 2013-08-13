aesPractice
===========

This is a some practice code that shows how to encrypte and decrypt with AES for C++ and perl based on examples I found from the internet. 

The example I found most helpful was written by Dave Stoddard (dgs at accelix.com). Both the C++ and Perl examples draw heavily from Dave Stoddard's code. Dave's original code is http://marc.info/?l=openssl-users&m=122919878204439 The licence is public domain.

I also used the base64 code from René Nyffenegger found at http://www.adp-gmbh.ch/cpp/common/base64.html
In accordance with Nyffenegger's licensing restriction I left the copyright statement and actual source in the comments of the code.

The code is as is with no warranty. Use at your own risk.


The C++ code can be compiled using scons

  $ cd aesPractice
  $ scons

To run it

  $ ./aesenc


To run the perl code you may have to install a number of perl dependencies. Once you do, simply run

  $ ./aesencdec.pl 


