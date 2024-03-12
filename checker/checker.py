#!/usr/bin/env python

import argparse
import subprocess
import os
import sys

# ANSI color escape sequences for colored output
ANSI_RED = "\033[91m"
ANSI_GREEN = "\033[92m"
ANSI_YELLOW = "\033[93m"
ANSI_BLUE = "\033[94m"
ANSI_RESET = "\033[0m"

# Constants
memLogFileFlag = "--log-file="
memLogDir = "valgrind_logs"
memLineCheck = "All heap blocks were freed -- no leaks are possible"
valgrindFlags = "--leak-check=full --show-leak-kinds=all --track-origins=yes"
withValgrind = False
memPoints = 0

# Helper functions for colored output
def print_red(message):
    print(ANSI_RED + message + ANSI_RESET)

def print_green(message):
    print(ANSI_GREEN + message + ANSI_RESET)

def print_yellow(message):
    print(ANSI_YELLOW + message + ANSI_RESET)

def print_blue(message):
    print(ANSI_BLUE + message + ANSI_RESET)

# Function to check if memory logs indicate no leaks
def check_mem_log(memLog):
    with open(memLog) as f:
        datafile = f.readlines()
    for line in datafile:
        if memLineCheck in line:
            return True
    return False

# Function to run a test suite and calculate points
def test_suite(type, typeScore, typeNo):
    global memPoints
    totalSuitePoints = 0
    for n in range(1, typeNo + 1):
        commandsFile = f"{testsDir}/commands_{type}_{n}.in"
        binaryFile = f"{testsDir}/sensors_{type}_{n}.dat"
        procString = f"{runExec} {binaryFile} < {commandsFile}"

        if withValgrind:
            procString = f"valgrind {memLogFileFlag}{memLogDir}/{type}_{n}.log {valgrindFlags} {procString}"
        proc = os.popen(procString)

        result = proc.read()
        expectedResult = open(f"{refDir}/{type}_{n}.ref", "r").read()
        proc.close()

        if args.output:
            outputFile = open(f"{outputDir}/{type}_{n}.out", "w")
            outputFile.write(result)
            outputFile.close()

        if expectedResult == result and withValgrind and check_mem_log(f"{memLogDir}/{type}_{n}.log"):
            totalSuitePoints += typeScore
            print_green(f"Test `{type}_{n}` PASSED: {typeScore} / {typeScore}")
        elif expectedResult == result and not withValgrind:
            print_yellow(f"Test `{type}_{n}` OUT = REF, without memory check: {typeScore} / {typeScore}")
            totalSuitePoints += typeScore
            memPoints = 0
        elif expectedResult == result:
            print_yellow(f"Test `{type}_{n}` OUT = REF, memory check failed: {typeScore} / {typeScore}")
            totalSuitePoints += typeScore
            memPoints = 0
        else:
            print_red(f"Test `{type}_{n}` FAILED: 0 / {typeScore}")

    print(f"\n`{type}` tests: {totalSuitePoints} / {typeScore * typeNo}\n")
    return totalSuitePoints

# Argument parsing
argParser = argparse.ArgumentParser(description="Python Checker", prog="python3 checker.py")
argParser.add_argument("-v", "--valgrind", action="store_true", help="Run tests with `valgrind`. Tests may take longer to finish.")
argParser.add_argument("-o", "--output", action="store_true", help="Create output files for the selected tests in `tmp` directory.")
argParser.add_argument("-c", "--clear", action="store_true", help="Run tests for `clear` command.")
argParser.add_argument("-a", "--analyse", action="store_true", help="Run tests for `analyse` command.")
argParser.add_argument("-p", "--print", action="store_true", help="Run tests for `print` command.")
argParser.add_argument("--all", action="store_true", help="Run all tests.")
args = argParser.parse_args()

if len(sys.argv) == 1:
    print(argParser.format_help())
    exit(0)

# Paths and directories
execName = "main"
execDir = "../src"
execPath = f"{execDir}/{execName}"
testsDir = "input"
outputDir = "tmp"
refDir = "output"
csChecker = "cs/cs.sh"
runExec = "./" + execPath
readme = execDir + "/README"
readmeMD = readme + ".md"

# Check if Valgrind is enabled and set points
if args.valgrind:
    withValgrind = True
    memPoints = 20

# Initialize points counter
points = 0

# Start the testing process
print_blue("\n======================== INIT TEST =========================\n")

# Compile the code
rc = subprocess.call(f"make -C {execDir}", shell=True)
if rc != 0:
    sys.stderr.write("make failed with status %d\n" % rc)
    sys.exit(rc)

if not os.path.exists(execPath):
    sys.stderr.write("The file %s is missing and could not be created with \'make\'" % execPath)
    sys.exit(-1)

print_blue("\n==================== README AND CS TEST ====================\n")

csRes = subprocess.Popen([f"./{csChecker}", f"{execDir}"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
csRes.wait()
csResString = csRes.stdout.read().decode()
if os.path.exists(readme) and len(open(readme).readlines()) > 25 or os.path.exists(readmeMD) and len(open(readmeMD).readlines()) > 25:
    if "ERROR" not in csResString:
        points += 10
        print_green(f"Coding Style OK | README/README.md found. {points} / 10")
    else:
        points += 0
        print_red(f"Coding Style ERROR | README/README.md found. {points} / 10\n")
        for line in csResString.splitlines():
            if "ERROR" in line:
                print(line)
else:
    if "ERROR" not in csResString:
        points += 10
        print_green(f"Coding Style OK | README/README.md not found or is empty. {points} / 10")
    else:
        print_red(f"Coding Style ERROR | README/README.md not found or is empty. {points} / 10\n")
        for line in csResString.splitlines():
            if "ERROR" in line:
                print(line)

if args.print or args.all:
    print_blue("\n==================== PRINT EASY TESTS ======================\n")
    printEasyTestNo = 5
    printEasyTestPoints = 2
    points += test_suite("print_easy", printEasyTestPoints, printEasyTestNo)

if args.print or args.all:
    print_blue("\n==================== PRINT HARD TESTS ======================\n")
    printHardTestNo = 5
    printHardTestPoints = 4
    points += test_suite("print_hard", printHardTestPoints, printHardTestNo)

if args.clear or args.all:
    print_blue("\n==================== CLEAR EASY TESTS ======================\n")
    clearEasyTestNo = 5
    clearEasyTestPoints = 2
    points += test_suite("clear_easy", clearEasyTestPoints, clearEasyTestNo)

if args.clear or args.all:
    print_blue("\n==================== CLEAR HARD TESTS ======================\n")
    clearHardTestNo = 5
    clearHardTestPoints = 2
    points += test_suite("clear_hard", clearHardTestPoints, clearHardTestNo)

if args.analyse or args.all:
    print_blue("\n====================== ANALYSE TESTS =======================\n")
    analyseTestNo = 5
    analyseTestPoints = 4
    points += test_suite("analyse", analyseTestPoints, analyseTestNo)

if (points < 50) and (withValgrind == True):
    memPoints = 0
else:
    print_red("\nIMPORTNAT: Use --all -v for whole test suite with valgrind\n")

print_blue(f"\n====================== TOTAL {points + memPoints} / 100 ======================\n")

# Clean executable and object files
print_yellow("\n======================== MAKE CLEAN =========================\n")

print_blue(f"\n======================== FINISH TEST  =========================\n")

# Call make clean
rc = subprocess.call(f"make clean -C {execDir}", shell=True)
if rc != 0:
    sys.stderr.write("make failed with status %d\n" % rc)
    sys.exit(rc)

quit()
