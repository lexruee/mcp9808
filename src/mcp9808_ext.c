/*
 * @author 	Alexander Rüedlinger <a.rueedlinger@gmail.com>
 * @date 	9.03.2015
 *
 * Python bindings for the MCP9808 driver written in C.
 *
 */

#include <Python.h>
#include <structmember.h>
#include "mcp9808.h"

typedef struct {
	PyObject_HEAD
	void *mcp9808;
} MCP9808_Object;



static void MCP9808_dealloc(MCP9808_Object *self) {
	mcp9808_close(self->mcp9808);
	self->ob_type->tp_free((PyObject*)self);
}



static PyObject *MCP9808_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
	MCP9808_Object *self;
	self = (MCP9808_Object *) type->tp_alloc(type, 0);
	return (PyObject *) self;
}



static int MCP9808_init(MCP9808_Object *self, PyObject *args, PyObject *kwds) {
	int address;
	const char *i2c_device;
	static char *kwlist[] = {"address", "i2c_devcie", NULL};

	if(!PyArg_ParseTupleAndKeywords(args, kwds, "is", kwlist, &address, &i2c_device))
		return -1;

	if(i2c_device) {
		self->mcp9808 = mcp9808_init(address, i2c_device);
		if(self->mcp9808 == NULL) {
			PyErr_SetString(PyExc_RuntimeError, "Cannot initialize sensor. Run program as root and check i2c device / address.");
			return -1;
		}
	}
	return 0;
}



static PyObject *MCP9808_temperature(MCP9808_Object *self) {
	PyObject *result;
	double temperature = mcp9808_temperature(self->mcp9808);
	result = PyFloat_FromDouble(temperature);
	return result;
}



static PyMethodDef MCP9808_methods[] = {
	{"temperature", (PyCFunction) MCP9808_temperature, METH_NOARGS, "Returns a temperature value"},
	{NULL}  /* Sentinel */
};



static PyMemberDef MCP9808_members[] = {
    {NULL}  /* Sentinel */
};



static PyTypeObject MCP9808_Type = {
	PyObject_HEAD_INIT(NULL)
	0,                         /*ob_size*/
	"tentacle_pi.MCP9808",             /*tp_name*/
	sizeof(MCP9808_Object),             /*tp_basicsize*/
	0,                         /*tp_itemsize*/
	(destructor)MCP9808_dealloc, /*tp_dealloc*/
	0,                         /*tp_print*/
	0,                         /*tp_getattr*/
	0,                         /*tp_setattr*/
	0,                         /*tp_compare*/
	0,                         /*tp_repr*/
	0,                         /*tp_as_number*/
	0,                         /*tp_as_sequence*/
	0,                         /*tp_as_mapping*/
	0,                         /*tp_hash */
	0,                         /*tp_call*/
	0,                         /*tp_str*/
	0,                         /*tp_getattro*/
	0,                         /*tp_setattro*/
	0,                         /*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	"MCP9808 objects",           /* tp_doc */
	0,		               /* tp_traverse */
	0,		               /* tp_clear */
	0,		               /* tp_richcompare */
	0,		               /* tp_weaklistoffset */
	0,		               /* tp_iter */
	0,		               /* tp_iternext */
	MCP9808_methods,             /* tp_methods */
	MCP9808_members,             /* tp_members */
	0,                         /* tp_getset */
	0,                         /* tp_base */
	0,                         /* tp_dict */
	0,                         /* tp_descr_get */
	0,                         /* tp_descr_set */
	0,                         /* tp_dictoffset */
	(initproc)MCP9808_init,      /* tp_init */
	0,                         /* tp_alloc */
	MCP9808_new,                 /* tp_new */
};



static PyMethodDef module_methods[] = {
	{NULL}  /* Sentinel */
};



PyMODINIT_FUNC initMCP9808(void) {
	PyObject *m;

	if(PyType_Ready(&MCP9808_Type) < 0)
		return;

	m = Py_InitModule3("MCP9808", module_methods, "MCP9808 extension module");

	if(m == NULL)
		return;

	Py_INCREF(&MCP9808_Type);
	PyModule_AddObject(m, "MCP9808", (PyObject *)&MCP9808_Type);
}
