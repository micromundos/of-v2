#!/bin/bash

apps=(  
'backend'
'proyector'
)

start() {

  if [[ -f "$PIDFILE" ]]; then
    echo "micromundos ya esta corriendo: hay un pidfile $PIDFILE" >& 2
    exit 1
  fi

  PID="$$"
  echo "$PID" >> "$PIDFILE"

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
  start)
    start
    ;;
  stop)
    stop
    ;;
esac

