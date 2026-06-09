#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<stdbool.h>
#include<time.h>
#include<omp.h>
#include<limits.h>

//=======file-reader.c functions=======//
int read_num_dims(const char *);
int *read_dims(const char *, int);
float *read_array(const char *, const int *, int);
int write_to_output_file(const char *, float *, int *, int);

//=======this file's functions=======//
int infect(int n, int *state, int *temp_state, float *p_occupy, int *time_steps, int REC_TIME, int R, int *active, int current_count, int *next_active, int *p_next_count, int *p_infected, unsigned int *my_seed);
float rand_value (int r, float *rand_val, unsigned int *my_seed);
void insert_zero(int n, int *data);
//void insert_temp_state(int n, int *temp_state);
//void insert_rec_time(int n, int *time_steps);
unsigned int xorshift32(unsigned int *status) {
    unsigned int x = *status;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    *status = x;
    return x;
}

int main (int argc, char* argv[]) {
    
    double frS = omp_get_wtime();

    int R = atoi(argv[1]);
    int REC_TIME = atoi(argv[2]);
    int MAX_RUNS = atoi(argv[3]);
    char *inFile = argv[4];
    
    //read file
    int dimention = read_num_dims(inFile);
    int *dims = read_dims(inFile, dimention);
    float *p_occupy = read_array(inFile, dims, dimention);
    int n = dims[0];

    int total_infected = 0;
    
    srand(time(NULL));  
    int run = 1;
    #pragma omp parallel 
    {
    

    #pragma omp for nowait schedule(static) reduction(+:total_infected)
    for (run = 1; run <= MAX_RUNS; run++){  

        //Thread-n
        int thread = omp_get_thread_num();
        //Total thread
        int num_threads = omp_get_num_threads();
        //int num_threads = omp_get_num_threads();

        /*
        printf("-------------------------\n");
        printf("Thread %d from total %d threads.\n", thread, num_threads);
        printf("Simulation %d in thread : %d\n\n", run, thread);
        */

        //Define variables
        int *state = malloc(n * n * sizeof(int));                     //Cells state
        int *temp_state = malloc(n * n * sizeof(int));                //Temporary cells state
        int *rec_time_counter = malloc(n * n * sizeof(int));          //Recovery timer counter 
        int *active = malloc(n * n * sizeof(int));                    //Infected cells
        int *next_active = malloc(n * n * sizeof(int));               //Next infected cells
        int infected = 0;                                             //Number of infected
        unsigned int my_seed = (unsigned int)time(NULL) + (unsigned int)omp_get_thread_num() + (unsigned int)run;          //Seed for unique random value

        //Setup all cells healthy
        insert_zero (n, state);        
        insert_zero (n, rec_time_counter);

        //Initiate patien-zero
        int x = xorshift32(&my_seed) % n;
        int y = xorshift32(&my_seed) % n;
        int index_state = x * n + y;                                  //Flattened
        state[index_state] = 1;
        //printf("x = %d\ny = %d\nIndex = %d\n", x , y, index_state);

        //Set infected cells to acitve
        int current_count = 0;
        active[current_count] = index_state;
        current_count++;
        
        //Set temporary state
        insert_zero(n, temp_state);    
        temp_state[index_state] = 1;
                                                                    
        int a = 1;                                                  //Time-step counter
        while (current_count > 0) {
            memcpy(temp_state, state, n*n*sizeof(int));             //Copy cell non-changing
            int next_count = 0;                                     //Reset next infected counter
            
            //Call infect function
            infect(n, state, temp_state, p_occupy, rec_time_counter, REC_TIME, R, active, current_count, next_active, &next_count, &infected, &my_seed);
            
            //Swap pointer to make temporary_state become actual_state 
            int *tmp = state;
            state = temp_state;
            temp_state = tmp;
            
            //Swap pointer to active cells
            int *temp = active;
            active = next_active;
            next_active = temp;
            
            current_count = next_count;                             
            a++;
        }

        total_infected += infected;

        /*
        printf("INFECTED: %d\n\n", infected);
        printf("Total Infected in simulation %d : %d\n\n", run, total_infected);
        */

        free(state);
        free(temp_state);
        free(rec_time_counter);
        
        
    }

    }
    
    double frE = omp_get_wtime();
    double total_time_taken = frE - frS;
    
    float avg_infected = (float) total_infected / (float) MAX_RUNS;
    printf("\n\nAVERAGE CELLS INFECTED: %.3f\n", avg_infected);
    printf("TOTAL TIME TAKEN: %.3f", total_time_taken);
    printf("\n===============ENDING SIMULATION===============\n\n");
    
    free(p_occupy);
    free(dims);

    return 0;

}

