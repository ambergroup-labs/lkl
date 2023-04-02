#ifndef _ASM_LKL_SYSCALL_H
#define _ASM_LKL_SYSCALL_H

static inline int syscall_get_arch(struct task_struct *task)
{
	//if (is_compat_thread(task_thread_info(task)))
	//	return AUDIT_ARCH_ARM;

	//return AUDIT_ARCH_AARCH64;
	return 0;
}

#endif /* _ASM_LKL_SYSCALL_H */
