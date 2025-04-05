#include <Python.h>
#include <iostream>

int main() {
    Py_Initialize();
    PyObject* sysPath = PySys_GetObject((char*)"path");
    PyObject_Print(sysPath, stdout, 0);
    Py_Finalize();
    return 0;
}