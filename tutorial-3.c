#include <Python.h>
#include <stdio.h>
#include <pthread.h>

static char* PY_MODULE_NAME = "tutorial-34";
static char* PY_WORKING_FUNCTION = "working";

int stop_event = 0;

void* run_python_function(void* arg)
{
    PyObject *pModule = NULL, *pFunc = NULL;

    Py_Initialize();
    PyObject* sysPath = PySys_GetObject((char*) "path");
    PyList_Append(sysPath, PyString_FromString("."));

    do
    {
        pModule = PyImport_ImportModule(PY_MODULE_NAME);
        if (pModule == NULL) break;

        pFunc = PyObject_GetAttrString(pModule, PY_WORKING_FUNCTION);
        if (pFunc == NULL) break;

        while (!stop_event)
        {
            PyObject_CallFunction(pFunc, "s", "worker");
        }

        break;
    } while (1);

    Py_XDECREF(pFunc);
    Py_XDECREF(pModule);

    Py_Finalize();

    return NULL;
}

int main(int argc, char* argv[])
{
    pthread_t tid;
    pthread_create(&tid, NULL, &run_python_function, NULL);

    for (int i = 0; i < 5; i++)
    {
        printf("main thread is running\n");
        sleep(1);
    }

    stop_event = 1;

    printf("finish\n");

    return 0;
}
