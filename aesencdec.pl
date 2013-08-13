#!/usr/bin/perl
# aesencdec.pl - encrypt and decrypt data using AES CBC with a 256 bit key
#
#  Dave Stoddard - 12/11/2008
#  dgs at accelix.com
#
#  This software is placed the public domain and free for any use.
#  No warrantees.  Use at your own risk.
#
#  Description
#  -----------
#  This is a demonstration program for illustrating the encryption and
#  decryption of input data in Perl using AES CBC encryption that is
#  interoperable with C language programming using OpenSSL.  Refer to
#  aesencdec.c for an equivalent of this program in C.
#
#  The program functions as a console application that prompts for user input,
#  encrypts and displays the hex encoded result, then decrypts the result and
#  displays the original input.  To exit the input loop, enter "exit" or
#  "quit" at the prompt.
#
use strict;
use warnings;
# use diagnostics;
 
use Crypt::CBC;
use String::HexConvert ':all';
use MIME::Base64;
 
my $obj     = "";
my $len     = "";
#my $key     = "01234567890123456789012345678901";
my $key     = "0123456789012345";
my $meth    = "Crypt::OpenSSL::AES";
my $encdata = "";
my $decdata = "";

#The iv is the md5 hash of the key
my $md5 = Digest::MD5->new;
$md5->add($key);
my $iv = $md5->digest;
print "IV is ".ascii_to_hex($iv)."\n";
 
print "input> ";
while (<STDIN>) {
  chomp;
  last if ($_ eq "exit");
  last if ($_ eq "quit");
 
  # create cipher object
  $obj = Crypt::CBC->new (
    -key         => $key,
    -literal_key => 1,
    -header      => "none",
    -iv          => $iv,
    -cipher      => $meth,
    -keysize     => 16
  );
 
  # encrypt data
  $encdata = $obj->encrypt ($_);
  $len     = length ($encdata);
#  my $hexencdata = ascii_to_hex($encdata);
  print "in hex: ".ascii_to_hex($encdata)." (len = $len)\n";

  print "in base64: ".encode_base64($encdata)."\n";
 
  # decrypt data
  $decdata = $obj->decrypt ($encdata);
  $len     = length ($decdata);
  print "normal: $decdata (len = $len)\n";
 
  # check for match
  if ($decdata eq $_) {
    print "compare OK.\n";
  } else {
    print "MISMATCH!!\n";
  }
 
  # input prompt
  print "\n";
  print "input> ";
}
exit 0;
