// CS340_UOA - tweedle.c 
//
// Demonstrate non-deterministic scheduling with help from 
// `fork()` and Lewis Carroll
//
// Cam Smith, 2017-08-15
// Copyright and related rights waived via CC0

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

void dee();
void dum();
void magic();

int main() {
    // Escape code reference: http://ascii-table.com/ansi-escape-sequences.php
    // and http://ascii-table.com/ansi-escape-sequences-vt-100.php

    // erase the display to make things easier to read
    printf("\E[2J");
    // move cursor to home position (top left corner)
    printf("\Ec \n");

    pid_t pid;
    pid = fork();

    // I guess this means tweedledee is technically the big brother
    if (pid == 0) 
        dee();
    else 
        dum();

}


void dee () {
    //    Escape code for green text                      escape to reset
    puts("\E[32m     TweedleDee reporting for duty!       \E[0m");
    magic();
                 
    puts("\E[32m     Tweedledum and Tweedledee            \E[0m");
    magic();
                 
    puts("\E[32m     Agreed to have a battle;             \E[0m");
    magic();
                 
    puts("\E[32m     For Tweedledum said Tweedledee       \E[0m");
    magic();    
                 
    puts("\E[32m     Had spoiled his nice new rattle.     \E[0m");
    magic();
}


void dum() {
    //    Escape code for blue text                       escape to reset
    puts("\E[34m     TweedleDum at your service!          \E[0m");
    magic();

    puts("\E[34m     Just then flew down a monstrous crow,\E[0m");
    magic();

    puts("\E[34m     As black as a tar-barrel;            \E[0m");
    magic();

    puts("\E[34m     Which frightened both the heroes so, \E[0m");
    magic();

    puts("\E[34m     They quite forgot their quarrel.     \E[0m");
    magic();
    // why do we set the colour at the start of each line again???
}

// This function sleeps to increase the chances of our threads being
// put to sleep by the kernel, and flushes the standard output to make
// this more obvious than nature has intended
void magic() {
    sleep(0.01);
    fflush(stdin);
}
