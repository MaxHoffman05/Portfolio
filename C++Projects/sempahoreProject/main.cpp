/*
Project 2 - Airport Problem
By : Max Hoffman
Date: 10/28/2023 -11/8/2023
 
Based on planeTest.cpp provided
*/

//used these sources for syntax
//https://man7.org/linux/man-pages/man2/semget.2.html
//https://man7.org/linux/man-pages/man2/semctl.2.html
//https://man7.org/linux/man-pages/man2/semop.2.html

#include "AirportAnimator.hpp"

#include <unistd.h>
#include <pthread.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include <cstdlib>
#include <iostream>

using namespace std;


#define SEM_NUM 1
#define SEM_NUM_LOAD 2
#define SEM_NUM_TOUR 3

int didWork;
int SEM_RUNWAY = 0;
int SEM_LOAD = 1;
int SEM_TOUR = 2;

int numOfTotalPassengers;//total number of passengers available
int completedTours = 0;//current total number of completed tours
int totalTours;//total number of tours

int semPassNum;
int sem_id;
int myKey = 33;


pthread_t thread1, thread2, thread3, thread4, thread5, thread6, thread7, thread8;//all of our threads one for each plane

int sem_id_load;
int loadkey = 34;

int sem_id_tour;
int tourkey = 35;

void* planeThread(void *arg) {//thread method for each of the planes
  //first have the passengers board
  //first semctl is to set the sem value for the runway semaphore
  
  //set the process id to the plane number
  int *planeNum = (int *) arg;
  
  //arrays and ints for each semaphore command array and semop() calls
  struct sembuf waitloadCommand[1];
  int semResultLoad;

  struct sembuf waitTourCommand[1];
  int semResultTour;
  
  struct sembuf waitCommand[1];
  int semResult;
  
  while(completedTours < totalTours) {//keep going through the cycle as long as the number of completedTours is less than the total 
  //tours, the if statement is for when the plane that is equal to the amount of total tours then it will break out of the loop and 
  //be set to done.
 
    if(completedTours != totalTours) {
      AirportAnimator::updateStatus(*planeNum, "BOARD");
      //load passengers
          for(int i = 1; i <= 12; i++ ) {//wait each time you add a passenger to a plane which takes away 1 from the semaphore value which is the total number of passengers
                waitloadCommand[0].sem_num = SEM_LOAD; 
                waitloadCommand[0].sem_op = -1; 
                waitloadCommand[0].sem_flg = 0; 
                semResultLoad = semop(sem_id_load, waitloadCommand, 1);
                AirportAnimator::updatePassengers(*planeNum, i);
                sleep(rand()%3);
              
          }
      
      //taxi to the runway
      AirportAnimator::updateStatus(*planeNum, "TAXI");
      AirportAnimator::taxiOut(*planeNum);

      
      //wait on all planes except 1, or 1 plane should be allowed on the runaway at one time
      if(semctl(sem_id, 0, GETVAL) <= 1) {//if there is a plane on the runway we want to wait
        //wait code from the semWait example we did in class
        waitCommand[0].sem_num = SEM_RUNWAY; 
        waitCommand[0].sem_op = -1; 
        waitCommand[0].sem_flg = 0; 
        semResult = semop(sem_id, waitCommand, 1);
      }

      //take off runway
      AirportAnimator::updateStatus(*planeNum, "TKOFF");
      AirportAnimator::takeoff(*planeNum);

      //signal to say that the runway is clear
      sleep(1);
      waitCommand[0].sem_op = +1; 
      semResult = semop(sem_id, waitCommand, 1);
  

      //animate the tour and sleep
      AirportAnimator::updateStatus(*planeNum, "TOUR");
      sleep(15+rand() % 31);

      //request to land
      AirportAnimator::updateStatus(*planeNum, "LNDRQ");

      //we need to wait till the runway is clear 
      waitCommand[0].sem_op = -1; 
      semResult = semop(sem_id, waitCommand, 1);
   
      //animate landing 
      AirportAnimator::updateStatus(*planeNum, "LAND");
      AirportAnimator::land(*planeNum);

      //signal again letting the other planes know that we already landed
      sleep(1);
      waitCommand[0].sem_op = +1; 
      semResult = semop(sem_id, waitCommand, 1);

      //animate taxi-ing back
      AirportAnimator::updateStatus(*planeNum, "TAXI");
      AirportAnimator::taxiIn(*planeNum);
  
      //deplane
      AirportAnimator::updateStatus(*planeNum, "DEPLN");
      for(int j = 11; j >= 0; j--) {//instead of waiting we now signal to add 1 to the semaphore value
      // since passengers need to return to the pool of available passengers to be used in more flights
        waitloadCommand[0].sem_num = SEM_LOAD; 
        waitloadCommand[0].sem_op = +1; 
        waitloadCommand[0].sem_flg = 0; 
        semResultLoad = semop(sem_id_load, waitloadCommand, 1);
        AirportAnimator::updatePassengers(*planeNum, j);
        sleep(1);
      }
    

      //update the number of tours
      //semaphore to protect number of tours since two threads could try 
      //to update the number at the same time as described in the directions
      if(semctl(sem_id_tour, 0, GETVAL) <= 1) {
      
        waitTourCommand[0].sem_num = SEM_TOUR; 
        waitTourCommand[0].sem_op = -1; 
        waitTourCommand[0].sem_flg = 0; 
        semResultTour = semop(sem_id_tour, waitTourCommand, 1);
        //one thread will not wait while any others will have to wait here

        //then signal since we are done
        completedTours += 1;
        AirportAnimator::updateTours(completedTours);
        waitTourCommand[0].sem_op = +1; 
        semResultTour = semop(sem_id_tour, waitTourCommand, 1);
      }
    
      //keep repeating tours until we are done
      }
      else {
        break;//if we have reached the end of the tours
      }
    }
  
  //update our status to done then complete the thread by returning
  AirportAnimator::updateStatus(*planeNum, "DONE");
  return NULL;
}

