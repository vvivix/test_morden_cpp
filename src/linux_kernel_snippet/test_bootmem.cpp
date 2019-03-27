
#include <sys/mman.h>
#include "gtest/gtest.h"

int numnodes = 1;	/* Initialized for UMA platforms */


#define BUG()		__bug(__FILE__, __LINE__, NULL)

#define	KERN_EMERG	"<0>"	/* system is unusable			*/
#define	KERN_ALERT	"<1>"	/* action must be taken immediately	*/
#define	KERN_CRIT	"<2>"	/* critical conditions			*/
#define	KERN_ERR	"<3>"	/* error conditions			*/
#define	KERN_WARNING	"<4>"	/* warning conditions			*/
#define	KERN_NOTICE	"<5>"	/* normal but significant condition	*/
#define	KERN_INFO	"<6>"	/* informational			*/
#define	KERN_DEBUG	"<7>"	/* debug-level messages			*/

void __bug(const char *file, int line, void *data)
{
    printf(KERN_CRIT"kernel BUG at %s:%d!", file, line);
    if (data)
        printf(KERN_CRIT" - extra data = %p", data);
    printf("\n");
    exit(-1);
}

/*
 * Page offset: = 3GB
 */
#define PAGE_OFFSET	(0xC0000000UL)
#define PHYS_OFFSET     (0x08000000UL)

#define __virt_to_phys__is_a_macro
#define __phys_to_virt__is_a_macro
#define __virt_to_phys(vpage) (vpage - PAGE_OFFSET + PHYS_OFFSET)
#define __phys_to_virt(ppage) (ppage - PHYS_OFFSET + PAGE_OFFSET)

#define __virt_to_bus__is_a_macro
#define __virt_to_bus(x)	__virt_to_phys(x)
#define __bus_to_virt__is_a_macro
#define __bus_to_virt(x)	__phys_to_virt(x)

#define PHYS_TO_NID(addr)	(0)

#define __pa(x)			__virt_to_phys((unsigned long)(x))
#define __va(x)			((void *)__phys_to_virt((unsigned long)(x)))

static inline unsigned long virt_to_phys(volatile void *x)
{
    return __virt_to_phys((unsigned long)(x));
}

static inline void *phys_to_virt(unsigned long x)
{
    return (void *)(__phys_to_virt((unsigned long)(x)));
}


#define PAGE_SHIFT	12
#define PAGE_SIZE       (1UL << PAGE_SHIFT)
#define PAGE_MASK       (~(PAGE_SIZE-1))
#define PAGE_ALIGN(addr)	(((addr)+PAGE_SIZE-1)&PAGE_MASK)

#define NR_NODES	1
#define O_PFN_DOWN(x)	((x) >> PAGE_SHIFT)
#define V_PFN_DOWN(x)	O_PFN_DOWN(__pa(x))

#define O_PFN_UP(x)	(PAGE_ALIGN(x) >> PAGE_SHIFT)
#define V_PFN_UP(x)	O_PFN_UP(__pa(x))

#define PFN_SIZE(x)	((x) >> PAGE_SHIFT)
#define PFN_RANGE(s,e)	PFN_SIZE(PAGE_ALIGN((unsigned long)(e)) - \
				(((unsigned long)(s)) & PAGE_MASK))

#define DOMAIN_USER	0
#define DOMAIN_KERNEL	1  /* 可在DOMAIN_CLIENT和DOMAIN_MANAGER之间切换*/
#define DOMAIN_TABLE	1
#define DOMAIN_IO	2  /* add by sun ??? 访问权限是什么?*/


#define L_PTE_PRESENT		(1 << 0)
#define L_PTE_YOUNG		(1 << 1)
#define L_PTE_BUFFERABLE	(1 << 2)
#define L_PTE_CACHEABLE		(1 << 3)
#define L_PTE_USER		(1 << 4)
#define L_PTE_WRITE		(1 << 5)
#define L_PTE_EXEC		(1 << 6)
#define L_PTE_DIRTY		(1 << 7)

/* PMD types (actually level 1 descriptor) */
#define PMD_TYPE_MASK		0x0003
#define PMD_TYPE_FAULT		0x0000
#define PMD_TYPE_TABLE		0x0001
#define PMD_TYPE_SECT		0x0002
#define PMD_UPDATABLE		0x0010
#define PMD_SECT_CACHEABLE	0x0008
#define PMD_SECT_BUFFERABLE	0x0004
#define PMD_SECT_AP_WRITE	0x0400
#define PMD_SECT_AP_READ	0x0800
#define PMD_DOMAIN(x)		((x) << 5)


#define PMD_SHIFT		20
#define PGDIR_SHIFT		20

#define PMD_SIZE		(1UL << PMD_SHIFT)
#define PMD_MASK		(~(PMD_SIZE-1))
#define PGDIR_SIZE		(1UL << PGDIR_SHIFT)
#define PGDIR_MASK		(~(PGDIR_SIZE-1))

#define PTRS_PER_PTE		256
#define PTRS_PER_PMD		1
#define PTRS_PER_PGD		4096

/* PMD types (actually level 1 descriptor) */
#define PMD_TYPE_MASK		0x0003
#define PMD_TYPE_FAULT		0x0000
#define PMD_TYPE_TABLE		0x0001
#define PMD_TYPE_SECT		0x0002
#define PMD_UPDATABLE		0x0010
#define PMD_SECT_CACHEABLE	0x0008
#define PMD_SECT_BUFFERABLE	0x0004
#define PMD_SECT_AP_WRITE	0x0400
#define PMD_SECT_AP_READ	0x0800
#define PMD_DOMAIN(x)		((x) << 5)

/* PTE types (actually level 2 descriptor) */
#define PTE_TYPE_MASK		0x0003
#define PTE_TYPE_FAULT		0x0000
#define PTE_TYPE_LARGE		0x0001
#define PTE_TYPE_SMALL		0x0002
#define PTE_AP_READ		0x0aa0
#define PTE_AP_WRITE		0x0550
#define PTE_CACHEABLE		0x0008
#define PTE_BUFFERABLE		0x0004

