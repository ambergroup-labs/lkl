#ifndef _ASM_LKL_MMU_CONTEXT_H
#define _ASM_LKL_MMU_CONTEXT_H

static inline void switch_mm(struct mm_struct *prev,
			struct mm_struct *next,
			struct task_struct *tsk)
{
}

#include <asm-generic/mmu_context.h>


#endif /* _ASM_LKL_MMU_CONTEXT_H */