int main(int argc, char *argv[]) {
  int totalPass;
  int tourNum;
  //start the id's at 0 since there is 8 planes plane 0 to plane 8

  //try to access the semaphore first
  sem_id = semget(myKey, SEM_NUM, 0);
  
  //if we did not find the semaphore with the key value make one
  if(sem_id == -1) {
    sem_id = semget(myKey, SEM_NUM, IPC_CREAT|IPC_EXCL|06660);
  }
  
  
  //make the second semaphore for making sure we protect the passenger resource
  sem_id_load = semget(loadkey, SEM_NUM_LOAD, 0);

  if(sem_id_load == -1) {
    sem_id_load = semget(loadkey, SEM_NUM_LOAD, IPC_CREAT|IPC_EXCL|06660);
  }
 
  //make the third semaphore for making sure we protect the tour resource
  sem_id_tour = semget(tourkey, SEM_NUM_TOUR, 0);

  if(sem_id_tour == -1) {
    sem_id_tour = semget(tourkey, SEM_NUM_TOUR, IPC_CREAT|IPC_EXCL|06660);
  }
 
  //id for each thread, which is the same number as the plane number
  int id1 = 0;
  int id2 = 1;
  int id3 = 2;
  int id4 = 3;
  int id5 = 4;
  int id6 = 5;
  int id7 = 6;
  int id8 = 7;


  if(argc != 3) {//if the user didnt give the required number of arguments when running we wont even run the program
    cout << "You did not specify any parameters " << endl;
    totalPass = 0;
    tourNum = 0;
  }
  else {
    //get the number of passengers and number of tours from input
    totalPass = atoi(argv[1]);
    tourNum = atoi(argv[2]);
    numOfTotalPassengers = totalPass;
    cout << "Total # of Tours :  " << tourNum << endl;
    cout << "Total Passengers : " << numOfTotalPassengers << endl;
    totalTours = tourNum;
 
  }

  sleep(2);
  
  
  
  if(tourNum <= 0) {
    cout << "You specified no tours to be completed! " << endl;

  }
  else {
    AirportAnimator::init();

    //first semaphore used for protecting the runway
    semctl(sem_id, SEM_RUNWAY, SETVAL, 1);

    //second semaphore set to the number of passengers
    didWork = semctl(sem_id_load, SEM_LOAD, SETVAL, numOfTotalPassengers);

    //third semaphore for protecting tours
    semctl(sem_id_tour, SEM_TOUR, SETVAL, 1);

    
    //establish the 8 threads
    pthread_create(&thread1, NULL, planeThread, (void *) &id1);
    pthread_create(&thread2, NULL, planeThread, (void *) &id2);
    pthread_create(&thread3, NULL, planeThread, (void *) &id3);
    pthread_create(&thread4, NULL, planeThread, (void *) &id4);
    pthread_create(&thread5, NULL, planeThread, (void *) &id5);
    pthread_create(&thread6, NULL, planeThread, (void *) &id6);
    pthread_create(&thread7, NULL, planeThread, (void *) &id7);
    pthread_create(&thread8, NULL, planeThread, (void *) &id8);
   
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_join(thread4, NULL);
    pthread_join(thread5, NULL);
    pthread_join(thread6, NULL);
    pthread_join(thread7, NULL);
    pthread_join(thread8, NULL);

    sleep(3);
    AirportAnimator::end();
  }
 
  
 
}
