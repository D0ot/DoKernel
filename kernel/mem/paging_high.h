#ifndef _PAGING_HIGH_H
#define _PAGING_HIGH_H

#include <stdint.h>
#include "arch/x86/paging.h"
#include "mem/memory.h"

/**
 *  ALL THE OPERATIONS ARE IN KERNEL SPACE
 * 
 *  Workflow of high level paging operations.
 *    1. Init a Paging_Struct
 *          Acquire a Buddy_Block(1 4KiB Page) to store 'Wrapper_Buddy_Block's 
 *          when wbb_count is beyond 4KiB / sizeof(Wrapper_Buddy_Block)
 *          we can acquire a large Buddy_Block and move data, release the original
 *          one.
 *      
 */


enum {
    PAGE_SIZE_4M,
    PAGE_SIZE_4K
};

typedef struct Wrapper_Buddy_Block_tag
{
    uint32_t last;
    Buddy_Block bb;
    uint32_t next;

} Wrapper_Buddy_Block;


typedef struct Paging_Strcut_tag
{
    Data_In_CR3 root;

    Buddy_Block meat_bb;
    
    // record how many resources that
    // this Paging Structure accquired
    // add this pointer is in kernel's address space
    Wrapper_Buddy_Block *wbb;

    // elements stored in wbb
    uint32_t wbb_count;
} Paging_Strcut;

/**
 *  \brief Initialize a Paging Structure
 *  \param ps is the Paging_Struct to init.
 *  \param wt write-throught 1 set, 0 clear
 *  \param cd cache-disable 1 set, 0 clear
 *  \return 0 for success, others for failure.
 */
uint8_t paging_init(Paging_Strcut *ps, uint8_t wt, uint8_t cd);


/**
 *  \brief Deinitialize a Paging Structure
 *  \param ps is the Paging_Struct to init.
 *  \return 0 for success, others for failure.
 *  
 *      release resources the Paging Structre consumes
 */
uint8_t paging_deinit(Paging_Strcut *ps);



/**
 *  \breif Add a new page in Paging Structure
 *  \param paging_size is PAGE_SIZE_4M or PAGING_SIZE_4K
 *  \param linear_address is where the new page frame mapped to
 *  \return 0 for success, others for failure
 */
uint8_t paging_add(Paging_Strcut *ps, uint8_t page_size, void *linear_address);

/**
 *  \breif Add a new page in Paging Structure
 *  \param paging_size is PAGE_SIZE_4M or PAGING_SIZE_4K
 *  \param linear_address is where the new page frame mapped to
 *  \param physical_address is the new page frame's physical address
 *  \return 0 for success, others for failure
 */
uint8_t paging_add_fixed(Paging_Strcut *ps, uint8_t page_size, void *linear_address, void *physical_address);


/**
 *  \breif Delete a page in Paging Structre
 *  \param linerar_address is the page to delete
 */

uint8_t pagign_del(Paging_Strcut *ps, void *linear_address);


/**
 *  \breif Transalte a linear address to physical address
 *  \return 0 for success, others for failure
 *      
 *      the translation is according to ps
 */
void *paging_translate(Paging_Strcut *ps, void *linear_address);

/**
 *  \brief modify the attribute of 4K Page.
 *  \param only attributes in pte affect.
 */

uint8_t paging_set_attribute(Paging_Strcut *ps, void *linear_address, Page_Table_Entry pte);




#endif