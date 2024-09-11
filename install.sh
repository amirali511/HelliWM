#!/bin/bash

echo "
  HHH     HHH   EEEEEEEEE   LLL         LLL         III   WWW              WWWWWW              WWW   MMMMMM               MMMMMM
  HHH     HHH   EEEEEEEEE   LLL         LLL         III    WWW            WWW  WWW            WWW    MMM  MMM            MMM MMM
  HHH     HHH   EEE         LLL         LLL         III     WWW          WWW    WWW          WWW     MMM   MMM          MMM  MMM
  HHHHHHHHHHH   EEEEEEEEE   LLL         LLL         III      WWW        WWW      WWW        WWW      MMM    MMM        MMM   MMM
  HHHHHHHHHHH   EEEEEEEEE   LLL         LLL         III       WWW      WWW        WWW      WWW       MMM     MMM      MMM    MMM
  HHH     HHH   EEE         LLL         LLL         III        WWW    WWW          WWW    WWW        MMM      MMM    MMM     MMM
  HHH     HHH   EEEEEEEEE   LLLLLLLLL   LLLLLLLLL   III         WWW  WWW            WWW  WWW         MMM       MMM  MMM      MMM
  HHH     HHH   EEEEEEEEE   LLLLLLLLL   LLLLLLLLL   III          WWWWWW              WWWWWW          MMM        MMMMMM       MMM
";

cd src;
python3 Generator.py;
awk -f parser.awk;
meson setup build;
cp config.h build/config.h;
ninja -C build;
sudo ninja -C build install;