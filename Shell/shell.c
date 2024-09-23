#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h> // for pid_t
#include <fcntl.h>
// we can include sys/wait.h for waitpid() function, which is better than wait
// since we can tell it which child to wait for by specifying a pid
// but here we will only have 1 child for execution so we won't need it

#include <limits.h>
// for defying MAX_CHAR_SIZE and etc.

// #include <sys/rfork.h> ne radi?


#define MAX_MACHINENAME_LENGTH 256
#define MAX_USERNAME_LENGTH 256
#define MAX_ARGS_NUM 16
#define MAX_COMMAND_LENGTH 1024
#define HISTORY_SIZE 16
#define FORTUNE_SIZE 5

#define ANSI_COLOR_ORANGE "\x1b[33m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_PURPLE "\x1b[35m"
#define SHELL_NAME "SHELLDON"

char* history[HISTORY_SIZE];
int historyIndex = 0;
// prompt is used to display the prompt and it shoudn't return anything
void displayPrompt() {
	char machinename[MAX_MACHINENAME_LENGTH];
	char username[MAX_USERNAME_LENGTH];
	
	gethostname(username, sizeof(machinename));
	getlogin_r(machinename, sizeof(username));

	// printf("%s@%s:~$ ", machinename, username);
	// printf(ANSI_COLOR_PURPLE "%s@%s:~$ " ANSI_COLOR_CYAN, SHELL_NAME, username);
	printf(ANSI_COLOR_ORANGE "%s" ANSI_COLOR_CYAN, SHELL_NAME);
	printf("@");
        printf(ANSI_COLOR_PURPLE "%s" ANSI_COLOR_CYAN, username);
        printf(":~$ ");
}

void displaySimplePrompt() {
	printf("prompt$ ");
}

void executeCommand(char* args[]) {
	pid_t pid = fork();
	if(pid < 0) { perror("error with fork"); }
        else if(pid == 0) {
        	execvp(args[0], args);
                perror("Execvp error");
                exit(1);
         }
	 else {
         	wait(NULL);
	}
}

void copy(char* args[]) {
	if(args[1] == NULL || args[2] == NULL) {
		printf("You must specify the source and destination files\nUsage: cp srcFile dstFile\n");
		return;
	}
	// F_OK checks the file existence
	if(access(args[1], F_OK) == -1) {
		printf("Source file: %s not found\n", args[1]);
		return;
	}
	if(access(args[2], F_OK) == -1) {
		printf("Destination file: %s already exists\n", args[2]);
		return;
	}

	FILE* source = fopen(args[1], "r");
	FILE* destination = fopen(args[2], "w");

	if(source == NULL || destination == NULL) {
		printf("Problem opening the file/s\n");
		return;
	}

	char buffer[2048];
	size_t bytesRead;
	
	while((bytesRead = fread(buffer, 1, sizeof(buffer), source)) > 0) {
		fwrite(buffer, 1, bytesRead, destination);
	}
	fclose(source);
	fclose(destination);
	printf("Files copied successfully\n");
}

// displays the 16 most recent commands
// index 1: OLDEST
// index 16: NEWEST
void displayHistory() {
	printf("Command history:\n");
	// index used to represent the history order more clearly
	int index = 1;
	// this for loop goes form the beggining of the historyIndex(meaning
	// from the last command entered) to the end of the array
	for(int i = historyIndex; i < HISTORY_SIZE; i++) {
		if(history[i] != NULL) {
			printf("%d. %s\n", index++, history[i]);
		}
	}
	// we need this for loop because, if histIndex is at 5, and it
	// traversed the whole array multiple times, we need to
	// display the commands from index 0 to 5 (the newest commands)
	for(int i = 0; i < historyIndex; i++) {
		if(history[i] != NULL) {
			printf("%d. %s\n", index++, history[i]);
		}
	}
}

void displayFree() {
	// /proc/meminfo contains all information about the memory
	// we open the file in read-only mode
	FILE* meminfo = fopen("/proc/meminfo", "r");
	if(meminfo == NULL) {
		printf("Error opening meminfo file");
		return;
	}
	char meminfoLine[128];
	while(fgets(meminfoLine, sizeof(meminfoLine), meminfo)) {
		// \n not needed in the printf because it is included at the
		// end of every line in the meminfo file
		printf("%s", meminfoLine);
	}
}

void displayFortune() {
	char* fortunes[FORTUNE_SIZE] = {
	"The only way to do great work is to love what you do.",
	"In the middle of difficulty lies opportunity.",
	"The best way to predict the future is to invent it.",
	"Triput mjeri jednom sijeci",
	"Believe you can and you're halfway there."
	};
	int index = rand() % FORTUNE_SIZE;
	printf("Fortune: %s\n", fortunes[index]);
}

