//including required header files
#include <stdio.h>
#include <stdlib.h>
#include "buffer_mgr.h"
#include "storage_mgr.h"
#include <math.h>
#include "buffer_mgr_stat.h"
#include "dt.h"
#include "dberror.h"
#include "pthread.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

typedef struct BufferPageManager {
	SM_PageHandle data;
	bool dirtyBit; 
	bool isFilled;
	int fixedCount; 
	int pageNum; 
    	struct timeval insertedTime;
	struct timeval lastUsedTime; 
    	int usedFrequency;
} BufferPageManager;

typedef struct BufferPoolManager {
	struct BufferPageManager *begin; 
	struct timeval lastUsedTime; 
	int filledCount; 
	int totalCount; 
    	int writeIOCount;
	int readIOCount;
} BufferPoolManager;

SM_FileHandle filehandler; //stores file address of file
/**
 *This function intialise BufferPool.
 *
 *
 * @author  Dhayalini
 * @param   BM_BufferPool(contains a pointer to the pool),
 *          pageFileName,numPages,
 *	    ReplacementStrategy strategy,
 * 	    stratData
 * @return  RC value(defined in dberror.h)
 * @since   2016-28-02
 */

RC initBufferPool(BM_BufferPool *const bm, const char *const pageFileName, const int numPages, ReplacementStrategy strategy,  void *stratData)
{	

	if (bm == NULL) {
		return RC_INVALID_HANDLE;
	}
	if (pageFileName == NULL) {
		return RC_INVALID_PAGE_FILE_NAME;
	}
	if (numPages < 0) {
		return RC_PAGENUM_INVALID;
	}

	bm->pageFile = (char*) pageFileName;
	bm->numPages = numPages;
	bm->strategy = strategy;
	BufferPoolManager *poolManager = (BufferPoolManager *)malloc(sizeof(BufferPoolManager));
	int val = openPageFile(bm->pageFile, &filehandler);
	if (val == RC_FILE_NOT_FOUND) {
		return RC_FILE_NOT_FOUND;
	}
	poolManager->begin = (BufferPageManager*)malloc(sizeof(BufferPageManager) * bm->numPages);
	BufferPageManager* newPageFrame = poolManager->begin;
	int i;
	for ( i = 0; i < bm->numPages; i++)
	{
		newPageFrame[i].isFilled = false;
        	newPageFrame[i].data = NULL;
		newPageFrame[i].pageNum = -1;
		newPageFrame[i].dirtyBit = false;
		newPageFrame[i].usedFrequency = 0;
	}
	
	poolManager->totalCount = numPages;
  	poolManager->filledCount = 0;
	poolManager->readIOCount = 0;
	poolManager->writeIOCount = 0;
	gettimeofday(&(poolManager->lastUsedTime), NULL);
  	bm->mgmtData = poolManager;
  	return RC_OK;
}

/**
 *This function shutdown BufferPool.
 *
 *
 * @author  Dhayalini
 * @param   BM_BufferPool(contains a pointer to the pool)
 * @return  RC value(defined in dberror.h)
 * @since   2016-28-02
 */

RC shutdownBufferPool(BM_BufferPool *const bm)
{
	if (bm == NULL) {
		return RC_INVALID_HANDLE;
	}
	BufferPoolManager *mgmtInfo = (BufferPoolManager*) bm->mgmtData;
    	BufferPageManager *startpage = mgmtInfo->begin;
	int i;
    for (i=0; i < bm->numPages; i++)
    {
		if (startpage[i].fixedCount > 0)
		{
			return RC_PINNED_PAGES_STILL_IN_BUFFER; 
		}
	}
	forceFlushPool(bm);
	for (i=0; i < bm->numPages; i++)
	{
		free(startpage[i].data);
	}
	free(mgmtInfo->begin);
	free(mgmtInfo);
	bm->mgmtData = NULL;
	bm->pageFile = NULL;
		
    return RC_OK;
}

/**
 *This function flushes the BufferPool.
 *
 *
 * @author  Dhayalini
 * @param   BM_BufferPool(contains a pointer to the pool)
 * @return  RC value(defined in dberror.h)
 * @since   2016-28-02
 */


