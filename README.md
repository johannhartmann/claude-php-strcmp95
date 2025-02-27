# strcmp95 PHP Extension

A PHP extension that implements the Jaro-Winkler string similarity algorithm (also known as strcmp95).

## Overview

The strcmp95 extension provides a function that calculates the similarity between two strings using the Jaro-Winkler algorithm. This algorithm is particularly useful for comparing names or short strings where character transpositions are common, such as typing errors.

## Installation

### From Source

```bash
# Clone the PHP source or navigate to the extension directory
cd /path/to/php-src/ext/strcmp95

# Configure and build the extension
phpize
./configure
make
make install
```

Then add the extension to your php.ini file:

```
extension=strcmp95.so
```

### From PHP Source

When building PHP from source, you can enable the extension with:

```bash
./configure --enable-strcmp95
make
make install
```

## Usage

```php
<?php
// Calculate similarity between two strings (returns a value between 0.0 and 1.0)
$similarity = strcmp95("MARTHA", "MARHTA");
echo "Similarity: " . $similarity . "\n";  // Outputs approximately 0.9611

// Specify a custom prefix weight (between 0.0 and 0.25)
$similarity = strcmp95("PREFIX", "PREFIXX", 0.25);  // Maximum prefix weight
echo "Similarity with max prefix weight: " . $similarity . "\n";  // Outputs approximately 0.9278
?>
```

## Function Parameters

```php
float strcmp95(string $str1, string $str2, float $prefix_weight = 0.1): float
```

- `$str1`: The first string to compare
- `$str2`: The second string to compare
- `$prefix_weight`: Optional weight for common prefixes (default 0.1, must be between 0.0 and 0.25)

## Algorithm Details

The Jaro-Winkler similarity metric is an extension of the Jaro similarity metric, giving more favorable ratings to strings that match from the beginning (share a common prefix).

The Jaro similarity is based on:
1. The number of matching characters between the strings
2. The number of transpositions needed to align these matching characters
3. The lengths of the original strings

The Winkler modification increases the similarity score for strings that share a common prefix.

## License

This extension is released under the PHP License v3.01.