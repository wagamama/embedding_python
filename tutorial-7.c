#include <Python.h>
#include <stdio.h>

static PyObject* callback(PyObject* self, PyObject* args)
{
    char* str;
    PyArg_ParseTuple(args, "s", &str);
    printf("receive event from %s\n", str);

    Py_RETURN_NONE;
}

int main(int argc, char* argv[])
{
    PyEval_InitThreads();
    Py_Initialize();
    PyObject* sysPath = PySys_GetObject((char*) "path");
    PyList_Append(sysPath, PyString_FromString("."));

    PyThreadState* save = PyEval_SaveThread();

    PyObject *pCallbackFunc = NULL, *pModule = NULL, *pClass = NULL, *pInst = NULL, *pArgs = NULL;
    do
    {
        PyGILState_STATE state = PyGILState_Ensure();
        {
            PyMethodDef CFunc = {"callback", callback, METH_VARARGS, ""};
            pCallbackFunc = PyCFunction_New(&CFunc, NULL);
            if (pCallbackFunc == NULL) break;

            pModule = PyImport_ImportModule("worker-callback");
            if (pModule == NULL) break;

            pClass = PyObject_GetAttrString(pModule, "ThreadManager");
            if (pClass == NULL) break;

            pArgs = Py_BuildValue("(OO)", pCallbackFunc, pCallbackFunc);
            if (pArgs == NULL) break;

            pInst = PyObject_Call(pClass, pArgs, NULL);
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

    PyEval_RestoreThread(save);

    Py_XDECREF(pCallbackFunc);
    Py_XDECREF(pArgs);
    Py_XDECREF(pInst);
    Py_XDECREF(pClass);
    Py_XDECREF(pModule);

    Py_Finalize();
    return 0;
}
