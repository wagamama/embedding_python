#include <Python.h>
#include <stdio.h>

static char* FUNCTION1_NAME = "callback1";
static char* FUNCTION2_NAME = "callback2";
static PyObject* callback1(PyObject* self, PyObject* args)
{
    char* str;
    PyArg_ParseTuple(args, "s", &str);
    printf("%s push event\n", str);

    Py_RETURN_NONE;
}
static PyObject* callback2(PyObject* self, PyObject* args)
{
    char* str;
    PyArg_ParseTuple(args, "s", &str);
    printf("%s start heavy computing\n", str);

    PyThreadState* save = PyEval_SaveThread();
    {
        sleep(5);
    }
    PyEval_RestoreThread(save);

    Py_RETURN_NONE;
}

static char* PY_MODULE_NAME = "tutorial-8";
static char* PY_THREAD_MANAGER_CLASS = "ThreadManager";
static char* PY_START_THREAD_FUNCTION = "start_thread";
static char* PY_STOP_THREAD_FUNCTION = "stop_thread";

int main(int argc, char* argv[])
{
    PyObject *pCallback1Func = NULL, *pCallback2Func = NULL;
    PyObject *pModule = NULL, *pClass = NULL, *pInst = NULL, *pArgs = NULL;
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
            PyMethodDef CFunc1 = {FUNCTION1_NAME, callback1, METH_VARARGS, ""};
            pCallback1Func = PyCFunction_New(&CFunc1, NULL);
            if (pCallback1Func == NULL) break;

            PyMethodDef CFunc2 = {FUNCTION2_NAME, callback2, METH_VARARGS, ""};
            pCallback2Func = PyCFunction_New(&CFunc2, NULL);
            if (pCallback2Func == NULL) break;

            pModule = PyImport_ImportModule(PY_MODULE_NAME);
            if (pModule == NULL) break;

            pClass = PyObject_GetAttrString(pModule, PY_THREAD_MANAGER_CLASS);
            if (pClass == NULL) break;

            pArgs = Py_BuildValue("(OO)", pCallback1Func, pCallback2Func);
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

    Py_XDECREF(pCallback1Func);
    Py_XDECREF(pCallback2Func);
    Py_XDECREF(pArgs);
    Py_XDECREF(pInst);
    Py_XDECREF(pClass);
    Py_XDECREF(pModule);

    Py_Finalize();
    return 0;
}
