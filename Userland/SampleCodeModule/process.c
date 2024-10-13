#include "include/process.h"

procesess proc[QTY_PROC];

void initProcesess()
{

    proc[0].name = "testprocesses";
    proc[0].entryPoint = test_processes;

    proc[1].name = "testprio";
    proc[1].entryPoint = test_prio;

    proc[2].name = "endlessloop";
    proc[2].entryPoint = endless_loop;

}

int8_t runProcess(char *name,char * bgState, char *argv[] )
{
    int8_t bg = atoi(bgState);
    if(bg != 0 && bg != 1){
        printErr("Invalid background state\n");
        return -1;
    }
    printf("%s\n",name);
    for (int i = 0; i < QTY_PROC; i++)
    {
        if (strcmp(proc[i].name, name) == 0)
        {
            printf("Creating process... \n");
            return my_create_process(proc[i].entryPoint, 0, argv, LOW, bg);
        }
    }
    printErr("Invalid process name\n");
    return -1;
}