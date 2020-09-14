#ifndef dv_ReadPolyData_h
#define dv_ReadPolyData_h

#include <vtksys/SystemTools.hxx>

#include <vtkBYUReader.h>
#include <vtkOBJReader.h>
#include <vtkPLYReader.h>
#include <vtkPolyDataReader.h>
#include <vtkSTLReader.h>
#include <vtkXMLPolyDataReader.h>

#include <vtkPolyData.h>

namespace dv
{
vtkSmartPointer<vtkPolyData> ReadPolyData(const std::string& fileName);
}

#endif
