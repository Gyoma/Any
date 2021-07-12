#pragma once
#include <typeinfo>

class Any
{
   template <typename T>
   struct AnyType
   {
      using type = std::remove_reference_t<T>;
   };

   template<typename T>
   using AnyType_t = typename AnyType<T>::type;

   struct AnyHandler
   {
      template<typename T>
      AnyHandler(AnyType<T>&&);

      AnyHandler(const AnyHandler& other);
      AnyHandler& operator=(const AnyHandler& other);

      std::type_info const&   (*type)();
      void*                   (*clone)(void*);
      void*                   (*move)(void*&);
      void                    (*destroy)(void*&);
      bool                    (*empty)(void*);
   };

public:

   template<typename T, typename ...Args>
   static Any make(Args&&... args);

   template<typename T>
   Any(T&& value);

   Any(const Any& other);
   Any(Any&& other) noexcept;

   ~Any();

   Any& operator=(const Any& other);
   Any& operator=(Any&& other) noexcept;

   template<typename R>
   R& get();

   template<typename R>
   R const& get() const;

   bool empty() const;
   void reset();

private:

   template<typename T, typename ...Args>
   Any(AnyType<T>&& type, Args&&... args);

   AnyHandler  _handler;
   void*       _data;
};

template<typename T>
Any::AnyHandler::AnyHandler(AnyType<T>&&) :
   type{ []() -> std::type_info const& { return typeid(AnyType_t<T>); } },
   clone{ [](void* data) -> void* { return new AnyType_t<T>(*static_cast<AnyType_t<T>*>(data)); } },
   move{ [](void*& data) -> void* { return std::exchange(data, nullptr); } },
   destroy{ [](void*& data) -> void { delete static_cast<AnyType_t<T>*>(std::exchange(data, nullptr)); } },
   empty{ [](void* data) -> bool { return data == nullptr; } }
{}

template<typename T, typename ...Args>
Any Any::make(Args&&... args)
{
   return Any{ AnyType<T>{}, std::forward<Args>(args)... };
}

template<typename T>
Any::Any(T&& value) :
   _handler{ AnyType<T>{} },
   _data{ new AnyType_t<T>{std::forward<T>(value)} }
{}

template<typename R>
R& Any::get()
{
   if (_handler.empty(_data) || _handler.type() != typeid(R)) throw std::bad_cast{};

   return *static_cast<R*>(_data);
}

template<typename R>
R const& Any::get() const
{
   if (_handler.type() != typeid(R)) throw std::bad_cast{};

   return *static_cast<R*>(_data);
}

template<typename T, typename ...Args>
Any::Any(AnyType<T>&& type, Args&&... args) :
   _handler{ std::move(type) },
   _data{ new AnyType_t<T>{std::forward<Args>(args)... } }
{}