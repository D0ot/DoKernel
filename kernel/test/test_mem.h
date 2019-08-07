#ifndef _TSET_MEM_H_
#define _TEST_MEM_H_

#include "mem/memory.h"
#include "others/gdata.h"
#include "log/kdebug.h"


void simple_alloc_free_test(void)
{
    LOG_DEBUG("{ simple_alloc_free_test start");
    uint32_t arr[] = {1, 3, 5, 7, 8, 10, 12, 11, 33, 55, 77, 88, 100, 1212};
    Buddy_Block bbs[20];

    buddy_debug_show(&global_data->physical_mem);

    LOG_DEBUG("simple_alloc_free_test case 1");
    for(uint8_t i = 0; i < sizeof(arr)/sizeof(*arr); ++i)
    {
        bbs[i] = buddy_alloc_bypage(&global_data->physical_mem, arr[i]);
        if(bbs[i].size == 0)
        {
            LOG_WARNING("bbs[%d].size = 0", i);
        }
        buddy_debug_show(&global_data->physical_mem);
    }

    for(uint8_t i = 0; i < sizeof(arr)/sizeof(*arr); ++i)
    {
        buddy_free_byaddr(&global_data->physical_mem, bbs[i].addr);
        buddy_debug_show(&global_data->physical_mem);
    }

    LOG_DEBUG("simple_alloc_free_test case 2");
    for(uint8_t i = 0; i < sizeof(arr)/sizeof(*arr); ++i)
    {
        bbs[i] = buddy_alloc_bypage(&global_data->physical_mem, arr[i]);
        if(bbs[i].size == 0)
        {
            LOG_WARNING("bbs[%d].size = 0", i);
        }
        buddy_debug_show(&global_data->physical_mem);
    }

    for(int8_t i = sizeof(arr)/sizeof(*arr) -1 ; i >=0 ; --i)
    {
        buddy_free_byaddr(&global_data->physical_mem, bbs[i].addr);
        buddy_debug_show(&global_data->physical_mem);
        LOG_INFO("i : %d", i);
    }

    LOG_DEBUG("simple_alloc_free_test case 3");
    for(int8_t i = sizeof(arr)/sizeof(*arr) -1 ; i >=0 ; --i)
    {
        bbs[i] = buddy_alloc_bypage(&global_data->physical_mem, arr[i]);
        if(bbs[i].size == 0)
        {
            LOG_WARNING("bbs[%d].size = 0", i);
        }
        buddy_debug_show(&global_data->physical_mem);
    }

    for(uint8_t i = 0; i < sizeof(arr)/sizeof(*arr); ++i)
    {
        buddy_free_byaddr(&global_data->physical_mem, bbs[i].addr);
        buddy_debug_show(&global_data->physical_mem);
    }


    LOG_DEBUG("simple_alloc_free_test case 4");
    for(int8_t i = sizeof(arr)/sizeof(*arr) -1 ; i >=0 ; --i)
    {
        bbs[i] = buddy_alloc_bypage(&global_data->physical_mem, arr[i]);
        if(bbs[i].size == 0)
        {
            LOG_WARNING("bbs[%d].size = 0", i);
        }
        buddy_debug_show(&global_data->physical_mem);
    }

    for(int8_t i = sizeof(arr)/sizeof(*arr) -1 ; i >=0 ; --i)
    {
        buddy_free_byaddr(&global_data->physical_mem, bbs[i].addr);
        buddy_debug_show(&global_data->physical_mem);
    }


    LOG_DEBUG("simple_alloc_free_test end }");
}



#endif 