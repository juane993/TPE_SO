#include <process.h>


#define EOF -1
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

int8_t run_test_proc(char * argv, char * bgState)
{
    BackgroundState bg = strcmp(bgState, "bg") == 0 ? BACKGROUND : FOREGROUND;
    int8_t child = my_create_process(test_processes, 1, &argv, HIGH, bg);
//    if(bg == FOREGROUND){
//        my_waitPid(child);
//    } else{
//        my_waitPidNoHang(child);
//    }
    return child;
}

int8_t run_test_prio(char * bgState){
    BackgroundState bg = strcmp(bgState, "bg") == 0 ? BACKGROUND : FOREGROUND;
    int8_t child = my_create_process(test_prio, 0, (void *) 0, HIGH, bg);
//    if(bg == FOREGROUND){
//        my_waitPid(child);
//    } else{
//        my_waitPidNoHang(child);
//    }
    return child;
}

int8_t run_test_sync(char * arg1, char * argv2, char * argv3, char * bgState){
    char * argv[] = {arg1, argv2, argv3};
    BackgroundState bg = strcmp(bgState, "bg") == 0 ? BACKGROUND : FOREGROUND;
   // my_openPipe(5);
    int8_t child = my_create_process(test_sync, 3, argv, HIGH, bg);

   // my_mapFD(child, STDOUT, 5);

//    if(bg == FOREGROUND){
//        my_waitPid(child);
//    } else{
//        my_waitPidNoHang(child);
//    }


    return child;
}

int8_t run_test_mem(char * arg1, char * bgState)
{
    BackgroundState bg = strcmp(bgState, "bg") == 0 ? BACKGROUND : FOREGROUND;
    char * argv[] ={arg1};
    int8_t child = my_create_process(test_mm, 1, argv, HIGH, bg);
    return child;
}

void idleProcess(){
    while(1){
        _hlt();
    }
}

int8_t run_idle_process()
{
    return my_create_process(idleProcess, 0, (void *) 0, LOW, BACKGROUND);
}

int8_t filter() {
    char c;
    while ((int) (c = getchar()) != EOF) {
        if (toLower(c) == 'a' || toLower(c) == 'e' || toLower(c) == 'i' || toLower(c) == 'o' || toLower(c) == 'u')
            putchar(c);
    }
    putchar('\n');
    return 0;
}

int8_t wc() {
    char c;
    int lineCounter = 0;
    while ((int) (c = getchar()) != EOF)
        lineCounter += (c == '\n');
    printf("La cantidad de lineas es: %d\n", lineCounter);
    return 0;
}

int8_t cat() {
    char c;
    while ((int) (c = getchar()) != EOF) {
        putchar(c);
    }
    return 0;
}

int8_t run_filter(char * bgState){
    BackgroundState bg = strcmp(bgState, "bg") == 0 ? BACKGROUND : FOREGROUND;


    int8_t child = my_create_process(filter, 0, (void *)0, HIGH, bg);

//    if(bg == FOREGROUND){
//        my_waitPid(child);
//    } else{
//        my_waitPidNoHang(child);
//    }
    return child;


}

int8_t run_cat(char * bgState){

    if(strcmp(bgState, "bg") == 0){
        printErr("Foreground execution is required for cat\n");
        return -1;
    }
    if(strcmp(bgState, "fg") != 0){
        printErr("Invalid background state\n");
        return -1;
    }

    BackgroundState bg = FOREGROUND;

    int8_t child = my_create_process(cat, 0, (void *)0, HIGH, bg);

//    if(bg == FOREGROUND){
//        my_waitPid(child);
//    } else{
//        my_waitPidNoHang(child);
//    }
    return child;
}

int8_t run_wc(char * bgState){

    BackgroundState bg = strcmp(bgState, "bg") == 0 ? BACKGROUND : FOREGROUND;

    int8_t child = my_create_process(wc, 0, (void *)0, HIGH, bg);


    return child;
}

int8_t loop(char *sec) {

    uint16_t pid = my_getPid();
    while (1) {
        my_sleep(atoi(sec));
        printf("Hola, soy el proceso: %d\n", pid);
    }
    return 0;
}



int8_t run_loop(char * arg1, char * bgState){

    char * argv[] = {arg1};
    BackgroundState bg = strcmp(bgState, "bg") == 0 ? BACKGROUND : FOREGROUND;


    int8_t child = my_create_process(loop, 1, argv, LOW, bg);

//    if(bg == FOREGROUND){
//        my_waitPid(child);
//    } else{
//        my_waitPidNoHang(child);
//    }

    return child;
}