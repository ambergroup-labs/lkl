/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_LKL_KASAN_H
#define _ASM_LKL_KASAN_H

#include <linux/const.h>
#include <uapi/asm/host_ops.h>
extern unsigned long memory_start, memory_end;
extern unsigned long lkl_kasan_shadow_start;
extern unsigned long lkl_kasan_shadow_end;
extern unsigned long lkl_kasan_stack_start;
extern unsigned long lkl_kasan_stack_end;
extern unsigned long lkl_kasan_stack_shadow_start;
extern unsigned long lkl_kasan_stack_shadow_end;

#define KASAN_SHADOW_SCALE_SHIFT 3
#define KASAN_SHADOW_OFFSET (lkl_kasan_shadow_start - (memory_start >> KASAN_SHADOW_SCALE_SHIFT))
#define KASAN_STACK_SHADOW_OFFSET (lkl_kasan_stack_shadow_start - (lkl_kasan_stack_start >> KASAN_SHADOW_SCALE_SHIFT))
/*
 * Compiler uses shadow offset assuming that addresses start
 * from 0. Kernel addresses don't start from 0, so shadow
 * for kernel really starts from compiler's shadow offset +
 * 'kernel address space start' >> KASAN_SHADOW_SCALE_SHIFT
 */
//#define KASAN_SHADOW_START	(KASAN_SHADOW_OFFSET - (memory_start >> KASAN_SHADOW_SCALE_SHIFT))
#define KASAN_SHADOW_START	(lkl_kasan_shadow_start)
/*
 * 47 bits for kernel address -> (47 - KASAN_SHADOW_SCALE_SHIFT) bits for shadow
 * 56 bits for kernel address -> (56 - KASAN_SHADOW_SCALE_SHIFT) bits for shadow
 */
//#define KASAN_SHADOW_END	(KASAN_SHADOW_START + (mem_size >> KASAN_SHADOW_SCALE_SHIFT))
#define KASAN_SHADOW_END	(lkl_kasan_shadow_end)
#ifndef __ASSEMBLY__

#ifdef CONFIG_KASAN
void kasan_early_init(void);
void kasan_init(void);
unsigned long lkl_kasan_init2(struct lkl_host_operations* lkl_ops, unsigned long mem_sz);
#else
static inline void kasan_early_init(void) { }
static inline void kasan_init(void) { }
#endif

#endif //ASSEM

#endif //KASAN_H
