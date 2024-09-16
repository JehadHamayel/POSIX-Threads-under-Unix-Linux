///Header file

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/shm.h>
#include <errno.h>
#include <signal.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <string.h>
#include <sys/shm.h>
#include <unistd.h>

///For project3.c (Functions definitions, variables and structs)
void *checkAndProduceLiquid(void *);
void *checkAndProducepills(void *);

struct liquidMedicine
{
    int Liquid_level;
    bool Liquid_color;
    bool sealed;
    bool label_placed;
    bool date_printed;
};

struct pillsMedicine
{
    bool container_miss_pill;
    bool pills_color_and_size;
    bool date_printed;
};

int final_midicine_num_of_type_bottles = 0;
int dropped_bottles = 0;
pthread_mutex_t final_midicine_num_of_type_bottles_mutex = PTHREAD_MUTEX_INITIALIZER;

int final_midicine_num_of_type_pills = 0;
int dropped_pills = 0;
pthread_mutex_t final_midicine_num_of_type_pills_mutex = PTHREAD_MUTEX_INITIALIZER;
int Number_Of_Production_Lines;
int threshold_of_Production;
int threshold_of_bottled_medicine;
int threshold_of_pill_based_medicine;
int end_time_of_simulation;
int number_of_workers_for_each_line;
int normal_range_of_Liquid_level[2];
int probability_of_Liquid_color_matches_expected_color;
int probability_of_Medicine_bottles_are_properly_sealed;
int probability_of_Correct_label_is_properly_placed_on_the_bottle;
int probability_of_Medicine_expiry_date_is_clearly_printed_on_the_bottle_and_is_correct;
int probability_of_No_plastic_container_is_missing_any_pill;
int probability_of_Pills_in_the_plastic_containers_have_the_correct_color_and_size;
int probability_of_Medicine_expiry_date_is_clearly_printed_on_the_plastic_container_back;
int threshold_of_max_time;
bool stop1 = false;
bool stop2 = false;
bool stop3 = false;
bool productLineNeedEmployee[6] = {false, false, false, false, false, false};
bool addEmployeeFlag = false;
bool removeEmployeeFlag = false;
int indexOfShared = 0;
int *shmid;
void StopTheSimulationByFactoryHasProducedAMedicineTypeAboveThreshold(int signum);
void TooManyBottledMedicineAreOutOfSpec(int signum);
void TooManyPillBasedMedicineAreOutOfSpec(int signum);
void loopForUdpateSpeedOftheProductionLineBottles();
void loopForUdpateSpeedOftheProductionLinePills();
void productLin1NeedEmployee(int signum);
void productLin2NeedEmployee(int signum);
void productLin3NeedEmployee(int signum);
void productLin4NeedEmployee(int signum);
void productLin5NeedEmployee(int signum);
void productLin6NeedEmployee(int signum);
void addEmployee(int signum);
void removeEmployee(int signum);

int speedOftheProductionLine;
int maxNumberOfEmployees = 20;
pthread_t *threads;
typedef void (*sighandler_t)(int);
sighandler_t sigset(int sig, sighandler_t disp);
