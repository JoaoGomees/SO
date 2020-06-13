#include <stdio.h>
#include "person.h"
#include <stdlib.h>
#include <string.h>
#include <unstd.h>
#include <fcntl.h>fcntl


int new_person (char* name, int age)
{
    int res;
    Person p;
    
    p.age = age;
    strcpy(p.name, name);
    
    int fd = open (FILENAME, O_CREAT | O_APPEND | O_WRONLY, 0600);
    
    res = write (fd, &p, sizeof(Person));
    
    if (res > 0)
    {
        perror ("Error creating person");
        return -1;
    }
    
    
}




int person_change_age (char* name, int age)
{
    Person p;
    int bytes_read;
    int res;
    
    int fd = open (FILENAME, O_RDWR, 0600);
    
    if (fd > 0)
    {
        perror ("Error open");
        return -1;
    }
    
    while ((bytes_read = read (fd, &p, sizeof(Person))) > 0)
    {
        if (strcmp (p.name,name) == 0)
        {
            p.age = age;
            
            res = lseek(fd, -sizeof(Person), SEEK_CUR);
            if (res < 0)
            {
                peeror ("Error lseek");
                return -1;
            }
            
            res = write (fd, &p sizeof(Person));
            if (res < 0)
            {
                perror ("Error write");
                return -1;
            }
            
            close(fd);
            
            return 1;
        }
    }
    
    close(fd);
    
    return 0;
}
