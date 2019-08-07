#ifndef _TSET_MEM_H_
#define _TEST_MEM_H_

#include "mem/memory.h"
#include "others/gdata.h"
#include "log/kdebug.h"


void simple_alloc_free_test(Buddy_Control *bc)
{
    LOG_DEBUG("{ simple_alloc_free_test start");
    uint32_t arr[] = {1, 3, 5, 7, 8, 10, 12, 11, 33, 55, 77, 88, 100, 1212};
    Buddy_Block bbs[20];

    buddy_debug_show(bc);

    LOG_DEBUG("simple_alloc_free_test case 1");
    for(int8_t i = 0; i < sizeof(arr)/sizeof(*arr); ++i)
    {
        bbs[i] = buddy_alloc_bypage(bc, arr[i]);
        if(bbs[i].size == 0)
        {
            LOG_WARNING("bbs[%d].size = 0", i);
        }
        buddy_debug_show(bc);
    }

    for(int8_t i = 0; i < sizeof(arr)/sizeof(*arr); ++i)
    {
        buddy_free_byaddr(bc, bbs[i].addr);
        buddy_debug_show(bc);
    }

    LOG_DEBUG("simple_alloc_free_test case 2");
    for(int8_t i = 0; i < sizeof(arr)/sizeof(*arr); ++i)
    {
        bbs[i] = buddy_alloc_bypage(bc, arr[i]);
        if(bbs[i].size == 0)
        {
            LOG_WARNING("bbs[%d].size = 0", i);
        }
        buddy_debug_show(bc);
    }

    for(int8_t i = sizeof(arr)/sizeof(*arr) -1 ; i >=0 ; --i)
    {
        buddy_free_byaddr(bc, bbs[i].addr);
        buddy_debug_show(bc);
        LOG_INFO("i : %d", i);
    }

    LOG_DEBUG("simple_alloc_free_test case 3");
    for(int8_t i = sizeof(arr)/sizeof(*arr) -1 ; i >=0 ; --i)
    {
        bbs[i] = buddy_alloc_bypage(bc, arr[i]);
        if(bbs[i].size == 0)
        {
            LOG_WARNING("bbs[%d].size = 0", i);
        }
        buddy_debug_show(bc);
    }

    for(int8_t i = 0; i < sizeof(arr)/sizeof(*arr); ++i)
    {
        buddy_free_byaddr(bc, bbs[i].addr);
        buddy_debug_show(bc);
    }


    LOG_DEBUG("simple_alloc_free_test case 4");
    for(int8_t i = sizeof(arr)/sizeof(*arr) -1 ; i >=0 ; --i)
    {
        bbs[i] = buddy_alloc_bypage(bc, arr[i]);
        if(bbs[i].size == 0)
        {
            LOG_WARNING("bbs[%d].size = 0", i);
        }
        buddy_debug_show(bc);
    }

    for(int8_t i = sizeof(arr)/sizeof(*arr) -1 ; i >=0 ; --i)
    {
        buddy_free_byaddr(bc, bbs[i].addr);
        buddy_debug_show(bc);
    }


    LOG_DEBUG("simple_alloc_free_test end }");
}


void fixed_address_test(Buddy_Control *bc)
{
    char *func_name = "fixed_address_test";
    LOG_DEBUG("{ %s start", func_name);

    buddy_debug_show(bc);
    Buddy_Block bb1 = buddy_alloc_byaddr(bc, bc->base, 0);

    buddy_debug_show(bc);
    Buddy_Block bb2 = buddy_alloc_byaddr(bc, bc->base + PAGE_SIZE * 2, 1);

    buddy_debug_show(bc);
    buddy_alloc_byaddr(bc, bc->base + PAGE_SIZE * 2 + 1, 1);

    buddy_debug_show(bc);
    buddy_alloc_byaddr(bc, bc->base + PAGE_SIZE * 3, 1);

    buddy_debug_show(bc);

    buddy_free_byaddr(bc, bb1.addr);
    buddy_debug_show(bc);
    buddy_free_byaddr(bc, bb2.addr);
    buddy_debug_show(bc);

    LOG_DEBUG("%s end }", func_name);
}



#endif 