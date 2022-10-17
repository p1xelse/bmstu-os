package main

import (
	"strconv"
	"strings"
)

func skip(str string, opts Options) string {
	fields := strings.Split(str, " ")

	if len(fields) < opts.f {
		return ""
	}

	str = strings.Join(fields[opts.f:], " ")

	if len(str) < opts.s {
		return ""
	}

	return str[opts.s:]
}

func addStrInResult(result []string, curStr string, count int, opts Options) []string {
	switch {
	case opts.c:
		result = append(result, strconv.Itoa(count)+" "+curStr)
	case opts.d && count > 1:
		result = append(result, curStr)
	case opts.u && count == 1:
		result = append(result, curStr)
	case !opts.d && !opts.u:
		result = append(result, curStr)
	}

	return result
}

func Uniq(lines []string, opts Options) []string {
	var result []string
	var curLine string

	count := 1
	prevLine := skip(lines[0], opts)

	for idx := 1; idx < len(lines); idx++ {
		curLine = lines[idx]

		if opts.i {
			curLine = strings.ToUpper(curLine)
			prevLine = strings.ToUpper(prevLine)
		}

		curLine = skip(curLine, opts)

		if prevLine == curLine {
			count++
			continue
		}

		result = addStrInResult(result, lines[idx-count], count, opts)
		prevLine = curLine
		count = 1
	}

	result = addStrInResult(result, lines[len(lines)-count], count, opts)

	return result
}
