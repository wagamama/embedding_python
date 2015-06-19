#include <Python.h>

int main(int argc, char* argv[])
{
    Py_Initialize();
    PyObject* sysPath = PySys_GetObject((char*) "path");
    PyList_Append(sysPath, PyString_FromString("."));

    PyRun_SimpleString("print 'Hello World from Python'");

    Py_Finalize();
    return 0;
}
