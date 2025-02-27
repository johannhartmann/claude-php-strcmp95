--TEST--
strcmp95() test parameter validation
--EXTENSIONS--
strcmp95
--FILE--
<?php
// Test required parameters
try {
    var_dump(strcmp95());
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

try {
    var_dump(strcmp95("test"));
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

// Test prefix_weight validation
try {
    var_dump(strcmp95("test", "test", -0.1));
} catch (ValueError $e) {
    echo $e->getMessage(), "\n";
}

try {
    var_dump(strcmp95("test", "test", 0.3));
} catch (ValueError $e) {
    echo $e->getMessage(), "\n";
}

// Valid prefix_weight
var_dump(strcmp95("test", "test", 0.0));
var_dump(strcmp95("test", "test", 0.1));
var_dump(strcmp95("test", "test", 0.25));
?>
--EXPECTF--
strcmp95() expects at least 2 arguments, 0 given
strcmp95() expects at least 2 arguments, 1 given
prefix_weight must be between 0.0 and 0.25
prefix_weight must be between 0.0 and 0.25
float(1)
float(1)
float(1)