typedef struct { unsigned int pte; } pte_t;
typedef struct { unsigned int pmd; } pmd_t;
typedef struct { unsigned int pgd; } pgd_t;
typedef struct { unsigned int pgprot; } pgprot_t;

#define pte_val(x)      ((x).pte)
#define pmd_val(x)      ((x).pmd)
#define pgd_val(x)      ((x).pgd)
#define pgprot_val(x)   ((x).pgprot)

#define __pte(x)        ((pte_t) { (x) } )
#define __pmd(x)        ((pmd_t) { (x) } )
#define __pgd(x)        ((pgd_t) { (x) } )
#define __pgprot(x)     ((pgprot_t) { (x) } )

#define pmd_none(pmd)		(!pmd_val(pmd))

/*
 * Conversion functions: convert a page and protection to a page entry,
 * and a page entry and page directory to the page they refer to.
 */
static inline pte_t mk_pte_phys(unsigned long physpage, pgprot_t pgprot)
{
    pte_t pte;
    pte_val(pte) = physpage | pgprot_val(pgprot);
    return pte;
}

static inline void cpu_set_pmd(pmd_t* pmdp, pmd_t pmd) {
    // CPU special. a simple way to simulate.
    *pmdp = pmd;
    if (pmd_val(pmd)) {
        printf("pmd addr [0x%08x] = 0x%08x\n", (void*)pmdp, pmd_val(pmd));
    }
}

static inline void cpu_set_pte(pte_t* ptep, pte_t pte) {
    //    str	r1, [r0], #-1024		@ linux version
    //
    //    eor	r1, r1, #LPTE_PRESENT | LPTE_YOUNG | LPTE_WRITE | LPTE_DIRTY  @sunjh 清除相应的位?
    //
    //    bic	r2, r1, #0xff0                @sunjh 清除11~4BIT
    //    bic	r2, r2, #3                    @sunjh 清除1~0BIT
    //    orr	r2, r2, #HPTE_TYPE_SMALL      @sunjh设置成SmallPage
    //
    //    tst	r1, #LPTE_USER | LPTE_EXEC	@ User or Exec? @sunjh 有设置User/Exec标记?
    //    orrne	r2, r2, #HPTE_AP_READ   @sunjh 是的,则设置READ标记
    //
    //    tst	r1, #LPTE_WRITE | LPTE_DIRTY	@ Write and Dirty?
    //    orreq	r2, r2, #HPTE_AP_WRITE  @sunjh 是的,则设置WRITE标记,注意504行,因此使用eq而不是ne
    //
    //    tst	r1, #LPTE_PRESENT | LPTE_YOUNG	@ Present and Young? @sunjh,物理页不存在或将swap out?
    //    movne	r2, #0  @sunjh 是的,直接使用缺页值
    //
    //    str	r2, [r0]			@ hardware version @sunjh 更新二级表项
    //    mov	r0, r0                                 @sunjh 空操作,等待流水线
    //    mcr	p15, 0, r0, c7, c10, 1		@ clean D entry  @清空CPU DCache 为什么没有清空ICache
    //    mcr	p15, 0, r0, c7, c10, 4		@ drain WB
    //    mov	pc, lr

    // TODO: WATCH OUT , MAYBE SOME BUG HERE.
    pte_t h_pte, *h_ptep = ptep - 1024; // hardware version pte
    *ptep = pte;
    if (pte_val(pte)) {
        printf("pte addr [0x%08x] = 0x%08x\n", (void*)ptep, pte_val(pte));
    }

    pte_val(h_pte) = pte_val(pte) ^ (L_PTE_PRESENT | L_PTE_YOUNG | L_PTE_WRITE | L_PTE_DIRTY);
    pte_val(h_pte) &= 0xfffff00C; //  清除11~4BIT, 0,1 BIT
    pte_val(h_pte) |= PTE_TYPE_SMALL;

    if ((pte_val(pte) & L_PTE_USER) || (pte_val(pte) & L_PTE_EXEC)) {
        pte_val(h_pte) |= PTE_AP_READ;
    }

    if ((pte_val(pte) & L_PTE_WRITE) && (pte_val(pte) & L_PTE_DIRTY)) {
        pte_val(h_pte) |= PTE_AP_WRITE;
    }

    if ((pte_val(pte) & L_PTE_USER) || (pte_val(pte) & L_PTE_EXEC)) {
        pte_val(h_pte) |= PTE_AP_READ;
    }

    if ((pte_val(pte) & L_PTE_PRESENT) && (pte_val(pte) & L_PTE_YOUNG)) {
        pte_val(h_pte) = 0;
    }

    *h_ptep = h_pte;
}

#define set_pmd(pmdp,pmd)	cpu_set_pmd(pmdp,pmd)
#define set_pte(ptep, pte)	cpu_set_pte(ptep,pte)

static inline pmd_t __mk_pmd(pte_t *ptep, unsigned long prot)
{
    unsigned long pte_ptr = (unsigned long)ptep;
    pmd_t pmd;

    pte_ptr -= PTRS_PER_PTE * sizeof(uint32_t);

    /*
     * The pmd must be loaded with the physical
     * address of the PTE table
     */
    pmd_val(pmd) = __virt_to_phys(pte_ptr) | prot;

    return pmd;
}

#define _PAGE_USER_TABLE	(PMD_TYPE_TABLE | PMD_DOMAIN(DOMAIN_USER))
#define _PAGE_KERNEL_TABLE	(PMD_TYPE_TABLE | PMD_DOMAIN(DOMAIN_KERNEL))

struct list_head {
    struct list_head *next, *prev;
};

#define LIST_HEAD_INIT(name) { &(name), &(name) }


static inline unsigned long pmd_page(pmd_t pmd)
{
    unsigned long ptr;

    ptr = pmd_val(pmd) & ~(PTRS_PER_PTE * sizeof(uint32_t) - 1);

    ptr += PTRS_PER_PTE * sizeof(uint32_t);

    return __phys_to_virt(ptr);
}


/* Find an entry in the second-level page table.. */
#define pmd_offset(dir, addr)	((pmd_t *)(dir))

