#!/bin/bash
addons_loc=../../addons
cwd=$(pwd)
echo "install add addons on "$addons_loc
addons=(  
git@github.com:ofTheo/ofxKinectV2.git 
)
cd $addons_loc ;
for i in "${addons[@]}"
do
  :
  git clone $i
done
cd $cwd ;
echo "done"
