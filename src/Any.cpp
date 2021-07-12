#include "Any.h"


Any::AnyHandler::AnyHandler(const AnyHandler& other) :
   type{ other.type },
   clone{ other.clone },
   move{ other.move },
   destroy{ other.destroy },
   empty{ other.empty }
{}

Any::AnyHandler& Any::AnyHandler::operator=(const AnyHandler& other)
{
   if (this != &other)
   {
      type = other.type;
      clone = other.clone;
      //move = other.move;
      destroy = other.destroy;
      //empty = other.empty;
   }

   return *this;
}

Any::Any(const Any& other) :
   _handler{ other._handler },
   _data{ _handler.clone(other._data) }
{}

Any::Any(Any&& other) noexcept :
   _handler{ other._handler },
   _data{ _handler.move(other._data) }
{}

Any& Any::operator=(const Any& other)
{
   if (this != &other)
   {
      _handler.destroy(_data);
      _handler = other._handler;
      _data = _handler.clone(other._data);
   }

   return *this;
}

Any& Any::operator=(Any&& other) noexcept
{
   if (this != &other)
   {
      _handler = other._handler;
      _data = _handler.move(other._data);
   }

   return *this;
}

bool Any::empty() const
{
   return _handler.empty(_data);
}

void Any::reset()
{
   _handler.destroy(_data);
}

Any::~Any()
{
   reset();
}
