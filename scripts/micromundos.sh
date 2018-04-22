#!/bin/bash

apps=(  
'backend'
'proyector'
)

isdev=false

dev() {
  isdev=true 
  start
}

start() {

  if [[ -f "$PIDFILE" ]]; then
    echo "micromundos ya esta corriendo: hay un pidfile $PIDFILE" >& 2
    exit 1
  fi

  PID="$$"
  echo "$PID" >> "$PIDFILE"

  if [ "$isdev" = true ]; then
    echo "micromundos dev apps (pid = $PID)"
    for i in "${apps[@]}"
    do
      :
      open -a $i
    done

  else
    while true
    do
      echo "micromundos apps (pid = $PID)"
      for i in "${apps[@]}"
      do
        :
        open -a $i
      done
      sleep 10
    done
  fi
}

stop() {

  if [[ ! -f "$PIDFILE" ]]; then
    echo "micromundos no esta corriendo: no hay pidfile $PIDFILE" >& 2
    exit 1
  fi

  PIDS=`cat "$PIDFILE"`
  echo "kill micromundos apps (pids = $PIDS)"
  for i in "${apps[@]}"
    do
      :
      echo "kill $i"
      pkill $i
    done
  kill $PIDS
  rm -f "$PIDFILE"
}

PIDFILE="/tmp/micromundos.pids"

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
esac

