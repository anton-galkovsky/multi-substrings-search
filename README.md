Spring 2018

# Multiple Substrings Search

The program implements a tree that allows you to quickly search for multiple substrings in the given string. The algorithm is an extension of the Boyer–Moore algorithm. The vertices of the corresponding tree are the letters of substrings. While symbol-by-symbol moving through the line, there is descending through the tree. In the absence of possible coincidences, a transition occurs along the special edges to the upper vertices of the tree