RC forceFlushPool(BM_BufferPool * const bm) 
{
	if (bm == NULL) {
		return RC_INVALID_HANDLE;
	}	
	BufferPoolManager *mgmtInfo = (BufferPoolManager*) bm->mgmtData;;
    BufferPageManager *pageFrame = mgmtInfo->begin;
	BM_PageHandle *h = MAKE_PAGE_HANDLE();
	int i;
	for (i=0; i < bm->numPages; i++)
    {
		if (pageFrame[i].isFilled == true)
	    {
			h->pageNum = pageFrame[i].pageNum;
			h->data = pageFrame[i].data;
			forcePage(bm, h);
	    }
    }
    free(h);
    return RC_OK;
}

/**
 *This function sets the used page to dirty.
 *
 *
 * @author  Dhayalini
 * @param   BM_BufferPool(contains a pointer to the pool),
 *          BM_PageHandle
 * @return  RC value(defined in dberror.h)
 * @since   2016-28-02
 */
	
/* Sets the currently used page to dirty so that it can be written back to memory before being flushed */ 
RC markDirty(BM_BufferPool * const bm, BM_PageHandle * const page)
{
	if (bm == NULL) {
		return RC_INVALID_HANDLE;
	}
	if (page == NULL) {
		return RC_INVALID_HANDLE;
	}

    BufferPoolManager *mgmtInfo = (BufferPoolManager*) bm->mgmtData;;
    BufferPageManager *pageFrame = mgmtInfo->begin;

    int i;
	for (i=0; i < bm->numPages; i++)
    {
	    if (pageFrame[i].pageNum == page->pageNum) 
	    {
	       pageFrame[i].dirtyBit = TRUE;
		   pageFrame[i].data = page->data;

	       break;
	    } 
    }
    return RC_OK;
}

/**
 *This function unpins a page from memory.
 *
 *
 * @author  Smriti Raj
 * @param   BM_BufferPool(contains a pointer to the pool),
 *          BM_PageHandle(points to the area in memory
 *          storing the page data)
 * @return  RC value(defined in dberror.h)
 * @since   2016-28-02
 */
RC unpinPage(BM_BufferPool * const bm, BM_PageHandle * const page) {

	RC rcVal = isBufferValid(bm);
	if (rcVal != RC_OK) {
		return rcVal;
	}
	BufferPoolManager *mgmtInfo = (BufferPoolManager*) bm->mgmtData;;
	BufferPageManager *startpage = mgmtInfo->begin;
	int i;
	for (i=0; i < bm->numPages; i++)
    {
        if (0 < startpage[i].fixedCount && startpage[i].pageNum == page->pageNum && startpage[i].isFilled == true) {
		    startpage[i].fixedCount = startpage[i].fixedCount - 1;
			
		}
	}

    return RC_OK;
}

/**
 *This function forces a page to be written
 *
 *
 * @author  Smriti Raj
 * @param   BM_BufferPool(contains a pointer to the pool),
 *          BM_PageHandle(points to the area in memory
 *          storing the page data)
 * @return  RC value(defined in dberror.h)
 * @since   2016-28-02
 */
RC forcePage(BM_BufferPool * const bm, BM_PageHandle * const page) {

	RC returnVal = isBufferValid(bm);
	if (returnVal != RC_OK) {
		return returnVal;
	}
	BufferPoolManager *mgmtInfo = (BufferPoolManager*) bm->mgmtData;;
	BufferPageManager *startpage = mgmtInfo->begin;
	int i;
	for (i=0; i < bm->numPages; i++)
    { 
		if (startpage[i].pageNum == page->pageNum) {
			if (startpage[i].dirtyBit == TRUE && startpage[i].isFilled == true)
			{
				if (writeBlock(startpage[i].pageNum, &filehandler, startpage[i].data) != RC_OK) {
					printf("starting writeBlock \n");
					return RC_WRITE_FAILED;
				} else {
					
					startpage[i].dirtyBit = FALSE;
					mgmtInfo->writeIOCount = mgmtInfo->writeIOCount + 1;
					free(startpage[i].data);
					startpage[i].data = NULL;
				}
				break;
			}
			else 
			{
				return RC_WRITE_FAILED;
			}
		}
	}

        return RC_OK;
}



