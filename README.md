# Contex

Build:

```
cmake -H. -Bbuild
make -C build

```

Run:
```
./build/contex -f "corpusdata/frankens.txt" -l "list.txt"
```

The list.txt has the following format
```
workindex,numberofwordsinquery
```

This workflow is subject to change. We would like to pass in words that we would like to query and how many words either
side. This will be implemented shortly.


The above input outputs 

```

WONDERLAND *** ALICE’S ADVENTURES IN WONDERLAND Lewis Carroll THE MILLENNIUM FULCRUM EDITION 3 .
 her sister on the bank, and of having nothing to do: once or twice she had peeped into the book her sister ... when suddenly a White Rabbit with pink eyes ran close by her .
 TOOK A WATCH OUT OF ITS WAISTCOAT-POCKET, and looked at it, and then hurried on, Alice started to her ... straight on like a tunnel for some way, and then dipped suddenly ... of the well, and noticed that they were filled with cupboards and ... else to do, so Alice soon began talking again .

```
