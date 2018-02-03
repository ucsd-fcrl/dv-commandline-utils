#ifndef dvCycle_h
#define dvCycle_h

#include <type_traits>
#include <cassert>

namespace dv
{

template<typename T>
class Cycle
{
static_assert(std::is_integral<T>::value, "T must be an integral type.");

public:
Cycle(T _Start, T _Range, T _Current)
: Start(_Start), Range(_Range), Current(_Current)
{
  assert(this->Range > 0);
  assert((this->Start <= this->Current) && (this->Current < (this->Start + this->Range)));
}

Cycle(T _Range)
: Start(0), Range(_Range), Current(0)
{}

T GetStart() const { return this->Start; }
T GetRange() const { return this->Range; }
T GetCurrent() const { return this->Current; }
T GetNext() const
{
return (this->Current - this->Start + 1) % this->Range + this->Start;
}
T GetPrevious() const
{
return (this->Current == this->Start) ? this->Current + this->Range  - 1: this->Current - 1;
}
T Increment()
{
this->Current = this->GetNext();
return this->Current;
}
T Decrement()
{
this->Current = this->GetPrevious();
return this->Current;
}

private:

const T Start;
const T Range;
T Current;

};

}

#endif
