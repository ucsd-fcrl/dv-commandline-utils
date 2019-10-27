#ifndef dv_DICOMHeaderAttributes_h
#define dv_DICOMHeaderAttributes_h

#include <gdcmAttribute.h>

namespace dv {

template<typename TAttribute>
bool
SortByAttribute(gdcm::DataSet const& lhs, gdcm::DataSet const& rhs)
{

  TAttribute a1, a2;
  a1.Set(lhs);
  a2.Set(rhs);
  return a1 < a2;
}

template<typename TAttribute>
bool
RevSortByAttribute(gdcm::DataSet const& lhs, gdcm::DataSet const& rhs)
{

  TAttribute a1, a2;
  a1.Set(lhs);
  a2.Set(rhs);
  return a1 < a2;
}

using TSeriesNumber = gdcm::Attribute<0x0020, 0x0011>;
using TImageNumber = gdcm::Attribute<0x0020, 0x0013>;
using TImageTime = gdcm::Attribute<0x0008, 0x0033>;

// using TSliceLocation        = gdcm::Attribute<0x0020,0x1041>;
// using TPatientPosition = gdcm::Attribute<0x0020,0x0032>;
// using TInstanceCreatingTime = gdcm::Attribute<0x0008,0x0013>; // Time of
// RECONSTRUCTION using TAcquisitionTime      = gdcm::Attribute<0x0008,0x0032>;
// // Time of ACQUISITION using TAcquisitionNumber    =
// gdcm::Attribute<0x0020,0x0012>; using TImageIndex           =
// gdcm::Attribute<0x0054,0x1330>;

constexpr auto SortBySeriesNumber = &SortByAttribute<TSeriesNumber>;
constexpr auto SortByImageNumber = &SortByAttribute<TImageNumber>;
constexpr auto SortByImageTime = &SortByAttribute<TImageTime>;

constexpr auto RevSortBySeriesNumber = &RevSortByAttribute<TSeriesNumber>;
constexpr auto RevSortByImageNumber = &RevSortByAttribute<TImageNumber>;
constexpr auto RevSortByImageTime = &RevSortByAttribute<TImageTime>;

// constexpr auto SortBySliceLocation   = &SortByAttribute<TSliceLocation>;
// constexpr auto SortByPatientPosition = &SortByAttribute<TPatientPosition>;

};

#endif
