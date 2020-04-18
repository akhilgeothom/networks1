# README
## Computer Networks -1 
## Assignment-1
---------------------

####Done by,
#### Akhil George Thomas  -- CS17BTECH11047

-------------------------------------
The socket is opened using IPv6 TCP protocol. AF_INET6 is used as argument while creating the socket. The data structure used to store the address is sockaddr_in6 instead of sockaddr_in. This supports additional data members like sin6_flowinfo and sin6_scope_id.

####Client:
All the rules to start a IPv6 connection socket are similar to that of a IPv4 socket. 


####Server:
All the rules to start a IPv6 connection socket are similar to that of a IPv4 socket. 

How to run:
$ gcc client.c -o c

$ gcc server.c -o s

To start the server program:
$ ./s 8080

To start the client program:
$ ./c 8080 Hello


###References:

http://man7.org/linux/man-pages/man7/ip.7.html

http://man7.org/linux/man-pages/man7/ipv6.7.html

https://www.cs.cmu.edu/afs/cs/academic/class/15213-f16/www/lectures/21-netprog1.pptx

https://www.cs.cmu.edu/~dga/15-441/S08/lectures/03-socket.ppt

https://stackoverflow.com/questions/5956516/getaddrinfo-and-ipv6

https://gist.github.com/inaz2/0e77c276a834ad8e3131