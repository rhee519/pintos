#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

#include "user/syscall.h" /* System call numbers. */

void syscall_init(void);

/**
 * [PROJECT-1] Jiho Rhee
 * syscall function definition.
 */
void syscall_halt(void);
void syscall_exit(int status);
pid_t syscall_exec(const char *file);
int syscall_wait(pid_t pid);
int syscall_read(int fd, void *buffer, unsigned size);
int syscall_write(int fd, const void *buffer, unsigned size);

/* Return nth Fibonacci number. */
int syscall_fibonacci(int num);

/* Return maximum of 4 integers given. */
int syscall_max_of_four_int(int num1, int num2, int num3, int num4);

/**
 * [PROJECT-2] Jiho Rhee
 * filesys syscall functions.
 */
bool syscall_create(const char *file, unsigned initial_size);
bool syscall_remove(const char *file);
int syscall_open(const char *file);
int syscall_filesize(int fd);
int syscall_read(int fd, void *buffer, unsigned size);
int syscall_write(int fd, const void *buffer, unsigned size);
void syscall_seek(int fd, unsigned position);
unsigned syscall_tell(int fd);
void syscall_close(int fd);

#endif /* userprog/syscall.h */