/**
 *This function is to read and update the page details.
 *
 *
 * @author  VijayBharathi
 * @param   BM_BufferPool(contains a pointer to the pool),
 *          PageNumber, BM_PageHandle
 * @return  RC value(defined in dberror.h)
 * @since   2016-28-02
 */


RC ReadAndUpdatePageDetails(BM_BufferPool *const bm, BM_PageHandle *const page, const PageNumber pageNum, int i)
{
 
     BufferPoolManager *mgmtInfo = (BufferPoolManager*) bm->mgmtData;;
     BufferPageManager *pageFrame = mgmtInfo->begin;

     if(pageFrame[i].data != NULL){
     	free(pageFrame[i].data);
     }
     pageFrame[i].data = (SM_PageHandle) malloc(PAGE_SIZE);
     readBlock(pageNum, &filehandler, pageFrame[i].data);
     pageFrame[i].dirtyBit = false;
     pageFrame[i].fixedCount = 1;
     pageFrame[i].pageNum = pageNum;
     pageFrame[i].usedFrequency = 1;
     pageFrame[i].isFilled = true;
     gettimeofday (&(pageFrame[i].insertedTime) , NULL);
     gettimeofday (&(pageFrame[i].lastUsedTime) , NULL);
     page->pageNum = pageNum;
     page->data = pageFrame[i].data;
     return RC_OK;
}


/**
 *This function used to pin the page.
 *
 *
 * @author  VijayBharathi
 * @param   BM_BufferPool(contains a pointer to the pool),
 *          PageNumber, BM_PageHandle
 * @return  RC value(defined in dberror.h)
 * @since   2016-28-02
 */

RC pinPage (BM_BufferPool *const bm, BM_PageHandle *const page, const PageNumber pageNum)
{

	BufferPoolManager *mgmtInfo = (BufferPoolManager*) bm->mgmtData;
   	BufferPageManager *startpage = mgmtInfo->begin;

	RC rcVal = isBufferValid(bm);
	if (rcVal != RC_OK) {
		return rcVal;
	}

	if (mgmtInfo->filledCount > 0)
	{
		int i;
		for (i=0; i < bm->numPages; i++)
		{
     		if (startpage[i].isFilled == true && startpage[i].pageNum == pageNum)
			{
				startpage[i].fixedCount++;
				startpage[i].usedFrequency++;
				gettimeofday(&(startpage[i].lastUsedTime), NULL);
				page->pageNum = pageNum;
				page->data = startpage[i].data;				
				return RC_OK;
			}
		}
	}

	if (mgmtInfo->filledCount < bm->numPages)
	{
		int i;
		for (i=0; i < bm->numPages; i++)
		{
     		if (startpage[i].isFilled == false)
			{               
				mgmtInfo->filledCount++;
                mgmtInfo->readIOCount++;
	         	RC code = ReadAndUpdatePageDetails(bm, page, pageNum, i);
				return code;
			}
		}
	}
	else
	{
		if(bm->strategy == RS_FIFO)
		{
			return FIFO(bm, page, pageNum);
		}	
		else if (bm->strategy == RS_LRU)				
		{
			return LRU(bm, page, pageNum);
		}
		else if (bm->strategy == RS_LFU)
		{
			return LFU(bm, page, pageNum);
		}
		else
		{
			return RC_ALGORITHM_NOT_IMPLEMENTED;
		} 
	}
	
}


/**
 *This function is used for FIFO Page Replacement.
 *
 *
 * @author  VijayBharathi
 * @param   BM_BufferPool(contains a pointer to the pool),
 *          PageNumber, BM_PageHandle
 * @return  RC value(defined in dberror.h)
 * @since   2016-28-02
 */
 