/* Find an entry in the third-level page table.. */
#define __pte_offset(addr)	(((addr) >> PAGE_SHIFT) & (PTRS_PER_PTE - 1))
#define pte_offset(dir, addr)	((pte_t *)pmd_page(*(dir)) + __pte_offset(addr))



typedef struct { volatile int counter; } atomic_t;

typedef struct rb_node_s
{
    struct rb_node_s * rb_parent;
    int rb_color;
#define	RB_RED		0
#define	RB_BLACK	1
    struct rb_node_s * rb_right;
    struct rb_node_s * rb_left;
} rb_node_t;

typedef struct rb_root_s
{
    struct rb_node_s * rb_node;
} rb_root_t;

typedef struct {
    volatile unsigned long lock;
} spinlock_t;

/* The ARM doesn't have a mmu context */
typedef struct { } mm_context_t;

typedef __signed__ int __s32;

struct rw_semaphore {
    __s32			activity;
    spinlock_t		wait_lock;
    struct list_head	wait_list;
#if RWSEM_DEBUG
    int			debug;
#endif
};

#define ATOMIC_INIT(i)	{ (i) }
#define RB_ROOT	(rb_root_t) { NULL, }


#define __RWSEM_INITIALIZER(name) \
{ 0, SPIN_LOCK_UNLOCKED, LIST_HEAD_INIT((name).wait_list) }

#define SPIN_LOCK_UNLOCKED (spinlock_t) { 0 }

static unsigned long swapper_pg_dir = 0xc0008000 - 0x4000;

struct mm_struct {
    mm_struct() {
        mm_rb=		RB_ROOT;
	    pgd=		(pgd_t *)swapper_pg_dir;
	    mm_users=	ATOMIC_INIT(2);
	    mm_count=	ATOMIC_INIT(1);
	    mmap_sem=	__RWSEM_INITIALIZER(mmap_sem);
	    page_table_lock= SPIN_LOCK_UNLOCKED;
	    mmlist=		LIST_HEAD_INIT(mmlist);
    }

    struct vm_area_struct * mmap;		/* list of VMAs */
    rb_root_t mm_rb;
    struct vm_area_struct * mmap_cache;	/* last find_vma result */
    pgd_t * pgd;
    atomic_t mm_users;			/* How many users with user space? */
    atomic_t mm_count;			/* How many references to "struct mm_struct" (users count as 1) */
    int map_count;				/* number of VMAs */
    struct rw_semaphore mmap_sem;
    spinlock_t page_table_lock;		/* Protects task page tables and mm->rss */

    struct list_head mmlist;		/* List of all active mm's.  These are globally strung
						 * together off init_mm.mmlist, and are protected
						 * by mmlist_lock
						 */

    unsigned long start_code, end_code, start_data, end_data;
    unsigned long start_brk, brk, start_stack;
    unsigned long arg_start, arg_end, env_start, env_end;
    unsigned long rss, total_vm, locked_vm;
    unsigned long def_flags;
    unsigned long cpu_vm_mask;
    unsigned long swap_address;

    unsigned dumpable:1;

    /* Architecture-specific MM context */
    mm_context_t context;
};

struct mm_struct init_mm;

#define pgd_index(addr)		((addr) >> PGDIR_SHIFT)
#define pgd_offset(mm, addr)	((char*)(mm)->pgd+sizeof(int)*pgd_index(addr))

/* to find an entry in a kernel page-table-directory */
#define pgd_offset_k(addr)	pgd_offset(&init_mm, addr)

#define __init		__attribute__ ((__section__ (".text.init")))
#define __initdata	__attribute__ ((__section__ (".data.init")))

struct node_info {
    unsigned int start;
    unsigned int end;
    int bootmap_pages;
};

#define pmd_clear(pmdp)		set_pmd(pmdp, __pmd(0))

/*
 * Clear any PGD mapping.  On a two-level page table system,
 * the clearance is done by the middle-level functions (pmd)
 * rather than the top-level (pgd) functions.
 */
static inline void clear_mapping(unsigned long virt)
{
    pmd_clear(pmd_offset(pgd_offset_k(virt), virt));
}

/*
 * Access to this subsystem has to be serialized externally. (this is
 * true for the boot process anyway)
 */
unsigned long max_low_pfn;
unsigned long min_low_pfn;
/* return the number of _pages_ that will be allocated for the boot bitmap */
unsigned long __init bootmem_bootmap_pages (unsigned long pages)
{
    unsigned long mapsize;

    mapsize = (pages+7)/8;//单位为BYTE,
    mapsize = (mapsize + ~PAGE_MASK) & PAGE_MASK;
    mapsize >>= PAGE_SHIFT;//单位为page

    return mapsize;
}

/*
 * Memory map description
 */
#define NR_BANKS 8

struct meminfo {
    int nr_banks;
    unsigned long end;
    struct {
        unsigned long start;
        unsigned long size;
        int           node;
    } bank[NR_BANKS];
};



typedef struct free_area_struct {
    struct list_head	free_list;
    unsigned long		*map;
} free_area_t;

#define MAX_ORDER 10

#define PTRS_PER_PTE		256
#define PTRS_PER_PMD		1
#define PTRS_PER_PGD		4096

struct pglist_data;

/*
 * On machines where it is needed (eg PCs) we divide physical memory
 * into multiple physical zones. On a PC we have 3 zones:
 *
 * ZONE_DMA	  < 16 MB	ISA DMA capable memory
 * ZONE_NORMAL	16-896 MB	direct mapped by the kernel
 * ZONE_HIGHMEM	 > 896 MB	only page cache and user processes
 */
typedef struct zone_struct {
    /*
     * Commonly accessed fields:
     */
    spinlock_t		lock;
    unsigned long		free_pages;//空闲页总和
    /*
    min, low, high三个警戒水平
    high   不swapout
    low
    min
    */
    unsigned long		pages_min, pages_low, pages_high;
    int			need_balance;

    /*
     * free areas of different sizes
     */
    free_area_t		free_area[MAX_ORDER];//用于伙伴算法

    /*
     * Discontig memory support fields.
     */
    struct pglist_data	*zone_pgdat;//所在的Node
    struct page		*zone_mem_map;//Page Array
    unsigned long		zone_start_paddr;//开始物理地址(Byte)
    unsigned long		zone_start_mapnr;//zone_mem_map在mem_map的偏移, 对ARM没有

    /*
     * rarely used fields:
     */
    char			*name;
    unsigned long		size;
} zone_t;

