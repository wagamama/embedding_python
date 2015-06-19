#include <Python.h>
#include <stdio.h>

static char* PY_MODULE_NAME = "multiply";
static char* PY_FUNCTION_NAME = "multiply";

int main(int argc, char* argv[])
{
    int a, b;
    PyObject *pModule, *pFunc, *pArgs, *pValue;

    Py_Initialize();
    PyObject* sysPath = PySys_GetObject((char*) "path");
    PyList_Append(sysPath, PyString_FromString("."));

    printf("Input two integers separated by space:\n");
    scanf("%d %d", &a, &b);

    pModule = PyImport_ImportModule(PY_MODULE_NAME);
    pFunc = PyObject_GetAttrString(pModule, PY_FUNCTION_NAME);
    pArgs = Py_BuildValue("ii", a, b);
    pValue = PyObject_Call(pFunc, pArgs, NULL);
    printf("Result of call: %ld\n", PyInt_AsLong(pValue));

    Py_DECREF(pValue);
    Py_DECREF(pArgs);
    Py_DECREF(pFunc);
    Py_DECREF(pModule);

    Py_Finalize();
    return 0;
}
