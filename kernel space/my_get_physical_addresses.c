#include <linux/kernel.h>//printk
#include <linux/syscalls.h>
#include <linux/mm.h>       // Data structure and functions related to memory management
#include <asm/pgtable.h>
#include <linux/pid.h>      // To obtain process info
#include <linux/sched.h>    // Definition of process control area
#include <linux/highmem.h>  // Functions related to page table operations
#include <asm/page.h>       // Functions related to page table operations

SYSCALL_DEFINE1(my_get_physical_addresses, void *, vaddr) {
    struct task_struct *task = current; // �����e�i�{
    struct mm_struct *mm = task->mm;      // �����e�i�{�����s�޲z���c
    pgd_t *pgd;
    p4d_t *p4d;
    pud_t *pud;
    pmd_t *pmd;
    pte_t *pte;
    struct page *page;
    unsigned long physical_address;
    //none �M bad �O���֭���ާ@������ӭ��n�����A�Ω��ˬd������ت����A�A���O�N������ت��u���s�b�v�M�u�L�ġv�C

    
    pgd = pgd_offset(mm, (unsigned long)vaddr);
    if (pgd_none(*pgd) || pgd_bad(*pgd)) {
	    printk("pgd not found!!!\n");
        return 0;
    }
   

    p4d = p4d_offset(pgd, (unsigned long)vaddr);
    if (p4d_none(*p4d) || p4d_bad(*p4d)) {
	    printk("p4d not found!!!\n");
        return 0;
    }
 

    pud = pud_offset(p4d, (unsigned long)vaddr);
    if (pud_none(*pud) || pud_bad(*pud)) {
	    printk("pud not found!!!\n");
        return 0;
    }
    

    pmd = pmd_offset(pud, (unsigned long)vaddr);
    if (pmd_none(*pmd) || pmd_bad(*pmd)) {
	    printk("pmd not found!!!\n");
        return 0;
    }
   

   
    pte = pte_offset_map(pmd, (unsigned long)vaddr);
    if (pte_none(*pte)) {
	    printk("pte not found!!!\n");
        return 0;
    }
   

    
    page = pte_page(*pte);
    if (!page) {
	    printk("page not found!!!\n");
        return 0;
    }
   

    //page��ipage_to_pfn()�o�쭶�د��ޡA�M��V����12bits
    //~PAGE_MASK�G�N PAGE_MASK ���ϡCpage�j�p�O 4KB�APAGE_MASK �N�O 0xFFFFF000�A
    //�� ~PAGE_MASK �N�|�O 0x00000FFF�C�o�ˤ@�ӡAvaddr & ~PAGE_MASK �u�O�d�Fvirtual memory �� Offset�C
    //index��virtual memory��offset��OR �۷��X�ֱo�����O�����m
    physical_address = (page_to_pfn(page) << PAGE_SHIFT) | ((unsigned long)vaddr & ~PAGE_MASK);

    return (void *)physical_address;

}

