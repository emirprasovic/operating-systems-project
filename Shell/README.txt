Members:
	-Emir Prašović
	-Faris Gigić

Instructions:
	-Navigate to the "Shell" folder inside your terminal and execute the command ./shell
	-Note: do not test the forkbomb command
	-For 1.3 Basic and Intermediate, call forkwaitexec/rforkexeclp as a command input
	1.2 Intermediate, call guess/draw
	
Outline:
	For this assignment we managed to do everything the 	project requires, except 
	1.2 Advanced, where we need to implement piping
	
	
Question 1.5.1: Do the following actions require the OS to use kernel mode or user mode is sufficient? Explain.
1. A program wants to read from disk.
2. Reading the current time from the hardware clock.

1. When we want to read files from a disk, the OS must use the kernel mode since reading the disk involves interacting with hardware components, and accessing hardware directly must be a privilidged operation to ensure security and other benefits.

2. Reading the current time from the hardware clock can be done in user mode since it doesn't involve modifying system state or accessing sensitive data, there is no need for the kernel mode access

--------------

Question 1.5.2: Explain the purpose of a system call. There are different sets of system calls: list them and give at least 2 examples of a system call for each category.

The purpose of a system call is to provide a controlled way for a user-level process to interact with the kernel. They allow user-level programs to request services from the operating system
Different types of system calls:
-For the file system: open(), read(), write(), close()...
-For process control: fork(), wait(), execvp(), exit()...
-For memory management: brk(), sbrk(), mmap()...
-For interprocess communication: pipe(), socket()...
-For device management: WriteConsole(), Open(), ReadConsole()...

---------------

Question 1.5.3: What are the possible outputs printed from this program shown below? You may assume that the program runs on a modern Linux-like OS. You may ignore any output generated from “some executable”.

int ret = fork();
if(ret == 0) {
	printf("Hello1\n");
	exec("some_executable);
	printf("Hello2\n");
}
else if(ret > 0) {
	wait();
	printf("Hello3\n");
}
else {
	printf("Hello4\n");
	
}

1. In case the fork call fails (ret < 0), the child process is not created and the program will only print out "Hello4"
2. In case the fork call is successful (ret == 0), the child process will print out "Hello1", and some_executable will take over, meaning "Hello2" will not get printed. Even if the exec() fails, it will terminate the child process and "Hello2" will not get printed. Now since the parent process (ret < 0) has a wait(), it will wait for the child to finish and then print out "Hello3". So the output will be "Hello1" "Hello3"
