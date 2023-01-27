#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched/signal.h>
#include <linux/mm_types.h>
#include <linux/vmalloc.h>
#include <asm/io.h>
#include <linux/slab.h>
#include <linux/pagewalk.h>
#include <linux/pgtable.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/stat.h>
extern struct task_struct init_task;
#define for_each_process(p) \
        for (p = &init_task ; (p = next_task(p)) != &init_task ; )

int pid = 94238;
unsigned long virtual_address = 0xaaaaecc36eb0;
// module_param(pid, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
// MODULE_PARM_DESC(pid, "PID of the process");
// module_param(virtual_address, long, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
// MODULE_PARM_DESC(virtual_address, "Virtual address in the process space");

static struct page *walk_page_table(struct task_struct* c, unsigned long addr)
{
    pgd_t *pgd;
    p4d_t *p4d;
    pte_t *ptep, pte;
    pud_t *pud;
    pmd_t *pmd;

    struct page *page = NULL;
    struct mm_struct *mm = c->mm;

    pgd = pgd_offset(mm, addr);
    if (pgd_none(*pgd) || pgd_bad(*pgd))
        goto out;

    p4d = p4d_offset(pgd, addr);
    if (p4d_none(*p4d) || p4d_bad(*p4d))
        goto out;

    pud = pud_offset(p4d, addr);
    if (pud_none(*pud) || pud_bad(*pud))
        goto out;

    pmd = pmd_offset(pud, addr);
    if (pmd_none(*pmd) || pmd_bad(*pmd))
        goto out;

    ptep = pte_offset_map(pmd, addr);
    if (!ptep)
        goto out;
    pte = *ptep;

    page = pte_page(pte);
 out:
    return page;
}

static int find_running_processes(void)
{
    struct task_struct *task_list;
    printk(KERN_INFO "Virtual to Physical address\n");
    printk(KERN_INFO "Virtual address : 0x%lx\n", virtual_address);

    if(virtual_address <= 0xffffffffffffffff && virtual_address >= 0xffff800000000000) {
        // This is a kernel address;
        printk(KERN_INFO "Physical Address: 0x%llx\n", virt_to_phys((void *)virtual_address));
    } else {
        int k = 0;
        for_each_process(task_list) {
            if(task_list->mm && task_list->pid == pid) {
                struct page* p = walk_page_table(task_list, virtual_address);
                if(p == NULL) {
                    printk(KERN_INFO "No mapping exists\n");
                    k = 1;
                } else {
                    printk(KERN_INFO "Physical Address: 0x%lx\n", ((unsigned long)&p & (virtual_address & 0b111111111111)));
                    k = 1;
                }
                break;
            }
        }
        if(k == 0) {
            printk(KERN_INFO "PID not found\n");
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