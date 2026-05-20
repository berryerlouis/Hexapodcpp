#!/bin/sh
set -e  # Exit on error

cmake -S . -B ./build/hexapod-RPI-GRAPHVIZ \
  -Wno-dev \
  -DGTEST=0 \
  -DTARGET=RPI \
  -DCMAKE_BUILD_TYPE=Debug \
  --graphviz="build/hexapod-RPI-GRAPHVIZ/cmake-graph.dot"

dot -Tpng build/hexapod-RPI-GRAPHVIZ/cmake-graph.dot -o cmake-graph.png
