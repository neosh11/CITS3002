from hashlib import blake2b
import os
import sys
import re
import pickle
import random
import controller.questionServe

SECRET_KEY_TOKEN = b'Lalanoidwe3094yf2Nioedhoied3fc4093fasfkmakfuf209u32z'
SECRET_KEY_PASS = b'CAcas@@3jieowiofewhf8943hfn3cin'
AUTH_SIZE = 64
KEY = blake2b(digest_size=AUTH_SIZE, key=SECRET_KEY_TOKEN)
PASSKEY = blake2b(digest_size=AUTH_SIZE, key=SECRET_KEY_PASS)
MAX_Q = int(controller.questionServe.sizeQuestion())
CHOOSE_Q = 5

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
        self.currentQuestion = 0
        #question set is made when user set init

    def updateToken(self):
        self.token = hashit(self.name)

    def makeQuestionSet(self):
        s = random.sample(range(1, MAX_Q), CHOOSE_Q)
        map_q = []
        for x in s:
            map_q.append({"qnum":x, "ans":None, "tries": 0, "correct": False})
        self.questionSet = map_q
    def setCurrentQuestion(self, num):
        if(num >=0 and num < CHOOSE_Q):
            self.currentQuestion = num

users = []
# Initialize all users
def init_users_map(fileName):
    x = os.path.join(os.path.dirname(os.path.realpath(sys.argv[0])), fileName)
    with open(x, 'rb') as f:
        content = f.readlines()
    # you may also want to remove whitespace characters like `\n` at the end of each line
    content = [x.strip() for x in content]
    for info in content:
        m = re.match(r"(\w+) (.+)", info.decode())
        users.append(UserDetails(m.group(1), m.group(2)))

    # Create a user map
    usernameMap = {}
    for u in users:
        u.makeQuestionSet()
        usernameMap[u.name] = u
    return usernameMap



def dumpMap(map, fileName):
    x = os.path.join(os.path.dirname(os.path.realpath(sys.argv[0])), fileName)
    with open(x, 'wb') as f:
        pickle.dump(map, f, pickle.HIGHEST_PROTOCOL)

def loadMapFromDump(fileName):
    loadma = {}
    x = os.path.join(os.path.dirname(os.path.realpath(sys.argv[0])), fileName)
    with open(x, 'rb') as f:
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

# testuse = "admin"
# x = login(map, testuse, "admin")
# print(verify(map, "admin", x))


# print(map['admin'].questionSet)
