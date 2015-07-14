#include <Python.h>
#include <stdio.h>

static char* PY_MODULE_NAME = "tutorial-2";
static char* PY_FUNCTION_NAME = "multiply";

int main(int argc, char* argv[])
{
    int a, b;
    PyObject *pModule = NULL, *pFunc = NULL, *pArgs = NULL, *pValue = NULL;

    Py_Initialize();
    PyObject* sysPath = PySys_GetObject((char*) "path");
    PyList_Append(sysPath, PyString_FromString("."));

    printf("Input two integers separated by space:\n");
    scanf("%d %d", &a, &b);

    do
    {
        pModule = PyImport_ImportModule(PY_MODULE_NAME);
        if (pModule == NULL) break;

        pFunc = PyObject_GetAttrString(pModule, PY_FUNCTION_NAME);
        if (pFunc == NULL) break;

        pArgs = Py_BuildValue("ii", a, b);
        if (pArgs == NULL) break;

        pValue = PyObject_Call(pFunc, pArgs, NULL);
        if (pValue == NULL) break;
        printf("Result of call: %ld\n", PyInt_AsLong(pValue));
    } while (0);

    Py_XDECREF(pValue);
    Py_XDECREF(pArgs);
    Py_XDECREF(pFunc);
    Py_XDECREF(pModule);

    Py_Finalize();
    return 0;
}
