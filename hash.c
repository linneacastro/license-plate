// HASH FUNCTIONS (prototypes inside plate.h) //

/* PA2 A License Plate and Name Database with Hash Table Written in C */
// NAME: Linnea P. Castro
// DATE: 26 JAN 2023
// COURSE: CSE 222
// ASSIGNMENT: PA2
// SYNOPSIS:
/*
This hash.c file contains all the function bodies for the plate.c program.  These functions are generaly quite concise because
they make use of the functions in list.c to carry out their operations.  Hash functions include: hashInit, the actual hash 
function, hashAdd, hashFind, hashDist, hashDump, and hashFree. More detailed information on each function can be found below. 
*/

#include "plate.h"

// GLOBAL VARIABLE //
int hashsize; // Global variable established for use in hashInit function


// HASH FUNCTION //
/*
This hash function is used on each plate within the database.  A sum variable is created and then each character within the
plate (its ascii value) is put through the function and incremented to create a final sum for that plate.  This final sum
determines where in the hash table the plate and its driver information will be stored.  This function returns that sum to 
the main program as an int.
*/
int hash(char *plate)
{
  int sum; // Variable to keep track of running sum
  sum = 0; // Initialize sum to 0
  for (int i = 0 ; i <= strlen(plate)-1 ; i++){ // Want to loop through for every character in plate-1
    sum=sum+((2*i+3)*(2*i+3)*(plate[i])); // Run each character in plate through hash function and add to sum
//printf("char=<%c>,sum=%d\n",plate[i],sum);
  }
  sum = sum%hashsize; // Each char in plate has been accounted for, take modulo hashsize
  return(sum); // Function then returns sum
}


// HASH INIT FUNCTION //
/*
This function constructs the initial hash table.  When it is built, the hash table is empty.  The number of indices of the 
hashtable will depend on the command line argument the user specified in the main program or 50 by default.  One important thing
to note here is that the size of the hashtable passed in from the main function is stored as "hashsize".  This function is 
important because it calls listInit which allows for an empty sentinel node to be created for each index value in the hash
table.    
*/
hashEntry *hashInit(int size) // Global variable, hashsize declared at the top of the hash.c file
{
  hashsize=size; // Passes hashsize in as size from outside function, and then we assign it as hashsize to use inside function
  hashEntry* hashTable; // This declares the hash table as an array of hashEntries
  hashTable=(malloc(sizeof(hashEntry) * hashsize)); // Must free this memory at the end
  for (int i = 0 ; i <= hashsize-1 ; i++){ // Making a new list with sentinel node to hashsize-1 (bc starting at 0)
    hashTable[i]=listInit(); // Makes an empty list (calling listInit) with just a sentinel node for each i
  }
  return(hashTable);
}


// HASH ADD FUNCTION //
/*
This function makes a new entry in the hash table for this specific plate, first name, and last name combination.
Data is added to the beginning of the list, right after the sentinel node.  This is where the plate read from the
database actually gets put through the hash function and assigned an index.  With the plate assigned an index,
listAdd now knows exactly what list to add the plate and name information to.   
*/
void hashAdd(hashEntry *hashTable, char *plate, char *first, char *last)
{ 
  int index; // variable to store result of hash function
  index = hash(plate); // We are hashing the PLATE and putting this through the hash function
  listAdd(hashTable[index], plate, first, last); // Use listAdd function to add this plate/name combination to linked list in hashed position
}


// HASH FIND FUNCTION //
/*
This function searches for the given plate inside the hash table.  If it is found, the matching first and last name is loaded 
into first and last and the function returns 1.  If no match is found, the function returns 0.  This function runs
using listFind as its foundation.  The plate the user wants to search for is hashed, so the program knows exactly where the 
program would be stored if indeed it does exist.  Listfind then searches the chain of lists in that specific index to see if the 
plate is there.  It passes back a 0 or a 1 to this function, to give the user feedback on who the plate belongs to if it exists,
or to let the user know the plate was not found.
*/
int hashFind(hashEntry *hashTable, char *plate, char *first, char *last){
  int index = hash(plate); // Run the plate through hash function to find its index in the hashEntry array
  int result = listFind(hashTable[index], plate, first, last); // Use listFind to pass 1/0 to result variable
  return(result); // Return 0 or 1 to main function, as passed by listFind
}


// HASH DIST FUNCTION //
/*
This function prints the number of plates stored inside each hash cell.  It is like the print function but with a counter.  When the user gives the command
*DIST, the main program will display the number of plates stored in each cell of the hash table.  This is the length of the
corresponding linked list stored at that hash table index value (not including the sentinel node).  This program works with help from the
listLen function which counts the chain of lists stored in that index and feeds it back to the hashDist function as distance.  A new distance value
will be returned for each index in the hash table.  If there is an empty index with only a sentinel node, the length will be default, 0. 
*/
void hashDist(hashEntry *hashTable){
  for(int i = 0 ; i < hashsize ; i++){ // for loop to increment through every index in hash table 
    int distance; // variable to track distance
    distance = 0; // default to 0, will only change if list has more than just sentinel node
    distance=listLen(hashTable[i]); // assign listLen return value to distance variable
    printf("Entry %d: length=%d\n", i, distance); // print index and distance
  }
  return; // Function has no return value
}

// HASH DUMP FUNCTION //
/*
This prints the list contents  of cell, cellNum.  The hash dump function is called by the user with the command *DUMP or *DUMP n, where n is an integer
corresponding to the cell number.  DUMP* will list contents of every single cell in the hash table, whereas if an integer is specified it will 
only give information for that one cell. HashDump uses the listPrint function which has formatted the node plate/first/last data within print statements.
The number of cells printed depends on the command given by user, and for multiple cells, is controlled by a for loop which increments from 0 to the 
integer provided to give cell contents. 
*/
void hashDump(hashEntry *hashTable, int cellNum){
  listPrint(hashTable[cellNum]); // call listPrint function on the hash table at the cellNum index
}

// HASH FREE FUNCTION //
/*
This function frees all memory associated with the creation of the hashTable and linked list, and it uses listFree to accomplish this.  It travels from
index 0 to index hashsize-1, freeing memory with the linked list stored at that location.  
*/
void hashFree(hashEntry *hashTable){ // Takes argument hashtable array
  for(int i = 0 ; i <= hashsize-1 ; i++){ // For loop to free all memory for all index values corresponding to hashsize-1
    listFree(hashTable[i]); // Use listFree function to free hashTable memory
  }
}
