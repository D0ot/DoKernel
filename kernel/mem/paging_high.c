#include <stdint.h>
#include <stddef.h>
#include "log/kdebug.h"
#include "common/common.h"
#include "mem/memory.h"
#include "paging_high.h"
#include "others/gdata.h"





uint8_t paging_init(Paging_Strcut *ps, Buddy_Control *phy_bc, Buddy_Control *lin_bc, 
                    Buddy_Block meta_bb_phy, Buddy_Block meta_bb_lin, uint8_t wt, uint8_t cd)
{
    ps->root.pwt = wt;
    ps->root.pcd = cd;
    ps->root.address = (uint32_t)meta_bb_phy.addr >> 12;
    ps->physical_mem = phy_bc;
    ps->linear_mem = lin_bc;
    ps->meta_bb_phy = meta_bb_phy;
    ps->meta_bb_lin = meta_bb_lin;
    memset(meta_bb_lin.addr, 0, PAGE_SIZE);
    return 0;
}

uint8_t paging_deinit(Paging_Strcut *ps)
{
    // Release Resources.

    return 1;
}


uint8_t paging_add(Paging_Strcut *ps, uint8_t page_size, void **linear_address)
{
    Buddy_Block phy_bb, lin_bb;
    uint8_t level = 0;
    if(page_size == PAGE_SIZE_4K)
    {
        level = 0;
    } 
    else if(page_size == PAGE_SIZE_4M)
    {
        level = 10;
    }

    phy_bb = buddy_alloc_bylevel(ps->physical_mem, level);
    lin_bb = buddy_alloc_bylevel(ps->linear_mem, level);
    if(phy_bb.size == 0 || lin_bb.size == 0)
    {
        LOG_WARNING("paging_add, Buddy_Block acquire failed");
        return 1;
    }
    *linear_address = lin_bb.addr;
    return paging_add_backend(ps, page_size, lin_bb.addr, phy_bb.addr);
}


uint8_t paging_add_backend(Paging_Strcut *ps, uint8_t page_size, void *linear_address, void *physical_address)
{
    if(page_size == PAGE_SIZE_4M)
    {
        uint32_t pdes_index = (uint32_t)linear_address >> 22;
        Page_Directory_Entry* pdes_ptr = (ps->meta_bb_lin.addr);
        memset((void*)(pdes_ptr + pdes_index), 0, sizeof(*pdes_ptr));

        pdes_ptr[pdes_index].p = 1;
        pdes_ptr[pdes_index].rw = 1;
        pdes_ptr[pdes_index].ps = 1;
        pdes_ptr[pdes_index].address = (uint32_t)physical_address >> 22;
    }
    else if(page_size == PAGE_SIZE_4K)
    {
        LOG_ERROR("add 4k page not impelemented");
    }



}