#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched/signal.h>
#include <linux/mm_types.h>

extern struct task_struct init_task;
#define for_each_process(p) \
        for (p = &init_task ; (p = next_task(p)) != &init_task ; )

static int find_running_processes(void)
{
    struct task_struct *task_list;
    unsigned long int max_heap_usage = 0;
    int pid_max;
    char cmd_max[500];
    for_each_process(task_list) {
        if(task_list->mm != NULL) {
            if(task_list->mm->brk - task_list->mm->start_brk > max_heap_usage) {
                max_heap_usage = task_list->mm->brk - task_list->mm->start_brk;
                pid_max = task_list->pid;
                sprintf(cmd_max, "%s", task_list->comm);
            }
        }
    }
    printk(KERN_INFO "PROCESS USING MAXIMUM HEAP\n");
    printk(KERN_INFO "PID:[%d]\tCMD: %s\tMAX HEAP MEM USAGE(in Bytes):%lu\n",pid_max, cmd_max, max_heap_usage);
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