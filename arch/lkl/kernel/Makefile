# disable kasan for arch/lkl/kernel folder
KASAN_SANITIZE := n

extra-y := vmlinux.lds

obj-y = setup.o threads.o irq.o time.o syscalls.o misc.o console.o \
	syscalls_32.o cpu.o
