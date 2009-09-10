/*
 * UML statechart framework (http://uml-statecharts.sourceforge.net)
 *
 * Copyright (C) 2006-2007 Christian Mocek (klangfarbe@users.sourceforge.net)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 */
#ifndef SMARTPOINTER_H
#define SMARTPOINTER_H

#include <statechart/util/mutex.h>
#include <statechart/util/scopemutex.h>

namespace statechart {
  namespace util {
//------------------------------------------------------------------------------
// Intrusive pointer base class
//------------------------------------------------------------------------------
    /**
     * Abstract base class for Elements which should be able to use
     * the smart pointer.
     */
    class IntrusiveSmartPointer {
    private:
      /// The references count
      unsigned int smartPointerReferencesCount;

      /// the mutex to lock access to this object
      util::Mutex smartPointerMutex;

    protected:
      IntrusiveSmartPointer() : smartPointerReferencesCount(1) {
      };

    public:
      void atomicIncrement() {
        smartPointerMutex.lock();
        smartPointerReferencesCount++;
        smartPointerMutex.unlock();
      };

      void atomicDecrement() {
        smartPointerMutex.lock();
        smartPointerReferencesCount--;
        smartPointerMutex.unlock();
      };

      bool isLast() {
        return smartPointerReferencesCount == 0;
      };

      unsigned int getReferenceCount() {
        return smartPointerReferencesCount;
      }
    };

    /**
     * Implements a simple intrusive reference counting smart
     * pointer.
     */
    template <class T> 
    class SmartPointer {
//------------------------------------------------------------------------------
// Methods
//------------------------------------------------------------------------------
    public:
      /**
       * Default constructor
       */
      SmartPointer() : object(0) {
      };

      /**
       * Pointer from new object
       */
      SmartPointer(T* pointer) : object(pointer) {
      };

      /**
       * Pointer from new object
       */
      template <class U>
      SmartPointer(U* pointer) : object(dynamic_cast<T*>(pointer)) {
      };

      /**
       * Pointer from existing object
       */
      SmartPointer(const SmartPointer<T>& value) : 
        object(value.object) {
        if(object) {
          ((IntrusiveSmartPointer*)object)->atomicIncrement();
        }
      };

      /**
       * Pointer from existing object
       */
      template <class U>
      SmartPointer(const SmartPointer<U>& value) : 
        object(dynamic_cast<T*>(value.object)) {
        if(object) {
          ((IntrusiveSmartPointer*)object)->atomicIncrement();
        }
      };

      /**
       * Default destructor
       */
      virtual ~SmartPointer() {
        if(object) {
          ((IntrusiveSmartPointer*)object)->atomicDecrement();
          if(((IntrusiveSmartPointer*)object)->isLast()) {
            delete object;
          }
        }
      };

      /**
       * Der operator
       */
      T& operator*() const {
        return *object;
      };

      /**
       * Pointer operator
       */
      T* operator->() const {
        return object;        
      };

      /**
       * Assignment operator
       */
      SmartPointer<T>& operator=(const SmartPointer<T>& value) {        
        if(object != value.object) {
          if(object) {
            ((IntrusiveSmartPointer*)object)->atomicDecrement();
            if(((IntrusiveSmartPointer*)object)->isLast()) {
              delete object;
              object = 0;
            } 
          }

          object = value.object;
          if(object) {
            ((IntrusiveSmartPointer*)object)->atomicIncrement();
          }
        }
        return *this;
      };

      /// Allows if(!smartpointer) ...
      bool operator!() const {
        return object == 0;
      };

      // non-templated versions
      inline friend bool operator==(const SmartPointer& lhs, const T* rhs) {
        return lhs.object == rhs;
      };

      friend bool operator==(const T* lhs, const SmartPointer& rhs) {
        return  lhs == rhs.object;
      };

      friend bool operator!=(const SmartPointer& lhs, const T* rhs) {
        return lhs.object != rhs;
      };

      friend bool operator!=(const T* lhs, const SmartPointer& rhs) {
        return  lhs != rhs.object;
      };

      // templated versions
      template <class U>
      friend bool operator==(const SmartPointer& lhs, const U* rhs) {
        return lhs.object == rhs;
      };

      template <class U>
      friend bool operator==(const U* lhs, const SmartPointer<T>& rhs) {
        return  lhs == rhs.object;
      };

      template <class U>
      friend bool operator!=(const SmartPointer& lhs, const U* rhs) {
        return lhs.object != rhs;
      };

      template <class U>
      friend bool operator!=(const U* lhs, const SmartPointer& rhs) {
        return lhs != rhs.object;
      };

      // for ambiguity
      template <class U>
      bool operator==(const SmartPointer<U>& rhs) const {
        return object == rhs.object;
      };

      template <class U>
      bool operator!=(const SmartPointer<U>& rhs) const {
        return object != rhs.object;
      };

      // Helper
      template <class U>
      friend U* spGetImpl(SmartPointer<U>& value);

      template<class Y> friend class SmartPointer;

//------------------------------------------------------------------------------
// Attributes
//------------------------------------------------------------------------------
    private:
      /// The referenced object
      T* object;      
    };

    /// Helper function to get the raw pointer
    template <class T> 
    inline T* spGetImpl(SmartPointer<T>& value) {
      return value.object;
    };
  }
}

#endif

