#include <Python.h>

int main(int argc, char* argv[])
{
    Py_Initialize();
    PyObject* sysPath = PySys_GetObject((char*) "path");
    PyList_Append(sysPath, PyString_FromString("."));

    PyRun_SimpleString("from time import time, ctime\n"
                        "print 'Today is', ctime(time())\n");

    Py_Finalize();
    return 0;
}
