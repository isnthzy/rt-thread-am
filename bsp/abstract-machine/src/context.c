#include <am.h>
#include <klib.h>
#include <rtthread.h>

Context** rt_switch_from;
Context** rt_switch_to;
static Context* ev_handler(Event e, Context *c) {
  switch (e.event) {
    case EVENT_YIELD:
      if(rt_switch_to){
        if(rt_switch_from) *rt_switch_from=c;
        c=*rt_switch_to;
      }
      break;
    case EVENT_IRQ_IODEV:
      break;
    case EVENT_IRQ_TIMER:
      break;
    default: printf("Unhandled event ID = %d\n", e.event); assert(0);
  }
  return c;
}

void __am_cte_init() {
  cte_init(ev_handler);
}

void rt_hw_context_switch_to(rt_ubase_t to) {
  rt_switch_from=NULL;
  rt_switch_to=(Context **)to;
  yield();

}

void rt_hw_context_switch(rt_ubase_t from, rt_ubase_t to) {
  rt_switch_from=(Context **)from;
  rt_switch_to=(Context **)to;
  yield();
}

void rt_hw_context_switch_interrupt(void *context, rt_ubase_t from, rt_ubase_t to, struct rt_thread *to_thread) {
  assert(0);
}

#define STACK_SIZE (4096 * 8)
typedef struct{
  void (*tentry)(void *);
  void *parameter;
  void (*texit)(void);
}rt_wrapped_arg;


void rt_wrapped_tentry(void *arg){
  rt_wrapped_arg* tentry_arg=(rt_wrapped_arg *)arg;
  tentry_arg->tentry(tentry_arg->parameter);
  tentry_arg->texit();
  rt_free(tentry_arg);
}
rt_uint8_t *rt_hw_stack_init(void *tentry, void *parameter, rt_uint8_t *stack_addr, void *texit) {
  stack_addr=(rt_uint8_t *)((uintptr_t)stack_addr&~(sizeof(uintptr_t)-1));
  rt_wrapped_arg *tentry_arg=rt_malloc(sizeof(rt_wrapped_arg)+1);
  tentry_arg->tentry=tentry;
  tentry_arg->parameter=parameter;
  tentry_arg->texit=texit;
  return (rt_uint8_t *)kcontext((Area){stack_addr-sizeof(Context *),stack_addr},rt_wrapped_tentry,tentry_arg);
}