#define ZONE_DMA		0
#define ZONE_NORMAL		1
#define ZONE_HIGHMEM		2
#define MAX_NR_ZONES		3

typedef struct zonelist_struct {
    zone_t * zones [MAX_NR_ZONES+1]; // NULL delimited
} zonelist_t;

#define GFP_ZONEMASK	0x0f

typedef struct bootmem_data {
    unsigned long node_boot_start;//内存开始地址,单位BYTE
    unsigned long node_low_pfn;//内存结束地址,单位page
    void *node_bootmem_map;//BITMAP空间
    unsigned long last_offset;//上次分配的页内偏移
    unsigned long last_pos;//上次分配的页索引号
} bootmem_data_t;

struct bootmem_data;
typedef struct pglist_data {
    pglist_data(bootmem_data *bdata) : bdata(bdata) {}
    zone_t node_zones[MAX_NR_ZONES];
    zonelist_t node_zonelists[GFP_ZONEMASK+1];
    int nr_zones;// 有效node_zones的最大值 1~3,用于swap
    struct page *node_mem_map;//page array
    unsigned long *valid_addr_bitmap;//对ARM来说没用
    struct bootmem_data *bdata;//用于支持bootmem allocator
    unsigned long node_start_paddr;//开始物理地址
    unsigned long node_start_mapnr;//node_mem_map在mem_map的偏移量,对ARM没用
    unsigned long node_size;//node_mem_map数组有效个数,包含空洞
    int node_id;//ID号,从0开始, node_id=0区域用于存放kernel
    struct pglist_data *node_next;//用于组成链表
} pg_data_t;

static bootmem_data_t contig_bootmem_data;
static pg_data_t contig_page_data(&contig_bootmem_data);

static pg_data_t *pgdat_list = NULL;//Q:没有初始化? A:__mmap_switched会初始BSS段


static struct meminfo meminfo __initdata = { 0, };

struct map_desc {
    unsigned long virt;
    unsigned long physical;
    unsigned long length;
    int domain:4,
            prot_read:1,
            prot_write:1,
            cacheable:1,
            bufferable:1,
            last:1;
};

#define NODE_DATA(nid)		(&contig_page_data)

static unsigned long _stext = 0xc0008000;
static unsigned long _end = 0xc0308000;

/*
 * FIXME: We really want to avoid allocating the bootmap bitmap
 * over the top of the initrd.  Hopefully, this is located towards
 * the start of a bank, so if we allocate the bootmap bitmap at
 * the end, we won't clash.
 */
static unsigned int __init
find_bootmap_pfn(int node, struct meminfo *mi, unsigned int bootmap_pages)
{
    unsigned int start_pfn, bank, bootmap_pfn;

    start_pfn   = V_PFN_UP(_end);
    bootmap_pfn = 0;

    for (bank = 0; bank < mi->nr_banks; bank ++) {
        unsigned int start, end;

        if (mi->bank[bank].node != node)
            continue;

        start = O_PFN_UP(mi->bank[bank].start);
        end   = O_PFN_DOWN(mi->bank[bank].size +
                           mi->bank[bank].start);

        if (end < start_pfn)
            continue;

        if (start < start_pfn)
            start = start_pfn;

        if (end <= start)
            continue;

        if (end - start >= bootmap_pages) {
            bootmap_pfn = start;
            break;
        }
    }

    if (bootmap_pfn == 0)
        BUG();

    return bootmap_pfn;
}

/*
 * Scan the memory info structure and pull out:
 *  - the end of memory
 *  - the number of nodes
 *  - the pfn range of each node
 *  - the number of bootmem bitmap pages
 */
static unsigned int __init
find_memend_and_nodes(struct meminfo *mi, struct node_info *np)
{
    unsigned int i, bootmem_pages = 0, memend_pfn = 0;

    for (i = 0; i < NR_NODES; i++) {
        np[i].start = -1U;
        np[i].end = 0;
        np[i].bootmap_pages = 0;
    }

    for (i = 0; i < mi->nr_banks; i++) {
        unsigned long start, end;
        int node;

        if (mi->bank[i].size == 0) {
            /*
             * Mark this bank with an invalid node number
             */
            mi->bank[i].node = -1;
            continue;
        }

        node = mi->bank[i].node;

        if (node >= numnodes) {
            numnodes = node + 1;

            /*
             * Make sure we haven't exceeded the maximum number
             * of nodes that we have in this configuration.  If
             * we have, we're in trouble.  (maybe we ought to
             * limit, instead of bugging?)
             */
            if (numnodes > NR_NODES)
                BUG();
        }

        /*
         * Get the start and end pfns for this bank
         */
        start = O_PFN_UP(mi->bank[i].start);
        end   = O_PFN_DOWN(mi->bank[i].start + mi->bank[i].size);

        if (np[node].start > start)
            np[node].start = start;

        if (np[node].end < end)
            np[node].end = end;

        if (memend_pfn < end)
            memend_pfn = end;
    }

    /*
     * Calculate the number of pages we require to
     * store the bootmem bitmaps.
     */
    for (i = 0; i < numnodes; i++) {
        if (np[i].end == 0)
            continue;

        np[i].bootmap_pages = bootmem_bootmap_pages(np[i].end -
                                                    np[i].start);
        bootmem_pages += np[i].bootmap_pages;
    }

    /*
     * This doesn't seem to be used by the Linux memory
     * manager any more.  If we can get rid of it, we
     * also get rid of some of the stuff above as well.
     */
    max_low_pfn = memend_pfn - O_PFN_DOWN(PHYS_OFFSET);
    mi->end = memend_pfn << PAGE_SHIFT;

    return bootmem_pages;
}

static int __init check_initrd(struct meminfo *mi)
{
    int initrd_node = -2;
    return initrd_node;
}


