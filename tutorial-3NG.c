#include <Python.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    PyEval_InitThreads();
    Py_Initialize();
    PyObject* sysPath = PySys_GetObject((char*) "path");
    PyList_Append(sysPath, PyString_FromString("."));

    PyObject *pModule = NULL, *pClass = NULL, *pInst = NULL;
    do
    {
        PyGILState_STATE state = PyGILState_Ensure();
        {
            pModule = PyImport_ImportModule("worker");
            if (pModule == NULL) break;

            pClass = PyObject_GetAttrString(pModule, "ThreadManager");
            if (pClass == NULL) break;

            pInst = PyObject_CallObject(pClass, NULL);
            if (pInst == NULL) break;

            PyObject_CallMethod(pInst, "start_thread", NULL);
        }
        PyGILState_Release(state);

        for (int i = 0; i < 5; i++)
        {
            printf("main thread is running\n");
            sleep(1);
        }

        state = PyGILState_Ensure();
        {
            PyObject_CallMethod(pInst, "stop_thread", NULL);
        }
        PyGILState_Release(state);

        printf("finish\n");
    } while (0);

    Py_XDECREF(pInst);
    Py_XDECREF(pClass);
    Py_XDECREF(pModule);

    Py_Finalize();
    return 0;
}
