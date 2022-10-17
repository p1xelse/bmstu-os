package main

import (
	"bufio"
	"flag"
	"fmt"
	"io"
	"os"
)

func usage() {
	fmt.Println("usage: uniq [-c | -d | -u] [-i] [-f fields] [-s chars] [input [output]]")
}

func readStrings(inputFile *os.File) (strArr []string, err error) {
	strReader := bufio.NewReader(inputFile)

	for {
		line, err := strReader.ReadString('\n')
		if err != nil {
			if err == io.EOF {
				break
			} else {
				return nil, err
			}
		}
		strArr = append(strArr, line)
	}

	return strArr, nil
}

func main() {
	opts, err := initFlags()
	if err != nil {
		usage()
		return
	}
	lenArgs := len(flag.Args())
	inFile := os.Stdin
	oFile := os.Stdout

	switch lenArgs {
	case 0:
	case 1:
		inFile, err = os.Open(flag.Args()[0])
		if err != nil {
			fmt.Println(err)
			return
		}

		defer inFile.Close()

	case 2:
		inFile, err = os.Open(flag.Args()[0])
		if err != nil {
			fmt.Println(err)
			return
		}
		defer inFile.Close()

		oFile, err = os.Create(flag.Args()[1])
		if err != nil {
			fmt.Println(err)
			return
		}
		defer oFile.Close()

	default:
		usage()
	}

	opts.inputFile = inFile
	opts.outputFile = oFile

	strings, err := readStrings(opts.inputFile)

	if err != nil {
		fmt.Println(err)
		return
	}

	result := Uniq(strings, opts)

	for _, str := range result {
		_, err = opts.outputFile.WriteString(str)
		if err != nil {
			fmt.Println(err)
			return
		}
	}

	return
}
