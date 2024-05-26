import socket
import sys
import json

def print_args():
    sys.stdout.write("Arguments: [IP Address of CompetFFCK notification server] [port]\n")

def process_data(data):
    if (data[0]!=0):
        #print(data)
        try:
            obj = json.loads(data)    
            if (obj['key']!='<on_course>'):
                print(obj)
        except json.decoder.JSONDecodeError as e:
            print(e.msg)
            print(e.pos)
            print(data)

# ##################################################################

if (len(sys.argv)<3) :
    print_args()
    sys.exit(1)

competIP = sys.argv[1] 
competPort = sys.argv[2] 

sys.stdout.write("Trying to connect to competFFCK at "+competIP+":"+competPort+"\n")
sys.stdout.flush()        

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((competIP, int(competPort)))

sys.stdout.write("Connected to competFFCK at "+competIP+":"+competPort+"\n")
sys.stdout.flush()        

while True:
    data = s.recv(1000)
    if (len(data)==0):
        print("It looks like competFFCK has quit")
        sys.exit(0)
    process_data(data)
