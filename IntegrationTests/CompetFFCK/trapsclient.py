import socket
import sys

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

def prompt():
    sys.stdout.write("> ")
    sys.stdout.flush()

def print_args():
    sys.stdout.write("Arguments: [IP Address of CompetFFCK with TRAPS server] [port]\n")

def print_help():
    sys.stdout.write("Command list:\n\n")
    sys.stdout.write("quit              : Quit this terminal\n")
    sys.stdout.write("penalty 2 3 50    : Set penalty 50 at gate 3 for bib 2\n")
    sys.stdout.write("penalty 2 10 50 2 : Set penalty 50 at gate 10 for bib 2 boat 2\n")
    sys.stdout.write("chrono 5 32500    : Set chrono to 32500 milliseconds for bib 5\n")
    sys.stdout.write("\n")
    sys.stdout.flush()


# ##################################################################

if (len(sys.argv)<3) :
    print_args()
    sys.exit(1)

competIP = sys.argv[1] 
competPort = sys.argv[2] 

sys.stdout.write("Trying to connect to competFFCK at "+competIP+":"+competPort+"\n")
sys.stdout.flush()        

s.connect((competIP, int(competPort)))

sys.stdout.write("Connected to competFFCK at "+competIP+":"+competPort+"\n")
sys.stdout.flush()        
    
#keep reading stdin
while True:

    prompt()
    commandline = input()

    if commandline == "quit":
        s.close()
        sys.exit(0)
            
    # split with spaces
    cmdItemList = commandline.split(' ') 

    #penalties
    if len(cmdItemList)>3 and cmdItemList[0]=="penalty":
        penalty = cmdItemList[3]
        gate = cmdItemList[2]
        bib = cmdItemList[1]
        boat = 1
        if (len(cmdItemList)>4): 
            boat = cmdItemList[4] 
        sys.stdout.write("Setting penalty "+penalty+" at gate "+gate+" for bib "+bib+" boat "+boat+"\n")
        sys.stdout.flush()    
        message = 'penalty '+bib+' '+gate+' '+boat+' '+penalty+'\r'    
        s.send(message.encode())
        continue

    #chrono
    if len(cmdItemList)==3 and cmdItemList[0]=="chrono":
        bib = cmdItemList[1]
        chrono = cmdItemList[2]
        message = 'chrono '+bib+' '+chrono+'\r'    
        s.send(message.encode())
        continue

    print_help()
    



    