/*
 * Called once to set up the allocator itself.
 */
static unsigned long __init init_bootmem_core (pg_data_t *pgdat,
                                               unsigned long mapstart, unsigned long start, unsigned long end)
{
    bootmem_data_t *bdata = pgdat->bdata;
    unsigned long mapsize = ((end - start)+7)/8;

    pgdat->node_next = pgdat_list;
    pgdat_list = pgdat;

    mapsize = (mapsize + (sizeof(long) - 1UL)) & ~(sizeof(long) - 1UL);
//    bdata->node_bootmem_map = phys_to_virt(mapstart << PAGE_SHIFT);
    bdata->node_bootmem_map = malloc(mapsize); // modify for test.
    bdata->node_boot_start = (start << PAGE_SHIFT);
    bdata->node_low_pfn = end;

    /*
     * Initially all pages are reserved - setup_arch() has to
     * register free RAM areas explicitly.
     */
    memset(bdata->node_bootmem_map, 0xff, mapsize);

    return mapsize;
}

unsigned long __init init_bootmem_node (pg_data_t *pgdat, unsigned long freepfn, unsigned long startpfn, unsigned long endpfn)
{
    return(init_bootmem_core(pgdat, freepfn, startpfn, endpfn));
}

#define test_and_clear_bit(nr, addr) clear_bit(nr, addr)
#define test_and_set_bit(nr, addr)   set_bit(nr, addr)

extern __inline__ int set_bit(int nr,long * addr)
{
    int	mask, retval;

    addr += nr >> 5;
    mask = 1 << (nr & 0x1f);
//    cli();
    retval = (mask & *addr) != 0;
    *addr |= mask;
//    sti();
    return retval;
}

static inline int clear_bit(int nr, long * addr)
{
    int	mask, retval;

    addr += nr >> 5;
    mask = 1 << (nr & 0x1f);
//    cli();
    retval = (mask & *addr) != 0;
    *addr &= ~mask;
//    sti();
    return retval;
}

__inline__ int test_bit(int nr, long * addr)
{
    int	mask;

    addr += nr >> 5;
    mask = 1 << (nr & 0x1f);
    return ((mask & *addr) != 0);
}


static void __init free_bootmem_core(bootmem_data_t *bdata, unsigned long addr, unsigned long size)
{
    unsigned long i;
    unsigned long start;
    /*
     * round down end of usable mem, partially free pages are
     * considered reserved.
     */
    unsigned long sidx;
    unsigned long eidx = (addr + size - bdata->node_boot_start)/PAGE_SIZE;
    unsigned long end = (addr + size)/PAGE_SIZE;

    if (!size) BUG();
    if (end > bdata->node_low_pfn)
        BUG();

    /*
     * Round up the beginning of the address.
     */
    start = (addr + PAGE_SIZE-1) / PAGE_SIZE;
    sidx = start - (bdata->node_boot_start/PAGE_SIZE);

    for (i = sidx; i < eidx; i++) {
        test_and_clear_bit(i, (long*)bdata->node_bootmem_map);
    }
}

void __init free_bootmem_node (pg_data_t *pgdat, unsigned long physaddr, unsigned long size)
{
    return(free_bootmem_core(pgdat->bdata, physaddr, size));
}

/*
 * Register all available RAM in this node with the bootmem allocator.
 */
static inline void free_bootmem_node_bank(int node, struct meminfo *mi)
{
    pg_data_t *pgdat = NODE_DATA(node);
    int bank;

    for (bank = 0; bank < mi->nr_banks; bank++)
        if (mi->bank[bank].node == node)
            free_bootmem_node(pgdat, mi->bank[bank].start,
                              mi->bank[bank].size);
}

/*
 * Marks a particular physical memory range as unallocatable. Usable RAM
 * might be used for boot-time allocations - or it might get added
 * to the free page pool later on.
 */
static void __init reserve_bootmem_core(bootmem_data_t *bdata, unsigned long addr, unsigned long size)
{
    unsigned long i;
    /*
     * round up, partially reserved pages are considered
     * fully reserved.
     */
    unsigned long sidx = (addr - bdata->node_boot_start)/PAGE_SIZE;
    unsigned long eidx = (addr + size - bdata->node_boot_start +
                          PAGE_SIZE-1)/PAGE_SIZE;
    unsigned long end = (addr + size + PAGE_SIZE-1)/PAGE_SIZE;

    if (!size) BUG();

    if (sidx < 0)
        BUG();
    if (eidx < 0)
        BUG();
    if (sidx >= eidx)
        BUG();
    if ((addr >> PAGE_SHIFT) >= bdata->node_low_pfn)
        BUG();
    if (end > bdata->node_low_pfn)
        BUG();
    for (i = sidx; i < eidx; i++)
        if (test_and_set_bit(i, (long*)bdata->node_bootmem_map))
            printf("hm, page %08lx reserved twice.\n", i*PAGE_SIZE);
}

void __init reserve_bootmem_node (pg_data_t *pgdat, unsigned long physaddr, unsigned long size)
{
    reserve_bootmem_core(pgdat->bdata, physaddr, size);
}
/*
 * Reserve the various regions of node 0
 */
static __init void reserve_node_zero(unsigned int bootmap_pfn, unsigned int bootmap_pages)
{
    pg_data_t *pgdat = NODE_DATA(0);

    /*
     * Register the kernel text and data with bootmem.
     * Note that this can only be in node 0.
     */
    reserve_bootmem_node(pgdat, __pa(_stext), _end - _stext);


    /*
	 * Reserve the page tables.  These are already in use,
	 * and can only be in node 0.
	 */
	reserve_bootmem_node(pgdat, __pa(swapper_pg_dir),
			     PTRS_PER_PGD * sizeof(void *));

    /*
     * And don't forget to reserve the allocator bitmap,
     * which will be freed later.
     */
    reserve_bootmem_node(pgdat, bootmap_pfn << PAGE_SHIFT,
                         bootmap_pages << PAGE_SHIFT);
}

/*
 * Initialise the bootmem allocator for all nodes.  This is called
 * early during the architecture specific initialisation.
 */
