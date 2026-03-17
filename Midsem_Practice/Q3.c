/*
Author :

███████╗████████╗ █████╗ ██████╗  ███╗   ███╗ █████╗ ███╗   ██╗
██╔════╝╚══██╔══╝██╔══██╗██╔══██╗ ████╗ ████║██╔══██╗████╗  ██║
███████╗   ██║   ███████║██████╔╝ ██╔████╔██║███████║██╔██╗ ██║
╚════██║   ██║   ██╔══██║██║  ██║ ██║╚██╔╝██║██╔══██║██║╚██╗██║
███████║   ██║   ██║  ██║██║  ██║ ██║ ╚═╝ ██║██║  ██║██║ ╚████║
╚══════╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝ ╚═╝     ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝  STARMAN248
*/

/*
    Hostel Room Allocation using Semaphores

    Develop a C/C++ program to simulate a Hostel Room Allocation system using Operating System concepts with the following requirements:

        a) The Parent process reads the number of students along with their Student ID and request (arrival) times.

        b) The system has only 5 rooms available. The program should take input for the number of students requesting rooms.
            • If rooms are available, allocate them to students.
            • If no vacancy is available, display an appropriate message and make the student wait until a room becomes free.
            • Use semaphores to manage room allocation.

        c) Create child processes that dynamically allocate rooms to students in the order of their arrival times.
            • Ensure proper synchronization using semaphores during allocation and release of rooms.
*/


