#ifndef dv_Heap_h
#define dv_Heap_h

#include <vector>
#include <cassert>

/*
- Root node is position 0 in data array
- Keeps lowest number there
*/

namespace dv
{

template<typename T>
class Heap
{
public:
void push(const T& item)
{
  this->data.push_back(item);
  this->upheap(data.size() - 1);
}

T pop()
{
  const auto temp = this->data[0];
  this->data[0] = this->data.back();
  this->data.pop_back();
  this->downheap(0);
  return temp;
}

std::size_t size() { return this->data.size(); }

//protected:
std::vector<T> data;

void upheap(std::size_t index)
{

  if (index == 0) return;

  std::size_t parent = this->get_parent(index);
  const auto value = this->data[index];
  while (index > 0)
    {
    if (this->data[parent] > value)
      {
      this->data[index] = this->data[parent];
      index = parent;
      parent = this->get_parent(index);
      }
    else
      {
      break;
      }
    }
  this->data[index] = value;
}

void downheap(std::size_t index)
{
  std::size_t child = this->get_left_child(index);
  const auto value = this->data[index];

  while (child < this->data.size()) // Make sure left child exists
    {
    // If right child exists, set to lesser of left and right
    if (((child + 1) < this->data.size()) && (this->data[child] > this->data[child + 1]))
      {
      ++child;
      }

    // Switch the Current node and the Child node
    if (value > this->data[child])
      {
      this->data[index] = this->data[child];
      index = child;
      child = this->get_left_child(index);
      }
    else
      {
      break;
      }
    }
  this->data[index] = value;
}

std::size_t get_parent(const std::size_t index)
{
  return (index - 1) / 2;
}

std::size_t
get_left_child(const std::size_t index)
{
  return 2 * (index + 1) - 1;
}

std::size_t
get_right_child(const std::size_t index)
{
  return this->get_left_child(index) + 1;
}
};

}

#endif
