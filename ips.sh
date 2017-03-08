#!/bin/bash

stdbuf -oL ./dnslookup gla.ac.uk qq.com amazon.com delfi.lt facebook.com vk.com bbc.co.uk juranec.github.io stackoverflow.com isoc.org | {
  while IFS= read -r line
  do
    ip=`echo $line|cut -d" "  -f3`
    ipv=`echo $line|cut -d" "  -f2`
    if [ "$ipv" = "IPV4" ]; then
      traceroute -4 -q 1 -n $ip
    fi
    if [ "$ipv" = "IPV6" ]; then
      traceroute -6 -q 1 -n $ip
    fi
  done
}