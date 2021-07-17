# Philosophers - 1337 school project

![](https://user-images.githubusercontent.com/43875634/87864747-54f0a580-c9a7-11ea-8bea-aa9c0d4aaf05.png)

## The problem
It goes like this. There are 5 Philosophers sitting at a round dining table. There is a bowl of rice in the middle which anyone can eat from (philosophers don’t care much for flavour) but only if they have two chopsticks. The problem is that there are only five chopsticks which are placed between each philosopher. Each philosopher can only pick up the chopsticks to their left and right. Apparently Philosophers can only eat or think. Talking is not an option for them. So you need to find a system to let everyone eat from the bowl of rice. It is also assumed that there is an infinite amount of rice in the bowl.

This project is a training to multi-threads/multi-process programming with the use of mutex and semaphore.
It contains 2 different programs simulating a twist of the famous Dining Philosophers problem, all with the same basic rules.

## Installation 🖥

To test the project, go to one of the folders philo, philo_bonus and make a make. Then, you can test the project using an executable.

## usage :

./philo number_philosopher time_to_die time_to_eat time_to_sleep [number_of_time_each_philosophers_must_eat] arguments in square brackets are not required (number_of_time_each_philosophers_must_eat)

## exemple :

./philo_one 4 500 200 200

## Project documentation 📚

 __*[Process vs Thread](https://www.guru99.com/mutex-vs-semaphore.html)*__
 
 __*[Mutex vs Semaphore](https://www.guru99.com/difference-between-process-and-thread.html)*__

__*[Semaphore EN video](https://www.youtube.com/watch?v=ukM_zzrIeXs)*__

__*[Fork explain in FR](https://www.commentcamarche.net/faq/10611-que-fait-un-fork)*__ 
