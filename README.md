# togTA

# Automata-Powered Syntax Highlighter

This project is a C++ tool that uses **finite automata and regular expressions** to process and analyze text/code files. It detects patterns, highlights them in an HTML output, and performs light static analysis such as bracket and semicolon validation.

---

## 🚀 Features

- ✅ Reads input files and tokenizes by lines and words
- ✅ Loads multiple group definitions (regex + style) from configuration files
- ✅ Converts regex → ENFA → DFA → minimized DFA
- ✅ Highlights matching words using group-specific styles
- ✅ Detects:
    - Unbalanced brackets
    - Missing semicolons (non-control lines)
- ✅ Suggests corrections for mistyped tokens (e.g. `std::vect` → `std::vector`)
- ✅ Generates HTML output with styled and plain text
- ✅ Logs info/errors for debugging
