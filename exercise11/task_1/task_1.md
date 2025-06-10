- What is the difference between dynamic and static linking?

| static | dynamic |
| --    | --    |
| compiles all used libraries into the resulting executable | supports the use of shared libraries. Meaning system libraries get linked to the program during runtime |
- - -
- When would you use dynamic linking?

Its usefull when you want small executable sizes. And is safe when using the executable on system which all have the same shared libraries available
- - -
- When would you use static linking?

You built a executable which should run on embedded systems or in contexts where you cannot guarantee on what system the binary is executed and if a dynamic linker is available. Then it's valid to bundle all used depencencies into the executable, maybe bloating it's size
- - -
- What is _position independent code_, and why is it useful?

Machine code that can be executed properly regardless where in memory its loaded. This is used by shared libraries because you cannot guarantee that the library is always loaded in the same place.
- - -
- What is the purpose of the `LD_LIBRARY_PATH` environment variable?

Env variable that tells the linker where to look for shared libraries to dynamicly link them to an executable. Its useful to be able to extend that variable to include own search locations for the linker. For example to test local versions of a shared library or you dont have the permission.