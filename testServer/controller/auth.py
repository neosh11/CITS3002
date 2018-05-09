from hashlib import blake2b
import os
import sys
import re
import pickle


SECRET_KEY_TOKEN = b'Lalanoidwe3094yf2Nioedhoied3fc4093fasfkmakfuf209u32z'
SECRET_KEY_PASS = b'CAcas@@3jieowiofewhf8943hfn3cin'
AUTH_SIZE = 64
KEY = blake2b(digest_size=AUTH_SIZE, key=SECRET_KEY_TOKEN)
PASSKEY = blake2b(digest_size=AUTH_SIZE, key=SECRET_KEY_PASS)

# Initialize secrets
KEY.update(b"fdafvgrrewv3w")
PASSKEY.update(b"wrecivcrewvwre342")
def hashit(x):
    cpy = KEY.copy()
    cpy.update(bytes(x,'utf-8'))
    return cpy.hexdigest()

def hashpass(x):
    cpy = PASSKEY.copy()
    cpy.update(bytes(x,'utf-8'))
    return cpy.hexdigest()

class UserDetails:
    def __init__(self, name, password):
        self.name = name
        self.password = hashpass(password)
        self.token = hashit(name)

    def updateToken(self):
        self.token = hashit(self.name)

users = []
# Initialize all users
def init_users_map(fileName):
    x = os.path.dirname(os.path.realpath(sys.argv[0]))+"\\"
    with open(x+fileName, 'rb') as f:
        content = f.readlines()
    # you may also want to remove whitespace characters like `\n` at the end of each line
    content = [x.strip() for x in content]
    for info in content:
        m = re.match(r"(\w+) (.+)", info.decode())
        users.append(UserDetails(m.group(1), m.group(2)))

    # Create a user map
    usernameMap = {}
    for u in users:
        usernameMap[u.name] = u
    return usernameMap



def dumpMap(map, fileName):
    x = os.path.dirname(os.path.realpath(sys.argv[0])) +'\\'
    with open(x+fileName, 'wb') as f:
        pickle.dump(map, f, pickle.HIGHEST_PROTOCOL)

def loadMapFromDump(fileName):
    loadma = {}
    x = os.path.dirname(os.path.realpath(sys.argv[0]))+'\\'
    with open(x+fileName, 'rb') as f:
        content = f.read()
        loadma = pickle.loads(content)
    return loadma

def login(usernameMap, username, password):
    if(username == None or password == None or usernameMap == None):
        return None
    if(usernameMap[username].password == hashpass(password)):
        return usernameMap[username].token
    else:
        return None

def verify(usernameMap, username, token):
    if(usernameMap[username].token == token):
        return True
    else:
        return False

# map = init_users_map("./../resources/user")
# dumpMap(map, './../resources/user_dump')

# testuse = "Tim"
# x = login(map, testuse, "Jimmy-boy")
# print(verify(map, "Tim", x))