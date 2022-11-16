#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

/* [PROJECT-1] */
#include "userprog/process.h"
#include <debug.h>            /* ASSERT(), NOT_REACHED() */
#include "devices/shutdown.h" /* shutdown_power_off() */
#include "threads/vaddr.h"    /* check vaddr validity */
#include <console.h>          /* putbuf() */
#include "devices/input.h"    /* input_getc() */
#include <string.h>

/* [PROJECT-2] */
#include "filesys/file.h"
#include "filesys/filesys.h"
#include "threads/synch.h" /* struct lock */

/* synchronization of file syscall APIs */
struct lock filesys_lock;

static void syscall_handler(struct intr_frame *);

/* argc of each syscall functions */
static int syscall_argc[SYS_MAX] = {
    0,
};

static inline void
check_user_addr(const void *vaddr)
{
  // printf("check %p\n", vaddr);
  if (!vaddr || !is_user_vaddr(vaddr) || is_kernel_vaddr(vaddr))
    syscall_exit(-1);
}

void syscall_init(void)
{
  lock_init(&filesys_lock);
  intr_register_int(0x30, 3, INTR_ON, syscall_handler, "syscall");

  /* [PROJECT-1] Initialize argc of each syscall function. */
  syscall_argc[SYS_HALT] = 0;
  syscall_argc[SYS_EXIT] = 1;
  syscall_argc[SYS_EXEC] = 1;
  syscall_argc[SYS_WAIT] = 1;

  /* [PROJECT-2] filesys system call functions. */
  syscall_argc[SYS_CREATE] = 2;
  syscall_argc[SYS_REMOVE] = 1;
  syscall_argc[SYS_OPEN] = 1;
  syscall_argc[SYS_FILESIZE] = 1;
  syscall_argc[SYS_READ] = 3;
  syscall_argc[SYS_WRITE] = 3;
  syscall_argc[SYS_SEEK] = 2;
  syscall_argc[SYS_TELL] = 1;
  syscall_argc[SYS_CLOSE] = 1;

  syscall_argc[SYS_MMAP] = 2;
  syscall_argc[SYS_MUNMAP] = 1;

  syscall_argc[SYS_CHDIR] = 1;
  syscall_argc[SYS_MKDIR] = 1;
  syscall_argc[SYS_READDIR] = 2;
  syscall_argc[SYS_ISDIR] = 1;
  syscall_argc[SYS_INUMBER] = 1;

  /* [PROJECT-1] Additional System Calls. */
  syscall_argc[SYS_FIBO] = 1;
  syscall_argc[SYS_MAXOF4INT] = 4;
}

