#!/usr/bin/perl

use strict;
use warnings;

 
use Crypt::CBC;
use String::HexConvert ':all';
use MIME::Base64;
 
my $key     = "0123456789012345";


my $usage = "Usage: <base64 encoded encrypted data>\n";
die $usage unless scalar @ARGV >= 1;


#The iv is the md5 hash of the key
my $md5 = Digest::MD5->new;
$md5->add($key);
my $iv = $md5->digest;
print "IV is ".ascii_to_hex($iv)."\n";
 
# create cipher object
my $obj = Crypt::CBC->new (
    -key         => $key,
    -literal_key => 1,
    -header      => "none",
    -iv          => $iv,
    -cipher      => "Crypt::OpenSSL::AES",
    -keysize     => 16
);
 
my $encdata = MIME::Base64::decode_base64($ARGV[0]);
my $decdata = $obj->decrypt ($encdata);
my $len     = length ($decdata);
print "decrypted (len = $len): $decdata \n";
  


