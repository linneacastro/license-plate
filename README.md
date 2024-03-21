// MAIN PROGRAM //

/* PA2 A License Plate and Name Database with Hash Table Written in C */
// NAME: Linnea P. Castro
// DATE: 26 JAN 2023
// COURSE: CSE 222
// ASSIGNMENT: PA2
// SYNOPSIS:
/*
PA2 is a license plate and name database which uses a hash function to store driver information in a hash table.
The program uses the concept of chaining to handle collisions which occur when multiple driver information gets
hashed to the same location.  This collision is managed through a linked list that exists at each index value of
the hash table.  Initially, the hash table is completely empty, save for a sentinel node in each spot.  When
data is added to the hash table, it gets inserted in its hashed to index, after the sentinel node (at the
beginning of the list).  If another driver gets hashed to the same location (very likely if the hashsize is small),
then a new node is created for that driver, after the sentinel node, and alongside preexisting drivers.  

This program made use of much of the list functions code from PA1, but added on an extra layer with the hash table.
The list functions were the foundation for the hash functions.  The main program was streamlined by using these
encapsulated functions.  

The program takes between 2 and 3 command line arguments, one being a database file and the other, an optional
hashsize integer.  If a hashzise integer is given, the program will create a corresponding hash table with that 
many indices.  

In this program, the user had 4 options: enter a plate to search, *DUMP all contents of the hash table, *DUMP only 
the contents of a within-range cell, or *DIST to get an overall feel for the distribution of contents stored within
each index of the has table.  I used a while loop together with continue statements to keep the program flow, which
prompted the user for a plate to search for until the user ctrl D'd from the program.

Upon exiting the program, all memory associated with the linked list and hash table are freed.  More detailed 
information on functions can be found in hash.c and list.c files.  

Skills practiced include: implementing a hash function, dynamically allocating memory depending on size of string,
using gdb to debug, adjusting code from previous linked list assignment to fit new specifications of this project,
Using loops within a program to call a function multiple times, setting up a user menu, using continue statements,
storing command line arguments appropriately to store either an integer or pointer to a file.  
*/
