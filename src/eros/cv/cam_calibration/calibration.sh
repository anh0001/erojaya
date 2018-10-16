#!/bin/bash

./imagelist_creator images_list.xml images/*.jpg
./calibration -w=9 -h=9 -s=0.025 -pt=chessboard -d=100 -o=out_camera.xml -op -oe -su images_list.xml
