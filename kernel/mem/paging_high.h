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

    // map linear to physicl
    // they are abstruct
    Buddy_Control *physical_mem;
    Buddy_Control *linear_mem;


    // kernel address space
    // all things are accessible are in kernel's address space
    Buddy_Control *kernel_mem;

    // physical adress to store PDES
    // caller of paging_init shoud ensure 
    // meta_bb_phy and meta_bb_lin are valid
    Buddy_Block meta_bb_phy;

    // linear address to access PDES
    Buddy_Block meta_bb_lin;



    // for Bootstrap
    // caller is responsible for these 2 Buddy_Block
    Buddy_Block first_bb_phy;
    Buddy_Block first_bb_lin;


    
    // record how many resources that
    // this Paging Structure accquired
    // add this pointer is in linear_mem's address space
    Wrapper_Buddy_Block *wbb;

    // elements stored in wbb
    uint32_t wbb_count;
} Paging_Strcut;

/**
 *  \brief Initialize a Paging Structure
 *  \param ps is the Paging_Struct to init.
 *  \param wt write-throught 1 set, 0 clear
 *  \param cd cache-disable 1 set, 0 clear
 *  \param lin_bc is the current linear address space 's Budyy_Control
 *         and there will be access to this linear address space
 *  \param meta_bb_phy is the physical address where the PDES are stored
 *  \param meta_bb_lin is the linear address of PDES 
 *  \return 0 for success, others for failure.
 *      if the init is about kernel's Page Directory, manually modify the PDES 
 *      are necessary.
 */
uint8_t paging_init(Paging_Strcut *ps, Buddy_Control *phy_bc, Buddy_Control *lin_bc, 
                    Buddy_Block meta_phy_bb, Buddy_Block meta_lin_bb, 
                    Buddy_Block pre_lin_bb, Buddy_Block pre_phy_bb,
                    uint8_t wt, uint8_t cd);


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
 *  \param linear_address is output
 *  \return 0 for success, others for failure
 *      it will automatically arrange the address
 *      it will call paging_add_backend
 * 
 */
uint8_t paging_add(Paging_Strcut *ps, uint8_t page_size, void** linear_address);

/**
 *  \breif Add a new page in Paging Structure
 *  \param paging_size is PAGE_SIZE_4M or PAGING_SIZE_4K
 *  \param linear_address is where the new page frame mapped to
 *  \param physical_address is the new page frame's physical address
 *  \return 0 for success, others for failure
 */
uint8_t paging_add_backend(Paging_Strcut *ps, uint8_t page_size, void *linear_address, void *physical_address);


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

/**
 *  \brief calculate the address in PDE which references a PTE
 *  \pararm  
 */

uint32_t paging_aux_addr_inpde(void *phy_address);


/**
 *  \brief calculate the address in PTE which references a Page Frame
 */

uint32_t paging_aux_addr_inpte(void *phy_address);

/**
 *  \brief get offset of 'PDE_t *' from linear address
 */
uint32_t paging_aux_offset_pde(void *lin_address);

/**
 *  \brief get offset of 'PTE_t *' from linear address
 */

uint32_t paging_aux_offset_pte(void *lin_address);



#endif