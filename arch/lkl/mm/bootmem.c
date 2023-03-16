// SPDX-License-Identifier: GPL-2.0
#include <linux/memblock.h>
#include <linux/mm.h>
#include <linux/swap.h>
#include <asm/kasan.h>
#include <linux/kasan.h>

unsigned long memory_start, memory_end;
static unsigned long _memory_start, mem_size;

void *empty_zero_page;

void __init __no_sanitize_address bootmem_init(unsigned long mem_sz)
{
	mem_size = mem_sz;

	if (lkl_ops->page_alloc) {
		mem_size = PAGE_ALIGN(mem_size);
		_memory_start = (unsigned long)lkl_ops->page_alloc(mem_size);
	} else {
		_memory_start = (unsigned long)lkl_ops->mem_alloc(mem_size);
	}

	memory_start = _memory_start;
	BUG_ON(!memory_start);
	memory_end = memory_start + mem_size;

	if (PAGE_ALIGN(memory_start) != memory_start) {
		mem_size -= PAGE_ALIGN(memory_start) - memory_start;
		memory_start = PAGE_ALIGN(memory_start);
		mem_size = (mem_size / PAGE_SIZE) * PAGE_SIZE;
	}
	pr_info("memblock address range: 0x%lx - 0x%lx\n", memory_start,
		memory_start+mem_size);

#ifdef CONFIG_KASAN
	/* init kasan shadow map */
	lkl_kasan_init2(lkl_ops, mem_size >> KASAN_SHADOW_SCALE_SHIFT);
        pr_info("kasan shadow range: 0x%lx - 0x%lx\n", lkl_kasan_shadow_start,
                lkl_kasan_shadow_end);
        pr_info("mem_base: 0x%lx\n", memory_start);
        pr_info("shadow for mem_base: 0x%lx\n", (unsigned long)(kasan_mem_to_shadow((void *)memory_start)));
        pr_info("KASAN_SHADOW_START:  0x%lx\n", KASAN_SHADOW_START);
        pr_info("KASAN_SHADOW_END:    0x%lx\n", KASAN_SHADOW_END);
#endif

	/*
	 * Give all the memory to the bootmap allocator, tell it to put the
	 * boot mem_map at the start of memory.
	 */
	max_low_pfn = virt_to_pfn(memory_end);
	min_low_pfn = virt_to_pfn(memory_start);
	memblock_add(memory_start, mem_size);

	empty_zero_page = memblock_alloc(PAGE_SIZE, PAGE_SIZE);
	memset((void *)empty_zero_page, 0, PAGE_SIZE);

	{
		unsigned long zones_size[MAX_NR_ZONES] = {0, };
		//zones_size[ZONE_NORMAL] = (mem_size) >> PAGE_SHIFT;
		zones_size[ZONE_NORMAL] = PFN_DOWN(memblock_end_of_DRAM());
		free_area_init(zones_size);
	}
}

void __init mem_init(void)
{
	max_mapnr = (((unsigned long)high_memory) - PAGE_OFFSET) >> PAGE_SHIFT;
	/* this will put all memory onto the freelists */
	memblock_free_all();
	//totalram_pages_add(memblock_free_all());
	pr_info("Memory available: %luk/%luk RAM\n",
		(nr_free_pages() << PAGE_SHIFT) >> 10, mem_size >> 10);
}

/*
 * In our case __init memory is not part of the page allocator so there is
 * nothing to free.
 */
void free_initmem(void)
{
}

void free_mem(void)
{
	if (lkl_ops->page_free)
		lkl_ops->page_free((void *)_memory_start, mem_size);
	else
		lkl_ops->mem_free((void *)_memory_start);
}

void test_kasan(void);
void test_kasan()
{
	char* ti = kmalloc(1000*sizeof(char), GFP_KERNEL);
	// OOB write
	//*(ti+8+1000) = 4;
	// OOB read
	//char tmp = *(ti+8+1000);
	//pr_info("tmp = %c\n", tmp);	// use tmp otherwise GCC will optimize it out
	//*(ti+400+1000) = 4;
	kfree(ti);
	// UAF
	//*(ti + 8) = 4;
	// double free
	kfree(ti);
}
