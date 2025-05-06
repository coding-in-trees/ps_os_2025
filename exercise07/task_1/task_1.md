The steps to unlock the secret are the following:
1. Binary tries to open ./hint.txt but doesnt find it
2. Binary tries to open ./key.txt but doesnt find it
3. Binary found both files but the contents of the file ./key.txt do not contain a key

1. touch ./hint.txt
2. touch ./key.txt
3. secret binary placed key in hint.txt
4. copy key to key.txt
5. rerun secret

Obtained secret:
19963e9e