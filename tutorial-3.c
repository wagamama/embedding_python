#include <Python.h>
#include <stdio.h>

static char* PY_MODULE_NAME = "tutorial-34";
static char* PY_THREAD_MANAGER_CLASS = "ThreadManager";
static char* PY_START_THREAD_FUNCTION = "start_thread";
static char* PY_STOP_THREAD_FUNCTION = "stop_thread";

int main(int argc, char* argv[])
{
    PyObject *pModule = NULL, *pClass = NULL, *pInst = NULL;
    PyGILState_STATE state;

    PyEval_InitThreads();
    Py_Initialize();
    PyObject* sysPath = PySys_GetObject((char*) "path");
    PyList_Append(sysPath, PyString_FromString("."));

    PyThreadState* save = PyEval_SaveThread();

    do
    {
        state = PyGILState_Ensure();
        {
            pModule = PyImport_ImportModule(PY_MODULE_NAME);
            if (pModule == NULL) break;

            pClass = PyObject_GetAttrString(pModule, PY_THREAD_MANAGER_CLASS);
            if (pClass == NULL) break;

            pInst = PyObject_CallObject(pClass, NULL);
            if (pInst == NULL) break;

            PyObject_CallMethod(pInst, PY_START_THREAD_FUNCTION, NULL);
        }
        PyGILState_Release(state);

        for (int i = 0; i < 5; i++)
        {
            printf("main thread is running\n");
            sleep(1);
        }

        state = PyGILState_Ensure();
        {
            PyObject_CallMethod(pInst, PY_STOP_THREAD_FUNCTION, NULL);
        }
        PyGILState_Release(state);

        printf("finish\n");
        break;
    } while (1);

    PyEval_RestoreThread(save);

    Py_XDECREF(pInst);
    Py_XDECREF(pClass);
    Py_XDECREF(pModule);

    Py_Finalize();
    return 0;
}