static void
syscall_handler(struct intr_frame *f UNUSED)
{
  int syscall_num = 0;

  /**
   * [PROJECT-1] Jiho Rhee
   * esp: 4-byte stack pointer.
   * We can read syscall_num & arguments by referencing esp.
   *
   * ex.
   * esp[0] == *(esp) == syscall_num
   * esp[1] == *(esp+1) == *(uint32_t *)(f->esp+4) == arg[0]
   * ...
   */
  uint32_t *esp = f->esp;
  check_user_addr(esp); /* syscall_num */

  syscall_num = *esp;
  if (syscall_num < 0 || syscall_num >= SYS_MAX)
    syscall_exit(-1);

  /* Check if address of each argument is valid. */
  for (int i = 1; i <= syscall_argc[syscall_num]; i++)
    check_user_addr(&esp[i]);

  switch (syscall_num)
  {
  /* Projects 2 and later. */
  case SYS_HALT: /* [PROJECT-1] Halt the operating system. */
    syscall_halt();
    break;
  case SYS_EXIT: /* [PROJECT-1] Terminate this process. */
    syscall_exit((int)esp[1]);
    break;
  case SYS_EXEC: /* [PROJECT-1] Start another process. */
    f->eax = (uint32_t)syscall_exec((const char *)esp[1]);
    break;
  case SYS_WAIT: /* [PROJECT-1] Wait for a child process to die. */
    f->eax = syscall_wait((pid_t)esp[1]);
    break;

  case SYS_CREATE: /* [PROJECT-2] Create a file. */
    f->eax = syscall_create(
        (const char *)esp[1],
        (unsigned int)esp[2]);
    break;
  case SYS_REMOVE: /* [PROJECT-2] Delete a file. */
    f->eax = syscall_remove((const char *)esp[1]);
    break;
  case SYS_OPEN: /* [PROJECT-2] Open a file. */
    f->eax = syscall_open((const char *)esp[1]);
    break;
  case SYS_FILESIZE: /* [PROJECT-2] Obtain a file's size. */
    f->eax = syscall_filesize((int)esp[1]);
    break;
  case SYS_READ: /* [PROJECT-1] [PROJECT-2] Read from a file. */
    f->eax = syscall_read(
        (int)esp[1],
        (void *)esp[2],
        (unsigned)esp[3]);
    break;
  case SYS_WRITE: /* [PROJECT-1] [PROJECT-2] Write to a file. */
    f->eax = syscall_write(
        (int)esp[1],
        (void *)esp[2],
        (unsigned)esp[3]);
    break;
  case SYS_SEEK: /* [PROJECT-2] Change position in a file. */
    syscall_seek(
        (int)esp[1],
        (unsigned int)esp[2]);
    break;
  case SYS_TELL: /* [PROJECT-2] Report current position in a file. */
    f->eax = syscall_tell((int)esp[1]);
    break;
  case SYS_CLOSE: /*[PROJECT-2] Close a file. */
    syscall_close((int)esp[1]);
    break;

  /* Project 3 and optionally project 4. */
  case SYS_MMAP: /* Map a file into memory. */
    // TODO
    break;
  case SYS_MUNMAP: /* Remove a memory mapping. */
    // TODO
    break;

  /* Project 4 only. */
  case SYS_CHDIR: /* Change the current directory. */
    // TODO
    break;
  case SYS_MKDIR: /* Create a directory. */
    // TODO
    break;
  case SYS_READDIR: /* Reads a directory entry. */
    // TODO
    break;
  case SYS_ISDIR: /* Tests if a fd represents a directory. */
    // TODO
    break;
  case SYS_INUMBER: /* Returns the inode number for a fd. */
    // TODO
    break;

  /* [PROJECT-1] Additional System Calls. */
  case SYS_FIBO:
    f->eax = syscall_fibonacci((int)esp[1]);
    break;
  case SYS_MAXOF4INT:
    f->eax = syscall_max_of_four_int(
        (int)esp[1],
        (int)esp[2],
        (int)esp[3],
        (int)esp[4]);
    break;
  default:
    NOT_REACHED();
    break;
  }
}

/**
 * [PROJECT-1] syscall implementation.
 */
void syscall_halt(void)
{
  shutdown_power_off();
}

void syscall_exit(int status)
{
  struct thread *cur = thread_current();
  cur->exit_status = status;
  for (int fd = 0; fd < FD_MAX; fd++)
  {
    syscall_close(fd);
  }

  printf("%s: exit(%d)\n", cur->name, cur->exit_status);
  thread_exit();
}

pid_t syscall_exec(const char *file)
{
  tid_t child_tid = process_execute(file);
  struct thread *child_t, *cur = thread_current();
  struct list_elem *e;

  for (e = list_begin(&cur->child); e != list_end(&cur->child); e = list_next(e))
  {
    child_t = list_entry(e, struct thread, child_elem);

    if (child_tid == child_t->tid)
    {
      if (!child_t->loaded)
        return (pid_t)TID_ERROR;
    }
  }

  return (pid_t)child_tid;
}

int syscall_wait(pid_t pid)
{
  return process_wait((tid_t)pid);
}

int syscall_read(int fd, void *buffer, unsigned size)
{
  check_user_addr(buffer);

  lock_acquire(&filesys_lock);
  int bytes_read = 0;
  if (fd == 0) /* STDIN */
  {
    for (bytes_read = 0; bytes_read < (int)size; bytes_read++)
    {
      uint8_t input = input_getc();
      *(uint8_t *)(buffer + bytes_read) = input;
    }
  }
  else if (fd <= 2) /* STDOUT, STDERR */
  {
    lock_release(&filesys_lock);
    syscall_exit(-1);
  }
  else if (fd >= FD_MAX) /* FD is out-of-range */
  {
    lock_release(&filesys_lock);
    syscall_exit(-1);
  }
  else /* Read file with file descriptor. */
  {
    struct thread *cur = thread_current();
    struct file *f = cur->fd_table[fd];
    if (f == NULL)
    {
      lock_release(&filesys_lock);
      syscall_exit(-1);
    }
    bytes_read = (int)file_read(f, buffer, (off_t)size);
  }

  lock_release(&filesys_lock);
  return bytes_read;
}

