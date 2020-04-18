# README
## Computer Networks -1 
## Assignment-1
---------------------

####Done by,
#### Akhil George Thomas  -- CS17BTECH11047

-------------------------------------
A clientNum is used to identify each client uniquely, an unsigned integer from 0 to N.

####Client:
2 threads are created: one to send messages to the server,while the other is used to receive any messages from the server.
The threads were created using simple fork().
The destination's clientNum is encoded into the last character of the message to the server. 


####Server:
A maximum of N+1 threads run at a time. N threads handle recv() from each client while one thread handles any new client requests. 
A new POSIX thread is created when a new client request is made.
The last character of the received message are truncated, and the clientNum is found.


###How to run:
$ gcc client.c -o c

$ gcc server.c -o s -lpthread

To start the server program:
$ ./s 8080

To start the client program:
$ ./c 127.0.0.1 8080   #This might be executed upto N times in parallel.

###References:

http://man7.org/linux/man-pages/man7/ip.7.html

http://man7.org/linux/man-pages/man7/ipv6.7.html

https://www.cs.cmu.edu/afs/cs/academic/class/15213-f16/www/lectures/21-netprog1.pptx

https://www.cs.cmu.edu/~dga/15-441/S08/lectures/03-socket.ppt

https://stackoverflow.com/questions/5956516/getaddrinfo-and-ipv6

https://gist.github.com/inaz2/0e77c276a834ad8e3131