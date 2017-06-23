				****************************************************************		 
							Buffer Manager

 				****************************************************************

The buffer manager manages a fixed number of pages in memory that represent pages from a page file managed by the storage manager implemented in assignment 1. 
The memory pages managed by the buffer manager are called page frames or frames for short. We call the combination of a page file and the page frames storing 
pages from that file a Buffer Pool. The Buffer manager should is able to handle more than one open buffer pool at the same time. However, there can only be 
one buffer pool for each page file. Each buffer pool uses one page replacement strategy that is determined when the buffer pool is initialized. 
We have implemented 2 replacement strategies FIFO,LRU.

*****************************************
			Team Members
	
1.Praveen Kumar Selvan
CWID:20351349

2.Dhayalini Nagaraj
CWID:20359686

3.Vijay Bharrathi
CWID:20356386

************** CONTENTS *****************
    * Source Files
    * MakeMe File
    * ReadMe

******* INSTRUCTIONS TO RUN **************

1)Instructions to run the code
-------------------------------
a)For executing Buffer Manager


1) In the terminal,navigate to the file directory.


2) Type: 
	make -f make_file.mk


3) ./buffermgr


******* FUNCTIONS IMPLEMENTED ************

******************************************
	NAME : 		initBufferPool
	ARGUMENTS : BM_BufferPool *const bm, const char *const pageFileName, const int 
				numPages, ReplacementStrategy strategy, void *stratData
	PURPORSE :  Initializes buffer pool. Creates and initializes the buffer pool
	RETURNS : 	returns RC_OK if successfull
				returns following error codes if checks fails 
				-RC_INVALID_HANDLE
				-RC_INVALID_PAGE_FILE_NAME	
				-RC_PAGENUM_INVALID
******************************************
	
******************************************
	NAME : 		shutdownBufferPool
	ARGUMENTS : BM_BufferPool *const bm
	PURPORSE :  shutdown a buffer pool and free up all associated resources
	RETURNS : 	returns RC_OK if successfull
				returns following error codes if checks fails
				-RC_INVALID_HANDLE	-RC_SHUTDOWN_POOL_ERROR
******************************************	
	
	
******************************************
	NAME : 		forceFlushPool
	ARGUMENTS : BM_BufferPool *const bm
	PURPORSE :  force the buffer manager to write all dirty pages to disk 
	RETURNS : 	returns RC_OK if successfull
				returns following error codes if checks fails
				-RC_INVALID_HANDLE	
******************************************	

	
******************************************
	NAME : 		markDirty
	ARGUMENTS : BM_BufferPool *const bm, BM_PageHandle *const page
	PURPORSE :  Marks a page as dirty.
	RETURNS : 	returns RC_OK if successfull
				returns following error codes if checks fails
				-RC_INVALID_HANDLE	
******************************************			
	
	
******************************************
	NAME : 		unpinPage 
	ARGUMENTS : BM_BufferPool *const bm, BM_PageHandle *const page
	PURPORSE :  unpins the page page
	RETURNS : 	returns RC_OK if successfull
				returns following error codes if checks fails
				-RC rcVal	
******************************************	

******************************************
	NAME : forcePage  
	ARGUMENTS : BM_BufferPool *const bm, BM_PageHandle *const page
	PURPORSE :  forcePage write the current content of the page back to the page file on disk.
	RETURNS : 	returns RC_OK if successfull
				returns following error codes if checks fails
				-RC rcVal	
******************************************				
	
	
******************************************
	NAME : 		pinPage   
	ARGUMENTS : BM_BufferPool *const bm, BM_PageHandle *const page
	PURPORSE :  Pins page to an empty page frame if it doesn't already exist in any 
				of the frames, otherwise data from already pinned page is returned
	RETURNS : 	returns RC_OK if successfull
	
******************************************			

******************************************
	NAME : 		getFrameContents    
	ARGUMENTS : BM_BufferPool *const bm
	PURPORSE :  The getFrameContents function returns an array of PageNumbers (of
				size numPages) where the ith element is the number of the page stored in the ith page frame.
	RETURNS : 	returns an array of PageNumbers
	
******************************************	

******************************************
	NAME : 		getDirtyFlags 
	ARGUMENTS : BM_BufferPool *const bm
	PURPORSE :  The getDirtyFlags function returns an array of bools (of size 
				numPages) where the ith element is TRUE if the page stored in the 
				ith page frame is dirty
	RETURNS : 	returns an array of bools
	
******************************************	
	
******************************************
	NAME : 		getFixCounts  
	ARGUMENTS : BM_BufferPool *const bm
	PURPORSE :  The getFixCounts function returns an array of ints (of size 
				numPages) where the ith element is the fix count of the page
				stored in the ith page frame
	RETURNS : 	returns an array of bools
	
******************************************		

******************************************
	NAME : 		getNumReadIO   
	ARGUMENTS : BM_BufferPool *const bm
	PURPORSE :  The getNumReadIO function returns the number of pages that have 
				been read from disk since a buffer pool has been initialized.
	RETURNS : 	returns the number of pages
	
******************************************	

******************************************
	NAME : 		getNumWriteIO    
	ARGUMENTS : BM_BufferPool *const bm
	PURPORSE :  returns the number of pages written to the page file since the 
				buffer pool has been initialized
	RETURNS : 	 returns the number of pages
	
*****************************************
	
	
	Additional Functions Implemented:

	NAME : 		isBufferValid
	ARGUMENTS : BM_BufferPool
	PURPORSE :  check if the buffer is valid or not
	RETURNS : 	returns RC_OK


	NAME : 		ReadAndUpdatePageDetails
	ARGUMENTS : BM_BufferPool(contains a pointer to the pool),PageNumber, BM_PageHandle
	PURPORSE :  To read and update the page details
	RETURNS : 	returns RC_OK

***********************************************************			
	Additional Error Codes Added:

	#define RC_ERROR 5
	#define RC_ALGORITHM_NOT_IMPLEMENTED 6
	#define RC_PINNED_PAGES_STILL_IN_BUFFER 7
	#define RC_FILE_DELETE_FAILED 8
	#define RC_FILE_HANDLE_NOT_INITIATED 9
	#define RC_PAGENUM_INVALID 10
	#define RC_FILE_CANNOT_BE_CLOSED 11
	#define RC_INVALID_PAGE_FILE_NAME 12


	#define RC_BUFFER_PROB 304
	#define RC_INVALID_HANDLE 305

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
