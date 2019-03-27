//
// Created by vvivi on 2019/2/22.
//
#include <gtest/gtest.h>

#define __GFP_DMA	0x01
#define __GFP_HIGHMEM	0x02

#define ZONE_DMA		0
#define ZONE_NORMAL		1
#define ZONE_HIGHMEM		2
#define MAX_NR_ZONES		3


/*
 * On machines where it is needed (eg PCs) we divide physical memory
 * into multiple physical zones. On a PC we have 3 zones:
 *
 * ZONE_DMA	  < 16 MB	ISA DMA capable memory
 * ZONE_NORMAL	16-896 MB	direct mapped by the kernel
 * ZONE_HIGHMEM	 > 896 MB	only page cache and user processes
 */
typedef struct zone_struct {
    const char* name;
    unsigned long		size;
} zone_t;
/*
 * One allocation request operates on a zonelist. A zonelist
 * is a list of zones, the first one is the 'goal' of the
 * allocation, the other zones are fallback zones, in decreasing
 * priority.
 *
 * Right now a zonelist takes up less than a cacheline. We never
 * modify it apart from boot-up, and only a few indices are used,
 * so despite the zonelist table being relatively big, the cache
 * footprint of this construct is very small.
 */
typedef struct zonelist_struct {
    zone_t * zones [MAX_NR_ZONES+1]; // NULL delimited
} zonelist_t;

#define GFP_ZONEMASK	0x0f

typedef struct pglist_data {
    zone_t node_zones[MAX_NR_ZONES];
    zonelist_t node_zonelists[GFP_ZONEMASK+1];
} pg_data_t;

static inline void build_zonelists(pg_data_t *pgdat)
{
    int i, j, k;

    for (i = 0; i <= GFP_ZONEMASK; i++) {
        zonelist_t *zonelist;
        zone_t *zone;

        zonelist = pgdat->node_zonelists + i;
        memset(zonelist, 0, sizeof(*zonelist));

        j = 0;
        k = ZONE_NORMAL;
        if (i & __GFP_HIGHMEM)
            k = ZONE_HIGHMEM;
        if (i & __GFP_DMA)
            k = ZONE_DMA;

        switch (k) {
            default:
                printf("BUG...");
                /*
                 * fallthrough:
                 */
            case ZONE_HIGHMEM:
                zone = pgdat->node_zones + ZONE_HIGHMEM;
                if (zone->size) {
                    zonelist->zones[j++] = zone;
                }
            case ZONE_NORMAL:
                zone = pgdat->node_zones + ZONE_NORMAL;
                if (zone->size)
                    zonelist->zones[j++] = zone;
            case ZONE_DMA:
                zone = pgdat->node_zones + ZONE_DMA;
                if (zone->size)
                    zonelist->zones[j++] = zone;
        }
        zonelist->zones[j++] = NULL;
    }
}

static inline void dump_zone_list(pg_data_t* pg_data) {
    printf("to dump zone_list\n");
    for (int i = 0; i < GFP_ZONEMASK+1; i++) {
        zonelist_t zl = pg_data->node_zonelists[i];
        printf("no[%c%c%c] :\t", (i & __GFP_HIGHMEM) ? '1' : '0',
                                 (i & __GFP_DMA) ? '1' : '0',
                                 (i & 0) ? '1' : '0');
        if (zl.zones[0]) printf("\t%s", zl.zones[0]->name);
        if (zl.zones[1]) printf("\t%s", zl.zones[1]->name);
        if (zl.zones[2]) printf("\t%s", zl.zones[2]->name);
        printf("\n");
    }
}

TEST(build_zonelists, case_1) {
    pg_data_t pg_data;
    pg_data.node_zones[ZONE_DMA].size = 1*1024*1024;
    pg_data.node_zones[ZONE_DMA].name = "DMA";

    pg_data.node_zones[ZONE_NORMAL].size = 16*1024*1024;
    pg_data.node_zones[ZONE_NORMAL].name ="NORMAL";

    pg_data.node_zones[ZONE_HIGHMEM].size = 16*1024*1024;
    pg_data.node_zones[ZONE_HIGHMEM].name = "HIGHMEM";
    build_zonelists(&pg_data);
    dump_zone_list(&pg_data);
}