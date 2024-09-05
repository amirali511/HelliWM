# AWK Parser version 1.5 (HelliParse 1.5)
# WMRC version 1.2

BEGIN {
	parser_version = "1.5"
	print "// Auto-generated by HelliParse-"parser_version"\n" > "config.h"
	print "// Edit only if you know what you're doing!\n" > "config.h"
	print "#define DEFAULT XCB_NONE\n" > "config.h"

	# print "// Auto-generated by HelliParse-"parser_version"\n"
	# print "// Edit only if you know what you're doing!\n"
	# print "#define DEFAULT XCB_NONE\n"

	indexer = 1
	flag = 1

	while (1) {
		getline store < "wmrc";
		if (store == EOF){
			exit
			break
		}
		split(store, SP, " ")
		
		for (x in SP) {
			
			if (SP[x] == "%") {
				print "// "store > "config.h"
				
				# print "// "store
			}
			
			if (SP[1] == "define") {
				
				if (SP[4] == "_" && SP[5] != "_") {
					print "#ifndef "SP[2] > "config.h"
					print "#define "SP[2]" "SP[5] > "config.h"
					print "#endif" > "config.h"
					print "\n" > "config.h"

					# print "#ifndef "SP[2]
					# print "#define "SP[2]" "SP[5]
					# print "#endif"
					# print "\n"
					break
				} 
				
				if (SP[4] != "_" && SP[5] == "_") {
					print "#ifndef "SP[2] > "config.h"
					print "#define "SP[2]" "SP[4] > "config.h"
					print "#endif" > "config.h"
					print "\n" > "config.h"
					
					# print "#ifndef "SP[2]
					# print "#define "SP[2]" "SP[4]
					# print "#endif"
					# print "\n"
					
					break
				}
				
				if (SP[4] != "_" && SP[5] != "_") {
					print "#ifndef "SP[2] > "config.h"
					print "#define "SP[2]" "SP[4] > "config.h"
					print "#endif" > "config.h"
					print "\n" > "config.h"
					
					# print "#ifndef "SP[2]
					# print "#define "SP[2]" "SP[4]
					# print "#endif"
					# print "\n"
					
					break
				}
				
				if (SP[4] == SP[5] && SP[5] == "_") {
					print "// !!!ERROR!!!\nHelliParse could not parse line "indexer" because both the default and the override values were empty"
					flag = 0
				  exit
				}
			}
			
			if (SP[1] != "define" && SP[1] != "%") {
				print "// !!!ERROR!!!\nUnknown symbol in line "indexer
				flag = 0
				exit
			}
		}
		
		indexer++
	}
}

END {
	if (flag == 1) {
		print "\n// HelliParse "parser_version" finished parsing wmrc successfully.\n//Check the header files for any possible errors and fix the wmrc file and run this file again if needed.\n"
	}
	
	if (flag == 0) {
		print "\n// HelliParse "parser_version" finished parsing wmrc with errors, so look above to see the errors"
	}
}
