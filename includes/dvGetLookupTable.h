#ifndef dv_GetLookupTable_h
#define dv_GetLookupTable_h

#include <vtkLookupTable.h>
#include <vtkSmartPointer.h>

namespace dv {

namespace LUT {

vtkSmartPointer<vtkLookupTable>
Rainbow();

vtkSmartPointer<vtkLookupTable>
SQUEEZ();

} // end namespace LUT

} // end namespace dv

#endif