void forkbomb() {
	printf("Are you sure? y/n\n");
	char answer;
	scanf("%c", &answer);
	
	if(answer != 'y') {
		printf("Cancelling...\n");
		return;
	} else {
		printf("Forkbombing...\nCTRL+c to exit\n");
		
		int pid = fork();
		if(pid < 0) {
			perror("Fork error");
			return;
		} else if(pid == 0) {
			while(1) {
				fork();
			}
			// exit child process after fork()
			exit(0);
		}
	}
	return;
}

void forkWaitExecExample() {
	pid_t pid;

	 // Forking a child process
	pid = fork();

	if(pid < 0) {
		printf("Fork failed\n");
		exit(0);
   	 }
   	 else if(pid == 0) {
       		 printf("Child process executing...\n");
       		 execl("/bin/pwd", "pwd", NULL);
       		 perror("Fork failed");
    }
    else {
        wait(NULL);
        printf("Child process has finished. Finalizing parent process\n");
    }
}

void rforkExeclpExample() {
	// rfork() - advanced (resource) fork that allows better control over
	// shared resources of different processes
	pid_t pid = fork();
	
	if(pid < 0) {
		perror("Fork failed");
		exit(0);
	} else if(pid == 0) {
		printf("Child process executing with execlp...\n");
		execlp("ls", "ls", "-l", NULL);

		perror("Execlp Failed");
		exit(0);
	} else {
		printf("Child process finished. Finalizing parent process...\n");
	}
}

void draw(char* option) {
	if(strcmp(option, "-vietcong") == 0) {
		printf("_/﹋/_\n");
		printf("(҂`_´)\n");
		printf("<,︻╦╤─ ҉ - -\n");
		printf("/﹋|\n");
	} else if(strcmp(option, "-owl") == 0) {
		printf(",___,\n");
		printf("[O.o]\n");
		printf("/)__)\n");
		printf("-*--*-\n");
	}else if(strcmp(option, "default") == 0) {
		printf("( ) ( )\n");
		printf("(* .*)\n");
		printf("(”__”)\n");
	} else {
		printf("I don't know how to draw that :(\n");
	}
}

void guess() {
	printf("Welcome to the guessing game!\n");
	printf("Guess the number from 0 up to 100\n");
	printf("If you want to quit, enter -1\n");
	printf("Input your guesses:\n");
	int num = rand() % 100;
	int guess;
	int count = 0;
	while(1) {
		scanf("%d",&guess);
		count++;

		if(guess == -1) {
			printf("Goodbye :(\n");
			break;
			}

		if(guess == num) {
			printf("Bravo!\n");
			printf("It took you %d guesses\n", count);
			break;
		} else if(guess > num){
			printf("Lower!\n");
		} else {
			printf("Higher!\n");
		}
	}

}

int main() {

	char command[MAX_COMMAND_LENGTH];
	char* args[MAX_ARGS_NUM];
	char* input;
	int isRunning = 1;
	
	while(isRunning) {
		displayPrompt();
		input = fgets(command, sizeof(command), stdin);
		input[strcspn(input, "\n")] = '\0';

		history[historyIndex] = strdup(input);
		historyIndex++;
		if(historyIndex == HISTORY_SIZE) { historyIndex = 0; }

		if(input == NULL) { break; }

		char* split = strtok(command, "\n \t");
		int i = 0;
		// in the while loop, we must specity max_args - 1, the -1
		// beause after the while loop; i = 31, and we set args[31]
		// to NULL since we can't have args[32]
		while(split != NULL && i < MAX_ARGS_NUM - 1) {
			args[i++] = split;
			// with the line below, if we remove it, split = NULL
			// with it, we tell the split to tokenize the same
			// char array as before
			split = strtok(NULL, "\n \t");
		}
		args[i] = NULL;
		if(i == 0) { continue; }

		if(strcmp(args[0], "exit") == 0) { isRunning = 0; }
		else if(strcmp(args[0], "cp") == 0) { copy(args); }
		else if(strcmp(args[0], "history") == 0) { displayHistory(); }
		else if(strcmp(args[0], "free") == 0) { displayFree(); }
		else if(strcmp(args[0], "fortune") == 0) { displayFortune(); }
		else if(strcmp(args[0], "forkbomb") == 0) { forkbomb(); }
		else if(strcmp(args[0], "forkwaitexec") == 0) { forkWaitExecExample(); }
		else if(strcmp(args[0], "rforkexeclp") == 0) { rforkExeclpExample(); }
		else if(strcmp(args[0], "draw") == 0) {
 			if(args[1] == NULL) { draw("default"); }
			else { draw(args[1]); }
			
		 }
		else if(strcmp(args[0], "guess") == 0) { guess(); }
		else {
			executeCommand(args);
		}
	}


	return 0;
}
