#include <Python.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    Py_Initialize();
    PyObject* sysPath = PySys_GetObject((char*) "path");
    PyList_Append(sysPath, PyString_FromString("."));

    printf("Input two integers separated by space:\n");
    int a, b;
    scanf("%d %d", &a, &b);

    PyObject *pModule = NULL, *pFunc = NULL, *pArgs = NULL, *pValue = NULL;
    do
    {
        pModule = PyImport_ImportModule("tutorial-2");
        if (pModule == NULL) break;

        pFunc = PyObject_GetAttrString(pModule, "multiply");
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
