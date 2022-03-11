# competitive

Personal archive of coding competition submissions.
Designed for C++11 in Visual Studio Code.

Each folder, outside of `bin`, `build`, and `obj`, is a *project* representing a competition, whose names are prefixed as follows.

Prefix|Platform
-|-
`abc`, `arc`, `agc`, `atcoder`|AtCoder
`cf`|Codeforces
`coci`|COCI
`fb`|Facebook
`usaco`|USACO

Select *projects* without an eligible prefix are likely custom problems.

We provide six launch configurations, whose names are composed of tokens with meaning as follows.

Token|Meaning
-|-
`debug`|Debug build with no optimization.
`release`|Release build with maximum optimizations `O3`, `Ofast`, etc.
`x64`|64-bit executable. Notably, `std::size_t` is 64 bits.
`piped`|Standard I/O are piped from/to `build/i.default.txt` and `build/o.default.txt`. On MacOS, due to limitations with VSCode and LLDB, these pipes are implemented in-code with `freopen`.
`interactive`|Does not pipe standard I/O from/to any files.

Notably, no external console should be launched in any configuration. Currently, this is broken for the piped Windows configurations.
