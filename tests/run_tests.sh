#!/bin/bash
cd .. && make && cd -
make && ./metar_test && ./conv_test && ./utils_test