int syscall_write(int fd, const void *buffer, unsigned size)
{
  check_user_addr(buffer);

  lock_acquire(&filesys_lock);
  int bytes_written = 0;
  if (fd == 1) /* STDOUT */
  {
    putbuf(buffer, size);
    bytes_written = size;
  }
  else if (fd >= 3) /* Write to file with file descriptor. */
  {
    struct thread *cur = thread_current();
    struct file *f = cur->fd_table[fd];
    if (f == NULL)
    {
      lock_release(&filesys_lock);
      syscall_exit(-1);
    }
    if (!write_denied(f))
    {
      bytes_written = file_write(f, buffer, (off_t)size);
    }
  }

  lock_release(&filesys_lock);
  return bytes_written;
}

/* Return nth Fibonacci number. */
int syscall_fibonacci(int num)
{
  if (num < 0) /* input error */
    return -1;
  if (num == 0 || num == 1) /* base case */
    return num;

  int prev = 1, ret = 1;
  for (int i = 3; i <= num; i++)
  {
    int tmp = ret;
    ret += prev;
    prev = tmp;
  }

  return ret;
}

/* Return maximum of 4 integers given. */
int syscall_max_of_four_int(int num1, int num2, int num3, int num4)
{
  int ret = num1 > num2 ? num1 : num2;
  if (ret < num3)
    ret = num3;
  if (ret < num4)
    ret = num4;

  return ret;
}

/**
 * [PROJECT-2] filesys system call implementation.
 */
bool syscall_create(const char *file, unsigned initial_size)
{
  check_user_addr(file);

  if (file == NULL)
    syscall_exit(-1);

  bool success = filesys_create(file, (off_t)initial_size);
  return success;
}

bool syscall_remove(const char *file)
{
  check_user_addr(file);

  if (file == NULL)
    syscall_exit(-1);

  bool success = filesys_remove(file);
  return success;
}

int syscall_open(const char *file)
{
  check_user_addr(file);

  lock_acquire(&filesys_lock);
  if (file == NULL)
  {
    lock_release(&filesys_lock);
    return -1;
  }

  struct file *fp = filesys_open(file);
  int fd = FD_ERROR;

  // if (f)
  // {
  //   struct thread *cur = thread_current();
  //   fd = cur->fd_max;
  //   if (fd >= FILE_NUM_MAX)
  //   {
  //     file_close(f);
  //     lock_release(&filesys_lock);
  //     return -1;
  //   }
  //   cur->fd_table[cur->fd_max++] = f;
  //   if (!strcmp(cur->name, file))
  //     file_deny_write(f);
  // }

  if (fp == NULL)
  {
    lock_release(&filesys_lock);
    return FD_ERROR;
  }

  struct thread *cur = thread_current();
  for (fd = 3; fd < FD_MAX; fd++)
  {
    if (cur->fd_table[fd] == NULL)
    {
      cur->fd_table[fd] = fp;
      if (!strcmp(cur->name, file))
      {
        file_deny_write(fp);
      }
      break;
    }
  }

  lock_release(&filesys_lock);
  if (fd == FD_MAX)
  {
    return FD_ERROR;
  }

  return fd;
}

int syscall_filesize(int fd)
{
  struct thread *cur = thread_current();
  struct file *f = cur->fd_table[fd];
  if (f == NULL)
  {
    syscall_exit(-1);
  }

  off_t size = file_length(f);
  return size;
}

void syscall_seek(int fd, unsigned position)
{
  struct thread *cur = thread_current();
  struct file *f = cur->fd_table[fd];
  if (f == NULL)
    syscall_exit(-1);

  file_seek(f, (off_t)position);
}

unsigned syscall_tell(int fd)
{
  struct thread *cur = thread_current();
  struct file *f = cur->fd_table[fd];
  if (f == NULL)
    syscall_exit(-1);

  off_t pos = file_tell(f);
  return pos;
}

void syscall_close(int fd)
{
  struct thread *cur = thread_current();
  struct file *f = cur->fd_table[fd];
  file_close(f);
  cur->fd_table[fd] = NULL;
}
