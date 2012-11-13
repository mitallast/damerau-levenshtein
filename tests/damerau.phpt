--TEST--
Bug #1 (Unicode support)
--SKIPIF--
<?php extension_loaded('damerau') or die('skip damerau not available');?>
--FILE--
<?php
var_dump(damerau_levenshtein("123", "12"));
var_dump(damerau_levenshtein("qwe", "qwa"));
var_dump(damerau_levenshtein("awe", "qwe"));
var_dump(damerau_levenshtein("фыв", "фыа"));
echo "\n"; // transposition
var_dump(damerau_levenshtein("vvvqw", "vvvwq"));
var_dump(damerau_levenshtein("qw", "wq"));
var_dump(damerau_levenshtein("qq", "ww"));
var_dump(damerau_levenshtein("qw", "qw"));
var_dump(damerau_levenshtein("пионер", "плеер"));
var_dump(damerau_levenshtein("пионер", "пионеер"));
var_dump(damerau_levenshtein("пионер", "поинер"));
var_dump(damerau_levenshtein("pioner", "poner"));
var_dump(damerau_levenshtein("пионер", "понер"));
?>
--EXPECT--
int(1)
int(1)
int(1)
int(1)

int(1)
int(1)
int(2)
int(0)
int(3)
int(1)
int(1)
int(1)
int(1)