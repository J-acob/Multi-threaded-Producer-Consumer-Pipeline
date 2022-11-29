#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <math.h>
#include <string.h>
#include <stddef.h> 
#include <stdarg.h>

#define MAX_LINE_LENTGH 1000
#define MAX_LINE_COUNT 49
#define MAX_OUTPUT_SIZE 81




struct prodconshare
{

    /*Relationship1*/
    sem_t *is_full; /*Keep track of fullness of shared space*/
    sem_t *is_empty; /*Keep track of emptiness of shared space*/
    char *shared_space; /*Shared space of producer and consumer*/
    pthread_mutex_t mutex; /*Mutex for locking and unlock access to shared memory region*/

    /*Relationship2*/

    sem_t *is_full2; /*Keep track of fullness of shared space*/
    sem_t *is_empty2; /*Keep track of emptiness of shared space*/
    char *shared_space2; /*Shared space of producer and consumer*/
    pthread_mutex_t mutex2; /*Mutex for locking and unlock access to shared memory region*/


    /*Relationship3*/

    sem_t *is_full3; /*Keep track of fullness of shared space*/
    sem_t *is_empty3; /*Keep track of emptiness of shared space*/
    char *shared_space3; /*Shared space of producer and consumer*/
    pthread_mutex_t mutex3; /*Mutex for locking and unlock access to shared memory region*/

};


/*

Consumes: stdin (stream of char or line-buffered)

Collates input into lines

Produces:Lines of the form (char[]{..., '\n'})

*/
void *
input_thread(void *targs)
{

    /*Grab info from structs*/
    sem_t *semf = ((struct prodconshare *) targs)->is_full;
    sem_t *seme = ((struct prodconshare *) targs)->is_empty;
    char *shared_space = ((struct prodconshare *) targs)->shared_space;
    pthread_mutex_t mutex = ((struct prodconshare *) targs)->mutex;

    sem_t *semf2 = ((struct prodconshare *) targs)->is_full2;
    sem_t *seme2 = ((struct prodconshare *) targs)->is_empty2;
    char *shared_space2 = ((struct prodconshare *) targs)->shared_space2;
    pthread_mutex_t mutex2 = ((struct prodconshare *) targs)->mutex2;

    /*Output struct info*/
    sem_t *semf3 = ((struct prodconshare *) targs)->is_full3;
    sem_t *seme3 = ((struct prodconshare *) targs)->is_empty3;
    char *shared_space3 = ((struct prodconshare *) targs)->shared_space3;
    pthread_mutex_t mutex3 = ((struct prodconshare *) targs)->mutex3;

    

    /*Create buffer for reading*/
    /*Read from stdin*/
    size_t idx = 0;
    char buf[1024];

    while(1)
    {

        /*Read from STDIN*/
        if(fgets(buf, MAX_LINE_LENTGH, stdin) != NULL)
        {/*Produce string from STDIN*/


            if(strcmp(buf, "STOP\n") == 0)
            {


                sem_wait(seme2);

                exit(0);

            }
            else
            {

                /*Decrement empty semaphore*/
                sem_wait(seme);
                //Lock the first mutex for writing
                pthread_mutex_lock(&mutex);
                /*Copy the buffer into the shared space buffer*/
                strcpy(shared_space, buf);

                //Unlock after writing
                pthread_mutex_unlock(&mutex);
                //Increment semaphore
                sem_post(semf);
                

            }
            
        }
        else
        {/*EOF Or error, either way exit*/
            
            if (feof(stdin));
            if (ferror(stdin)); 
            exit(0);

        }

        
      
    }

    return targs;
}

