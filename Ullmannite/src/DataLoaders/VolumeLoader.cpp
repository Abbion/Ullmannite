#include "Ullpch.h"
#include "VolumeLoader.h"
#include "Logger/Logger.h"

#define PY_SSIZE_T_CLEAN
#ifdef _DEBUG
#undef _DEBUG
#include <python.h>
#define _DEBUG
#else
#include <python.h>
#endif

using namespace Ull;

std::shared_ptr<VolumeData> Ull::LoadVolumeData(const std::string filePath)
{
	Py_Initialize();

	PyRun_SimpleString("import sys; sys.path.append('.')");
	PyObject* program = PyUnicode_FromString("Tools.dicom_loader");
	PyObject* module = PyImport_Import(program);
	Py_DECREF(program);

	if (!module) 
	{
		PyErr_Print();
		ULOGE("Loading volume data failed. Module not imported.");
		return nullptr;
	}

	PyObject* function = PyObject_GetAttrString(module, "load_dicom_folder");

	if (!function || !PyCallable_Check(function))
	{
		PyErr_Print();
		ULOGE("Loading volume data failed. Function not found.");
		return nullptr;
	}

	PyObject* args = PyTuple_Pack(1, PyUnicode_FromString(filePath.c_str()));
	PyObject* output = PyObject_CallObject(function, args);

	if (!output)
	{
		PyErr_Print();
		ULOGE("Loading volume data failed. Function did not return.");
		return nullptr;
	}

	std::shared_ptr<VolumeData> volumeData = std::make_shared<VolumeData>();
	PyObject* pArray;

	PyArg_ParseTuple(output, "HHHO", &volumeData->width, &volumeData->height, &volumeData->depth, &pArray);

	Py_buffer view;
	PyObject_GetBuffer(pArray, &view, PyBUF_CONTIG_RO);

	const size_t bufferSize = static_cast<size_t>(volumeData->width) *
							  static_cast<size_t>(volumeData->height) *
							  static_cast<size_t>(volumeData->depth);
	volumeData->dataBuffer.resize(bufferSize);
	std::memcpy(volumeData->dataBuffer.data(), view.buf, bufferSize);

	PyBuffer_Release(&view);
	Py_DECREF(output);
	Py_DECREF(args);
	Py_DECREF(function);
	Py_DECREF(module);
	Py_Finalize();

	const auto elements = std::minmax_element(volumeData->dataBuffer.begin(), volumeData->dataBuffer.end());
	volumeData->minValue = *(elements.first);
	volumeData->maxValue = *(elements.second);

	return volumeData;
}