RC FIFO(BM_BufferPool *const bm, BM_PageHandle *const page, const PageNumber pageNum)
{
	BufferPoolManager *mgmtInfo = (BufferPoolManager*) bm->mgmtData;;
	BufferPageManager *startpage = mgmtInfo->begin;
    int FIFO = -1;    
	int i;
	for (i=0; i < bm->numPages; i++)
    {
		if (startpage[i].fixedCount == 0)
		{	
			if (FIFO == -1)
				FIFO = i;		
			else
			{
				if (startpage[FIFO].insertedTime.tv_usec > startpage[i].insertedTime.tv_usec)
					FIFO = i;
			}
		}
	}
	if (FIFO == -1)
		return RC_BUFFER_PROB;
	else 
	{
		if (startpage[FIFO].dirtyBit == TRUE)
		{
			page->pageNum = startpage[FIFO].pageNum;
			forcePage (bm, page);
		}
        mgmtInfo->readIOCount++;
		return ReadAndUpdatePageDetails(bm, page, pageNum, FIFO);
	}
}



/**
 *This function used for LRU Page Replacement.
 *
 *
 * @author  VijayBharathi
 * @param   BM_BufferPool(contains a pointer to the pool),
 *          PageNumber, BM_PageHandle
 * @return  RC value(defined in dberror.h)
 * @since   2016-28-02
 */

RC LRU(BM_BufferPool *const bm, BM_PageHandle *const page, const PageNumber pageNum)
{
	BufferPoolManager *mgmtInfo = (BufferPoolManager*) bm->mgmtData;;
	BufferPageManager *startpage = mgmtInfo->begin;
    int LRU = -1;
	int i;
	for (i=0; i < bm->numPages; i++)
    {
		if (startpage[i].fixedCount == 0)
		{	
			if (LRU == -1)
				LRU = i;
			else
			{
				if (startpage[LRU].lastUsedTime.tv_usec > startpage[i].lastUsedTime.tv_usec)
					LRU = i;
			}
		}
	}
	if (LRU == -1)
		return RC_BUFFER_PROB;
	else 
	{
		if (startpage[LRU].dirtyBit == TRUE)
		{
			page->pageNum =  startpage[LRU].pageNum;
			forcePage (bm, page);
		}
        mgmtInfo->readIOCount++;
		return ReadAndUpdatePageDetails(bm, page, pageNum, LRU);
	}
}

/**
 *This function used for LFU Page Replacement.
 *
 *
 * @author  VijayBharathi
 * @param   BM_BufferPool(contains a pointer to the pool),
 *          PageNumber, BM_PageHandle
 * @return  RC value(defined in dberror.h)
 * @since   2016-28-02
 */

RC LFU(BM_BufferPool *const bm, BM_PageHandle *const page, const PageNumber pageNum)
{
	BufferPoolManager *mgmtInfo = (BufferPoolManager*) bm->mgmtData;;
	BufferPageManager *startpage = mgmtInfo->begin;
    int LFU = -1; 

	int i;
	for (i=0; i < bm->numPages; i++)
    {
		if (startpage[i].fixedCount == 0)
		{	
			if (LFU == -1)
				LFU = i;
			else if (startpage[i].usedFrequency < startpage[LFU].usedFrequency)
			    LFU = i;
		}
	}
	if (LFU == -1)
		return RC_BUFFER_PROB;
	else 
	{
		if (startpage[LFU].dirtyBit == TRUE)
		{
			page->pageNum = startpage[LFU].pageNum;
			forcePage (bm, page);
		}
        mgmtInfo->readIOCount++;
		return ReadAndUpdatePageDetails(bm, page, pageNum, LFU);
	}
}

/**
 *This function is an additional function to check if the buffer is valid or not
 *
 *
 * @author  Smriti Raj
 * @param   BM_BufferPool(contains a pointer to the pool),
 *          BM_PageHandle(points to the area in memory
 *          storing the page data)
 *          PageNumber(value of the page number)
 * @return  RC value(defined in dberror.h)
 * @since   2016-28-02
 */
RC isBufferValid(BM_BufferPool * const bm) {
	if (NULL == bm || NULL == bm->pageFile || 0 > bm->numPages) {
		return RC_BUFFER_PROB;
	}
	return RC_OK;
}

