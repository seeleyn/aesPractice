aesPractice
===========

This is a some practice code that shows how to encrypte and decrypt with AES for C++ and perl based on examples I found from the internet. 

The example I found most helpful was written by Dave Stoddard (dgs at accelix.com). Both the C++ and Perl examples draw heavily from Dave Stoddard's code. Dave's original code is http://marc.info/?l=openssl-users&m=122919878204439 The licence is public domain.

I also used the base64 code from René Nyffenegger found at http://www.adp-gmbh.ch/cpp/common/base64.html
In accordance with Nyffenegger's licensing restriction I left the copyright statement and actual source in the comments of the code.

The code is as is with no warranty. Use at your own risk.

===============
The C++ Example
===============

To compile the c++ you'll have to install the following

  #[root] yum install -y scons
  #[root] yum install -y openssl
  #[root] yum install -y openssl-devel

Once these are installed, to compile do the following

  $ cd aesPractice
  $ scons

To run it

  $ ./aesenc

================
The Perl Example
=================

To run the perl code you may have to install a number of perl dependencies. cpan is the easiest way

  #[root] yum install -y cpan
  #[root] cpan -i String::HexConvert


Once you do, simply run

  $ ./aesencdec.pl 


