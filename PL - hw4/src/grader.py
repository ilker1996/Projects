import sys
import os

diffPrefix = "diff -q "
valgrindPrefix = "valgrind --leak-check=yes --error-exitcode=1 "

def checkOutput(inputName):
	# Diff
	diffCommand = diffPrefix + "%s_Output.txt %s_Results.txt" % (inputName, inputName)

	if (os.system(diffCommand) > 0): 
		return -1
	else: 
		return 1

def printOutput(testNo, testName, message):
	print "--------------"
	print "-Test %d - %s - %s" % (testNo, testName, message)
	print "--------------"

if __name__ == '__main__':

	makePartial = "make partial"
	makePartialDuel = "make partialDuel"
	makeSingle = "make single"
	makeMulti = "make multi"

	partialCommand = "./PokePartial"
	partialDuelCommand = "./PokePartialDuel"
	singleCommand = "./PokeSingle"
	multiCommand = "./PokeMulti"

	inputNameList = ["T2P1", "T2P3", "T2P9", "T16P3", "T16P16", "T64P25"]
	totalPoints = 0

	isProducingOutput = False

	print "Phase 1: Partial Test - Pokemon"

	if os.system(makePartial) > 0:
		print "---", makePartial, "failed. ---"
	else:
		val = os.system(partialCommand)

		if val > 0 and val < 21 * 256:
			totalPoints += val / 256

		if val == 20 * 256:
			print "Successful!"
		else:
			print "Failed!"

	print "Phase 1: Partial Test - Duel"

	if os.system(makePartialDuel) > 0:
		print "---", makePartialDuel, "failed. ---"
	else:
		val = os.system(partialDuelCommand)

		if val > 0 and val < 11 * 256:
			totalPoints += val / 256

		if val == 10 * 256:
			print "Successful!"
		else:
			print "Failed!"

	print "Phase 1: Ended, Grade: %d" % totalPoints

	print "Phase 2: Single Tournament(s)"

	if os.system(makeSingle) > 0:
		print "---", makeSingle, "failed. ---"
	else:
		for i in range(len(inputNameList)):
			testNo = i +1
			inputName = "./InputOutput/" + inputNameList[i]

			inputTuple = (singleCommand, inputName, testNo, inputName)

			if isProducingOutput:
				os.system("%s %s_Input.txt Tournament_%d > %s_Results.txt" % inputTuple);
				continue
			else:	
				os.system("%s %s_Input.txt Tournament_%d > %s_Output.txt" % inputTuple);

				testResult = checkOutput(inputName) == 1

				if testResult:
					totalPoints += 5
					printOutput(testNo, inputName, "Successful!")
				else:
					printOutput(testNo, inputName, "Failed!")

	print "Phase 2: Ended, Grade: %d" % totalPoints

	print "Phase 3: Multiple Tournament"

	if os.system(makeMulti) > 0:
		print "---", makeMulti, "failed. ---"
	else:
		inputName = "./InputOutput/Multiple"
		inputTuple = (multiCommand, inputName)

		if isProducingOutput:
			os.system("%s > %s_Results.txt" % inputTuple)
			
		else:
			os.system("%s > %s_Output.txt" % inputTuple)

			testResult = checkOutput(inputName) == 1

			if testResult:
				totalPoints += 20
				printOutput(7, inputName, "Successful!")
			else:
				printOutput(testNo, inputName, "Failed!")

	print "Phase 3: Ended, Grade: %d" % totalPoints

	print "Phase 4: Memory Test"

	if os.system("%s %s" % (valgrindPrefix, multiCommand)) > 0:
		print "Memory Test: Failed"
	else:
		totalPoints += 20
		print "Memory Test: Success"

	print "Phase 4: Ended"

	print 
	
	print "Grade :=>> " + str(totalPoints)
