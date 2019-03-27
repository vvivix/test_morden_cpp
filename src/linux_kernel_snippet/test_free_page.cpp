//
// Created by vvivi on 2019/2/24.
//
#include <gtest/gtest.h>


struct list_head {
    struct list_head *next, *prev;
};

typedef struct free_area_struct {
    struct list_head	free_list;
    unsigned long		*map;
} free_area_t;

#define MAX_ORDER 10

typedef struct zone_struct {
    unsigned long		free_pages;//空闲页总和
    free_area_t		free_area[MAX_ORDER];//用于伙伴算法
    struct page		*zone_mem_map;//Page Array
} zone_t;

typedef struct page {
    struct list_head list;		/* ->mapping has some page lists. */
    /*
    index含义:
    page作为文件映射, index为文件的偏移量;
    page作为swapper?这个没看懂;
    页空闲时,index为物理页的order,见__free_pages_ok;
    */
    unsigned long index;		/* Our offset within mapping. */
    unsigned long flags;		/* atomic flags, some possibly
					   updated asynchronously */
    struct zone_struct *zone;	/* Memory zone we are in. */
} mem_map_t;


static inline int __test_and_change_bit(int nr, volatile void *addr)
{
    unsigned int mask = 1 << (nr & 7);
    unsigned int oldval;

    oldval = ((unsigned char *) addr)[nr >> 3];
    ((unsigned char *) addr)[nr >> 3] = oldval ^ mask;
    return oldval & mask;
}

static __inline__ void __list_del(struct list_head * prev,
                                  struct list_head * next)
{
    next->prev = prev;
    prev->next = next;
}

static __inline__ void list_del(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
}

static __inline__ void __list_add(struct list_head * newhead,
                                struct list_head * prev,
                                struct list_head * next)
{
    next->prev = newhead;
    newhead->next = next;
    newhead->prev = prev;
    prev->next = newhead;
}


static __inline__ void list_add(struct list_head *newhead, struct list_head *head)
{
    __list_add(newhead, head, head->next);
}

#define INIT_LIST_HEAD(ptr) do { \
	(ptr)->next = (ptr); (ptr)->prev = (ptr); \
} while (0)

#define memlist_init(x) INIT_LIST_HEAD(x)
#define memlist_del list_del
#define memlist_add_head list_add
#define memlist_entry list_entry
#define memlist_next(x) ((x)->next)
#define memlist_prev(x) ((x)->prev)


static inline void __change_bit(int nr, volatile void *addr)
{
    ((unsigned char *) addr)[nr >> 3] ^= (1U << (nr & 7));
}

#define MARK_USED(index, order, area) \
	__change_bit((index) >> (1+(order)), (area)->map)

/**
* list_entry - get the struct for this entry
* @ptr:	the &struct list_head pointer.
* @type:	the type of the struct this is embedded in.
* @member:	the name of the list_struct within the struct.
*/
#define list_entry(ptr, type, member) \
	((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))
/**
* list_for_each	-	iterate over a list
* @pos:	the &struct list_head to use as a loop counter.
* @head:	the head for your list.
*/
#define list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)

#define LONG_ALIGN(x) (((x)+(sizeof(long))-1)&~((sizeof(long))-1))

static void __free_pages_ok (struct page *page, unsigned int order)
{
    unsigned long index, page_idx, mask, flags;
    free_area_t *area;
    struct page *base;
    zone_t *zone;

    zone = page->zone;

    mask = (~0UL) << order;
    base = zone->zone_mem_map;
    page_idx = page - base;
    if (page_idx & ~mask)
        printf("error 11");
    index = page_idx >> (1 + order);

    area = zone->free_area + order;

    zone->free_pages -= mask;

    while (mask + (1 << (MAX_ORDER-1))) {
        struct page *buddy1;

        if (area >= zone->free_area + MAX_ORDER)
            printf("ERROR");
        if (!__test_and_change_bit(index, area->map))
            /*
             * the buddy page is still allocated.
             */
            break;
        /*
         * Move the buddy up one level.
         */
        buddy1 = base + (page_idx ^ -mask);

        memlist_del(&buddy1->list);
        mask <<= 1;
        area++;
        index >>= 1;
        page_idx &= mask;
    }
    memlist_add_head(&(base + page_idx)->list, &area->free_list);
}

void dump_free_list(const zone_t &zone) {
	int power = 1;
    for (auto& area : zone.free_area) {

        printf("list (size %d): ", power);
        power *= 2;
        list_head* tmp;
        list_for_each(tmp, &area.free_list) {
            struct page *p;
            p = list_entry(tmp, struct page, list);
            printf("-->%d", static_cast<int>(p->index));
            for (int i = 1; i < power/2;i++) {
                printf(",%d", static_cast<int>(p->index + i));
            }
        }

        printf("\n");
    }
}

static inline struct page * expand (zone_t *zone, struct page *page,
                                    unsigned long index, int low, int high, free_area_t * area)
{
    unsigned long size = 1 << high;

    while (high > low) {
        area--;
        high--;
        size >>= 1;
        memlist_add_head(&(page)->list, &(area)->free_list);
        MARK_USED(index, high, area);
        index += size;
        page += size;
    }
    return page;
}


static struct page * rmqueue(zone_t *zone, unsigned int order)
{
    free_area_t * area = zone->free_area + order;
    unsigned int curr_order = order;
    struct list_head *head, *curr;
    struct page *page;

    do {
        head = &area->free_list;
        curr = memlist_next(head);

        if (curr != head) {
            unsigned int index;

            page = memlist_entry(curr, struct page, list);

            memlist_del(curr);
            index = page - zone->zone_mem_map;
            if (curr_order != MAX_ORDER-1)
                MARK_USED(index, curr_order, area);
            zone->free_pages -= 1UL << order;

            page = expand(zone, page, index, order, curr_order, area);

            return page;
        }
        curr_order++;
        area++;
    } while (curr_order < MAX_ORDER);

    return NULL;
}

TEST(free_page, case1)
{
    zone_t zone;
    struct page _page[16];
    zone.zone_mem_map = _page;
    zone.free_pages = 0;

    int i;
    int size = 16;
    for (i = 0; ; i++) {
        unsigned long bitmap_size;

        memlist_init(&zone.free_area[i].free_list);
        if (i == MAX_ORDER-1) {
            zone.free_area[i].map = NULL;
            break;
        }

        bitmap_size = (size-1) >> (i+4);
        bitmap_size = LONG_ALIGN(bitmap_size+1);
        zone.free_area[i].map = static_cast<unsigned long *>(calloc(bitmap_size, 1));
    }


    for (int idx = 0; idx < 16; idx++) {
        struct page * p = _page + idx;
        p->zone = &zone;
        p->index = static_cast<unsigned long>(idx);
        memlist_init(&p->list);
        __free_pages_ok(p, 0);
//        printf("Round %d : \n", idx);
//        dump_free_list(zone);
    }

    dump_free_list(zone);
    printf("to free 1\n");
    rmqueue(&zone, 0);
    dump_free_list(zone);

    printf("to free 2\n");
    rmqueue(&zone, 1);
    dump_free_list(zone);

    printf("to free 4\n");
    rmqueue(&zone, 2);
    dump_free_list(zone);

    printf("to free 8\n");
    rmqueue(&zone, 3);
    dump_free_list(zone);

    printf("to free 16\n");
    rmqueue(&zone, 4);
    dump_free_list(zone);
}


