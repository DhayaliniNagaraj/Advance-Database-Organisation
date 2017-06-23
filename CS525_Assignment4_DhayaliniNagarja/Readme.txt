****************************************************************	
					B+-Tree
 ****************************************************************
Implementing a B+-tree index, the index is backed up by a page file and pages of the index are through  buffer manager.B+-tree stores pointer to records (the RID i) index by a keys of a given datatype. Pointers to intermediate nodes are represented by the page number of the page the node it is stored in.

Implementation of B+-tree follows these conventions  

Leaf Split: In case a leaf node need to be split during insertion and n is even, the left node should get the extra key.

Non-Leaf Split: In case a non-leaf node needs to be split and n is odd, we cannot split the node evenly (one of the new nodes will have one more key). In this case the "middle" value inserted into the parent should be taken from the right node. 

Leaf Underflow: In case of a leaf underflow your implementation should first try to redistribute values from a sibling and only if this fails merge the node with one of its siblings.

*****************************************
			Team Members
	
1.Vijay Bharrathi
CWID:20356386

2.Praveen Kumar Selavn
CWID:20351349

3.Dhayalini Nagaraj
CWID:20359686

4.Smriti Raj
CWID: 20364719

************** CONTENTS *****************
    * Source Files
    * MakeMe File
    * ReadMe

******* INSTRUCTIONS TO RUN **************

1)Instructions to run the code
------------------------------

a)For executing Record Manager

1) In the terminal,navigate to the file directory.

2) Type: 
	make -f makefile.mk

3) ./btree_mgr




******* FUNCTIONS IMPLEMENTED ************

Index Manager Functions:

These functions are used to initialize the index manager 
and shut it down, freeing up all acquired resources.
	
	
	******************************************
	NAME : 		initIndexManager 
	ARGUMENTS : void *mgmtData
	PURPORSE :  Initialize index manager
	RETURNS : 	returns RC_OK if successfull
				
	******************************************			

	NAME : 		shutdownIndexManager 
	PURPORSE :  Shutdowns index Manager
	RETURNS : 	returns RC_OK if successfull
	
******************************************	

B+-tree Functions
These functions are used to create or delete a b-tree index

	NAME : 		createBtree
	ARGUMENTS :	char *idxId, DataType keyType, int n
	PURPORSE :  Create a new b-tree
	RETURNS : 	returns RC_OK if successfull
	
	******************************************
	
	NAME : 		openBtree 
	ARGUMENTS : BTreeHandle **tree, char *idxId
	PURPORSE :  open a b-tree
	RETURNS : 	returns RC_OK if successfull
	
	******************************************

	NAME : 		closeBtree 
	ARGUMENTS : BTreeHandle *tree
	PURPORSE :  Close a b-tree, also ensure that all new or modified
				pages of the index are flushed back to disk 
	RETURNS : 	returns RC_OK if successfull
				
	
	******************************************
	NAME : 		deleteBtree 
	ARGUMENTS : char *idxId
	PURPORSE :   Delete a b-tree
	RETURNS : 	returns RC_OK if successfull
				
**************************************************************************
**************************************************************************
	
	Access information about a b-tree
	
	
	NAME : 		getNumNodes 
	ARGUMENTS : BTreeHandle *tree, int *result)
	PURPORSE :  Gets number of nodes in b-tree
	RETURNS : 	returns number of nodes in b-tree
	
	******************************************
	
	NAME : 		getNumEntries 
	ARGUMENTS : BTreeHandle *tree, int *result)
	PURPORSE :  Gets number of Entries in b-tree
	RETURNS : 	returns number of entries in b-tree

	******************************************
	
	NAME : 		getKeyType 
	ARGUMENTS : BTreeHandle *tree, DataType *result
	PURPORSE :  gets Key type 
	RETURNS : 	returns key type

	
******************************************
******************************************
	
Key Functions:
These functions are used to find, insert, and delete keys
in/from a given B+-tree

	NAME : 		findKey 
	ARGUMENTS : BTreeHandle *tree, Value *key, RID *result
	PURPORSE :	return the RID for the entry with the search 
				key in the b-tree.if the new key violates primary key constraint.
	RETURNS : 	return the RID

	******************************************

	NAME : 		insertKey 
	ARGUMENTS : BTreeHandle *tree, Value *key, RID rid
	PURPORSE : insertKey inserts a new key and record pointer 
				pair into the index
	RETURNS : 	returns RC_OK if successfull
				return RC_IM_KEY_ALREADY_EXISTS if key is already stored 
	******************************************

	NAME : 		deleteKey 
	ARGUMENTS : BTreeHandle *tree, Value *key
	PURPORSE :  removes a key (and corresponding record pointer) from the index
	RETURNS : 	returns RC_OK if successfull
				return RC_IM_KEY_NOT_FOUND  if the key is not in the index.
	
	******************************************

	NAME : 		openTreeScan 
	ARGUMENTS : BTreeHandle *tree, BT_ScanHandle **handle
	PURPORSE : 	Start a Tree Scan
	RETURNS : 	returns RC_OK if successfull

	******************************************
	NAME : 		nextEntry 
	ARGUMENTS : BT_ScanHandle *handle, RID *result
	PURPORSE : 	Scan through next entry in BTree
	RETURNS : 	returns the next entry in b-tree
				return RC_IM_NO_MORE_ENTRIES if there are no more entries to be returned
	******************************************
	NAME : 		closeTreeScan 
	ARGUMENTS : BT_ScanHandle *handle
	PURPORSE : 	End b-tree Scan
	RETURNS : 	returns Offsetattributes.
			returns following error codes if checks fails 
			-RC_INVALID_HANDLE	
	******************************************

	NAME : 		printTree 
	ARGUMENTS : BTreeHandle *tree
	PURPORSE : 	used to create a string representation of a b-tree
	RETURNS : 	returns elements of b-tree as String
*************************************************************************
*************************************************************************
	
Additional Error Codes used:

#define RC_ERROR 5
#define RC_ALGORITHM_NOT_IMPLEMENTED 6
#define RC_PINNED_PAGES_STILL_IN_BUFFER 7
#define RC_INVALID_DATATYPE 8
#define RC_FILE_DELETE_FAILED 9
#define RC_FILE_HANDLE_NOT_INITIATED 10
#define RC_PAGENUM_INVALID 11
#define RC_FILE_CANNOT_BE_CLOSED 12
#define RC_INVALID_PAGE_FILE_NAME 13


#define RC_FILE_CANNOT_BE_CREATED 304
#define RC_BM_BUFFERPOOL_INIT_FAILED 305
#define RC_BUFFER_PROB 308
#define RC_PIN_PAGE_FAILED 306
#define RC_INVALID_HANDLE 307





