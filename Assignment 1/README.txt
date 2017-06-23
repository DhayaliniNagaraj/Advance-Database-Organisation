					****************************************************************		 
											Storage Manager

 					****************************************************************

storage manager is a module that is capable of reading blocks from a file on disk into memory and writing blocks from memory to a file on disk. The storage manager deals with pages (blocks) of fixed size (PAGE_SIZE). In addition to reading and writing pages from a file, it provides methods for creating, opening, and closing files.

************** CONTENTS *****************
    * Source Files
    * MakeMe File
    * ReadMe

******* INSTRUCTIONS TO RUN **************

1)Instructions to run the code
------------------------------

a)For executing create,Open,close and destroy test cases(Default test cases):

1) In the terminal,navigate to the file directory.

2) Type: 
	make -f makefile1.mk

3) ./storagembr1


/*----------------------------------*/
Before Running the next test case clear all object files & executables using the following command 

make -f makefile1.mk clean	

/*-----------------------------------*/

b) For executing additional test cases:

1) In the terminal,navigate to the file directory.

2) Type: 
	make -f makefile2.mk

3) ./storagembr2

To clear all object files & Executables using the following command  

make -f makefile2.mk clean	



******* FUNCTIONS IMPLEMENTED ************

******************************************
   NAME : createPageFile
   ARGUMENTS : char *fileName
   PURPORSE : Creates a new page. If a file already exists,it will be considered as an empty file. Size of file created will be one page and it will be filled with NULL bytes. 
   RETURNS : upon successfull completeion  RC_OK else RC_FILE_NOT_FOUND

******************************************

	     
   NAME : openPageFile
   ARGUMENTS : char *fileName, SM_FileHandle *fHandle
   PURPORSE : Check if the requested page exists.If it exists check gor size and initiate the structure elements. If not throw ERROR. 
   RETURNS : upon successfull completeion  RC_OK else RC_FILE_NOT_FOUND

******************************************

   NAME : closePageFile
   ARGUMENTS : SM_FileHandle *fHandle	
   PURPORSE : Upon success close the file and return success message. If not return appropriate ERROR message. 
   RETURNS : upon successfull completeion  RC_OK else RC_FILE_HANDLE_NOT_INIT

******************************************

   NAME : destroyPageFile
   ARGUMENTS : char *fileName	
   PURPORSE : Remove the file and return success message upon completion. Else throw ERROR message. 
   RETURNS : upon successfull completeion  RC_OK else RC_FILE_DELETE_FAILED

******************************************

   NAME : getBlock
   ARGUMENTS : int position, SM_FileHandle *fHandle, SM_PageHandle memPage	
   PURPORSE : A static function to get nth block from the page provided the position(n),filehandle(fHandle) and Buffer (memPage) 
   RETURNS : upon successfull completeion  RC_OK else RC_READ_NON_EXISTING_PAGE

******************************************

   NAME : readBlock
   ARGUMENTS : int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage	
   PURPORSE : Using file descriptors check if requested page exists and move the file descriptors position to the page requested if exists,and load it to the buffer memPage.
   RETURNS : upon successfull completeion  RC_OK else RC_READ_NON_EXISTING_PAGE

******************************************

   NAME : getBlockPos
   ARGUMENTS : SM_FileHandle *fHandle	
   PURPORSE :  To get the current block number that file handle points to.
   RETURNS : If not completed successfully throw error message : RC_READ_NON_EXISTING_PAGE

******************************************

   NAME : readFirstBlock
   ARGUMENTS : SM_FileHandle *fHandle, SM_PageHandle memPage	
   PURPORSE :  Check if the requested page is valid/exists. If exists read the content of the previous page to buffer with the help of file descriptor in file handle.
   RETURNS : upon successfull completeion  RC_OK else RC_READ_NON_EXISTING_PAGE

******************************************

   NAME : readPreviousBlock
   ARGUMENTS : SM_FileHandle *fHandle, SM_PageHandle memPage	
   PURPORSE :  Check if the requested page is valid/exists. If exists read the content of the first page of the file to buffer with the help of file descriptor in file handle.
   RETURNS : upon successfull completeion  RC_OK else RC_READ_NON_EXISTING_PAGE

******************************************

   NAME : readCurrentBlock
   ARGUMENTS : SM_FileHandle *fHandle, SM_PageHandle memPage	
   PURPORSE :  Check if the requested page is valid/exists. If exists read the content of the current page to buffer with the help of file descriptor in file handle.
   RETURNS : upon successfull completeion  RC_OK else RC_READ_NON_EXISTING_PAGE

******************************************

   NAME : readNextBlock
   ARGUMENTS : SM_FileHandle *fHandle, SM_PageHandle memPage	
   PURPORSE :  Check if the requested page is valid/exists. If exists read the content of the next page to buffer with the help of file descriptor in file handle.
   RETURNS : upon successfull completeion  RC_OK else RC_READ_NON_EXISTING_PAGE

******************************************

   NAME : readLastBlock
   ARGUMENTS : SM_FileHandle *fHandle, SM_PageHandle memPage	
   PURPORSE :  Check if the requested page is valid/exists. If exists read the content of the last page of the file to buffer with the help of file descriptor in file handle.
   RETURNS : upon successfull completeion  RC_OK else RC_READ_NON_EXISTING_PAGE

******************************************

   NAME : writeBlock
   ARGUMENTS : int pageNum, SM_FileHandle *fHandle, SM_PageHandle pageMem	
   PURPORSE :  Check if the file exists and get the current position of the file if it exists. Write the contents to the file and close it.
   RETURNS : upon successfull completeion  RC_OK else RC_INVALID_POSITION,RC_WRITE_FAILED

******************************************

   NAME : writeCurrentBlock
   ARGUMENTS : SM_FileHandle *fHandle, SM_PageHandle memPage	
   PURPORSE :  Check if the file exists and write the current block based on absolute position.
   RETURNS : upon successfull completeion  RC_OK else RC_INVALID_POSITION,RC_WRITE_FAILED

******************************************

   NAME : appendEmptyBlock
   ARGUMENTS : SM_FileHandle *fHandle	
   PURPORSE :  Check if the file exists and get the total number of pages. Add an empty block to the end and fill it with NULL bytes.
   RETURNS : upon successfull completeion  RC_OK 

******************************************

   NAME : ensureCapacity
   ARGUMENTS : int numberOfPages, SM_FileHandle *fHandle	
   PURPORSE :  Check if the file exists and calculate the number of pages that the file could accomodate. If it is sufficient return appropriate message else calculate the memory needed and allocate the same.
   RETURNS : upon successfull completeion  RC_OK 

******************************************

   NAME : checkFileInit
   ARGUMENTS : SM_FileHandle *fHandle	
   PURPORSE :  This function checks if the file handle is initiated.
   RETURNS : upon successfull completeion  RC_OK else RC_FILE_HANDLE_NOT_INITIATED

******************************************

   NAME : pageNumValid
   ARGUMENTS : int pNum	
   PURPORSE :  Checks if the page number passes is valid.
   RETURNS : upon successfull completeion  RC_OK else RC_PAGENUM_INVALID

******************************************

****ADDITIONAL TEST CASES IMPLEMENTED*****

** writeCurrentBlock
** appendEmptyBlock
** readBlock
** readLastBlock
** readCurrentBlock
** ensureCapacity














