#ifndef dv_LabeledITKPointSetWriter_h
#define dv_LabeledITKPointSetWriter_h

#include <fstream>

namespace dv {
template<typename TPointSet>
void
LabeledITKPointSetWriter(typename TPointSet::Pointer points, const std::string&file_name) {

  std::ofstream file(file_name);

  for (auto it = points->GetPoints()->Begin();
       it != points->GetPoints()->End();
       ++it) {

    file << it.Value()[0] << ' '
         << it.Value()[1] << ' '
         << it.Value()[2] << ' '
         << static_cast<int>(points->GetPointData()->ElementAt(it.Index())) << '\n';

  }

  file.close();

}
}

#endif
