#!/bin/bash

set -e
set -x

wget http://www.iausofa.org/2015_0209_F/sofa_f-20150209_a.tar.gz -O /tmp/sofa.tgz
tar -xzf /tmp/sofa.tgz
cd sofa/20150209_a/f77/src/ && make && make test && cd ../../../../

mkdir build
cd build

wget ftp://ftp.astron.nl/outgoing/Measures/WSRT_Measures.ztar
tar zxvf WSRT_Measures.ztar

ccache -M 80M

if [ "$TRAVIS_OS_NAME" = osx ]; then
  PATH=$HOME/miniconda/bin:$PATH
  source activate testenv
  PYTHON2_EXECUTABLE=$HOME/miniconda/bin/python
  PYTHON3_EXECUTABLE=$HOME/miniconda/bin/python
  which python
  python -c "import numpy as n; print (n.__version__); print(n.get_include());"
  ls /Users/travis/miniconda/envs/testenv/lib/python3.4/site-packages/numpy/core/include/numpy
  export PYTHONPATH=/Users/travis/miniconda/envs/testenv/lib/python${PYTHONVERSION}/site-packages:$PYTHONPATH
  if [ "$PYTHONVERSION" = "2.7" ]; then
    BUILD_PYTHON=On
    BUILD_PYTHON3=Off
  else
    ln -s /Users/travis/miniconda/envs/testenv/lib/libboost_python-mt.dylib /Users/travis/miniconda/envs/testenv/lib/libboost_python3-mt.dylib
    BUILD_PYTHON=Off
    BUILD_PYTHON3=On
  fi

  CMAKE_PREFIX_PATH=/Users/travis/miniconda/envs/testenv/
  ls /Users/travis/miniconda/envs/testenv/
else
  PYTHON2_EXECUTABLE=/usr/bin/python2.7
  PYTHON3_EXECUTABLE=/usr/bin/python3.4
  BUILD_PYTHON=On
  BUILD_PYTHON3=On
  CMAKE_PREFIX_PATH=
fi

echo $PYTHONPATH
echo $PATH
CXX="ccache $CXX" cmake .. \
    -DUSE_FFTW3=ON \
    -DBUILD_TESTING=ON \
    -DUSE_OPENMP=OFF \
    -DUSE_HDF5=ON \
    -DBUILD_PYTHON=${BUILD_PYTHON} \
    -DBUILD_PYTHON3=${BUILD_PYTHON3} \
    -DPYTHON2_EXECUTABLE=${PYTHON2_EXECUTABLE} \
    -DPYTHON3_EXECUTABLE=${PYTHON3_EXECUTABLE} \
    -DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH} \
    -DDATA_DIR=$PWD \
    -DSOFA_ROOT_DIR=$HOME \
    -DCMAKE_INSTALL_PREFIX=${TRAVIS_BUILD_DIR}/installed

