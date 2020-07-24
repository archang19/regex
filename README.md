# regex

Regular expression engine implementation in C++ that supports
concatenation, union, and closure operations. Constructs NFA from a
regular expression.

```cpp
nfa a;
string exp = "(c|d)*e";
vector<Node*> n = a.create_matcher(exp);
cout << a.search(n, "ce") << endl;
cout << a.search(n, "cde") << endl;
cout << a.search(n, "cccdddcde") << endl;
cout << a.search(n, "cdcdee") << endl;
```