void __init bootmem_init(struct meminfo *mi)
{
    struct node_info node_info[NR_NODES], *np = node_info;
    unsigned int bootmap_pages, bootmap_pfn, map_pg;
    int node, initrd_node;

    /*
    added by sunjh
    bootmap 为BootMem Allocator,见struct bootmem_data
    bootmap_pages  BITMAP大小,单位page
    bootmap_pfn   BITMAP开始位置,跟在kernel后面
    */
    bootmap_pages = find_memend_and_nodes(mi, np);/* */
    bootmap_pfn   = find_bootmap_pfn(0, mi, bootmap_pages);/*added by sunjh  */

    initrd_node   = check_initrd(mi);

    map_pg = bootmap_pfn;

    /*
     * Initialise the bootmem nodes.
     *
     * What we really want to do is:
     *
     *   unmap_all_regions_except_kernel();
     *   for_each_node_in_reverse_order(node) {
     *     map_node(node);
     *     allocate_bootmem_map(node);
     *     init_bootmem_node(node);
     *     free_bootmem_node(node);
     *   }
     *
     * but this is a 2.5-type change.  For now, we just set
     * the nodes up in reverse order.
     *
     * (we could also do with rolling bootmem_init and paging_init
     * into one generic "memory_init" type function).
     */
    np += numnodes - 1;
    for (node = numnodes - 1; node >= 0; node--, np--) {//为什么倒着来?
        /*
         * If there are no pages in this node, ignore it.
         * Note that node 0 must always have some pages.
         */
        if (np->end == 0) {
            if (node == 0)
                BUG();
            continue;
        }
        /*
         * Initialise the bootmem allocator.
         */
        init_bootmem_node(NODE_DATA(node), map_pg, np->start, np->end);
        free_bootmem_node_bank(node, mi);
        map_pg += np->bootmap_pages;

        /*
         * If this is node 0, we need to reserve some areas ASAP -
         * we may use bootmem on node 0 to setup the other nodes.
         */
        if (node == 0)
            reserve_node_zero(bootmap_pfn, bootmap_pages);
    }

    if (map_pg != bootmap_pfn + bootmap_pages)
        BUG();

}

#define alloc_bootmem(x) \
	__alloc_bootmem((x), SMP_CACHE_BYTES, __pa(MAX_DMA_ADDRESS))
#define alloc_bootmem_low(x) \
	__alloc_bootmem((x), SMP_CACHE_BYTES, 0)
#define alloc_bootmem_pages(x) \
	__alloc_bootmem((x), PAGE_SIZE, __pa(MAX_DMA_ADDRESS))
#define alloc_bootmem_low_pages(x) \
	__alloc_bootmem((x), PAGE_SIZE, 0)

/*
* alignment has to be a power of 2 value.
*/
static void * __init __alloc_bootmem_core (bootmem_data_t *bdata,
                                           unsigned long size, unsigned long align, unsigned long goal)
{
    unsigned long i, start = 0;
    void *ret;
    unsigned long offset, remaining_size;
    unsigned long areasize, preferred, incr;
    unsigned long eidx = bdata->node_low_pfn - (bdata->node_boot_start >>
                                                                       PAGE_SHIFT);

    if (!size) BUG();

    if (align & (align-1))
        BUG();

    offset = 0;
    if (align &&
        (bdata->node_boot_start & (align - 1UL)) != 0)
        offset = (align - (bdata->node_boot_start & (align - 1UL)));
    offset >>= PAGE_SHIFT;

    /*
     * We try to allocate bootmem pages above 'goal'
     * first, then we try to allocate lower pages.
     */
    if (goal && (goal >= bdata->node_boot_start) &&
        ((goal >> PAGE_SHIFT) < bdata->node_low_pfn)) {
        preferred = goal - bdata->node_boot_start;
    } else
        preferred = 0;

    preferred = ((preferred + align - 1) & ~(align - 1)) >> PAGE_SHIFT;
    preferred += offset;
    areasize = (size+PAGE_SIZE-1)/PAGE_SIZE;
    incr = align >> PAGE_SHIFT ? : 1;

restart_scan:
    for (i = preferred; i < eidx; i += incr) {
        unsigned long j;
        if (test_bit(i, (long*)bdata->node_bootmem_map))
            continue;
        for (j = i + 1; j < i + areasize; ++j) {
            if (j >= eidx)
                goto fail_block;
            if (test_bit (j, (long*)bdata->node_bootmem_map))
                goto fail_block;
        }
        start = i;
        goto found;
fail_block:;
    }
    if (preferred) {
        preferred = offset;
        goto restart_scan;
    }
    return NULL;
found:
    if (start >= eidx)
        BUG();

    /*
     * Is the next page of the previous allocation-end the start
     * of this allocation's buffer? If yes then we can 'merge'
     * the previous partial page with this allocation.
     */
    if (align <= PAGE_SIZE
        && bdata->last_offset && bdata->last_pos+1 == start) {
        offset = (bdata->last_offset+align-1) & ~(align-1);
        if (offset > PAGE_SIZE)
            BUG();
        remaining_size = PAGE_SIZE-offset;
        if (size < remaining_size) {
            areasize = 0;
            // last_pos unchanged
            bdata->last_offset = offset+size;
            ret = phys_to_virt(bdata->last_pos*PAGE_SIZE + offset +
                               bdata->node_boot_start);
        } else {
            remaining_size = size - remaining_size;
            areasize = (remaining_size+PAGE_SIZE-1)/PAGE_SIZE;
            ret = phys_to_virt(bdata->last_pos*PAGE_SIZE + offset +
                               bdata->node_boot_start);
            bdata->last_pos = start+areasize-1;
            bdata->last_offset = remaining_size;
        }
        bdata->last_offset &= ~PAGE_MASK;
    } else {
        bdata->last_pos = start + areasize - 1;
        bdata->last_offset = size & ~PAGE_MASK;
        ret = phys_to_virt(start * PAGE_SIZE + bdata->node_boot_start);
    }
    /*
     * Reserve the area now:
     */
    for (i = start; i < start+areasize; i++)
        if (test_and_set_bit(i, (long*)bdata->node_bootmem_map))
            BUG();
    memset(ret, 0, size);
    return ret;
}

