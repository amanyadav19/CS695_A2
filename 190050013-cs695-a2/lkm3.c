#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched/signal.h>
#include <linux/mm_types.h>
#include <linux/vmalloc.h>
#include <asm/thread_info.h>
extern struct task_struct init_task;
#define for_each_process(p) \
        for (p = &init_task ; (p = next_task(p)) != &init_task ; )

static int find_running_processes(void)
{
    struct task_struct *task_list;
    printk(KERN_INFO "Kstack ptr of PID = 1 process\n");
    for_each_process(task_list) {
        if(task_list->pid == 1) {
            printk(KERN_INFO "PID:[%d]\tKstack ptr: %px\n", task_list->pid, task_list->stack);
            break;
        }
    }
    return 0;
}

static void tasks_exit(void)
{

}

MODULE_DESCRIPTION("PS");
MODULE_AUTHOR("Aman Yadav");
MODULE_LICENSE("GPL");
module_init(find_running_processes);
module_exit(tasks_exit);