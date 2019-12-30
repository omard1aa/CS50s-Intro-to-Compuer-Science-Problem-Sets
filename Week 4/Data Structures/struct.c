#include <stdio.h>
#include <cs50.h>
#include "struct.h"

int main(int args, char* argv[]){
    const int length = get_int("Enter number of students: \n");
    student students[length];
    for(int i = 0; i < length; i ++){
        students[i].name = get_string("Enter student name: ");
        students[i].id = get_int("Enter student id: ");
    }

    for(int i = 0; i < length; i++){
        printf("Student id: %i \n", students[i].id);
        printf("Student name: %s\n", students[i].name);
    }
}