void * __init __alloc_bootmem (unsigned long size, unsigned long align, unsigned long goal)
{
    pg_data_t *pgdat = pgdat_list;
    void *ptr;

    while (pgdat) {
        if ((ptr = __alloc_bootmem_core(pgdat->bdata, size,
                                        align, goal)))
            return(ptr);
        pgdat = pgdat->node_next;
    }
    /*
     * Whoops, we cannot satisfy the allocation request.
     */
    printf(KERN_ALERT "bootmem alloc of %lu bytes failed!\n", size);
    printf("Out of memory");
    return NULL;
}

static inline void
alloc_init_page(unsigned long virt, unsigned long phys, int domain, int prot)
{
    pmd_t *pmdp;
    pte_t *ptep;

    pmdp = pmd_offset(pgd_offset_k(virt), virt);

    if (pmd_none(*pmdp)) {
        pte_t *ptep = (pte_t*)alloc_bootmem_low_pages(2 * PTRS_PER_PTE *
                                              sizeof(pte_t));

        ptep += PTRS_PER_PTE;

        set_pmd(pmdp, __mk_pmd(ptep, PMD_TYPE_TABLE | PMD_DOMAIN(domain)));
    }
    ptep = pte_offset(pmdp, virt);

    set_pte(ptep, mk_pte_phys(phys, __pgprot((unsigned long)prot)));
}

/*
 * Create a SECTION PGD between VIRT and PHYS in domain
 * DOMAIN with protection PROT
 */
static inline void
alloc_init_section(unsigned long virt, unsigned long phys, int prot)
{
    pmd_t pmd;

    pmd_val(pmd) = phys | prot;

    set_pmd(pmd_offset(pgd_offset_k(virt), virt), pmd);
}
/*
 * Create the page directory entries and any necessary
 * page tables for the mapping specified by `md'.  We
 * are able to cope here with varying sizes and address
 * offsets, and we take full advantage of sections.
 */
static void __init create_mapping(struct map_desc *md)
{
    unsigned long virt, length;
    int prot_sect, prot_pte;
    long off;

    prot_pte = L_PTE_PRESENT | L_PTE_YOUNG | L_PTE_DIRTY |
               (md->prot_read  ? L_PTE_USER       : 0) |
               (md->prot_write ? L_PTE_WRITE      : 0) |
               (md->cacheable  ? L_PTE_CACHEABLE  : 0) |
               (md->bufferable ? L_PTE_BUFFERABLE : 0);

    prot_sect = PMD_TYPE_SECT | PMD_DOMAIN(md->domain) |
                (md->prot_read  ? PMD_SECT_AP_READ    : 0) |
                (md->prot_write ? PMD_SECT_AP_WRITE   : 0) |
                (md->cacheable  ? PMD_SECT_CACHEABLE  : 0) |
                (md->bufferable ? PMD_SECT_BUFFERABLE : 0);

    virt   = md->virt;
    off    = md->physical - virt;
    length = md->length;

    while ((virt & 0xfffff || (virt + off) & 0xfffff) && length >= PAGE_SIZE) {
        alloc_init_page(virt, virt + off, md->domain, prot_pte);

        virt   += PAGE_SIZE;
        length -= PAGE_SIZE;
    }

    while (length >= PGDIR_SIZE) {
        alloc_init_section(virt, virt + off, prot_sect);

        virt   += PGDIR_SIZE;
        length -= PGDIR_SIZE;
    }

    while (length >= PAGE_SIZE) {
        alloc_init_page(virt, virt + off, md->domain, prot_pte);

        virt   += PAGE_SIZE;
        length -= PAGE_SIZE;
    }
}

/*
 * RAM definitions
 */
#define FLUSH_BASE_PHYS		0x80000000
#define FLUSH_BASE		0xdf000000
/*
 * Setup initial mappings.  We use the page we allocated for zero page to hold
 * the mappings, which will get overwritten by the vectors in trap_init().
 * The mappings must be in virtual address order.
 @SunjhQ: init_maps没有释放?
 */
void __init memtable_init(struct meminfo *mi)
{
    struct map_desc *init_maps, *p, *q;
    unsigned long address = 0;
    int i;

    init_maps = p = (struct map_desc *)alloc_bootmem_low_pages(PAGE_SIZE);
    for (i = 0; i < mi->nr_banks; i++) {
        if (mi->bank[i].size == 0)
            continue;

        p->physical   = mi->bank[i].start;
        p->virt    = __phys_to_virt(p->physical);
        p->length     = mi->bank[i].size;
        p->domain     = DOMAIN_KERNEL;
        /*
         * 内核空间权限: 内核可读写,用户不可访问
         */
        p->prot_read  = 0;
        p->prot_write = 1;
        p->cacheable  = 1;
        p->bufferable = 1;

        p ++;
    }


    p->physical   = FLUSH_BASE_PHYS;
	p->virt       = FLUSH_BASE;
	p->length     = PGDIR_SIZE;
	p->domain     = DOMAIN_KERNEL;
	p->prot_read  = 1;
	p->prot_write = 0;
	p->cacheable  = 1;
	p->bufferable = 1;

	p ++;

    /*
     * Go through the initial mappings, but clear out any
     * pgdir entries that are not in the description.
     */
    q = init_maps;
    do {
        if (address < q->virt || q == p) {
            clear_mapping(address);
            address += PGDIR_SIZE;
        } else {
            create_mapping(q);

            address = q->virt + q->length;
            address = (address + PGDIR_SIZE - 1) & PGDIR_MASK;

            q ++;
        }
    } while (address < UINT32_MAX );

    /*
     * Create a mapping for the machine vectors at virtual address 0
     * or 0xffff0000.  We should always try the high mapping.
     */
    init_maps->physical   = virt_to_phys(init_maps);
//    init_maps->virt    = vectors_base();
    init_maps->virt    = 0xffff0000;
    init_maps->length     = PAGE_SIZE;
    init_maps->domain     = DOMAIN_USER;
    /*
     * Q:user/write都没有置位,那不是不可访问?
     * A:create_mapping会强置dirty,
     *   因此效果与write置位一样;
     *   权限为:内核可读写
     */
    init_maps->prot_read  = 0;
    init_maps->prot_write = 0;
    init_maps->cacheable  = 1;
    init_maps->bufferable = 0;

    create_mapping(init_maps);

//    flush_cache_all(); // empty implement
}

