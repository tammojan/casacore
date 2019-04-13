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
        .def("get_unit", &getQuantityUnit);
      }
}
}
#endif
