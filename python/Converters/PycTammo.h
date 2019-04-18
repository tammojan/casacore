//# PycBasicData.h: Convert casa data types to/from python
//# Copyright (C) 2006
//# Associated Universities, Inc. Washington DC, USA.
//#
//# This library is free software; you can redistribute it and/or modify it
//# under the terms of the GNU Library General Public License as published by
//# the Free Software Foundation; either version 2 of the License, or (at your
//# option) any later version.
//#
//# This library is distributed in the hope that it will be useful, but WITHOUT
//# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
//# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
//# License for more details.
//#
//# You should have received a copy of the GNU Library General Public License
//# along with this library; if not, write to the Free Software Foundation,
//# Inc., 675 Massachusetts Ave, Cambridge, MA 02139, USA.
//#
//# Correspondence concerning AIPS++ should be addressed as follows:
//#        Internet email: aips2-request@nrao.edu.
//#        Postal address: AIPS++ Project Office
//#                        National Radio Astronomy Observatory
//#                        520 Edgemont Road
//#                        Charlottesville, VA 22903-2475 USA
//#
//# $Id: PycBasicData.h,v 1.5 2006/10/25 01:42:13 gvandiep Exp $

#ifndef PYRAP_PYCTAMMO_H
#define PYRAP_PYCTAMMO_H

// include python first to avoid _POSIX_C_SOURCE redefined warnings
#include <casacore/casa/BasicSL/String.h>
#include <casacore/casa/Quanta.h>
#include <pybind11/pybind11.h>

#include <vector>
#include <map>
#include <string>

#if PY_MAJOR_VERSION >= 3
#define IS_PY3K
#endif

namespace py = pybind11;

// Define classes and functions to convert the basic data types and
// containers to and from Python.

namespace pybind11 { namespace detail {
    template <> struct type_caster<casacore::String> {
    public:
        PYBIND11_TYPE_CASTER(casacore::String, _("String"));

        /**
         * Conversion part 1 (Python->C++): convert a PyObject into a String
         * instance or return false upon failure. The second argument
         * indicates whether implicit conversions should be applied.
         */
        bool load(handle src, bool) {
            /* Extract PyObject from handle */
            PyObject *source = src.ptr();
            /* Try converting into a Python integer value */
            PyObject *temp_bytes = PyUnicode_AsEncodedString(source, "ASCII", "strict"); // Owned reference
            if (!temp_bytes)
                return false;
            char* str_value;
            str_value = PyBytes_AS_STRING(temp_bytes);
            str_value = strdup(str_value);
            value = casacore::String(value);
            Py_DECREF(temp_bytes);
            /* Ensure return code was OK (to avoid out-of-range errors etc) */
            return !(str_value == 0 && !PyErr_Occurred());
        }

        /**
         * Conversion part 2 (C++ -> Python): convert an String instance into
         * a Python object. The second and third arguments are used to
         * indicate the return value policy and parent object (for
         * ``return_value_policy::reference_internal``) and are generally
         * ignored by implicit casters.
         */
        static handle cast(casacore::String src,
                           return_value_policy /* policy */,
                           handle /* parent */) {
            return PyBytes_FromString(src.c_str());
        }
    };
}} // namespace pybind11::detail

namespace casacore { namespace python {
    int fibonacci(int x);

    const std::string& getQuantityUnit(const Quantity& q) {
      return q.getUnit();
    }

    void test () {
      Quantity q(3,"m");
    }

    PYBIND11_MODULE(fibmod, m) {
      m.doc() = "pybind11 example plugin for fibonacci";

      m.def("fibonacci", &fibonacci, "Fibonacci function using C++");

      py::class_<Quantity>(m, "Quantity")
        .def(py::init<int, const std::string &>())
        .def("get_unit", &Quantity::getUnit);
      }
}
}
#endif
