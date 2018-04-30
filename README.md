# CITS3002


The web-browser, Testing-Server, and Question-Server, all execute on different computers (hardware) and, thus, must communicate using network protocols.

Browser -> Testing Server :  HTTP(S) and HTML

Testing-Server -> Question-Server : Undefined

Browser
-
Login screen

User sends some kind of authentication data with their messages.

Testing Server
-
Login, using text-based name and password.

**Secure** communication with the browser.

Responsible for sending HTML files to the browser securely.

Communicates with the Question Server. (Needs to be fairly secure)

Stores Student states (which questions they have completed/ marks / etc)?


Question server
-
Accepts answer/question number from testing server and sends correct/incorrect

Accepts question request from testing server. responds with Question

----------------------------------------------------------------

API for question server needs to be well defined and clear.

Clear API for the testing server too!

-----------------------------------------------------------------


