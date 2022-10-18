#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

/* [PROJECT1] */
#include <debug.h>            /* ASSERT(), NOT_REACHED() */
#include "user/syscall.h"     /* System call numbers. */
#include "devices/shutdown.h" /* shutdown_power_off() */
#include "threads/vaddr.h"    /* check vaddr validity */
#include <console.h>          /* STDIN, STDOUT */

static void syscall_handler(struct intr_frame *);

static void syscall_halt(void);
static void syscall_exit(int status);
static pid_t syscall_exec(const char *file);
static int syscall_wait(pid_t pid);
static int syscall_read(int fd, void *buffer, unsigned size);
static int syscall_write(int fd, const void *buffer, unsigned size);

#define syscall_return(EAX_PTR, VALUE) ({ *EAX_PTR = (int)(VALUE); })

static inline void check_user_addr(const void *vaddr)
{
  if (!vaddr || !is_user_vaddr(vaddr) || is_kernel_vaddr(vaddr))
    syscall_exit(-1);
}

void syscall_init(void)
{
  intr_register_int(0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler(struct intr_frame *f UNUSED)
{
  /**
   * TODO syscall 별로 argument 개수에 따라 address validity 체크해야 함!
   * ex. syscall_write(): arg 3개 ---> f->esp + 4, 8, 12 check!
   */
  check_user_addr(f->esp); /* syscall_num */
  int syscall_num = 0;
  pid_t pid = 0;

  syscall_num = *(uint32_t *)f->esp;

  int status = -1;
  switch (syscall_num)
  {
  /* Projects 2 and later. */
  case SYS_HALT: /* [PROJECT1] Halt the operating system. */
    syscall_halt();
    break;
  case SYS_EXIT: /* [PROJECT1] Terminate this process. */
    // hex_dump((uintptr_t)(f->esp), f->esp, PHYS_BASE - f->esp, true);
    status = (int)*(uint32_t *)(f->esp + 4);
    syscall_exit(status);
    break;
  case SYS_EXEC: /* [PROJECT1] Start another process. */
    // TODO
    break;
  case SYS_WAIT: /* [PROJECT1] Wait for a child process to die. */
    // TODO
    break;

  case SYS_CREATE: /* Create a file. */
    // TODO
    break;
  case SYS_REMOVE: /* Delete a file. */
    // TODO
    break;
  case SYS_OPEN: /* Open a file. */
    // TODO
    break;
  case SYS_FILESIZE: /* Obtain a file's size. */
    // TODO
    break;
  case SYS_READ: /* [PROJECT1] Read from a file. */
    // TODO
    break;
  case SYS_WRITE: /* [PROJECT1] Write to a file. */
    // hex_dump((uintptr_t)f->esp, f->esp, PHYS_BASE - f->esp, true);
    f->eax = syscall_write(
        (int)*(uint32_t *)(f->esp + 4),
        *(uint32_t *)(f->esp + 8),
        (unsigned)*(uint32_t *)(f->esp + 12));
    break;
  case SYS_SEEK: /* Change position in a file. */
    // TODO
    break;
  case SYS_TELL: /* Report current position in a file. */
    // TODO
    break;
  case SYS_CLOSE: /* Close a file. */
    // TODO
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
  default:
    NOT_REACHED();
    break;
  }

  // thread_exit();
}

/**
 * [PROJECT-1] syscall implementation.
 */
static void syscall_halt(void)
{
  shutdown_power_off();
}

static void syscall_exit(int status)
{
  struct thread *cur = thread_current();
  printf("%s: exit(%d)\n", cur->name, status);
  thread_exit();
}

static pid_t syscall_exec(const char *file)
{
}

static int syscall_wait(pid_t pid)
{
}

static int syscall_read(int fd, void *buffer, unsigned size)
{
}

static int syscall_write(int fd, const void *buffer, unsigned size)
{
  if (fd == 1) /* STDOUT */
  {
    putbuf(buffer, size);
    return size;
  }

  // TODO
  return -1;
}
