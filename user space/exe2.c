#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>             
#include <sys/types.h>
#include <sys/wait.h>
void *my_get_physical_addresses(void *addr){

    void *paddr;

    paddr = (void *)syscall(450, addr);

    return paddr;
}


int global_a = 123;

int main(){
	int loc_a;
    	void *parent_use, *child_use;
	printf("llllllllllllllllllllllllll  After Fork by child  llllllllllllllllllllllllllllllll\n");
        child_use = my_get_physical_addresses(&global_a);
        printf("******* pid=%d: global variable global_a:\n", getpid());
        printf("******* Offset of logical address:[%p]   Physical address:[%p]\n", &global_a, child_use);
        printf("llllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllll\n");
        printf("____________________________________________________________________________\n");
        printf("\n");
}

