#ifndef dvProgress_h
#define dvProgress_h

#include <cstdlib>
#include <iostream>
#include <string>

namespace dv {

class Progress
{
public:
  Progress(size_t _TotalUnits);
  void UnitCompleted();
  size_t GetTotalUnits();

protected:
  const size_t TotalUnits;
  size_t CurrentUnit = 0;

  double TotalUnitsAsFP();
  double CurrentUnitAsFP();
};

}

#endif