/*
* PageNumber *getFrameContents Method
* returns an array of PageNumbers (of size numPages) where the ith element 
* is the number of the page stored in the ith page frame
* 
* @param 
* @author Praveen
* @return 
*/

PageNumber* getFrameContents (BM_BufferPool *const bm)
{
	RC returnVal = isBufferValid(bm);
	if (returnVal != RC_OK) {
		return NULL;
	}
	BufferPoolManager *mgmtInfo = (BufferPoolManager*) bm->mgmtData;;
	BufferPageManager *startpage = mgmtInfo->begin;
	int i;
	PageNumber *pageNumber_Array=(PageNumber *)malloc(sizeof(PageNumber)*bm->numPages);
	
	for(i=0;i<bm->numPages;i++)
	{
		if (startpage[i].isFilled == true)
		{
			pageNumber_Array[i] = startpage[i].pageNum;
		}
		else 
		{
			pageNumber_Array[i] = NO_PAGE;
		}
	}
	return pageNumber_Array;
}

/*
* bool *getDirtyFlags Method
* returns an array of bools (of size numPages) where the ith element is 
* TRUE if the page stored in the ith page frame is dirty
* 
* @param 
* @author Praveen
* @return 
*/
bool *getDirtyFlags (BM_BufferPool *const bm)
{         
	RC returnVal = isBufferValid(bm);
	if (returnVal != RC_OK) {
		return NULL;
	}
	BufferPoolManager *mgmtInfo = (BufferPoolManager*) bm->mgmtData;;
	BufferPageManager *startpage = mgmtInfo->begin;
	int i;
	bool *dirty_Flag=(bool*)malloc(sizeof(bool)*bm->numPages);
	
	for(i=0;i<bm->numPages;i++)
	{
		if (startpage[i].isFilled == true)
		{
			dirty_Flag[i] = startpage[i].dirtyBit;
		}
		else 
		{
			dirty_Flag[i] = FALSE;
		}
	}
    
	return dirty_Flag;
}

/*
* int *getFixCounts Method
* returns an array of ints (of size numPages) where the ith element 
* is the fix count of the page stored in the ith page frame
* 
* @param 
* @author Praveen
* @return 
*/
int *getFixCounts (BM_BufferPool *const bm)
{
    RC returnVal = isBufferValid(bm);
	if (returnVal != RC_OK) {
		return NULL;
	}
	BufferPoolManager *mgmtInfo = (BufferPoolManager*) bm->mgmtData;;
	BufferPageManager *startpage = mgmtInfo->begin;
	int i;
	int *fixCounts_Array=(int *)malloc(sizeof(int)*bm->numPages);
	
	for(i=0;i<bm->numPages;i++)
	{
		if (startpage[i].isFilled == true)
		{
			fixCounts_Array[i] = startpage[i].fixedCount;
		}
		else 
		{
			fixCounts_Array[i] = 0;
		}
	}

	return fixCounts_Array;
}

/*
* getNumReadIO Method
* getNumReadIO function returns the number of pages that have been read from disk
* since a buffer pool has been initialized
* 
* @param BM_BufferPool *const bm-
* @author Praveen
* @return 
*/

int getNumReadIO (BM_BufferPool *const bm)
{
	RC returnVal = isBufferValid(bm);
	if (returnVal != RC_OK) {
		return returnVal;
	}
	BufferPoolManager *mgmtInfo = (BufferPoolManager*) bm->mgmtData;;
	return mgmtInfo->readIOCount;
}

/*
* getNumWriteIO Method
* getNumWriteIO function returns the number of pages written to the page file 
* since the buffer pool has been initialized
*
* @param BM_BufferPool *const bm-
* @author Praveen
* @return 
*
*/
int getNumWriteIO (BM_BufferPool *const bm)
{
	RC returnVal = isBufferValid(bm);
	if (returnVal != RC_OK) {
		return returnVal;
	}
	BufferPoolManager *mgmtInfo = (BufferPoolManager*) bm->mgmtData;;
	return mgmtInfo->writeIOCount;
}
