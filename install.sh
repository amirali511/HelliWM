#!/bin/bash

echo "
  HHH     HHH   EEEEEEEEE   LLL         LLL         III
  HHH     HHH   EEEEEEEEE   LLL         LLL         III
  HHH     HHH   EEE         LLL         LLL         III
  HHHHHHHHHHH   EEEEEEEEE   LLL         LLL         III
  HHHHHHHHHHH   EEEEEEEEE   LLL         LLL         III
  HHH     HHH   EEE         LLL         LLL         III
  HHH     HHH   EEEEEEEEE   LLLLLLLLL   LLLLLLLLL   III
  HHH     HHH   EEEEEEEEE   LLLLLLLLL   LLLLLLLLL   III
";

awk -f parser.awk;
meson setup build;
cp config.h build/config.h;
ninja -C build;
sudo ninja -C build install;