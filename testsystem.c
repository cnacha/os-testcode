#include <stdio.h>
#include <unistd.h>

void main(){
	system("ps");

	  for (int count = 0; count < 3; ++count) {
	    if (fork() == 0) {
	      system("ps");
	    }
	  }
}
