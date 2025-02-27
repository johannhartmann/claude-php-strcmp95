# strcmp95 PHP Extension

A PHP extension that implements the Jaro-Winkler string similarity algorithm (also known as strcmp95).

## Overview

The strcmp95 extension provides a function that calculates the similarity between two strings using the Jaro-Winkler algorithm. This algorithm is particularly useful for comparing names or short strings where character transpositions are common, such as typing errors or phonetic variations.

The algorithm returns a value between 0.0 (completely different) and 1.0 (identical), with higher values indicating greater similarity.

## Features

- Calculates string similarity based on the Jaro-Winkler algorithm
- Adjustable prefix weight parameter to control the importance of matching prefixes
- Fast C implementation for optimal performance
- Simple API with a single function call
- No dependencies beyond PHP itself

## Installation

### From PECL

```bash
pecl install strcmp95
```

Then add the extension to your php.ini file:

```
extension=strcmp95.so
```

### From Source

```bash
# Clone the repository
git clone https://github.com/php/php-src.git
cd php-src/ext/strcmp95/claude-php-strcmp95

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

### When Building PHP from Source

When building PHP from source, you can enable the extension with:

```bash
./configure --enable-strcmp95
make
make install
```

## Usage

```php
<?php
// Basic usage - calculate similarity between two strings
$similarity = strcmp95("MARTHA", "MARHTA");
echo "Similarity: " . $similarity . "\n";  // Outputs approximately 0.961

// With optional prefix weight parameter (between 0.0 and 0.25)
$similarity = strcmp95("PREFIX", "PREFIXX", 0.25);  // Maximum prefix weight
echo "Similarity with max prefix weight: " . $similarity . "\n";  // Outputs approximately 0.928

// Practical example - finding the closest match in a list of names
function find_closest_match($input, $options) {
    $best_match = null;
    $highest_similarity = 0;
    
    foreach ($options as $option) {
        $similarity = strcmp95($input, $option);
        if ($similarity > $highest_similarity) {
            $highest_similarity = $similarity;
            $best_match = $option;
        }
    }
    
    return [
        'match' => $best_match,
        'similarity' => $highest_similarity
    ];
}

$names = ["John Smith", "Jane Doe", "Mary Johnson", "Robert Brown"];
$result = find_closest_match("Jon Smyth", $names);
echo "Best match: {$result['match']} (similarity: {$result['similarity']})";
// Outputs: Best match: John Smith (similarity: 0.883...)
?>
```

## Function Reference

```php
float strcmp95(string $str1, string $str2, float $prefix_weight = 0.1): float
```

### Parameters

- `$str1`: The first string to compare
- `$str2`: The second string to compare
- `$prefix_weight`: Optional weight for common prefixes (default: 0.1)
  - Must be between 0.0 and 0.25
  - Higher values give more importance to common prefixes
  - Set to 0.0 to use the standard Jaro similarity without the Winkler modification

### Return Value

- Returns a float between 0.0 and 1.0, where:
  - 1.0 indicates identical strings
  - 0.0 indicates completely different strings
  - Values in between indicate varying degrees of similarity

### Exceptions

- `TypeError`: Thrown when incorrect number of arguments are passed
- `ValueError`: Thrown when the prefix_weight is outside the valid range (0.0 to 0.25)

## Algorithm Details

The Jaro-Winkler similarity metric consists of two parts:

1. **Jaro Similarity**: The base algorithm that considers:
   - The number of matching characters between the strings
   - The number of transpositions needed to align these matching characters
   - The lengths of the original strings

2. **Winkler Modification**: An extension that gives more favorable ratings to strings that share a common prefix (up to 4 characters).

The formula for Jaro similarity is:
```
jaro_sim = (m/|s1| + m/|s2| + (m-t)/m) / 3
```
Where:
- m is the number of matching characters
- |s1| and |s2| are the lengths of the two strings
- t is the number of transpositions

The Jaro-Winkler similarity adds the prefix bonus:
```
jaro_winkler_sim = jaro_sim + (prefix_length * prefix_weight * (1 - jaro_sim))
```

## Performance Considerations

- The time complexity is O(n²) where n is the length of the longer string
- The extension is optimized for strings of moderate length (names, words, short phrases)
- For very long strings, consider using other similarity metrics

## Use Cases

- Fuzzy name matching
- Spelling correction
- Record linkage and data deduplication
- Fraud detection
- Search with typo tolerance

## License

This extension is released under the PHP License v3.01.

## Credits

The Jaro-Winkler algorithm was developed by:
- Matthew A. Jaro, who introduced the Jaro distance
- William E. Winkler, who added the prefix adjustment

## Version History

- 1.0.0: Initial release