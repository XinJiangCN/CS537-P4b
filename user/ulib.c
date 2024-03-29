#include "types.h"
#include "stat.h"
#include "fcntl.h"
#include "user.h"
#include "x86.h"
#define PGSIZE 4096

void *stacks = NULL;
int counter = 0;

int 
thread_create(void (*start_routine)(void *, void *), void *arg1, void *arg2){

    if(stacks == NULL){
        stacks = malloc(65*PGSIZE);

        if((uint)stacks % PGSIZE)
            stacks = stacks + (PGSIZE - (uint)stacks % PGSIZE);
    }

    void *addr = stacks + counter*PGSIZE;
    int ret = clone(start_routine, arg1, arg2, addr);
    counter++;
    return ret;
}
int 
thread_join() {
  void *stack;
  int ret = join((&stack));
  counter--;
 
  if(counter == 0){
    free(stacks);
  }
  return ret;
}
void
lock_init(lock_t *lock) {
    lock->lock = 0;
}
//TODO make xchg to fetch-and-add
void lock_acquire(lock_t *lock) {
    while(xchg(&lock->lock, 1) != 0);
}

void lock_release(lock_t *lock) {
    xchg(&lock->lock, 0);
}

char*
strcpy(char *s, char *t)
{
  char *os;

  os = s;
  while((*s++ = *t++) != 0)
    ;
  return os;
}

int
strcmp(const char *p, const char *q)
{
  while(*p && *p == *q)
    p++, q++;
  return (uchar)*p - (uchar)*q;
}

uint
strlen(char *s)
{
  int n;

  for(n = 0; s[n]; n++)
    ;
  return n;
}

void*
memset(void *dst, int c, uint n)
{
  stosb(dst, c, n);
  return dst;
}

char*
strchr(const char *s, char c)
{
  for(; *s; s++)
    if(*s == c)
      return (char*)s;
  return 0;
}

char*
gets(char *buf, int max)
{
  int i, cc;
  char c;

  for(i=0; i+1 < max; ){
    cc = read(0, &c, 1);
    if(cc < 1)
      break;
    buf[i++] = c;
    if(c == '\n' || c == '\r')
      break;
  }
  buf[i] = '\0';
  return buf;
}

int
stat(char *n, struct stat *st)
{
  int fd;
  int r;

  fd = open(n, O_RDONLY);
  if(fd < 0)
    return -1;
  r = fstat(fd, st);
  close(fd);
  return r;
}

int
atoi(const char *s)
{
  int n;

  n = 0;
  while('0' <= *s && *s <= '9')
    n = n*10 + *s++ - '0';
  return n;
}

void*
memmove(void *vdst, void *vsrc, int n)
{
  char *dst, *src;
  
  dst = vdst;
  src = vsrc;
  while(n-- > 0)
    *dst++ = *src++;
  return vdst;
}
