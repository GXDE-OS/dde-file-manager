#!/bin/bash

process=`ps ax -o 'cmd' |grep 'gxde-file-manager$'`;
processd=`ps ax -o 'cmd' |grep 'gxde-file-manager -d$'`;
  
if [[ "$process" == ""&&"$processd" == "" ]]; then
    gxde-file-manager trash:///
else
    echo "{\"paths\":[\"trash:///\"]}" |socat - $XDG_RUNTIME_DIR/gxde-file-manager
fi