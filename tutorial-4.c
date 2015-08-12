#include <Python.h>
#include <stdio.h>
#include <pthread.h>

int stop_event = 0;

void* run_python_function(void* arg)
{
    PyGILState_STATE state = PyGILState_Ensure();

    PyObject *pModule = NULL, *pFunc = NULL;
    do
    {
        pModule = PyImport_ImportModule("tutorial-4");
        if (pModule == NULL) break;

        pFunc = PyObject_GetAttrString(pModule, "working");
        if (pFunc == NULL) break;

        char* name = *((char**) arg);
        while (!stop_event)
        {
            PyObject_CallFunction(pFunc, "s", name);
        }
    } while (0);

    Py_XDECREF(pFunc);
    Py_XDECREF(pModule);

    PyGILState_Release(state);

    pthread_exit(NULL);

    return NULL;
}

int main(int argc, char* argv[])
{
    PyEval_InitThreads();
    Py_Initialize();
    PyObject* sysPath = PySys_GetObject((char*) "path");
    PyList_Append(sysPath, PyString_FromString("."));

    PyThreadState* save = PyEval_SaveThread();

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

    PyEval_RestoreThread(save);
    Py_Finalize();

    pthread_exit(NULL);

    return 0;
}
