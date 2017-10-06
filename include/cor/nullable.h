/*
 * MIT License
 *
 * Copyright (c) 2016-2017 Abel Lucas <www.github.com/uael>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*!@file cor/nullable.h
 * @author uael
 */
#ifndef __COR_NULLABLE_H
# define __COR_NULLABLE_H

template<class T>
class Nullable {
protected:
  T *value;

public:
  Nullable<T>() : value(NULL) {};

  Nullable<T>(T value) {
    this->value = new T(value);
  };

  Nullable<T>(T *value) {
    if (value != NULL) {
      this->value = new T(*value);
    } else {
      this->value = NULL;
    }
  };

  Nullable<T>(const Nullable<T> &value) {
    T *tmp = (T *) value;
    if (tmp == NULL) {
      this->value = NULL;
    } else {
      this->value = new T(*tmp);
    }
  };

  virtual ~Nullable<T>() {
    if (this->value != NULL) {
      delete this->value;
    }
  };

  virtual operator T *() const {
    return this->value;
  }

  virtual Nullable<T> &
  operator=(const T *const value) {
    if (NULL == value) {
      if (this->value != NULL) {
        delete this->value;
      }
      this->value = NULL;
      return *this;
    }

    if (this->value == NULL) {
      this->value = new T(*value);
    } else {
      *this->value = *value;
    }
    return *this;
  }

  virtual Nullable<T> &
  operator=(const T &value) {
    if (NULL == &value) {

      if (this->value != NULL) {
        delete this->value;
      }
      this->value = NULL;
      return *this;
    }

    if (this->value == NULL) {
      this->value = new T(value);
    } else {
      *this->value = value;
    }
    return *this;
  }

  virtual Nullable<T> &
  operator=(const Nullable<T> &value) {
    if (this == &value) {
      return *this;
    }
    T *tmp = (T *) value;
    if (tmp == NULL) {
      if (this->value != NULL) {
        delete this->value;
      }
      this->value = NULL;
    } else {
      if (this->value == NULL) {
        this->value = new T;
      }
      *this->value = *tmp;
    }
    return *this;
  }
};

#endif /* !__COR_NULLABLE_H */
