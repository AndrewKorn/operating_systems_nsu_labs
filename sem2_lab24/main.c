#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct Module {
    sem_t* A_detail_sem;
    sem_t* B_detail_sem;
    sem_t* module_sem;
} Module;

typedef struct Widget {
    sem_t* C_detail_sem;
    sem_t* module_sem;
    sem_t* widget_sem;
} Widget;


void* A_detail_production(void* args) {
    sem_t* A_detail_sem = (sem_t*)args;	
    while(1) {
        sleep(1);
	if (sem_post(A_detail_sem) != 0) {
	    perror("sem_post A_detail_sem");
	    pthread_exit(NULL);
	}
    }
}

void* B_detail_production(void* args) {
    sem_t* B_detail_sem = (sem_t*)args;
    while(1) {
        sleep(2);
        if (sem_post(B_detail_sem) != 0) {
	    perror("sem_post B_detail_sem");
            pthread_exit(NULL);
	}
    }
}

void* module_production(void* args) {
    Module* module = (Module*)args;
    while(1) { 
        if (sem_wait(module->A_detail_sem) != 0) {
	    perror("sem_wait A_detail_sem");
	    pthread_exit(NULL);
	}
    	if (sem_wait(module->B_detail_sem) != 0) {
	    perror("sem_wait B_detail_sem");
            pthread_exit(NULL);
	}
	if (sem_post(module->module_sem) != 0) {
            perror("sem_post module_sem");
            pthread_exit(NULL);
	}
    }
}

void* C_detail_production(void* args) {
    sem_t* C_detail_sem = (sem_t*)args;
    while(1) {
        sleep(3);
        if (sem_post(C_detail_sem) != 0) {
            perror("sem_post C_detail_sem");
            pthread_exit(NULL);
	}
    }
}

void* widget_production(void* args) {
    Widget* widget = (Widget*)args;
    while(1) {
        if (sem_wait(widget->C_detail_sem) != 0) {
            perror("sem_wait C_detail_sem");
            pthread_exit(NULL);
	}
        if (sem_wait(widget->module_sem) != 0) {
            perror("sem_wait module_sem");
            pthread_exit(NULL);
	}
	if (sem_post(widget->widget_sem) != 0) {
            perror("sem_post widget_sem");
            pthread_exit(NULL);
	}
    }
}


int main() {
    sem_t A_detail_sem;
    sem_t B_detail_sem;
    sem_t module_sem;
    sem_t C_detail_sem;
    sem_t widget_sem;

    if (sem_init(&A_detail_sem, 0, 0) != 0) {
    	perror("sem_init A_detail_sem");
	pthread_exit(NULL);
    }
    if (sem_init(&B_detail_sem, 0, 0) != 0) {
	perror("sem_init B_detail_sem");
        pthread_exit(NULL);
    }
    if (sem_init(&module_sem, 0, 0) != 0) {
	perror("sem_init module_sem");
        pthread_exit(NULL);
    }
    if (sem_init(&C_detail_sem, 0, 0) != 0) {
        perror("sem_init C_detail_sem");
        pthread_exit(NULL);
    }
    if (sem_init(&widget_sem, 0, 0) != 0) {
        perror("sem_init widgit_sem");
        pthread_exit(NULL);
    }

    pthread_t A_detail_t;
    pthread_t B_detail_t;
    pthread_t module_t;
    pthread_t C_detail_t;
    pthread_t widget_t;

    if (pthread_create(&A_detail_t, NULL, A_detail_production, (void*)&A_detail_sem) != 0) {
    	printf("can't create thread A_detail_t\n");
	pthread_exit(NULL);
    }
    if (pthread_create(&B_detail_t, NULL, B_detail_production, (void*)&B_detail_sem) != 0) {
    	printf("can't create thread B_detail_t\n");
	pthread_cancel(A_detail_t);
	pthread_exit(NULL);
    }

    Module* module = malloc(sizeof(Module));
    if (module == NULL) {
    	printf("can't allocate memory for Module* module\n");
	pthread_cancel(A_detail_t);
	pthread_cancel(B_detail_t);
	pthread_exit(NULL);
    }
    module->A_detail_sem = &A_detail_sem;
    module->B_detail_sem = &B_detail_sem;
    module->module_sem = &module_sem;
    if (pthread_create(&module_t, NULL, module_production, (void*)module) != 0) {
    	printf("can't create thread module_t\n");
	pthread_cancel(A_detail_t);
	pthread_cancel(B_detail_t);
	free(module);
	pthread_exit(NULL);
    }
    
    if (pthread_create(&C_detail_t, NULL, C_detail_production, (void*)&C_detail_sem) != 0) {
    	printf("can't create thread C_detail_t\n");
	pthread_cancel(A_detail_t);
	pthread_cancel(B_detail_t);
	pthread_cancel(module_t);
	free(module);
	pthread_exit(NULL);
    }
    
    Widget* widget = malloc(sizeof(Widget));
    if (widget == NULL) {
    	printf("can't allocate memory for Widget* widget\n");
	pthread_cancel(A_detail_t);
        pthread_cancel(B_detail_t);
        pthread_cancel(module_t);
	pthread_cancel(C_detail_t);
        free(module);
        pthread_exit(NULL);

    }
    widget->C_detail_sem = &C_detail_sem;
    widget->module_sem = &module_sem;
    widget->widget_sem = &widget_sem;
    if (pthread_create(&widget_t, NULL, module_production, (void*)widget) != 0) {
    	printf("can't create thread widget_t");
	pthread_cancel(A_detail_t);
        pthread_cancel(B_detail_t);
        pthread_cancel(module_t);
        pthread_cancel(C_detail_t);
        free(module);
	free(widget);
        pthread_exit(NULL);
    }


    int A_detail_count = 0;
    int B_detail_count = 0;
    int module_count = 0;
    int C_detail_count = 0;
    int widget_count = 0;

    while(widget_count < 10) {
        sem_getvalue(&A_detail_sem, &A_detail_count);
        sem_getvalue(&B_detail_sem, &B_detail_count);
        sem_getvalue(&module_sem, &module_count);
        sem_getvalue(&C_detail_sem, &C_detail_count);
  	sem_getvalue(&widget_sem, &widget_count);
	printf("A: %d | B: %d | module: %d | C: %d | widget: %d\n", A_detail_count, B_detail_count, module_count, C_detail_count, widget_count);
    }

    pthread_cancel(A_detail_t);
    pthread_cancel(B_detail_t);
    pthread_cancel(module_t);
    pthread_cancel(C_detail_t);
    pthread_cancel(widget_t);

    sem_destroy(&A_detail_sem);
    sem_destroy(&B_detail_sem);
    sem_destroy(&module_sem);   
    sem_destroy(&C_detail_sem);
    sem_destroy(&widget_sem);

    free(module);
    free(widget);
    pthread_exit(NULL);
}
