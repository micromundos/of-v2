#!/bin/bash

apps=(  
'backend'
'plab'
'autitos'
)

pidfile="/tmp/micromundos.pids"
isdev=false

start() {

  if [[ -f "$pidfile" ]]; then
    echo "micromundos ya esta corriendo: hay un pidfile $pidfile" >& 2
    exit 1
  fi

  pid="$$"
  echo "$pid" >> "$pidfile"

  if [ "$isdev" = true ]; then
    echo "micromundos dev apps (pid = $pid)"
    for i in "${apps[@]}"
    do
      :
      open -a $i
    done
    open -a proyector

  else

    for i in "${apps[@]}"
    do
      :
      open -a $i
    done
    sleep 5

    while true
    do
      echo "micromundos apps (pid = $pid)"
      for i in "${apps[@]}"
      do
        :
        open -a $i
      done
      open -a proyector
      sleep 10
    done
  fi
}

stop() {

  if [[ ! -f "$pidfile" ]]; then
    echo "micromundos no esta corriendo: no hay pidfile $pidfile" >& 2
    exit 1
  fi

  pids=`cat "$pidfile"`
  echo "kill micromundos apps (pids = $pids)"
  for i in "${apps[@]}"
    do
      :
      echo "kill $i"
      pkill $i
    done
  echo "kill proyector"
  pkill proyector
  echo "kill pids $pids"
  kill $pids
  rm -f "$pidfile"
}

pack() {

  if [[ $# > 0 ]]; then
    path=$1
  else
    path=micromundos2
  fi
  echo "pack en $path"

  mkdir -p ./$path/data
  cp -r ./data ./$path/ 

  for i in "${apps[@]}"
  do
    :
    mkdir -p ./$path/$i/bin
    cp -r ./$i/bin/$i.app ./$path/$i/bin/
  done

  mkdir -p ./$path/scripts
  cp ./scripts/micromundos.sh ./$path/scripts/micromundos.sh 

  find ./$path/data -type f -name .DS_Store -delete
  find ./$path/data -type f -name *.swp -delete
  find ./$path/data -type f -name *.casa -delete
}

dev() {
  isdev=true 
  start
}

case "$1" in
  dev)
    dev
    ;;
  start)
    start
    ;;
  stop)
    stop
    ;;
  pack)
    pack $2
    ;;
esac

