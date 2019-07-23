#ifndef dvProgress_h
#define dvProgress_h

#include <cstdlib>
#include <string>
#include <iostream>

namespace dv
{

class Progress
{
public:

  Progress(std::size_t _TotalUnits);
  void UnitCompleted();
  size_t GetTotalUnits();

protected:

  const std::size_t TotalUnits;
  std::size_t CurrentUnit = 0;

  double TotalUnitsAsFP();
  double CurrentUnitAsFP();

};

}

#endif
