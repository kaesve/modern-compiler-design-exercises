
1. see `match-abcabc.c`. There is also a `timed.c`, which prints some timing information. This version runs my implementation, and an implementation by a friend of mine.
2. ---
3. ---
4. Newlines do not mean the same as a space in every context. Some contexts in which they differ are in string literals and in single line comments.
5. ---
6. 
    1. `/0*10*10*/`
    2. `/1?(0+1)*0*/`
    3. `/(0*10*10*)+/`

7. ---
8. 
    * `/a?*/` means one or zero a's, zero or more times -- or: zero or more a's.
    * `/a**/` means zero or more a's, zero or more times -- or: zero or more a's.

    These expressions are not erroneous. They are also not ambiguous in that there is no string that both matches and doesn't match the expression.
    
9. .
    1. .
        ```
            letter               = [A-Za-z]
            digit                = [0-9]
            letgit               = letter | digit
            letgit_hyphen        = letgit | '-'
            letgit_hyphen_string = letgit_hyphen+
            label                = letter '(' letgit_hyphen_string? letgit ')' '?'
            url                  = label ('.' label)*
        ```

    2. .
        ```
            url = [A-Za-z] '(' ([A-Za-z] | [digit] | '-')? ([A-Za-z] | [digit]) ')' '?' ('.' [A-Za-z] '(' ([A-Za-z] | [digit] | '-')? ([A-Za-z] | [digit]) ')' '?')*
        ```
        
10. 
    ``` c
    void skip_layout_and_comment(void) {
        while (is_layout(input_char)) { next_char(); }
        int nest_depth = 0;
        do {
            if (is_end_of_input(input_char)) { return; }
            else if (is_comment_starter(input_char)) { next_depth++; }
            else if (is_comment_stopper(input_char)) { next_depth--; }

            next_char();
        } while (nest_depth > 0);
        while (is_layout(input_char)) { next_char(); }
    }
    ```
11. ---
12. String literals could contain the other characters.
13. Because the algorithm tries to find the longest match of the regular experssion, not just any match.
14. In the case of the case where the dot is just before the closing parenthesis of a subexpression we have matched the subexpression and can move the dot to after the subexpression. If either of the operators `+` or `*` apply to the subexpression, another item should be added with the dot just before the subexpression.
15. A pattern that matches any character two consecutive times is complicated because it is not context free; you need information of previous characters to see if a current character matches. This cannot be done in linear time and regular expressions will not be able to do these matches. One solution is to limit the characters for which you want this pattern to match, and have a pattern for each character, e.g: `aa`, `bb`, ... A different method I would prefer is to analyze two characters at once in the lexer. This squares the number of different symbols the lexer has to deal with, but the time complexity can stay linear.
16. ---
17. ---
18. ---
