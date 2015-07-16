#include <Python.h>
#include <stdio.h>
#include <pthread.h>

static char* PY_MODULE_NAME = "tutorial-3";
static char* PY_WORKING_FUNCTION = "working";

int stop_event = 0;

void* run_python_function(void* arg)
{
    char* name = *((char**) arg);
    PyObject *pModule = NULL, *pFunc = NULL;

    do
    {
        pModule = PyImport_ImportModule(PY_MODULE_NAME);
        if (pModule == NULL) break;

        pFunc = PyObject_GetAttrString(pModule, PY_WORKING_FUNCTION);
        if (pFunc == NULL) break;

        while (!stop_event)
        {
            PyObject_CallFunction(pFunc, "s", name);
        }
    } while (0);

    Py_XDECREF(pFunc);
    Py_XDECREF(pModule);

    pthread_exit(NULL);

    return NULL;
}

int main(int argc, char* argv[])
{
    Py_Initialize();
    PyObject* sysPath = PySys_GetObject((char*) "path");
    PyList_Append(sysPath, PyString_FromString("."));

    pthread_t tid1, tid2;
    char* tname1 = "worker1";
    char* tname2 = "worker2";
    pthread_create(&tid1, NULL, &run_python_function, &tname1);
    pthread_create(&tid2, NULL, &run_python_function, &tname2);

    for (int i = 0; i < 5; i++)
    {
        printf("main thread is running\n");
        sleep(1);
    }

    stop_event = 1;
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    printf("finish\n");
    Py_Finalize();

    pthread_exit(NULL);

    return 0;
}
