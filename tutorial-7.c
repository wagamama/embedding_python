#include <Python.h>
#include <stdio.h>

static char* FUNCTION_NAME = "callback";
static PyObject* callback(PyObject* self, PyObject* args)
{
    char* str;
    PyArg_ParseTuple(args, "s", &str);
    printf("%s push event\n", str);

    Py_RETURN_NONE;
}

static char* PY_MODULE_NAME = "tutorial-7";
static char* PY_THREAD_MANAGER_CLASS = "ThreadManager";
static char* PY_START_THREAD_FUNCTION = "start_thread";
static char* PY_STOP_THREAD_FUNCTION = "stop_thread";

int main(int argc, char* argv[])
{
    PyObject *pCallbackFunc = NULL, *pModule = NULL, *pClass = NULL, *pInst = NULL, *pArgs = NULL;
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
            PyMethodDef CFunc = {FUNCTION_NAME, callback, METH_VARARGS, ""};
            pCallbackFunc = PyCFunction_New(&CFunc, NULL);
            if (pCallbackFunc == NULL) break;

            pModule = PyImport_ImportModule(PY_MODULE_NAME);
            if (pModule == NULL) break;

            pClass = PyObject_GetAttrString(pModule, PY_THREAD_MANAGER_CLASS);
            if (pClass == NULL) break;

            pArgs = Py_BuildValue("(O)", pCallbackFunc);
            if (pArgs == NULL) break;

            pInst = PyObject_Call(pClass, pArgs, NULL);
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
    } while (0);

    PyEval_RestoreThread(save);

    Py_XDECREF(pCallbackFunc);
    Py_XDECREF(pArgs);
    Py_XDECREF(pInst);
    Py_XDECREF(pClass);
    Py_XDECREF(pModule);

    Py_Finalize();
    return 0;
}
