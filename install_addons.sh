#!/bin/bash
addons_loc=../../addons
cwd=$(pwd)
echo "install add addons on "$addons_loc
addons=(  
'git@github.com:micromundos/ofxMicromundos.git'
'git@github.com:micromundos/ofxPlab.git'
'git@github.com:chparsons/ofxChilitags.git'
'git@github.com:chparsons/ofxGPGPU.git'
'--branch stable git@github.com:kylemcdonald/ofxCv.git'
'--branch v0.9.8_no_bin_buffer git@github.com:chparsons/ofxLibwebsockets.git'
)
cd $addons_loc ;
for i in "${addons[@]}"
do
  :
  git clone $i
done
cd $cwd ;
echo "done"
