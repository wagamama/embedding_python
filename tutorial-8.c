#include <Python.h>
#include <stdio.h>

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
    printf("%s blocking start\n", str);

    PyThreadState* save = PyEval_SaveThread();
    {
        sleep(5);
    }
    PyEval_RestoreThread(save);

    printf("%s blocking end\n", str);

    Py_RETURN_NONE;
}

int main(int argc, char* argv[])
{
    PyEval_InitThreads();
    Py_Initialize();
    PyObject* sysPath = PySys_GetObject((char*) "path");
    PyList_Append(sysPath, PyString_FromString("."));

    PyThreadState* save = PyEval_SaveThread();

    PyObject *pCallback1Func = NULL, *pCallback2Func = NULL;
    PyObject *pModule = NULL, *pClass = NULL, *pInst = NULL, *pArgs = NULL;
    do
    {
        PyGILState_STATE state = PyGILState_Ensure();
        {
            PyMethodDef CFunc1 = {"callback1", callback1, METH_VARARGS, ""};
            pCallback1Func = PyCFunction_New(&CFunc1, NULL);
            if (pCallback1Func == NULL) break;

            PyMethodDef CFunc2 = {"callback2", callback2, METH_VARARGS, ""};
            pCallback2Func = PyCFunction_New(&CFunc2, NULL);
            if (pCallback2Func == NULL) break;

            pModule = PyImport_ImportModule("tutorial-8");
            if (pModule == NULL) break;

            pClass = PyObject_GetAttrString(pModule, "ThreadManager");
            if (pClass == NULL) break;

            pArgs = Py_BuildValue("(OO)", pCallback1Func, pCallback2Func);
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

    Py_XDECREF(pCallback1Func);
    Py_XDECREF(pCallback2Func);
    Py_XDECREF(pArgs);
    Py_XDECREF(pInst);
    Py_XDECREF(pClass);
    Py_XDECREF(pModule);

    Py_Finalize();
    return 0;
}
