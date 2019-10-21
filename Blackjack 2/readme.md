# Blackjack Sockets

Blackjack game using sockets developed in C.

## Getting Started

Download the following files:
```
client.c
server.c
sockets.c
sockets.h
Makefile
```
### Prerequisites

You should have a valid gcc compiler installed in your computer

### Installing

A step by step series of examples that tell you how to get the casino working

1- Place all files in the same folder

2- Open the command line and navigate to the folder

3- Run the following command inside the folder
```
make
```
4- If everything is successful, no warnings or errors should appear and .o files should have been created

5- In the same window run the following command
```
./server 9999
```
6- Open another terminal window run the following command
```
./client localhost 9999
```
7- You can start the game

8- Do all the plays in the client terminal

###Notes
Write a numerical number for the starting money and bet. 
The bet should be less of your money. When asking you for confirmation write
"yes". Then you need to select if you hit, to get a new card or if you stand to see if the dealer gets over 21.
If the number of your hand diminishes is because you went over 21 and 
had an Ace so it became a 1 instead of 11.

## Authors

* **Me - Alejandro Gutierrez**

## Acknowledgments

* Thanks Gil for the sockets code
