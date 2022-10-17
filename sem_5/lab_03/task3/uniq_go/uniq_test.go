package main

import (
	"github.com/stretchr/testify/require"
	"testing"
)

func TestUniq(t *testing.T) {
	tests := map[string]struct {
		input  []string
		flags  Options
		result []string
	}{
		"Without_parameters": {
			input: []string{
				"I love music.",
				"I love music.",
				"I love music.",
				"",
				"I love music of Kartik.",
				"I love music of Kartik.",
				"Thanks.",
				"I love music of Kartik.",
				"I love music of Kartik.",
			},
			flags: Options{
				c: false,
				d: false,
				u: false,
				f: 0,
				s: 0,
				i: false,
			},
			result: []string{
				"I love music.",
				"",
				"I love music of Kartik.",
				"Thanks.",
				"I love music of Kartik.",
			},
		},
		"With_count_flag": {
			input: []string{
				"I love music.",
				"I love music.",
				"I love music.",
				"",
				"I love music of Kartik.",
				"I love music of Kartik.",
				"Thanks.",
				"I love music of Kartik.",
				"I love music of Kartik.",
			},
			flags: Options{
				c: true,
				d: false,
				u: false,
				f: 0,
				s: 0,
				i: false,
			},
			result: []string{
				"3 I love music.",
				"1 ",
				"2 I love music of Kartik.",
				"1 Thanks.",
				"2 I love music of Kartik.",
			},
		},
		"With_Repeat_Flag": {
			input: []string{
				"I love music.",
				"I love music.",
				"I love music.",
				"",
				"I love music of Kartik.",
				"I love music of Kartik.",
				"Thanks.",
				"I love music of Kartik.",
				"I love music of Kartik.",
			},
			flags: Options{
				c: false,
				d: true,
				u: false,
				f: 0,
				s: 0,
				i: false,
			},
			result: []string{
				"I love music.",
				"I love music of Kartik.",
				"I love music of Kartik.",
			},
		},
		"With_Unique_Flag": {
			input: []string{
				"I love music.",
				"I love music.",
				"I love music.",
				"",
				"I love music of Kartik.",
				"I love music of Kartik.",
				"Thanks.",
				"I love music of Kartik.",
				"I love music of Kartik.",
			},
			flags: Options{
				c: false,
				d: false,
				u: true,
				f: 0,
				s: 0,
				i: false,
			},
			result: []string{
				"",
				"Thanks.",
			},
		},
		"With_Ignore_Case_Flag": {
			input: []string{
				"I LOVE MUSIC.",
				"I love music.",
				"I LoVe MuSiC.",
				"",
				"I love MuSIC of Kartik.",
				"I love music of kartik.",
				"Thanks.",
				"I love music of kartik.",
				"I love MuSIC of Kartik.",
			},
			flags: Options{
				c: false,
				d: false,
				u: false,
				f: 0,
				s: 0,
				i: true,
			},
			result: []string{
				"I LOVE MUSIC.",
				"",
				"I love MuSIC of Kartik.",
				"Thanks.",
				"I love music of kartik.",
			},
		},
		"With_Skip_Field_Flag": {
			input: []string{
				"We love music.",
				"I love music.",
				"They love music.",
				"",
				"I love music of Kartik.",
				"We love music of Kartik.",
				"Thanks.",
			},
			flags: Options{
				c: false,
				d: false,
				u: false,
				f: 1,
				s: 0,
				i: true,
			},
			result: []string{
				"We love music.",
				"",
				"I love music of Kartik.",
				"Thanks.",
			},
		},
		"With_Skip_Chars_Flag": {
			input: []string{
				"I love music.",
				"A love music.",
				"C love music.",
				"",
				"I love music of Kartik.",
				"We love music of Kartik.",
				"Thanks.",
			},
			flags: Options{
				c: false,
				d: false,
				u: false,
				f: 0,
				s: 1,
				i: true,
			},
			result: []string{
				"I love music.",
				"",
				"I love music of Kartik.",
				"We love music of Kartik.",
				"Thanks.",
			},
		},
	}

	for name, test := range tests {
		test := test
		t.Run(name, func(t *testing.T) {
			got, expected := Uniq(test.input, test.flags), test.result
			require.Equal(t, expected, got)
		})
	}
}
