
#include <Python.h> 

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>

#include<vector>
#include <iostream>

double RosenBrock(const double *xx ) // real function
{
  const double x = xx[0];
  const double y = xx[1];
  const double tmp1 = y-x*x;
  const double tmp2 = 1-x;
  return 100*tmp1*tmp1+tmp2*tmp2;
}

std::vector<double> GetDataFromList(PyObject* listObject){
   std::vector<double> listVec;
   for(Py_ssize_t listIter=0; listIter<PyList_Size(listObject);++listIter){
               listVec.push_back(PyFloat_AsDouble(PyList_GetItem(listObject,listIter)));
         }
   return listVec;
}


static PyObject *wrap_function(PyObject *self, PyObject *values)
{
    auto params = GetDataFromList(values);
    auto r = RosenBrock(params.data());
    return PyFloat_FromDouble(r);
}

static PyObject *
spam_system(PyObject *self, PyObject *args)
{
    
   //auto params = GetDataFromList(args);
    //auto r = RosenBrock(params.data());

    if (!PyArg_ParseTuple(args, "s", &command))
        return NULL;
    std::cout<<"CALLE\n";
    return PyFloat_FromDouble(0);
}

static PyObject *SpamError;
static PyMethodDef SpamMethods[] = {
    {"system",  spam_system, METH_VARARGS,
     "Execute a shell command."},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

static struct PyModuleDef spammodule = {
    PyModuleDef_HEAD_INIT,
    "spam",   /* name of module */
    "spam_doc", /* module documentation, may be NULL */
    -1,       /* size of per-interpreter state of the module,
                 or -1 if the module keeps state in global variables. */
    SpamMethods
};

/*PyMODINIT_FUNC
PyInit_spam(void)
{
    return PyModule_Create(&spammodule);
}
*/

PyMODINIT_FUNC
PyInit_spam(void)
{
    PyObject *m;

    m = PyModule_Create(&spammodule);
    if (m == NULL)
        return NULL;

    SpamError = PyErr_NewException("spam.error", NULL, NULL);
    Py_XINCREF(SpamError);
    if (PyModule_AddObject(m, "error", SpamError) < 0) {
        Py_XDECREF(SpamError);
        Py_CLEAR(SpamError);
        Py_DECREF(m);
        return NULL;
    }

    return m;
}

int main(int argc, char *argv[])
{
    wchar_t *program = Py_DecodeLocale(argv[0], NULL);
    if (program == NULL) {
        fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
        exit(1);
    }    
/* Add a built-in module, before Py_Initialize */
    if (PyImport_AppendInittab("spam", PyInit_spam) == -1) {
        fprintf(stderr, "Error: could not extend in-built modules table\n");
        exit(1);
    }
    Py_SetProgramName(program);

    Py_Initialize();
    _import_array();
    PyObject *pmodule = PyImport_ImportModule("spam");
    if (!pmodule) {
        PyErr_Print();
        fprintf(stderr, "Error: could not import module 'spam'\n");
    }

   PyObject *mpmodule = PyModule_GetDict(pmodule);
   PyObject *fsystem = PyDict_GetItemString(mpmodule, "system");
   if (fsystem) Py_INCREF(fsystem);

    float Array [] = {0.0,0.0};
    npy_intp dims[1] = { 2 };
    PyObject *py_array;
    
   auto pycode = Py_BuildValue("()");
   PyObject *result = PyObject_CallObject(fsystem,pycode);
   Py_DECREF(pycode);



    PyObject *pName = PyUnicode_FromString("scipy.optimize");
   // Import the file as a Python module.
   // return object is a new reference !
   PyObject *fModuleOpt = PyImport_Import(pName);
   //Py_DECREF(pName);
   if (!fModuleOpt) {
    std::cerr<<"--- ERROR: Can't import scipy "<< std::endl;
   }
   PyObject *mDict = PyModule_GetDict(fModuleOpt);
   PyObject *fMinimize = PyDict_GetItemString(mDict, "minimize");
   if (fMinimize) Py_INCREF(fMinimize);
   
  // PyObject *pycode = Py_BuildValue("()", "root_scipy.wrap_function");
   //PyObject *result = PyObject_CallObject(fMinimize, pycode);
   //Py_DECREF(pycode);
   //pName = PyUnicode_FromString("root_scipy");
   //PyObject *fRootScipy = PyImport_Import(pName);
   //PyRun_String("root_scipy.test('a')", Py_single_input, NULL, NULL);
   //Py_DECREF(pName);
   //if (!fModuleOpt) {
   // std::cerr<<"--- ERROR: Can't import root_scipy "<< std::endl;
   //}
   //mDict = PyModule_GetDict(fRootScipy);
   //PyObject *fTest = PyDict_GetItemString(mDict, "test");
   //pycode = Py_BuildValue("()");
   //PyObject *result = PyObject_CallObject(fTest,NULL);
   //Py_DECREF(pycode);

   if (fMinimize) Py_DECREF(fMinimize);
    PyMem_RawFree(program);
    Py_Finalize();
    return 0;
}