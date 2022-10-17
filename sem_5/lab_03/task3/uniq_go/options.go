package main

import (
	"errors"
	"flag"
	"os"
)

type Options struct {
	c          bool
	d          bool
	u          bool
	i          bool
	f          int
	s          int
	inputFile  *os.File
	outputFile *os.File
}

func initFlags() (Options, error) {
	cFlagPtr := flag.Bool("c", false, "prefix lines by the number of occurrences")
	dFlagPtr := flag.Bool("d", false, "only print duplicate lines")
	uFlagPtr := flag.Bool("u", false, "only print unique lines")
	iFlagPtr := flag.Bool("i", false, "ignore differences in case when comparing")

	fFlagPtr := flag.Int("f", 0, "avoid comparing the first N fields")
	sFlagPtr := flag.Int("s", 0, "avoid comparing the first N characters")

	flag.Parse()

	opts := Options{
		c:          *cFlagPtr,
		d:          *dFlagPtr,
		u:          *uFlagPtr,
		i:          *iFlagPtr,
		f:          *fFlagPtr,
		s:          *sFlagPtr,
		inputFile:  os.Stdin,
		outputFile: os.Stdout,
	}

	if !opts.d && opts.c && opts.u || opts.d && !opts.c && opts.u || opts.d && opts.c && !opts.u {
		return Options{}, errors.New("invalid flags")
	}

	return opts, nil
}
