#include <Python.h>
#include <stdio.h>

static PyObject* callback(PyObject* self, PyObject* args)
{
    long c;
    PyArg_ParseTuple(args, "l", &c);
    printf("Result of call: %ld\n", c);

    Py_RETURN_NONE;
}

int main(int argc, char* argv[])
{
    Py_Initialize();
    PyObject* sysPath = PySys_GetObject((char*) "path");
    PyList_Append(sysPath, PyString_FromString("."));

    printf("Input two integers separated by space:\n");
    int a, b;
    scanf("%d %d", &a, &b);

    PyObject *pCallbackFunc = NULL, *pModule = NULL, *pFunc = NULL, *pArgs = NULL;
    do
    {
        PyMethodDef CFunc = {"callback", callback, METH_VARARGS, ""};
        pCallbackFunc = PyCFunction_New(&CFunc, NULL);
        if (pCallbackFunc == NULL) break;

        pModule = PyImport_ImportModule("func");
        if (pModule == NULL) break;

        pFunc = PyObject_GetAttrString(pModule, "multiply");
        if (pFunc == NULL) break;

        pArgs = Py_BuildValue("iiO", a, b, pCallbackFunc);
        if (pArgs == NULL) break;

        PyObject_Call(pFunc, pArgs, NULL);
    } while (0);

    Py_XDECREF(pCallbackFunc);
    Py_XDECREF(pArgs);
    Py_XDECREF(pFunc);
    Py_XDECREF(pModule);

    Py_Finalize();
    return 0;
}