/*
 * This is meant to be invoked by platforms whose physical memory starts
 * at a considerably higher value than 0. Examples are Super-H, ARM, m68k.
 * Should be invoked with paramters (0, 0, unsigned long *[], start_paddr).
 */
void __init free_area_init_node(int nid, pg_data_t *pgdat, struct page *pmap,
                                unsigned long *zones_size, unsigned long zone_start_paddr,
                                unsigned long *zholes_size)
{
    // TODO : add impl
//    free_area_init_core(0, &contig_page_data, &mem_map, zones_size,
//                        zone_start_paddr, zholes_size, pmap);
}

typedef struct page {
    struct list_head list;		/* ->mapping has some page lists. */
    struct address_space *mapping;	/* The inode (or ...) we belong to. */
    /*
    index含义:
    page作为文件映射, index为文件的偏移量;
    page作为swapper?这个没看懂;
    页空闲时,index为物理页的order,见__free_pages_ok;
    */
    unsigned long index;		/* Our offset within mapping. */
    struct page *next_hash;		/* Next page sharing our hash bucket in
					   the pagecache hash table. */
    atomic_t count;			/* Usage count, see below. */
    unsigned long flags;		/* atomic flags, some possibly
					   updated asynchronously */
    struct list_head lru;		/* Pageout list, eg. active_list;
					   protected by pagemap_lru_lock !! */
//    wait_queue_head_t wait;		/* Page locked?  Stand in line... */
    struct page **pprev_hash;	/* Complement to *next_hash. */
    /*
    由于dev的buffer小于page,因此一个page会尽量分成多个buffer使用;
    buffers->b_this_page组成同页的链表;
    */
    struct buffer_head * buffers;	/* Buffer maps us to a disk block. */
    /*
    see kmap_high
    */
    void *virt;			/* Kernel virtual address (NULL if
					   not kmapped, ie. highmem) */
    struct zone_struct *zone;	/* Memory zone we are in. */
} mem_map_t;

mem_map_t * mem_map;
/*
 * empty_zero_page is a special page that is used for
 * zero-initialized data and COW.
 */
struct page *empty_zero_page;
#define virt_to_page(kaddr)	(mem_map + (__pa(kaddr) >> PAGE_SHIFT) - \
				 (PHYS_OFFSET >> PAGE_SHIFT))

/*
 * paging_init() sets up the page tables, initialises the zone memory
 * maps, and sets up the zero page, bad page and bad page tables.
 */
void __init paging_init(struct meminfo *mi)
{
    void *zero_page;
    int node;

    memcpy(&meminfo, mi, sizeof(meminfo));

    /*
     * allocate the zero page.  Note that we count on this going ok.
    @SunjhQ: zero_page作用?
     */
    zero_page = alloc_bootmem_low_pages(PAGE_SIZE);

    /*
     * initialise the page tables.
     */
    memtable_init(mi);
//    flush_tlb_all();

    /*
     * initialise the zones within each node
     */
    for (node = 0; node < numnodes; node++) {
        /*
        zone_size - zhole_size = 内存大小
        */
        unsigned long zone_size[MAX_NR_ZONES];/*内存空间大小(包含空洞) */
        unsigned long zhole_size[MAX_NR_ZONES];/*空洞大小总和 */
        struct bootmem_data *bdata;
        pg_data_t *pgdat;
        int i;

        /*
         * Initialise the zone size information.
         */
        for (i = 0; i < MAX_NR_ZONES; i++) {
            zone_size[i]  = 0;
            zhole_size[i] = 0;
        }

        pgdat = NODE_DATA(node);
        bdata = pgdat->bdata;

        /*
         * The size of this node has already been determined.
         * If we need to do anything fancy with the allocation
         * of this memory to the zones, now is the time to do
         * it.
         */
        zone_size[0] = bdata->node_low_pfn -
                       (bdata->node_boot_start >> PAGE_SHIFT);

        /*
         * If this zone has zero size, skip it.
         */
        if (!zone_size[0])
            continue;

        /*
         * For each bank in this node, calculate the size of the
         * holes.  holes = node_size - sum(bank_sizes_in_node)
         */
        zhole_size[0] = zone_size[0];
        for (i = 0; i < mi->nr_banks; i++) {
            if (mi->bank[i].node != node)
                continue;

            zhole_size[0] -= mi->bank[i].size >> PAGE_SHIFT;
        }

//        /*
//         * Adjust the sizes according to any special
//         * requirements for this machine type.
//         */
//        arch_adjust_zones(node, zone_size, zhole_size);

        free_area_init_node(node, pgdat, 0, zone_size,
                            bdata->node_boot_start, zhole_size);
    }

    /*
     * finish off the bad pages once
     * the mem_map is initialised
     */
    memset(zero_page, 0x00, PAGE_SIZE);
    empty_zero_page = virt_to_page(zero_page);
//    flush_dcache_page(empty_zero_page);
}

TEST(LK_BOOTMEM_INIT, Case1) {
    struct meminfo meminfo;
    meminfo.nr_banks      = 1;
    meminfo.bank[0].start = PHYS_OFFSET;
    meminfo.bank[0].size  = 16*1024*1024;
    meminfo.bank[0].node  = PHYS_TO_NID(PHYS_OFFSET);

    void* sys_map = mmap((void*)PAGE_OFFSET, 16*1024*1024, PROT_WRITE |PROT_READ, MAP_FIXED|MAP_PRIVATE |MAP_ANONYMOUS , -1, 0);

    ASSERT_EQ(sys_map, (void*)PAGE_OFFSET) << "[errno]: " << strerror(errno);
    bootmem_init(&meminfo);
    paging_init(&meminfo);
}
