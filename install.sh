#!/bin/bash

RED='\033[0;31m';
GREEN='\033[0;32m';
BLUE='\033[0;34m';
YELLOW='\033[1;33m';
ORANGE='\033[0;33m';
PURPLE='\033[1;35m';
NOCOLOR='\033[0m';

echo -e "${PURPLE}
  HHH     HHH   EEEEEEEEE   LLL         LLL         III   WWW              WWWWWW              WWW   MMMMMM               MMMMMM
  HHH     HHH   EEEEEEEEE   LLL         LLL         III    WWW            WWW  WWW            WWW    MMM  MMM            MMM MMM
  HHH     HHH   EEE         LLL         LLL         III     WWW          WWW    WWW          WWW     MMM   MMM          MMM  MMM
  HHHHHHHHHHH   EEEEEEEEE   LLL         LLL         III      WWW        WWW      WWW        WWW      MMM    MMM        MMM   MMM
  HHHHHHHHHHH   EEEEEEEEE   LLL         LLL         III       WWW      WWW        WWW      WWW       MMM     MMM      MMM    MMM
  HHH     HHH   EEE         LLL         LLL         III        WWW    WWW          WWW    WWW        MMM      MMM    MMM     MMM
  HHH     HHH   EEEEEEEEE   LLLLLLLLL   LLLLLLLLL   III         WWW  WWW            WWW  WWW         MMM       MMM  MMM      MMM
  HHH     HHH   EEEEEEEEE   LLLLLLLLL   LLLLLLLLL   III          WWWWWW              WWWWWW          MMM        MMMMMM       MMM
${NOCOLOR}";


while [[ true ]]; do
	printf  "${BLUE}What do you want to do?${NOCOLOR} [${GREEN}y${NOCOLOR}, ${RED}n${NOCOLOR}, ${ORANGE}a${NOCOLOR}][${GREEN}install${NOCOLOR}, ${RED}uninstall${NOCOLOR}, ${ORANGE}abort${NOCOLOR}]: "
	read INPUT;
	printf "\n";
	if [[ $INPUT = y || $INPUT = install ]]; then
		printf "${GREEN}Installing HelliWM...${NOCOLOR}\n";
		cd src;
		python3 Generator.py;
		awk -f parser.awk;
		meson setup build;
		cp config.h build/config.h;
		ninja -C build;
		sudo ninja -C build install;
		printf "${BLUE}Install done.${NOCOLOR}\n";
		exit;
	elif [[ $INPUT = n || $INPUT = uninstall ]]; then
		printf "${RED}Uninstalling HelliWM...${NOCOLOR}\n";
		cd src;
		sudo ninja -C build uninstall;
		rm config.h;
		rm shortcut.h;
		rm -R build;
		printf "${BLUE}Uninstall done.${NOCOLOR}\n";
		exit;
	elif [[ $INPUT = a || $INPUT = abort ]]; then
		printf "${ORANGE}Aborting...${NOCOLOR}\n";
		printf "${BLUE}Aborted.${NOCOLOR}\n";
		exit;
	else
		printf "${YELLOW}Invalid input!${NOCOLOR}\n";
	fi
done
