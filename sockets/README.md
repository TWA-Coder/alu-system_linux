# C REST API - Sockets

This directory contains exercises covering the basics of network programming in C using sockets. The project gradually builds up to creating a simple HTTP REST API that manages a TO-DO list purely in RAM.

## Learning Objectives
* What is a socket and how it is represented on a Linux/UNIX system
* What are the different types of sockets
* What are the different socket domains
* How to create a socket
* How to bind a socket to an address/port
* How to listen and accept incoming traffic
* How to connect to a remote application
* What is the HTTP protocol
* How to create a simple HTTP server

## Requirements
* Allowed editors: `vi`, `vim`, `emacs`
* Compiled on Ubuntu 14.04 LTS using `gcc 4.8.4` with flags `-Wall -Werror -Wextra -pedantic`
* Strictly follows the Betty coding style
* No more than 5 functions per file
* Use of standard C library

## Exercises
* **0-server.c:** Opens an IPv4/TCP socket and listens to traffic on port 12345 indefinitely.
* **1-server.c:** Accepts a single incoming connection on port 12345, prints the client's IP address, and closes the connection.
* **2-client.c:** Connects to a listening server specified by host and port.
* **3-server.c:** Accepts a connection, receives a message from the client, prints it, and closes the connection.
* **todo_api_0.c:** Listens on port 8080, parses the first line of an incoming HTTP request (Method, Path, Version), and responds with a `200 OK`.
