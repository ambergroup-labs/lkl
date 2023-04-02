/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_LKL_STRING_H
#define _ASM_LKL_STRING_H

#define __HAVE_ARCH_MEMCPY
//extern void * memcpy(void *,const void *,__kernel_size_t);
void *__memcpy(void *,const void *,__kernel_size_t);
#define memcpy __memcpy

#define __HAVE_ARCH_MEMSET
void *__memset(void *, int, __kernel_size_t);
#define memset __memset
#endif /* _ASM_LKL_STRING_H */
