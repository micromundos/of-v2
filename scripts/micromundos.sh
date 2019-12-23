#!/bin/bash

apps=(  
'backend'
#'plab'
#'proyector'
)
app_proyector='plab'

pidfile="/tmp/micromundos.pids"

check_pid() {
  if [[ -f "$pidfile" ]]; then
    echo "micromundos ya esta corriendo: hay un pidfile $pidfile" >& 2
    exit 1
  fi
}

save_pid() {
  pid="$$"
  echo "$pid" >> "$pidfile"
}

start() { 

  kickoff
  sleep 10

  while true
  do
    echo "[micromundos] apps (pid = $pid)"
    for i in "${apps[@]}"
    do
      :
      echo "[micromundos] open app: $i"
      open -a $i
    done
    echo "[micromundos] open proyector app: $app_proyector"
    open -a $app_proyector
    sleep 10
  done
}

kickoff() {
  echo "[micromundos] apps (pid = $pid)"
  for i in "${apps[@]}"
  do
    :
    echo "open app: $i"
    open -a $i
  done
  sleep 2
  echo "open proyector app: $app_proyector"
  open -a $app_proyector
}

stop() {

  if [[ ! -f "$pidfile" ]]; then
    echo "micromundos no esta corriendo: no hay pidfile $pidfile" >& 2
    exit 1
  fi

  pids=`cat "$pidfile"`
  echo "[micromundos] kill apps (pids = $pids)"
  for i in "${apps[@]}"
    do
      :
      echo "[micromundos] kill $i"
      pkill $i
    done
  echo "[micromundos] kill $app_proyector"
  pkill $app_proyector
  echo "[micromundos] kill pids $pids"
  kill $pids
  rm -f "$pidfile"
}

deploy() {

  if [[ $# > 0 ]]; then
    path=$1
  else
    path=deploy
  fi
  echo "[micromundos] deploy en ./$path"

  mkdir -p ./$path/data
  cp -r ./data ./$path/ 

  for i in "${apps[@]}"
  do
    :
    mkdir -p ./$path/$i/bin
    cp -r ./$i/bin/$i.app ./$path/$i/bin/
  done

  mkdir -p ./$path/$app_proyector/bin
  cp -r ./$app_proyector/bin/$app_proyector.app ./$path/$app_proyector/bin/
  cp ./scripts/micromundos.sh ./$path/micromundos.sh 

  find ./$path/data -type f -name .DS_Store -delete
  find ./$path/data -type f -name *.swp -delete
  find ./$path/data -type f -name *.casa -delete
}

case "$1" in
  kickoff)
    check_pid
    save_pid
    kickoff
    ;;
  start)
    check_pid
    save_pid
    start
    ;;
  stop)
    stop
    ;;
  deploy)
    deploy $2
    ;;
esac

