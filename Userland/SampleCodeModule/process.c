#include <process.h>


#define EOF '3'
#define toLower(n) ((n) >= 'A' && (n) <= 'Z' ? (n) + ('a' - 'A') : (n))


procesess proc[QTY_PROC];

void initProcesess()
{

    proc[0].name = "testproc";
    proc[0].entryPoint = test_processes;

    proc[1].name = "testprio";
    proc[1].entryPoint = test_prio;

    proc[2].name = "endlessloop";
    proc[2].entryPoint = endless_loop;

    proc[3].name = "testsync";
    proc[3].entryPoint = test_sync;
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
            printf("Creating process...with pid%d\n",pid);
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

void run_test_proc(char * argv, char * bgState)
{
    BackgroundState bg = strcmp(bgState, "bg") == 0 ? BACKGROUND : FOREGROUND;
    uint8_t child = my_create_process(test_processes, 1, &argv, HIGH, bg);
    if(bg == FOREGROUND){
        my_waitPid(child);
    } else{
        my_waitPidNoHang(child);
    }
}

void run_test_prio(char * bgState){
    BackgroundState bg = strcmp(bgState, "bg") == 0 ? BACKGROUND : FOREGROUND;
    uint8_t child = my_create_process(test_prio, 0, (void *) 0, HIGH, bg);
    if(bg == FOREGROUND){
        my_waitPid(child);
    } else{
        my_waitPidNoHang(child);
    }
}

void run_test_sync(char * arg1, char * argv2, char * argv3, char * bgState){
    char * argv[] = {arg1, argv2, argv3};
    BackgroundState bg = strcmp(bgState, "bg") == 0 ? BACKGROUND : FOREGROUND;

    uint8_t child = my_create_process(test_sync, 3, argv, HIGH, bg);

    if(bg == FOREGROUND){
        my_waitPid(child);
    } else{
        my_waitPidNoHang(child);
    }


}


void idleProcess(){
    while(1){
        _hlt();
    }
}

void run_idle_process()
{
    my_create_process(idleProcess, 0, (void *) 0, LOW, BACKGROUND);
}

int filter() {
    char c;
    while ((int) (c = getchar()) != EOF) {
        if (toLower(c) == 'a' || toLower(c) == 'e' || toLower(c) == 'i' || toLower(c) == 'o' || toLower(c) == 'u')
            putchar(c);
    }
    putchar('\n');
    return 0;
}

int wc() {
    char c;
    int lineCounter = 0;
    while ((int) (c = getchar()) != EOF)
        lineCounter += (c == '\n');
    printf("La cantidad de lineas es: %d\n", lineCounter);
    return 0;
}

int cat() {
    char c;
    while ((int) (c = getchar()) != '3') {
        putchar(c);
    }
    return 0;
}

void run_filter(){
    my_create_process(filter, 0, (void *) 0, HIGH, FOREGROUND);
}

void run_cat(){
    my_create_process(cat, 0, (void *) 0, HIGH, FOREGROUND);
}

void run_wc(){
    my_create_process(wc, 0, (void *) 0, HIGH, FOREGROUND);

}


