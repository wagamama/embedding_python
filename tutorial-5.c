#include <Python.h>
#include <stdio.h>

static char* MODULE_NAME = "cmodule";
static char* FUNCTION_NAME = "callback";
static PyObject* callback(PyObject* self, PyObject* args)
{
    long c;
    PyArg_ParseTuple(args, "l", &c);
    printf("Result of call: %ld\n", c);

    Py_RETURN_NONE;
}

static char* PY_MODULE_NAME = "tutorial-5";
static char* PY_FUNCTION_NAME = "multiply";

int main(int argc, char* argv[])
{
    int a, b;
    PyObject *pModule = NULL, *pFunc = NULL, *pArgs = NULL;

    Py_Initialize();
    PyObject* sysPath = PySys_GetObject((char*) "path");
    PyList_Append(sysPath, PyString_FromString("."));

    printf("Input two integers separated by space:\n");
    scanf("%d %d", &a, &b);

    do
    {
        PyMethodDef CFunctions[] = {
            {FUNCTION_NAME, callback, METH_VARARGS, ""},
            {NULL, NULL, 0, NULL}
        };
        Py_InitModule(MODULE_NAME, CFunctions);

        pModule = PyImport_ImportModule(PY_MODULE_NAME);
        if (pModule == NULL) break;

        pFunc = PyObject_GetAttrString(pModule, PY_FUNCTION_NAME);
        if (pFunc == NULL) break;

        pArgs = Py_BuildValue("ii", a, b);
        if (pArgs == NULL) break;

        PyObject_Call(pFunc, pArgs, NULL);
    } while (0);

    Py_XDECREF(pArgs);
    Py_XDECREF(pFunc);
    Py_XDECREF(pModule);

    Py_Finalize();
    return 0;
}
