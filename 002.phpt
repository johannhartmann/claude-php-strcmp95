--TEST--
strcmp95() test with similar strings
--EXTENSIONS--
strcmp95
--FILE--
<?php
// Classic Jaro-Winkler test cases
var_dump(round(strcmp95("MARTHA", "MARHTA"), 4));
var_dump(round(strcmp95("DWAYNE", "DUANE"), 4));
var_dump(round(strcmp95("DIXON", "DICKSONX"), 4));

// Check sensitivity to prefix
var_dump(round(strcmp95("ABCDEF", "ABCXYZ"), 4));
var_dump(round(strcmp95("ABCDEF", "XYZABC"), 4));

// Different prefix weights
var_dump(round(strcmp95("PREFIX", "PREFIXX", 0.0), 4));   // No prefix bonus
var_dump(round(strcmp95("PREFIX", "PREFIXX", 0.1), 4));   // Default prefix weight
var_dump(round(strcmp95("PREFIX", "PREFIXX", 0.25), 4));  // Max prefix weight
?>
--EXPECTF--
float(0.9611)
float(0.84)
float(0.7667)
float(0.6667)
float(0.4444)
float(0.8889)
float(0.9)
float(0.9278)
