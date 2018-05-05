#!/bin/bash

cwd=$(pwd)
addons_loc=$cwd/../../addons
echo $addons_loc

if [[ ! -d "$addons_loc" ]]; then
  echo "no existe la carpeta addons"
  exit 1
fi

echo "install addons on "$addons_loc
addons=(  
'git@github.com:micromundos/ofxMicromundos.git'
'git@github.com:micromundos/ofxPlab.git'
'git@github.com:chparsons/ofxChilitags.git'
'git@github.com:chparsons/ofxGPGPU.git'
'git@github.com:chparsons/ofxColorBrewer.git'
'--branch stable git@github.com:kylemcdonald/ofxCv.git'
'--branch micromundos2 git@github.com:chparsons/ofxLibwebsockets.git'
'--branch OpenFrameworks-0.9 git@github.com:astellato/ofxSyphon.git'
'git@github.com:jeffcrouse/ofxJSON.git'
'git@github.com:obviousjim/ofxDelaunay.git'
'git@github.com:armadillu/ofxTimeMeasurements.git'
)
cd $addons_loc ;
for i in "${addons[@]}"
do
  :
  git clone $i
done

cd $cwd ;
echo "done"