/*

Consumes: Stream of characters

Transforms '\n' -> ' '(space)

Produces: Stream of characters

*/
void *
line_seperator_thread(void *targs)
{

    /*Input struct info*/
    sem_t *semf = ((struct prodconshare *) targs)->is_full;
    sem_t *seme = ((struct prodconshare *) targs)->is_empty;
    char *shared_space = ((struct prodconshare *) targs)->shared_space;
    pthread_mutex_t mutex = ((struct prodconshare *) targs)->mutex;

    /*output struct info*/
    sem_t *semf2 = ((struct prodconshare *) targs)->is_full2;
    sem_t *seme2 = ((struct prodconshare *) targs)->is_empty2;
    char *shared_space2 = ((struct prodconshare *) targs)->shared_space2;
    pthread_mutex_t mutex2 = ((struct prodconshare *) targs)->mutex2;

    
    size_t idx = 0;

    while(1)
    {
        /*Wait for the data to be ready, Lock the semaphore*/
        sem_wait(seme2);
        sem_wait(semf);

        //Lock for writing
        pthread_mutex_lock(&mutex);

        //fflush(stdout);
            

        /*Iterate over string to find \n*/
        size_t i=0;

        for(i = 0; i < strlen(shared_space); i++)
        {

            if(shared_space[i] == '\n')
            {/* Turn newline characters into spaces*/
                shared_space[i] = ' ';

            }

        }


        strcpy(shared_space2, shared_space);

        fflush(stdout);
        /*Clear out shm since our producer won't produce unless the buffer is empty*/

        memset(shared_space, 0, sizeof shared_space);
        pthread_mutex_unlock(&mutex);

        /*Unlock semaphores for next consumer and producer*/
        sem_post(seme);
        sem_post(semf2);

    }
     
    return targs;

}

/*

Consumes: Stream of characters

Transforms "++" -> "^"

Produces: Stream of characters



*/
void * 
plus_sign_thread(void *targs)
{

    /*Input struct info*/
    sem_t *semf2 = ((struct prodconshare *) targs)->is_full2;
    sem_t *seme2 = ((struct prodconshare *) targs)->is_empty2;
    char *shared_space2 = ((struct prodconshare *) targs)->shared_space2;
    pthread_mutex_t mutex2 = ((struct prodconshare *) targs)->mutex2;

    /*Output struct info*/
    sem_t *semf3 = ((struct prodconshare *) targs)->is_full3;
    sem_t *seme3 = ((struct prodconshare *) targs)->is_empty3;
    char *shared_space3 = ((struct prodconshare *) targs)->shared_space3;
    pthread_mutex_t mutex3 = ((struct prodconshare *) targs)->mutex3;


    size_t idx = 0;
    while(1)
    {


        sem_wait(seme3);
        sem_wait(semf2);

        pthread_mutex_lock(&mutex2);

        /*Iterate over the characters and find ++, turn to ^*/
        size_t i = 0;

        for(i = 0; i < strlen(shared_space2); i++)
        {

            if(shared_space2[i] == '+' && shared_space2[i+1] != '\n' && shared_space2[i+1] == '+')
            {/*Find instances of ++, make sure we don't acess memory that's not there*/

                /*Set the first instance of it to ^ */
                shared_space2[i] = '^';
                

                /*Copies new altered string to itself while removing the next occurance of +*/
                memmove(&shared_space2[i+1],  &shared_space2[i+2], strlen(shared_space2) - i);

            }

            /*Output current char to output buffer*/
        
        }   


        /*Copy shared spaces into eachother*/
        strcpy(shared_space3, shared_space2);

        /*Clear out shared space for future writing*/
        memset(shared_space2, 0, sizeof shared_space2);


        pthread_mutex_unlock(&mutex2);

        /*Unlock semaphores*/
        sem_post(semf3);
        sem_post(seme2);
        


    }


}

