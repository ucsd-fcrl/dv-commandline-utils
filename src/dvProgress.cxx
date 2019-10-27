#include <dvProgress.h>
#include <iomanip>

namespace dv {

Progress ::Progress(size_t _TotalUnits)
  : TotalUnits(_TotalUnits)
{}

void
Progress ::UnitCompleted()
{
  ++(this->CurrentUnit);
  double PercentCompleted =
    this->CurrentUnitAsFP() / this->TotalUnitsAsFP() * 100.0;
  double n = static_cast<int>(PercentCompleted / 2);
  std::cout << '\r' << '[' << std::string(n - 1, '=') << ">"
            << std::string(50 - n, '.') << "] " << std::setprecision(2)
            << std::fixed << PercentCompleted << "\% Completed.";
  if (this->CurrentUnit == this->TotalUnits) {
    std::cout << std::endl;
  } else {
    std::cout << std::flush;
  }
}

size_t
Progress ::GetTotalUnits()
{
  return this->TotalUnits;
}

double
Progress ::TotalUnitsAsFP()
{
  return static_cast<double>(this->TotalUnits);
}

double
Progress ::CurrentUnitAsFP()
{
  return static_cast<double>(this->CurrentUnit);
}

}
