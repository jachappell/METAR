#!/bin/bash
cd .. && make && cd -
make && ./cloud_test && ./phenom_test && ./metar_test && ./conv_test && ./utils_test
