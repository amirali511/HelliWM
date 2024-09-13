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

while [[ true ]]; do
	echo -n "What do you want to do? [y, n, a][install, uninstall, abort]: "
	read INPUT
	if [[ $INPUT = y || $INPUT = install ]]; then
		echo "Installing HelliWM...";
		cd src;
		python3 Generator.py;
		awk -f parser.awk;
		meson setup build;
		cp config.h build/config.h;
		ninja -C build;
		sudo ninja -C build install;
		exit;
	elif [[ $INPUT = n || $INPUT = uninstall ]]; then
		echo "Uninstalling HelliWM...";
		cd src;
		sudo ninja -C build uninstall;
		rm config.h;
		rm shortcut.h;
		rm -R build;
		exit;
	elif [[ $INPUT = a || $INPUT = abort ]]; then
		echo "Aborting...";
		exit;
	else
		echo "Invalid input!";
	fi
done
