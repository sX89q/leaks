import urllib2
import sys
 
#Simple Class Color
BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE = range(8)
 
def has_colours(stream):
    if not hasattr(stream, "isatty"):
        return False
    if not stream.isatty():
        return False
    try:
        import curses
        curses.setupterm()
        return curses.tigetnum("colors") > 2
    except:
       
        return False
has_colours = has_colours(sys.stdout)
 
def printout(text, colour=WHITE):
        if has_colours:
                seq = "\x1b[1;%dm" % (30+colour) + text + "\x1b[0m"
                sys.stdout.write(seq)
        else:
                sys.stdout.write(text)
               
# Resolver:
printout("*\-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\*\r\n"  , RED)
printout("Python ~ Skype Resolver By Phobia\r\n"  , CYAN)
printout(" Skype: 	classified                            \r\n"  , YELLOW)
printout("*\-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\*\r\n" , RED)
 
 
SKYPEAPI = "https://www.hackthatapi.com/?command=7&username="
printout("> Skype Username: " , BLUE)
SKYPEUSERNAME = raw_input()
SKYPEAPI = SKYPEAPI + SKYPEUSERNAME
webFile = urllib2.urlopen(SKYPEAPI).read()
 
#if webFile == ("Invalid Key"):
#  printout("Error", RED)
#else:
 
printout(webFile, GREEN)
print('\r\n')