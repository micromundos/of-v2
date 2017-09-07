#!/bin/bash
addons_loc=../../addons
cwd=$(pwd)
echo "install add addons on "$addons_loc
addons=(  
'git@github.com:micromundos/ofxMicromundos.git'
'git@github.com:chparsons/ofxChilitags.git'
'git@github.com:chparsons/ofxOrbbecAstra.git'
'git@github.com:chparsons/ofxGPGPU.git'
'git@github.com:chparsons/ofxGeom.git'
'git@github.com:ofTheo/ofxKinectV2.git'
'--branch stable git@github.com:kylemcdonald/ofxCv.git'
)
cd $addons_loc ;
for i in "${addons[@]}"
do
  :
  git clone $i
done
cd $cwd ;
echo "done"
