#include <stdint.h>
#include <stddef.h>
#include "log/kdebug.h"
#include "common/common.h"
#include "mem/memory.h"
#include "paging_high.h"


uint8_t paging_init(Paging_Strcut *ps, uint8_t wt, uint8_t cd)
{
    ps->root.pwt = wt;
    ps->root.pcd = cd;
    ps->meat_bb = buddy_alloc_bypage(1);
    if(ps->meat_bb.addr == NULL)
    {
        LOG_WARNING("paging_init, buddy_alloc_bypage returns NULL");
        return 1;
    }
    
}