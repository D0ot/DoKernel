#ifndef _PAGING_H
#define _PAGING_H

#include <stdint.h>

typedef struct Data_In_CR3_tag
{
    uint8_t :3;
    uint8_t pwt:1;
    uint8_t pcd:1;
    uint8_t :7;
    uint32_t address:20;
} __attribute__((packed)) Data_In_CR3;


typedef struct Page_Directory_Entry_tag
{
    // Present; must be 1 to map a 4-MByte Page
    uint8_t p : 1;

    // Read/write; if 0, writes may not be allowed to the 4MByte page referenced by this entry
    uint8_t rw : 1;

    // User/supervisor, if 0, user-mode accessed may not be allowed
    uint8_t us : 1;

    // Page-level write-through
    uint8_t pwt : 1;

    // Page-level cache disable
    uint8_t pcd : 1;

    // Accessed
    uint8_t a : 1;

    // Dirty
    uint8_t d : 1;

    // Page size; 1 for 4M page
    uint8_t ps : 1;

    // Global
    uint8_t g : 1; // only for 4M page. ignored for 4K Page

    // Ignored
    uint8_t : 3;

    // FOR 4M PAGE, (10 Bits) Bits 32:22 of physical address of the 4-MByte page referenced by this entry 
    // FOR PAGE TABLE, (20Bits) Address of page table
    uint32_t address: 20; // different for 4M page and 4K page

} __attribute__((packed)) Page_Directory_Entry;


typedef struct Page_Table_Entry_tag
{
    // Present
    uint8_t p : 1;

    // Read/write
    uint8_t rw : 1;

    // User/supervisor
    uint8_t us : 1;

    // Page level write-through
    uint8_t pwt : 1;

    // Page level cache-disable
    uint8_t pcd : 1;

    // Accessec
    uint8_t a : 1;

    // Dirty
    uint8_t d : 1;

    // Page Attribute Table
    uint8_t pat : 1;
    
    // Global
    uint8_t g : 1;
    
    // Ignored
    uint8_t : 3;

    // Address of 4K page frame
    uint32_t address: 20;

} __attribute__((packed)) Page_Table_Entry;


#endif