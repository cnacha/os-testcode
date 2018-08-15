#include <unistd.h>
#include <stdlib.h>

void main() {
	int i = 0;

	while (i < 2) {
		fork();
		system("ps -o pid,ppid,comm,stat");
		//wait();
		i++;

	}
}
