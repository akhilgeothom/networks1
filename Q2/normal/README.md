# README
## Computer Networks -1 
## Assignment-1
---------------------

####Done by,
#### Akhil George Thomas  -- CS17BTECH11047

-------------------------------------
A message from one client is forwarded to the other client.
Only two clients are allowed to connect to the server.

####Client:
2 threads are created: one to send messages to the server,while the other is used to receive any messages from the server.
The threads were created using simple fork().



####Server:
2 threads are created: one to handle messages from client1,while the other is used with client2.
The threads were created using simple fork().
The message is forwarded using send(). This is triggered when a message is received.


###How to run:
$ gcc client.c -o c

$ gcc server.c -o s

To start the server program:
$ ./s 8080

To start the client program:
$ ./c 127.0.0.1 8080

And again enter the same command from a different terminal:
$ ./c 127.0.0.1 8080


###References:

http://man7.org/linux/man-pages/man7/ip.7.html

http://man7.org/linux/man-pages/man7/ipv6.7.html

https://www.cs.cmu.edu/afs/cs/academic/class/15213-f16/www/lectures/21-netprog1.pptx

https://www.cs.cmu.edu/~dga/15-441/S08/lectures/03-socket.ppt

https://stackoverflow.com/questions/5956516/getaddrinfo-and-ipv6

https://gist.github.com/inaz2/0e77c276a834ad8e3131