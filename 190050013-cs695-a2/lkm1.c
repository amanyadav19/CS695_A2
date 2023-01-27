#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched/signal.h>

extern struct task_struct init_task;
#define for_each_process(p) \
        for (p = &init_task ; (p = next_task(p)) != &init_task ; )

int check_if_running(long state)
{
    if (state == TASK_RUNNING) return 1;
    else return 0;
}

static int find_running_processes(void)
{
    struct task_struct *task_list;
    printk(KERN_INFO "RUNNING and RUNNABLE processes\n");
    for_each_process(task_list) {
        if(check_if_running(task_list->__state)){
            printk(KERN_INFO "PID:[%d]\tCMD: %s\n",task_list->pid, task_list->comm);
        }
    }
    return 0;
}

static void tasks_exit(void)
{

}

MODULE_DESCRIPTION("LKM1");
MODULE_AUTHOR("Aman Yadav");
MODULE_LICENSE("GPL");
module_init(find_running_processes);
module_exit(tasks_exit);