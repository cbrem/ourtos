/* Demo of kronOS
We plan to build an RTOS. We will implement priority ceiling with mutex and a timer to handle periodic tasks. A serial buffer shall act as a shared resource for the mutex. We shall use serial to output details on current tasks and the state of the various tasks. We will use the timer interrupt to run a scheduler periodically. If we have time we will also implement yield.	
 * 
 * Spencer Barton (sebarton)
 * Connor Brem (cbrem)
 * Group C1
 * 18-348 Lab 11
 */