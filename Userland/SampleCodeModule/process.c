#include "include/process.h"

procesess proc[QTY_PROC];

void initProcesess()
{

    proc[0].name = "testproc";
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

            char pid = my_create_process(proc[i].entryPoint, 0, argv, HIGH, bg);
            printf("Creating process...with pid chuan: %d\n",pid);
            return pid;
        }
    }
    printErr("Invalid process name\n");
    return -1;
}
int8_t kill(char *pid)
{
    int8_t pidInt = atoi(pid);
    if(pidInt < 0){
        printErr("Invalid pid\n");
        return -1;
    }
    if(my_kill(pidInt) == -1){
        printErr("Error killing process\n");
        return -1;
    }
    return 0;
}

