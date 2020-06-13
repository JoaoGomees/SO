#include <stdio.h>
#include "person.h"
#include <stdlib.h>
#include <string.h>
#include <unstd.h>
#include <fcntl.h>

int new_person (char* name, int age)
{
    int res;
    Person p;
    p.age = age;
    strcpy (p.name,name);
    
    int fd = open(FILENAME, O_CREAT | O_APPEND | O_WRONLY, 0600);
    
    res = write (fd,&p, sizeof(Person));
    
    if (res < 0)
    {
        perror ("Error creating person");
        return -1;
    }
    
    int local = lseek (fd,-sizeof(Person), SEEK_CUR);
    
    return local / sizeof(Person);
}


