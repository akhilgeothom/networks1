# README
## Computer Networks -1 
## Assignment-1
---------------------

####Done by,
#### Akhil George Thomas  -- CS17BTECH11047

-------------------------------------
The following two features were added to the programs:
####1. Ping
Calculates the ping (the delay in milli sec) in the communication. This is an important metric to test the reachability of a node in a network.

####2. Parity
Implemented a parity character, like a parity bit, similar to a checksum available in portocols like HTTPv4.
It checks if the message were transmitted correctly.


###How to run:
$ gcc client.c -o c

$ gcc server.c -o s

To start the server program:
$ ./s 8080

To start the client program:
$ ./c 127.0.0.1 8080 Hello

###References:

http://man7.org/linux/man-pages/man7/ip.7.html

http://man7.org/linux/man-pages/man7/ipv6.7.html

https://www.cs.cmu.edu/afs/cs/academic/class/15213-f16/www/lectures/21-netprog1.pptx

https://www.cs.cmu.edu/~dga/15-441/S08/lectures/03-socket.ppt

https://stackoverflow.com/questions/5956516/getaddrinfo-and-ipv6

https://gist.github.com/inaz2/0e77c276a834ad8e3131