#ifndef dv_WritePolyData_h
#define dv_WritePolyData_h

#include <vtksys/SystemTools.hxx>

#include <vtkBYUWriter.h>
#include <vtkOBJWriter.h>
#include <vtkPLYWriter.h>
#include <vtkPolyDataWriter.h>
#include <vtkSTLWriter.h>
#include <vtkXMLPolyDataWriter.h>

#include <vtkPolyData.h>

namespace dv
{
void WritePolyData(vtkPolyData* data, const std::string& fileName);
}

#endif
