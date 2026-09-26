# Evaluate the Bracket Pairs of a String

1. Maintain the knowledge base in a dictionary.
2. Iterate through the string and whenever you encounter a closing bracket, check if it matches the string tracked from the last opening bracket. If it does, replace the bracket pair with the corresponding value from the knowledge base.
3. Else, replace the bracket pair with a question mark.
4. Continue this process until the entire string has been evaluated.

TC/SC: O(n+m)/O(n+m) where n is the length of the string and m is the number of bracket pairs in the knowledge base.
