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
} Data_In_CR3;


typedef struct Page_Directory_Entry_tag
{
    uint8_t p : 1;
    uint8_t rw : 1;
    uint8_t us : 1;
    uint8_t pwt : 1;
    uint8_t pcd : 1;
    uint8_t a : 1;
    uint8_t d : 1;
    uint8_t ps : 1;
    uint8_t g : 1; // only for 4M page. ignored for 4K Page
    uint8_t : 3;
    uint32_t address: 20; // different for 4M page and 4K page

} __attribute__((packed)) Page_Directory_Entry;


typedef struct Page_Table_Entry_tag
{
    uint8_t p : 1;
    uint8_t rw : 1;
    uint8_t us : 1;
    uint8_t pwt : 1;
    uint8_t pcd : 1;
    uint8_t a : 1;
    uint8_t d : 1;
    uint8_t pat : 1;
    uint8_t g : 1;
    uint8_t : 3;
    uint32_t address: 20;

} __attribute__((packed)) Page_Table_Entry;


#endif