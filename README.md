# encodeuP
Attribute encoding (userParameters) in "Active Directory"

This code is based on research done by [@Christian Herzog](https://github.com/daduke) and [@Elmar Heeb](https://github.com/elmar).

## Usage examples
```
$ echo "some string" | ./encodeuP
e38cb7e698b6e690b6e394b6e380b2e38cb7e390b7e388b7e3a4b6e694b6e39cb6e684b0

$ echo "e38cb7e698b6e690b6e394b6e380b2e38cb7e390b7e388b7e3a4b6e694b6e39cb6e684b0" | ./decodeuP
some string
```
