#!/bin/bash -x
 
cd $(dirname `ls -l $0 | awk '{print $NF;}'`)
wk_dir=`pwd`

set -o pipefail
set -o errexit

GCC=`which gcc`
GXX=`which g++`

if [ ! -d $wk_dir/build ]; then
  echo "all build ..."
  rm -rf $wk_dir/build || true
  mkdir -p $wk_dir/build 
  cd $wk_dir/build
  cmake -D CMAKE_C_COMPILER=${GCC} -D CMAKE_CXX_COMPILER=${GXX} $wk_dir
else
  cd $wk_dir/build
fi

make 

echo "======== $0 done ========="
