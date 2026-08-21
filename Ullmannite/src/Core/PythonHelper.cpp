#include "Ullpch.h"
#include "PythonHelper.h"

#define PY_SSIZE_T_CLEAN
#ifdef _DEBUG
#undef _DEBUG
#include <python.h>
#define _DEBUG
#else
#include <python.h>
#endif

using namespace Ull;

static PyThreadState* s_mainThreadState = nullptr;

void Ull::InitializePython()
{
	Py_Initialize();
	s_mainThreadState = PyEval_SaveThread();
}

void Ull::FinalizePython()
{
	if (s_mainThreadState)
	{
		PyEval_RestoreThread(s_mainThreadState);
		s_mainThreadState = nullptr;
	}
	Py_Finalize();
}
