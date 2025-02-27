--TEST--
Check if strcmp95 function works correctly with identical strings
--EXTENSIONS--
strcmp95
--FILE--
<?php
// Test identical strings
var_dump(strcmp95("MARTHA", "MARTHA"));
var_dump(strcmp95("", ""));

// Test with different prefix weights
var_dump(strcmp95("MARTHA", "MARTHA", 0.0));
var_dump(strcmp95("MARTHA", "MARTHA", 0.1));
var_dump(strcmp95("MARTHA", "MARTHA", 0.25));
?>
--EXPECT--
float(1)
float(1)
float(1)
float(1)
float(1)
