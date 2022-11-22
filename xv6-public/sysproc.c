#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  // cprintf("n=%d",n);  
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int 
sys_clone(void)
{
  // needs similar logic to fork
  //fetch 1st arg
  int fcn = 0;
  if(argint(0,&fcn) < 0)
    return -1;
  // argint(0,&fcn);
  //fetch 2nd arg
  int arg1 = 0;
  if(argint(1,&arg1) < 0)
    return -1;
  // argint(1,&arg1);
  //fetch 3rd arg
  int arg2 = 0;
  if(argint(2,&arg2) < 0)
    return -1;
  // argint(2,&arg2);

  //fetch 4th arg
  int stack = 0;
  if(argint(3,&stack) < 0)
    return -1;
  // // argint(3,&stack);
  if(fcn < 0 || arg1 < 0 || arg2 < 0 || stack< 0 )
    return -1;
  if( ((stack) % (2*sizeof(uint))) !=0){    
    return -1;
  }
  int addr=myproc()->sz;
  // cprintf("fcn,arg1,arg2,stack,sz: %d, %d, %d, %d, %d",fcn,arg1,arg2,stack,addr);
  if(addr - (uint)stack == PGSIZE/2|| stack>addr)
    return -1;
  return clone((void *)fcn,(void *)arg1,(void *)arg2,(void *)stack);
}

int 
sys_join(void){
  void **stack = 0;
  int tstack;
  argint(0,&tstack);
  // int st=argint(0,&tstack);
  // uint sz= myproc()->sz;
  // cprintf("\nsz=%d",sz);
  if(tstack< 0 || sz-tstack < sizeof(uint))
    return -1;
  // cprintf("tstack:%d",tstack);
  stack = (void **)tstack;
  return join(stack);
}
