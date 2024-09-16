// Jehad Hamayel 1200348
// Abdalkarim Eiss 1200015
// Musab Masalmah 1200078

#include "local.h"
// to store the shared memory for openGl
struct toOpenGl
{
    int workerNum[6];
    int medPassed[6];
    int medFailed[6];
    int lineSpeed[6];
};

// to store the shared memory for the production lines
int ShmidToOpenGl;
key_t keyToOpenGl = 12345;

int main()
{
    // Read the settings from the project 
    FILE *file = fopen("settings.txt", "r");

    // Check if the file was opened successfully
    if (file == NULL)
    {
        printf("Error opening the file.\n");
        return 1; // Exit with error
    }

    int rangeOfSpeed[2];
    // Read the settings from the file
    fscanf(file, "Production Lines and number of different medicines: %d\n", &Number_Of_Production_Lines);
    fscanf(file, "threshold of Production: %d\n", &threshold_of_Production);
    fscanf(file, "threshold of bottled medicine are out of spec: %d\n", &threshold_of_bottled_medicine);
    fscanf(file, "threshold of pill-based medicine are out of spec: %d\n", &threshold_of_pill_based_medicine);
    fscanf(file, "end time of the simulation: %d\n", &end_time_of_simulation);
    fscanf(file, "number of workers for each line: %d\n", &number_of_workers_for_each_line);
    fscanf(file, "Range Of speed Production Lines: %d,%d\n", &rangeOfSpeed[0], &rangeOfSpeed[1]);
    fscanf(file, "threshold of time: %d\n", &threshold_of_max_time);
    fscanf(file, "normal range of Liquid level: %d,%d\n", &normal_range_of_Liquid_level[0], &normal_range_of_Liquid_level[1]);
    fscanf(file, "probability of Liquid color matches expected color: %d\n", &probability_of_Liquid_color_matches_expected_color);
    fscanf(file, "probability of Medicine bottles are properly sealed: %d\n", &probability_of_Medicine_bottles_are_properly_sealed);
    fscanf(file, "probability of Correct label is properly placed on the bottle: %d\n", &probability_of_Correct_label_is_properly_placed_on_the_bottle);
    fscanf(file, "probability of Medicine expiry date is clearly printed on the bottle and is correct: %d\n", &probability_of_Medicine_expiry_date_is_clearly_printed_on_the_bottle_and_is_correct);
    fscanf(file, "probability of No plastic container is missing any pill: %d\n", &probability_of_No_plastic_container_is_missing_any_pill);
    fscanf(file, "probability of Pills in the plastic containers have the correct color and size: %d\n", &probability_of_Pills_in_the_plastic_containers_have_the_correct_color_and_size);
    fscanf(file, "probability of Medicine expiry date is clearly printed on the plastic container back: %d\n", &probability_of_Medicine_expiry_date_is_clearly_printed_on_the_plastic_container_back);
    fclose(file);

    // Create the shared memory for the production lines
    key_t key;
    char u_char = 'a';
    shmid = (int *)malloc(Number_Of_Production_Lines * sizeof(int));
    for (int i = 0; i < Number_Of_Production_Lines; i++)
    {
        key = ftok(".", ++u_char);
        shmid[i] = shmget(key, 1024, 0666 | IPC_CREAT);
        if (shmid[i] == -1)
        {
            printf("shmget() failed\n");
            exit(1);
        }
    }

    int status;
    pid_t pid, pid_array[Number_Of_Production_Lines];

    int half_of_production_lines;
    // split the production lines to two parts to make the first part produce liquid and the second part produce pills
    if (Number_Of_Production_Lines % 2 == 0)
    {
        half_of_production_lines = Number_Of_Production_Lines / 2;
    }
    else
    {
        half_of_production_lines = (Number_Of_Production_Lines / 2) + 1;
    }
    
    int *speed;
    srand(time(NULL));
    // loop for the production lines to create the production lines
    if (Number_Of_Production_Lines > 0)
        for (int i = 0; i < Number_Of_Production_Lines; i++)
        {
            // index of the production line
            indexOfShared = i;
            // random average speed worker for each production line
            speedOftheProductionLine = rangeOfSpeed[0] * 1000000 + (rand() % (rangeOfSpeed[1] * 1000000 - rangeOfSpeed[0] * 1000000 + 1));
            struct toOpenGl *Info;
            if ((ShmidToOpenGl = shmget(keyToOpenGl, sizeof(struct toOpenGl), IPC_CREAT | 0666)) < 0)
            {
                perror("shmget fail");
                exit(1);
            }
            // send the number of workers and the speed of the production line to the openGl by shared memory
            Info = (struct toOpenGl *)shmat(ShmidToOpenGl, 0, 0);
            Info[0].workerNum[indexOfShared] = number_of_workers_for_each_line;
            Info[0].lineSpeed[indexOfShared] = speedOftheProductionLine;
            shmdt(Info);

            if ((speed = (int *)shmat(shmid[i], 0, 0)) == (int *)-1)
            {
                perror("shmat() failed\n");
                exit(1);
            }
            // store the speed of average worker for each production line
            speed[0] = speedOftheProductionLine;
            shmdt(speed);

            // create the production line
            pid = fork();
            if (pid == 0)
            {
                // signal to add employee and remove employee from the production line how have high speed to the production line how have low speed 
                signal(SIGUSR1, addEmployee);
                signal(SIGUSR2, removeEmployee);
                // check if the production line is liquid or pills
                if (i < half_of_production_lines)
                {
                    // production line for liquid
                    
                    // malloc to store the max number threads for the workers of the production line with type liquid
                    threads = (pthread_t *)malloc(maxNumberOfEmployees * sizeof(pthread_t)); // Array to hold thread identifiers
                    // create the workers for the production line by threads
                    for (int x = 0; x < number_of_workers_for_each_line; x++)
                    {
                        pthread_create(&threads[x], NULL, checkAndProduceLiquid, NULL);
                    }
                    // update the speed of the production line and control the production line for liquid production line
                    loopForUdpateSpeedOftheProductionLineBottles();
                    for (int x = 0; x < number_of_workers_for_each_line; x++)
                    {
                        pthread_join(threads[x], NULL);
                    }
                    pause();
                }
                else
                {
                    // production line for pills
                    // malloc to store the max number threads for the workers of the production line with type pills
                    threads = (pthread_t *)malloc(maxNumberOfEmployees * sizeof(pthread_t)); // Array to hold thread identifiers
                    // create the workers for the production line by threads
                    for (int x = 0; x < number_of_workers_for_each_line; x++)
                    {
                        pthread_create(&threads[x], NULL, checkAndProducepills, NULL);
                    }
                    // update the speed of the production line and control the production line for pills production line
                    loopForUdpateSpeedOftheProductionLinePills();
                    for (int x = 0; x < number_of_workers_for_each_line; x++)
                    {
                        pthread_join(threads[x], NULL);
                    }
                    pause();
                }
            }
            else
            {
                // store the pid of the production line for the parent process how control the production lines when the simulation is done
                pid_array[i] = pid;
            }
        }
    // signal to stop the simulation by the end of the simulation time, The pharmaceutical factory has produced a medicine type above a user-defined threshold, Too many bottled medicine are out of spec, Too many pill-based medicine are out of spec
    signal(SIGUSR1, StopTheSimulationByFactoryHasProducedAMedicineTypeAboveThreshold);
    signal(SIGUSR2, TooManyBottledMedicineAreOutOfSpec);
    signal(SIGHUP, TooManyPillBasedMedicineAreOutOfSpec);
    /////////////////////////////////////////////////////
    // signal to transfer employee from the production line how have high speed to the production line how have low speed
    sigset(SIGABRT, productLin1NeedEmployee);
    sigset(SIGBUS, productLin2NeedEmployee);
    sigset(SIGFPE, productLin3NeedEmployee);
    sigset(SIGTRAP, productLin4NeedEmployee);
    sigset(SIGSEGV, productLin5NeedEmployee);
    sigset(SIGQUIT, productLin6NeedEmployee);
    /////////////////////////////////////////////////////
    int indexFrom = 0;
    int minDlay = 1000000000;
    int timeEnd = time(NULL) + end_time_of_simulation * 60;
    // loop for control the production lines when the simulation is done
    while (1)
    {
        // check if the simulation is done by the end of the simulation time
        if (time(NULL) > timeEnd)
        {
            for (int i = 0; i < Number_Of_Production_Lines; i++)
            {
                kill(pid_array[i], SIGKILL);
            }
            printf("The simulation is done by the end of the simulation time.\n");
            break;
        }
        // check if the simulation is done by The pharmaceutical factory has produced a medicine type above a user-defined threshold
        if (stop1 == true)
        {
            for (int i = 0; i < Number_Of_Production_Lines; i++)
            {
                kill(pid_array[i], SIGKILL);
            }
            printf("The simulation is done by The pharmaceutical factory has produced a medicine type above a user-defined threshold.\n");
            break;
        }
        // check if the simulation is done by Too many bottled medicine are out of spec
        if (stop2 == true)
        {
            for (int i = 0; i < Number_Of_Production_Lines; i++)
            {
                kill(pid_array[i], SIGKILL);
            }
            printf("The simulation is done by Too many bottled medicine are out of spec.\n");
            break;
        }
        // check if the simulation is done by Too many pill-based medicine are out of spec
        if (stop3 == true)
        {
            for (int i = 0; i < Number_Of_Production_Lines; i++)
            {
                kill(pid_array[i], SIGKILL);
            }
            printf("The simulation is done by Too many pill-based medicine are out of spec.\n");
            break;
        }
        // check if the production line need employee to transfer the employee from the production line how have high speed to the production line how have low speed 
        for (int t = 0; t < Number_Of_Production_Lines; t++)
        {
            if (productLineNeedEmployee[t] == true)
            {
                for (int i = 0; i < Number_Of_Production_Lines; i++)
                {
                    if ((speed = (int *)shmat(shmid[i], 0, 0)) == (int *)-1)
                    {
                        perror("shmat() failed\n");
                        exit(1);
                    }
                    if (minDlay > speed[0])
                    {
                        minDlay = speed[0];
                        indexFrom = i;
                    }
                    shmdt(speed);
                }

                kill(pid_array[indexFrom], SIGUSR2);
                kill(pid_array[t], SIGUSR1);
                sleep(0.2);
                printf("Transfer Employee from %d to %d\n", indexFrom, t);
                productLineNeedEmployee[t] = false;
            }
            minDlay = 1000000000;
            indexFrom = 0;
        }
    }

    return (0);
}
// 
void productLin1NeedEmployee(int signum)
{
    productLineNeedEmployee[0] = true;
}
void productLin2NeedEmployee(int signum)
{
    productLineNeedEmployee[1] = true;
}
void productLin3NeedEmployee(int signum)
{
    productLineNeedEmployee[2] = true;
}
void productLin4NeedEmployee(int signum)
{
    productLineNeedEmployee[3] = true;
}
void productLin5NeedEmployee(int signum)
{
    productLineNeedEmployee[4] = true;
}
void productLin6NeedEmployee(int signum)
{
    productLineNeedEmployee[5] = true;
}
void addEmployee(int signum)
{
    addEmployeeFlag = true;
}
void removeEmployee(int signum)
{
    removeEmployeeFlag = true;
}
// loop for control the production lines of type liquid
void loopForUdpateSpeedOftheProductionLineBottles()
{
    int min = 1000;
    int max = 3000;
    srand(time(NULL));
    int *speed;
    // // loop for control the production lines of type liquid
    while (final_midicine_num_of_type_bottles != threshold_of_Production && dropped_bottles != threshold_of_bottled_medicine)
    {

        pthread_mutex_lock(&final_midicine_num_of_type_bottles_mutex);
        // adding employee to the production line how have low speed
        if (addEmployeeFlag == true)
        {
            // create the worker for the production line
            pthread_create(&threads[number_of_workers_for_each_line], NULL, checkAndProduceLiquid, NULL);
            number_of_workers_for_each_line++;

            struct toOpenGl *Info;
            if ((ShmidToOpenGl = shmget(keyToOpenGl, sizeof(struct toOpenGl), IPC_CREAT | 0666)) < 0)
            {
                perror("shmget fail");
                exit(1);
            }

            Info = (struct toOpenGl *)shmat(ShmidToOpenGl, 0, 0);
            Info[0].workerNum[indexOfShared] = number_of_workers_for_each_line;

            speedOftheProductionLine -= 1500000;

            Info[0].lineSpeed[indexOfShared] = speedOftheProductionLine;
            shmdt(Info);
            addEmployeeFlag = false;
        }
        pthread_mutex_unlock(&final_midicine_num_of_type_bottles_mutex);
        pthread_mutex_lock(&final_midicine_num_of_type_bottles_mutex);
        // remove employee from the production line how have high speed
        if (removeEmployeeFlag == true)
        {
            number_of_workers_for_each_line--;
            struct toOpenGl *Info;
            if ((ShmidToOpenGl = shmget(keyToOpenGl, sizeof(struct toOpenGl), IPC_CREAT | 0666)) < 0)
            {
                perror("shmget fail");
                exit(1);
            }

            Info = (struct toOpenGl *)shmat(ShmidToOpenGl, 0, 0);
            Info[0].workerNum[indexOfShared] = number_of_workers_for_each_line;
            pthread_cancel(threads[number_of_workers_for_each_line]);
            speedOftheProductionLine += 500000;

            Info[0].lineSpeed[indexOfShared] = speedOftheProductionLine;
            shmdt(Info);
            removeEmployeeFlag = false;
        }
        pthread_mutex_unlock(&final_midicine_num_of_type_bottles_mutex);
        usleep(speedOftheProductionLine);
        if ((speed = (int *)shmat(shmid[indexOfShared], 0, 0)) == (int *)-1)
        {
            perror("shmat() failed\n");
            exit(1);
        }
        speed[0] = speedOftheProductionLine;
        shmdt(speed);
        // update the speed of the production line by number of workers
        speedOftheProductionLine += (min + (rand() % (max - min + 1))) * (maxNumberOfEmployees - number_of_workers_for_each_line);
        struct toOpenGl *Info;
        if ((ShmidToOpenGl = shmget(keyToOpenGl, sizeof(struct toOpenGl), IPC_CREAT | 0666)) < 0)
        {
            perror("shmget fail");
            exit(1);
        }

        Info = (struct toOpenGl *)shmat(ShmidToOpenGl, 0, 0);
        Info[0].lineSpeed[indexOfShared] = speedOftheProductionLine;
        shmdt(Info);
        // transfer employee from the production line how have high speed to the production line how have low speed when the speed of the production line is high
        if (speedOftheProductionLine >= threshold_of_max_time * 1000000 && number_of_workers_for_each_line != maxNumberOfEmployees)
        {
            if (indexOfShared == 0)
            {
                kill(getppid(), SIGABRT);
                sleep(0.2);
            }
            if (indexOfShared == 1)
            {
                kill(getppid(), SIGBUS);
                sleep(0.2);
            }
            if (indexOfShared == 2)
            {
                kill(getppid(), SIGFPE);
                sleep(0.2);
            }
            if (indexOfShared == 3)
            {
                kill(getppid(), SIGTRAP);
                sleep(0.2);
            }
            if (indexOfShared == 4)
            {
                kill(getppid(), SIGSEGV);
                sleep(0.2);
            }
            if (indexOfShared == 5)
            {
                kill(getppid(), SIGQUIT);
                sleep(0.2);
            }
        }
    }
}
// loop for control the production lines of type pills
void loopForUdpateSpeedOftheProductionLinePills()
{
    int min = 1000;
    int max = 3000;
    srand(time(NULL));
    int *speed;
    // loop for control the production lines of type pills
    while (final_midicine_num_of_type_pills != threshold_of_Production * 3 && dropped_pills != threshold_of_pill_based_medicine)
    {
        pthread_mutex_lock(&final_midicine_num_of_type_pills_mutex);
        if (addEmployeeFlag == true)
        {
            pthread_create(&threads[number_of_workers_for_each_line], NULL, checkAndProducepills, NULL);
            number_of_workers_for_each_line++;
            struct toOpenGl *Info;
            if ((ShmidToOpenGl = shmget(keyToOpenGl, sizeof(struct toOpenGl), IPC_CREAT | 0666)) < 0)
            {
                perror("shmget fail");
                exit(1);
            }
            // send the number of workers and the speed of the production line to the openGl by shared memory
            Info = (struct toOpenGl *)shmat(ShmidToOpenGl, 0, 0);
            Info[0].workerNum[indexOfShared] = number_of_workers_for_each_line;
            speedOftheProductionLine -= 1500000;

            Info[0].lineSpeed[indexOfShared] = speedOftheProductionLine;
            shmdt(Info);
            addEmployeeFlag = false;
        }
        pthread_mutex_unlock(&final_midicine_num_of_type_pills_mutex);
        pthread_mutex_lock(&final_midicine_num_of_type_pills_mutex);
        if (removeEmployeeFlag == true)
        {
            number_of_workers_for_each_line--;
            struct toOpenGl *Info;
            if ((ShmidToOpenGl = shmget(keyToOpenGl, sizeof(struct toOpenGl), IPC_CREAT | 0666)) < 0)
            {
                perror("shmget fail");
                exit(1);
            }
            // send the number of workers and the speed of the production line to the openGl by shared memory
            Info = (struct toOpenGl *)shmat(ShmidToOpenGl, 0, 0);
            Info[0].workerNum[indexOfShared] = number_of_workers_for_each_line;
            pthread_cancel(threads[number_of_workers_for_each_line]);
            speedOftheProductionLine += 500000;
            Info[0].lineSpeed[indexOfShared] = speedOftheProductionLine;
            shmdt(Info);
            removeEmployeeFlag = false;
        }
        pthread_mutex_unlock(&final_midicine_num_of_type_pills_mutex);
        usleep(speedOftheProductionLine);
        if ((speed = (int *)shmat(shmid[indexOfShared], 0, 0)) == (int *)-1)
        {
            perror("shmat() failed\n");
            exit(1);
        }
        speed[0] = speedOftheProductionLine;
        shmdt(speed);
        // update the speed of the production line by number of workers
        speedOftheProductionLine += (min + (rand() % (max - min + 1))) * (10 - number_of_workers_for_each_line);
        struct toOpenGl *Info;
        if ((ShmidToOpenGl = shmget(keyToOpenGl, sizeof(struct toOpenGl), IPC_CREAT | 0666)) < 0)
        {
            perror("shmget fail");
            exit(1);
        }

        Info = (struct toOpenGl *)shmat(ShmidToOpenGl, 0, 0);
        Info[0].lineSpeed[indexOfShared] = speedOftheProductionLine;
        shmdt(Info);
        // transfer employee from the production line how have high speed to the production line how have low speed when the speed of the production line is high
        if (speedOftheProductionLine >= threshold_of_max_time * 1000000 && number_of_workers_for_each_line != maxNumberOfEmployees)
        {

            if (indexOfShared == 0)
            {
                kill(getppid(), SIGABRT);
                sleep(0.2);
            }
            if (indexOfShared == 1)
            {
                kill(getppid(), SIGBUS);
                sleep(0.2);
            }
            if (indexOfShared == 2)
            {
                kill(getppid(), SIGFPE);
                sleep(0.2);
            }
            if (indexOfShared == 3)
            {
                kill(getppid(), SIGTRAP);
                sleep(0.2);
            }
            if (indexOfShared == 4)
            {
                kill(getppid(), SIGSEGV);
                sleep(0.2);
            }
            if (indexOfShared == 5)
            {
                kill(getppid(), SIGQUIT);
                sleep(0.2);
            }
        }
    }
}
// function to check the production line of type liquid
void *checkAndProduceLiquid(void *arg)
{
    struct liquidMedicine medicines;
    srand(time(NULL));
    while (1)
    {
        // sleep for the speed of the production line
        usleep(speedOftheProductionLine);
        // random the values of the production line for checking midicine
        medicines.Liquid_level = 80 + (rand() % (100 - 80 + 1));
        medicines.Liquid_color = (rand() % 101 < probability_of_Liquid_color_matches_expected_color) ? true : false;
        medicines.sealed = (rand() % 101 < probability_of_Medicine_bottles_are_properly_sealed) ? true : false;
        medicines.label_placed = (rand() % 101 < probability_of_Correct_label_is_properly_placed_on_the_bottle) ? true : false;
        medicines.date_printed = (rand() % 101 < probability_of_Medicine_expiry_date_is_clearly_printed_on_the_bottle_and_is_correct) ? true : false;

        pthread_mutex_lock(&final_midicine_num_of_type_bottles_mutex);
        // check if the production line is done by the end of the simulation time or the production line has produced a medicine type above a user-defined threshold
        if (final_midicine_num_of_type_bottles == threshold_of_Production || dropped_bottles == threshold_of_bottled_medicine)
        {
            if (final_midicine_num_of_type_bottles == threshold_of_Production)
            {
                kill(getppid(), SIGUSR1);
            }
            if (dropped_bottles == threshold_of_bottled_medicine)
            {
                kill(getppid(), SIGUSR2);
            }
            pthread_mutex_unlock(&final_midicine_num_of_type_bottles_mutex);
            break;
        }
        // check if the midicine is passed or not
        if (medicines.date_printed == true && medicines.label_placed == true && medicines.sealed == true && medicines.Liquid_color == true && medicines.Liquid_level > normal_range_of_Liquid_level[0] && medicines.Liquid_level < normal_range_of_Liquid_level[1])
        { // Access n using pointer to struct

            final_midicine_num_of_type_bottles++;
            struct toOpenGl *Info;
            if ((ShmidToOpenGl = shmget(keyToOpenGl, sizeof(struct toOpenGl), IPC_CREAT | 0666)) < 0)
            {
                perror("shmget fail");
                exit(1);
            }
            // 
            Info = (struct toOpenGl *)shmat(ShmidToOpenGl, 0, 0);
            Info[0].medPassed[indexOfShared] = final_midicine_num_of_type_bottles;
            shmdt(Info);
        }
        else
        {
            dropped_bottles++;
            struct toOpenGl *Info;
            if ((ShmidToOpenGl = shmget(keyToOpenGl, sizeof(struct toOpenGl), IPC_CREAT | 0666)) < 0)
            {
                perror("shmget fail");
                exit(1);
            }

            Info = (struct toOpenGl *)shmat(ShmidToOpenGl, 0, 0);
            Info[0].medFailed[indexOfShared] = dropped_bottles;
            shmdt(Info);
        }
        pthread_mutex_unlock(&final_midicine_num_of_type_bottles_mutex);
    }

    return NULL;
}
// function to check the production line of type pills
void *checkAndProducepills(void *arg)
{
    struct pillsMedicine medicines;
    srand(time(NULL));

    while (1)
    {   
        // sleep for the speed of the production line
        usleep(speedOftheProductionLine);
        // random the values of the production line for checking midicine
        medicines.container_miss_pill = (rand() % 101 < probability_of_No_plastic_container_is_missing_any_pill) ? true : false;
        medicines.date_printed = (rand() % 101 < probability_of_Pills_in_the_plastic_containers_have_the_correct_color_and_size) ? true : false;
        medicines.pills_color_and_size = (rand() % 101 < probability_of_Medicine_expiry_date_is_clearly_printed_on_the_plastic_container_back) ? true : false;
        pthread_mutex_lock(&final_midicine_num_of_type_pills_mutex);
        // check if the production line is done by the end of the simulation time or the production line has produced a medicine type above a user-defined threshold
        if (final_midicine_num_of_type_pills == threshold_of_Production || dropped_pills == threshold_of_pill_based_medicine)
        {
            if (final_midicine_num_of_type_pills == (threshold_of_Production * 3))
            {
                kill(getppid(), SIGUSR1);
            }
            if (dropped_pills == threshold_of_pill_based_medicine)
            {
                kill(getppid(), SIGHUP);
            }
            pthread_mutex_unlock(&final_midicine_num_of_type_pills_mutex);
            break;
        }
        // check if the midicine is passed or not
        if (medicines.date_printed == true && medicines.container_miss_pill == true && medicines.pills_color_and_size == true)
        { 

            final_midicine_num_of_type_pills++;
            struct toOpenGl *Info;
            if ((ShmidToOpenGl = shmget(keyToOpenGl, sizeof(struct toOpenGl), IPC_CREAT | 0666)) < 0)
            {
                perror("shmget fail");
                exit(1);
            }

            Info = (struct toOpenGl *)shmat(ShmidToOpenGl, 0, 0);
            Info[0].medPassed[indexOfShared] = final_midicine_num_of_type_pills;
            shmdt(Info);
        }
        else
        {
            dropped_pills++;
            struct toOpenGl *Info;
            if ((ShmidToOpenGl = shmget(keyToOpenGl, sizeof(struct toOpenGl), IPC_CREAT | 0666)) < 0)
            {
                perror("shmget fail");
                exit(1);
            }

            Info = (struct toOpenGl *)shmat(ShmidToOpenGl, 0, 0);
            Info[0].medFailed[indexOfShared] = dropped_pills;
            shmdt(Info);
        }
        pthread_mutex_unlock(&final_midicine_num_of_type_pills_mutex);
    }

    return NULL;
}
// 
void StopTheSimulationByFactoryHasProducedAMedicineTypeAboveThreshold(int signum)
{
    stop1 = true;
}
void TooManyBottledMedicineAreOutOfSpec(int signum)
{
    stop2 = true;
}
void TooManyPillBasedMedicineAreOutOfSpec(int signum)
{
    stop3 = true;
}