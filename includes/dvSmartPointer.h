#ifndef dv_SmartPointer_h
#define dv_SmartPointer_h

namespace dv {

template<typename T>
class SmartPointer
{

public:
  SmartPointer(T value)
  {
    this->ptr = new T;
    this->set(value);
    ++(*this->refs);
  }

  SmartPointer(T* _ptr)
  {
    this->ptr = _ptr;
    this->refs = new int;
    *(this->refs) = 1;
  }

  int get_reference_count() { return *(this->refs); }

  SmartPointer(SmartPointer<T>& other) { this->copy(other); }

  ~SmartPointer() { this->remove(); }

  SmartPointer<T>& operator=(SmartPointer<T>& other)
  {
    if (this == &other)
      return;
    this->remove();
    this->add(other);
  }

  T& operator*() { return *(this->ptr); }

  T* operator->() { return this->ptr; }

  T get() { return *(this->ptr); }
  void set(const T& val) { *(this->ptr) = val; }

protected:
  T* ptr;
  int* refs;

  void copy(SmartPointer<T>& other)
  {
    this->ptr = other.ptr;
    this->refs = other.refs;
    ++(*this->refs);
  }

  void remove()
  {
    --(*this->refs);
    if (*this->refs < 1) {
      delete this->ptr;
    }
  }
};

}

#endif
