#ifndef _JAYL_COMPARE_H
#define _JAYL_COMPARE_H

#include <cstddef>

#include "complex_types.h"

namespace jayl {
  namespace util {

    bool almostEqual(double a, double b, double maxRelativeError = 0.0001);

    bool almostEqual(float a, float b, float maxRelativeError = 0.0001);

    bool almostEqual(double_complex a, double_complex b,
      double maxRelativeError = 0.0001);

    bool almostEqual(float_complex a, float_complex b,
      float maxRelativeError = 0.0001);

/// Compares two arrays and returns true if the content is equal.
    template<typename T>
    inline
    bool compare(const T *ldata, const T *rdata, size_t num) {
      for (size_t i = 0; i < num; ++i) {
        if (ldata[i] != rdata[i]) {
          return false;
        }
      }
      return true;
    }

    template<>
    inline
    bool compare<float>(const float *ldata, const float *rdata, size_t num) {
      for (size_t i = 0; i < num; ++i) {
        if (!util::almostEqual(ldata[i], rdata[i], 0.001f)) {
          return false;
        }
      }
      return true;
    }

    template<>
    inline
    bool compare<double>(const double *ldata, const double *rdata, size_t num) {
      for (size_t i = 0; i < num; ++i) {
        if (!util::almostEqual(ldata[i], rdata[i], 0.001)) {
          return false;
        }
      }
      return true;
    }

    template<>
    inline
    bool compare<float_complex>(const float_complex *ldata,
      const float_complex *rdata, size_t num) {
      for (size_t i = 0; i < num; ++i) {
        if (!util::almostEqual(ldata[i], rdata[i], 0.001f)) {
          return false;
        }
      }
      return true;
    }

    template<>
    inline
    bool compare<double_complex>(const double_complex *ldata,
      const double_complex *rdata, size_t num) {
      for (size_t i = 0; i < num; ++i) {
        if (!util::almostEqual(ldata[i], rdata[i], 0.001)) {
          return false;
        }
      }
      return true;
    }

    template<typename T>
    inline
    bool compare(const void *ldata, const void *rdata, size_t num) {
      return compare(static_cast<const T *>(ldata),
        static_cast<const T *>(rdata), num);
    }

  }
}
#endif
