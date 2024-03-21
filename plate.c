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

#include "plate.h"

void main(int argc, char **argv) 
{ 
  char temp[1000]; // Char array for user input w fgets to be passed to sscanf 
  char command[120]; // Variable to store command for menu interface and fget/sscanf
  char *databasename; // Variable to store database name, which is file we must open

  int hashsize;
  hashsize=50; // Default value, this might change depending on user command line args
  
  int celltodump; // Variable for use with *DUMP n

  char plate[300]; // Next 3 variable lines for sscanf (memory is dynamically allocated by list functions)
  char first[300]; 
  char last[300];

  hashEntry *table; // Declaring table as an array of hashEntries

// CHECKING COMMAND LINE ARGUMENTS AND READING IN DATABASE AND HASHSIZE (IF SPECIFIED)
  if (argc ==1){
    printf("Error: Did not speficy database to open\n");
    return;
  }

  if (argc == 2){ // 2 command line arguments given, latter is database name, assume default hashsize of 50 since no size specified
    databasename=argv[1]; // whatever is in the command line argument is the database we're going to open
    //printf("database name is: %s\n", databasename); // Print statement to double check datatabasename stored
  }

  if (argc == 3){ // 3 command line arguments given: main pgm name, custom hash table size, and database name
    int testinghashsize;
    //int hashsize;  
    int flag=sscanf(argv[1], "%d", &testinghashsize);
    if (flag == 1 && testinghashsize < 1){ // Above pattern was found but hashsize is less than 1
      printf("ERROR: hashsize must be at least 1\n"); // Print error message
      return;
    }
    if (flag == 1 && testinghashsize >= 1){
      hashsize=testinghashsize;
    }
    databasename=argv[2]; // Third item in command line gets saved as the databasename
    //printf("database name is: %s\n", databasename); // Print statement to test
    //printf("hash size is: %d\n", hashsize); // Print statement to test
  }

// CREATE HASH TABLE 
  table=hashInit(hashsize);

// READING IN DATABASE 
  FILE *fp;// Preparing to open file, declaring file pointer
  fp = fopen(databasename, "r"); // Opeing file stored as database
  if (fp == NULL){ // If file opens and returns NULL, print error message
    printf("Error: database cannot be read\n"); // Error message
    return;
  }
  while(NULL != fgets(temp,1000,fp)){ // While loop to read database line by line
    temp[strlen(temp)-1]='\0'; // Remove trailing newline
    sscanf(temp, "%s %s %s", plate, first, last);
    hashAdd(table, plate, first, last); // Calling hashAdd function to read lines in to get hashed and stored
    //printf("<%s>\n", plate); // Print statement to test
    //printf("<%s>\n", first); // Print statement to test
    //printf("<%s>\n", last); // Print statement to test
  }
  fclose(fp); // Close the database we opened, important to prevent memory leak


// USER MENU INTERFACE
    printf("Enter plate or command: ");
    while(NULL != fgets(command, 120, stdin)){ // Beginning of while loop... using this and continue statements instead of infinite loop
      command[strlen(command)-1]='\0'; // Remove newline from what fgets stored
     
       if(0==strcmp(command, "*DIST")){ // If command matches *DIST
        hashDist(table); // Call hashDist function with table as argument
        
        printf("Enter plate or command: ");
        continue; // Goes back to top of while loop
       }
     
       if(0==strcmp(command, "*DUMP")){ // If command matches *DUMP, we want to dump contents of ALL cells
         // Do this stuff for *DUMP
         for(int cellNum = 0 ; cellNum < hashsize ; cellNum++){ // For loop to print contents of all cells from 0 to hashsize-1
           printf("Contents of cell %d\n", cellNum);
           hashDump(table, cellNum); // Call hashDump inside loop, once for each value of cellNum
           printf("---------------------------\n");
         }
         printf("Enter plate or command: ");
         continue; // Back to top of while loop
       }

       if (1==sscanf(command, "*DUMP %d", &celltodump)){ // Pattern match was found, there is a cell to dump
         if(celltodump > hashsize-1){  // Very important to check user has inputted legal value.  User's input is not legit.
           printf("Error: cell not within range\n"); // Error message, input not legal
           printf("Enter plate or command: ");
           continue; // Back to top of while loop
         }
         if(celltodump < 0){  // User inputted negative number, not legal
           printf("Error: cell not within range\n");
           printf("Enter plate or command: ");
           continue; // Back to top of while loop
         } 
         printf("Contents of cell %d\n", celltodump); // Legal cell selected, hashDump function will be called to dump only the contents of that cell
         hashDump(table, celltodump);
         printf("---------------------------\n");
         printf("Enter plate or command: ");
         continue;
       }

       else{ // One of the above commands was NOT entered, search for plate
         char searchforthisplate[300]; 
         sscanf(command, "%s", searchforthisplate);
         int searchresult=hashFind(table, searchforthisplate, first, last); // Call hashFind function, which hashes user input and then searches corresp list
         //printf("searchresult is: %d (1 found, 0 not found)\n", searchresult); // Print statement to test
         if(searchresult==0){ // Plate was not found
           printf("Plate not found.\n");
           printf("Enter plate or command: ");
           continue; // Plate not found, go back to while loop
         }
         else{ // Plate match was found
           printf("First name: %s\n", first); // Print matching first and last name passed back 
           printf("Last name: %s\n", last); 
           printf("Enter plate or command: ");
           continue; // Go back to while loop
         } 
       }
    printf("Enter plate or command: ");
    }
  hashFree(table); // Call hashFree function, free memory in all linked lists and sentinel nodes stored at each index
  free(table); // Free memory from table itself

}

