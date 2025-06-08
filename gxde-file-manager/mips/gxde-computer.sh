#!/bin/bash

process=`ps ax -o 'cmd' |grep 'gxde-file-manager$'`;
processd=`ps ax -o 'cmd' |grep 'gxde-file-manager -d$'`;
  
if [[ "$process" == ""&&"$processd" == "" ]]; then
    gxde-file-manager computer:///
else
    echo "{\"paths\":[\"computer:///\"]}" |socat - $XDG_RUNTIME_DIR/gxde-file-manager
fi