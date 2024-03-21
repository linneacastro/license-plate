// LINKED LIST FUNCTIONS //

/* PA2 A License Plate and Name Database with Hash Table Written in C */
// NAME: Linnea P. Castro
// DATE: 26 JAN 2023
// COURSE: CSE 222
// ASSIGNMENT: PA2
// SYNOPSIS:
/*
All list functions are stored within this list.c file.  List functions include: listInit, listAdd, listFind,
listLen, listPrint, and listFree.  Generally, the list functions are the foundation which the hash functions
draw from.  listInit creates an empty linked list with only a sentinel node for each hash index in the 
hash array.  listAdd creates a new node in the linked list (after sentinel node) and stores information
drawn from the database into plate, first, and last name fields within the node. listFind is the foundation
of default condition of our program, which asks the user to enter a plate.  listLen helps build the *DIST
command which counts the number of nodes within each index of the hash table.  listPrint services the *DUMP
command, and listFree works in tandem with hashFree to free all memory associated with the creation of 
hash table and linked list.  More detailed information below. 
*/

#include "plate.h"

// LINKED LIST INIT FUNCTION //
/*
The list init function creates an empty list with just one sentinel node for each index inside the hashtable.
The sentinel node is empty, with its next field holding NULL.  The next pointer is only updated if a plate
ends up being stored inside that associated index.
*/
struct node *listInit() // Creates an empty list with just a sentinel node
{
  struct node *sent; // Pointer to sentinel node
  sent=malloc(sizeof(struct node)); // Allocating memory for sentinel node, big enough to hold struct node (variable type)
  //plate=malloc(sizeof(struct node)); // Unsuccessful attempt to get rid of "conditional jump" comments from valgrind, scrapping for now 
  //first=malloc(sizeof(struct node)); 
  //last=malloc(sizeof(struct node));  
  sent->next=NULL; // Sentinel node points to NULL meaning it is first and only node in our list at the moment
  return(sent); // Returns a pointer to empty list
}


// LINKED LIST ADD FUNCTION //
/*
The list add function adds a new node to the beginning of the list, right after the sentinel node.  The list add
function is called when a plate gets hashed to an index and needs to be stored.  The new node created is 
different from the sentinel node in that it has a plate, first, and last pointers.  Memory is dynamically allocated
to hold the specific string it is passed.  
*/
void listAdd(struct node *sent, char *plate, char *first, char *last)
{
  struct node *new=malloc(sizeof(struct node)); // Adding a new node and mallocing memory for it
  
  new->plate=malloc((strlen(plate)+1)*sizeof(char)); // Need to be able to store the null character, hence +1
  strcpy(new->plate, plate); // Use strcpy to add plate to new->plate, new plate node just created
  
  new->first=malloc((strlen(first)+1)*sizeof(char)); // Mallocing space for first name in new node
  strcpy(new->first, first); // Strcpying firstname into new node

  new->last=malloc((strlen(last)+1)*sizeof(char)); // Mallocing space for last name in new node
  strcpy(new->last, last); // Strcpying lastname into new node

  new->next=sent->next; // new->next is pointing to what sent->next used to point to before new node was added
  sent->next=new; // Sentinel now points to new node
}


// LINKED LIST FIND FUCTION //
/*
This function is used by the hashFind function when the user wants to find a plate owner's name inside the database.
This function searches for a given plate within the linked list that the user has entered on the command line.  
If a match is found, associated first and last names are distributed into first and last and the function returns a 1.  
If no match found, the function returns a 0.
*/
int listFind(struct node *sent, char *plate, char *first, char *last)
{
  struct node *temp; // Creating a pointer called temp to help us traverse list
  temp=sent->next;
  while(temp != NULL){ // As long as there are nodes to search, do the following
    if(0 == strcmp(plate, temp->plate)){ // Match found
      strcpy(first, temp->first); // Distribute temp->first to first
      strcpy(last, temp->last); // Distribute temp->last to last
      return(1); // Match found and dist complete, function returns 1
    }
    temp=temp->next; // Set temp to be the next thing temp points to to "increment" the search and keep looking
  }
  return(0); // All nodes searched and no matches found, return 0
}


// LINKED LIST LEN FUNCTION //
/*
The list len function is used together with the hashDist function to report the number of plate/user containing nodes
within a given hash table index.  This function counts the number of nodes (nodes with data, not sentinel nodes), 
and returns this as the function value.  The hashDist function contains a loop which increments the hash table index,
so a new tally is reported for each index value.
*/
int listLen(struct node *sent)
{
  struct node *temp; // Creating a pointer called temp to help us traverse list
  int len = 0; // Initializing len variable to 0
  temp=sent->next; // temp is what the sent->next points to
  
  while(temp != NULL){ // As long as sent->next isn't null, there is more list to traverse
    len++; // Increment len
    temp=temp->next; // Must increment temp to look at next node
  }
  return(len); // Return number of nodes (len) to hashDist function
}


// LINKED LIST PRINT FUNCTION //
/*
This function prints the contents of the list for the hashDump function.  It takes in a pointer to the sentinel node
as its argument and uses a temp pointer to traverse the list. Formatting for the way the nodes are printed
are handled inside this function rather than in the main program.
*/
void listPrint(struct node *sent)
{
  struct node *temp; // Creating a pointer called temp to help us traverse the list
  temp=sent; // This is the sentinel node, we don't want to print it
  temp=temp->next; // This is the real first node in our list, the one right after sentinel node

  while (temp != NULL){ // Until we reach the very last node with NULL stored in it, do this:
    printf("Plate: <%s> ", temp->plate); // Print plate
    printf("Name: %s, %s\n", temp->last, temp->first); // Print last name, first name
    temp=temp->next; // Traverse to next node in the list by making temp the thing that temp->next points to
  }
  return; // Print function returns nothing to main function
}

// LINKED LIST FREE MEMORY FUNCTION //
/*
This function releases all memory associated with the list.  This was a function is essential to exiting out of the 
program with no memory leaks.  It establishes two pointers for traversing the list, previous, and current.
An important thing to note is that all nodes and all of their data nodes must be freed (plate, first, and last),
so it is essential to use previous->plate for example, because just freeing previous alone will not do the job!
*/
void listFree(struct node *sent)
{
  struct node *current; // Initialize current pointer
  struct node *previous; // Initialize previous pointer

  current=sent->next; // First node to check is the one after sentinel node, current node
  previous=sent; // Node before the one we are checking, the sentinel node

  while(1){ // Infinite loop
    if(current != NULL){
      free(previous->plate); // Free memory of previous node, first one freed will be sentinel
      free(previous->first); // Free memory of previous node, first one freed will be sentinel
      free(previous->last); // Free memory of previous node, first one freed will be sentinel
      free(previous); // Free memory of previous node, first one freed will be sentinel
      previous=current; // Update pointers so previous becomes current
      current=current->next; // Update pointers so new current becomes the thing old current was pointing to
    }
    else{
      break; // Otherwise if current is null and we are on the last node, break out of this loop and...
    }
  } 
  free(previous->plate); // Free previous memory allocation to release memory of last/only node
  free(previous->first); // Free previous memory allocation to release memory of last/only node
  free(previous->last); // Free previous memory allocation to release memory of last/only node
  free(previous); // Free previous memory allocation to release memory of last/only node MUST DO THIS ONE LAST!
}