int infect(int n, int *state, int *temp_state, float *p_occupy, int *rec_time_counter, int rec_time, int R, int *active, int current_count, int *next_active, int *p_next_count, int *p_infected, unsigned int *my_seed){
       
    //
    float p_infection;   
    int distance;
    float *rand_val = malloc(((2*R*R)+(2*R)) * sizeof(float));    //Random Value
    int rand_val_counter;
    int index_state;
    int index_temp_state;

    (*p_next_count) = 0;
 
    for (int i = 0; i < current_count; i++) {
        
        int index_state = active[i];
        
        if (state[index_state] == 1){
            temp_state[index_state] = 1;

            int rows = index_state / n;                              //Infected cell rows
            int column = index_state % n;                            //Infected cell column

            int min_x = (rows - R < 0) ? 0 : rows - R;
            int max_x = (rows + R >= n) ? n - 1 : rows + R; 
            int min_y = (column - R < 0) ? 0 : column - R;
            int max_y = (column + R >= n) ? n - 1 : column + R;
            
            //Initiace random value
            rand_value(R, rand_val, my_seed);
            rand_val_counter = 0;
            
            p_infection = 0.0;   
            for (int x = min_x ; x <= max_x; x++){                 
                for (int y = min_y; y <= max_y; y++){
                    int index_temp_state = x * n + y;
                    if (state[index_temp_state] == 0){
                        distance = abs(x-rows) + abs(y-column);
                        if (distance == 0 || distance > R) {continue;}   //Exclude its infected cells and target cells beyond R

                        if (distance > 0 && distance <= R) {
                            p_infection = p_occupy[index_temp_state] / (float)distance;           //Count p_infection     
                            rand_val_counter++;
                            if (p_infection >= rand_val[rand_val_counter] && rec_time_counter[index_temp_state] == 0 ) {                //Check if target cells can be infected or not
                                if (state[index_temp_state] != 2) {                                                                     //Check if 
                                    if (temp_state[index_temp_state] == 0) {                                                            // <-- CEK apakah sudah ditandai terinfeksi di timestep ini
                                        temp_state[index_temp_state] = 1;
                                        next_active[(*p_next_count)] = index_temp_state;
                                        (*p_next_count) += 1;
                                    }
                                    p_infection = 0;
                                }
                            }         
                            else {
                                continue;
                            }
                        }      
                        else if (distance > R){
                            p_infection = 0; 
                        }    
                    }
                    else if (state[index_temp_state] == 1){
                        continue;
                    }
                    else if (state[index_temp_state] == 2){
                        continue;
                    }
                }          
            }

            rec_time_counter[index_state] += 1;
            if (rec_time_counter[index_state] >= rec_time){
                temp_state[index_state] = 2;
                *p_infected += 1;
                } 
            else if (rec_time_counter[index_state] < rec_time){
                temp_state[index_state] = 1;
                next_active[(*p_next_count)] = index_state;
                (*p_next_count) += 1;
            }
        }
    }

    free(rand_val);
    
    return 0;
}

void insert_zero(int n, int *data) {
    for (int idx = 0; idx <= n*n-1; idx++) {
        data[idx] = 0;
    }
}

float rand_value (int R, float *rand_val, unsigned int *seed) {
    for (int i = 1; i <= (2*R*R)+(2*R); i++) {
        rand_val[i] = (float)xorshift32(seed) / (float)UINT_MAX;
    }
}