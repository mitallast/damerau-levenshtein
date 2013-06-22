# About

This is C extension for PHP.
It's contains multibyte (utf-8 compatible), memory efficient implementation of Damerau-Levenshtein algorithm, more information at [wikipedia.org](http://en.wikipedia.org/wiki/Damerau–Levenshtein_distance).

[![Build Status](https://api.travis-ci.org/mitallast/damerau-levenshtein.png)](https://travis-ci.org/mitallast/damerau-levenshtein)

# Description

damerau_levenshtein – calculate Damerau-Levenshtein distance between two strings.

    int damerau_levenshtein(string str1, string str2)
    int damerau_levenshtein(string str1, string str2, int cost_ins, int cost_sub, int cost_del, int cost_tran)


The Levenshtein distance is defined as the minimal number of characters you have to replace, insert, delete or transitive to transform str1 into str2. The complexity of the algorithm is O(m*n), where n and m are the length of str1 and str2 (rather good when compared to similar_text(), which is O(max(n,m)**3), but still expensive).

In its simplest form the function will take only the two strings as parameter and will calculate just the number of insert, replace, delete and transitive operations needed to transform str1 into str2.

A second variant will take four additional parameters that define the cost of insert, replace, delete and transitive operations. This is more general and adaptive than variant one, but not as efficient.

## Parameters

* *str1* – One of the strings being evaluated for Levenshtein distance.
* *str2* – One of the strings being evaluated for Levenshtein distance.
* *cost_ins* – Defines the cost of insertion.
* *cost_rep* – Defines the cost of replacement.
* *cost_del* – Defines the cost of deletion.
* *cost_tran* – Defines the cost of transition

## Return values

This function returns the Levenshtein-Distance between the two argument strings or -1, if one of the argument strings is longer than the limit of 255 characters.

# Examples

    var_dump(damerau_levenshtein("apple", "apple"));  // int(1)
    var_dump(damerau_levenshtein("aple", "apple"));   // int(1)
    var_dump(damerau_levenshtein("appple", "apple")); // int(1)
    var_dump(damerau_levenshtein("apnle", "apple"));  // int(1)
    var_dump(damerau_levenshtein("aplpe", "apple"));  // int(1)

# Instalation

For mac and linux, minimal instructions are:

    phpize
    ./configure --with-damerau
    make
    make install

Sometimes, like gentoo, need more operations:

    phpize
    aclocal
    libtoolize --force
    autoheader
    autoconf
    ./configure --enable-damerau
    make
    make install

When make install correctly completed, add to your php.ini

    extension=damerau.so
