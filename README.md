# socket-programming

This is a command line interface.
After we form a connection between server and client and then, 
we transfer the text file. The first response sent from the client to the server is the file name. Then, we check the presence of file and send the size of file if it is there. if there is no file then we send -1. 

We read the buffer for the integer sent. If, it is -1
then, there is no file. If, the file is there, we send the size and
we read the buffer till we get all the data in the text file. If, the file is present we rewrite it. Finally, When exit is enterred the socket is closed and the connection is closed.