/*


Consumes: Stream of characters

Collates charcters into 80-line char lines

Produces: Writes blocks of char[81]{...., '\n'} to stdout


*/
void * 
output_thread(void *targs)
{


    /*Input struct info*/
    sem_t *semf3 = ((struct prodconshare *) targs)->is_full3;
    sem_t *seme3 = ((struct prodconshare *) targs)->is_empty3;
    char *shared_space3 = ((struct prodconshare *) targs)->shared_space3;
    pthread_mutex_t mutex3 = ((struct prodconshare *) targs)->mutex3;

    char out_put_buffer[81];

    char *print_buf_ptr;
    char print_buf[1024];

    char *prev_it_buf_ptr;
    char prev_it_buf[1024];

    
    int num_chars = 0;
    int total_chars = 0;

    
    
    while(1)
    {

        sem_wait(semf3);
        pthread_mutex_lock(&mutex3);
        print_buf_ptr = print_buf;

        /*Copy passed string into array for iteration*/
        strcpy(print_buf, shared_space3);

        while(*print_buf_ptr != 0)
        {/*Iterate over our copied buffer*/
            
            //fflush(stdout);
            total_chars++;

            if(num_chars == 80)
            {/*Reset everything, append \n, print*/

                printf("%s", out_put_buffer);
                printf("\n");
                fflush(stdout);

                num_chars = 0;


            }
            else
            {/*If if we're not at 80 characters, place current char onto buffer*/

                out_put_buffer[num_chars] = *print_buf_ptr;
                num_chars++;
                print_buf_ptr++;

            }

            
        }
        
        memset(shared_space3, 0, sizeof shared_space3);
        pthread_mutex_unlock(&mutex3);
        sem_post(seme3); 

    }



}


/*
Creates semaphores for all prod-cons relationships
Creates buffer for all prod-con relationships

Runs threads for prod-consumer relationships 
*/

int main(int argc, char *argv[])
{

    /*For the input and line seperator relationship*/
    struct prodconshare input_linesep_share;
    sem_t shared_sem_full;
    input_linesep_share.is_full = &shared_sem_full;
    sem_init(&shared_sem_full, 0, 0);
    sem_t shared_sem_empty;
    input_linesep_share.is_empty = &shared_sem_empty;
    sem_init(&shared_sem_empty, 0, 1);
    pthread_mutex_init(&input_linesep_share.mutex, NULL);
    char input_linesep_buf[1024];
    input_linesep_share.shared_space = input_linesep_buf;


    /*For the line seperator and plusplus thread relationship*/
    sem_t lsps_sem_full;
    input_linesep_share.is_full2 = &lsps_sem_full;
    sem_init(&lsps_sem_full, 0, 0);
    sem_t lsps_sem_empty;
    input_linesep_share.is_empty2 = &lsps_sem_empty;
    sem_init(&lsps_sem_empty, 0, 1);
    pthread_mutex_init(&input_linesep_share.mutex2, NULL);
    char linesep_plusplus_buf[1024];
    input_linesep_share.shared_space2 = linesep_plusplus_buf;

    /*For the plusplus and output thread relationship*/
    sem_t out_sem_full;
    input_linesep_share.is_full3 = &out_sem_full;
    sem_init(&out_sem_full, 0, 0);
    sem_t out_sem_empty;
    input_linesep_share.is_empty3 = &out_sem_empty;
    sem_init(&out_sem_empty, 0, 1);
    pthread_mutex_init(&input_linesep_share.mutex3, NULL);
    char output_plusplus_buf[1024];
    input_linesep_share.shared_space3 = output_plusplus_buf;

    setbuf(stdout,  NULL);

    /*Initializing pthreads for all the threads*/
    pthread_t in_thread, line_thread, plus_thread, out_thread;

    /*Creating threads for the threads, connecting them to respective functions*/
    pthread_create(&in_thread, NULL, &input_thread, &input_linesep_share);
    pthread_create(&line_thread, NULL, &line_seperator_thread, &input_linesep_share);
    pthread_create(&plus_thread, NULL, &plus_sign_thread, &input_linesep_share);
    pthread_create(&out_thread, NULL, &output_thread, &input_linesep_share);

    //usleep(10);

    //Exiting
    pthread_join(in_thread, NULL);
    pthread_join(line_thread, NULL);
    pthread_join(plus_thread, NULL);
    pthread_join(out_thread, NULL);
    
    //Destroy semaphores for cleanup
    sem_destroy(&shared_sem_full);
    sem_destroy(&shared_sem_empty);
    sem_destroy(&lsps_sem_empty);
    sem_destroy(&lsps_sem_full);
    sem_destroy(&out_sem_full);
    sem_destroy(&out_sem_empty);

    //Final exit
    pthread_exit(NULL);
}