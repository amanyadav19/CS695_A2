#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched/signal.h>
#include <linux/mm_types.h>
#include <linux/vmalloc.h>
#include <asm/io.h>
#include <linux/slab.h>
#include <linux/pagewalk.h>
#include <linux/pgtable.h>
extern struct task_struct init_task;
#define for_each_process(p) \
        for (p = &init_task ; (p = next_task(p)) != &init_task ; )

int pid = 104053;

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
    printk(KERN_INFO "Virtual Address Space and Physical Address Space size\n");

    
    for_each_process(task_list) {
        if(task_list->mm) {
            struct vm_area_struct *vma = 0;
            unsigned long sum = 0;
            unsigned long psum = 0;
            if (task_list->mm && task_list->mm->mmap)
                for (vma = task_list->mm->mmap; vma; vma = vma->vm_next) {
                    sum += vma->vm_end - vma->vm_start;
                    if(walk_page_table(task_list, vma->vm_start)) {
                        psum += vma->vm_end - vma->vm_start;
                    }
                }
            printk(KERN_INFO "PID:[%d]\ttotal virtual memory (in Bytes): %lu, physical_vm (in Bytes) %lu\n", task_list->pid, sum, psum);
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