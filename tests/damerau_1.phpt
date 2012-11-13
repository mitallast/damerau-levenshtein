--TEST--
Bug #1 (Memory leaks)
--SKIPIF--
<?php extension_loaded('damerau') or die('skip damerau not available');?>
--FILE--
<?php
for($i=1000;$i;$i--)
    damerau_levenshtein("vvvqw", "vvvwq");
echo("correct");
?>
--EXPECT--
correct