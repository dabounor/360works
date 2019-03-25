int tswitch();

int sleep(int event)
{
  printf("proc %d going to sleep on event=%d\n", running->pid, event);

  running->event = event;
  running->status = SLEEP;
  enqueue(&sleepList, running);
  printList("sleepList", sleepList);
  tswitch();
}

int wakeup(int event)
{
  PROC *temp, *p;
  temp = 0;
  printList("sleepList", sleepList);

  while (p = dequeue(&sleepList)){
     if (p->event == event){
	printf("wakeup %d\n", p->pid);
	p->status = READY;
	enqueue(&readyQueue, p);
     }
     else{
	enqueue(&temp, p);
     }
  }
  sleepList = temp;
  printList("sleepList", sleepList);
}

int kexit(int exitValue)
{
  if(running->pid == 1)
  {
	printf("P1 is immortal\n");
	return -1;
  }
  PROC *temp = running;
  while(temp->ppid != 1)
  {
	temp = temp->parent;
  }
  while(temp->sibling)
  {
	temp = temp->sibling;
  }
  PROC *changeParent = running->child;
  while(changeParent)
  {
	changeParent->ppid = 1;
	changeParent = changeParent->sibling;
  }
  temp->sibling = running->child;
  printf("proc %d in kexit(), value=%d\n", running->pid, exitValue);

  running->exitCode = exitValue;
  running->status = ZOMBIE;
  tswitch();
}


  
