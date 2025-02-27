<?php

/**
 * @generate-class-entries
 */

/**
 * Calculates the Jaro-Winkler similarity between two strings
 *
 * @param string $str1 The first string
 * @param string $str2 The second string
 * @param float $prefix_weight Optional prefix weight (default: 0.1)
 * @return float Returns a value between 0.0 and 1.0 (higher = more similar)
 */
function strcmp95(string $str1, string $str2, float $prefix_weight = 0.1): float {}
