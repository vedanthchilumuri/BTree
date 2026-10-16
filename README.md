# B-Tree Index

An in-memory B-Tree in C++ supporting insertion, search, and sorted traversal.

## Features

- Configurable minimum degree `t`
- O(log n) search and insert
- Proactive node splitting during insertion
- In-order traversal that returns keys in sorted order

## How it works

A B-Tree is a balanced search tree where each node holds many keys instead of one, which keeps the tree shallow. This matters for databases, where each node corresponds to one disk read.

With minimum degree `t`:

- Every node holds at most `2t - 1` keys, and every non-root node holds at least `t - 1`.
- A node with `k` keys has `k + 1` children (unless it is a leaf).
- Keys within a node are sorted, and each child covers the range between two neighboring keys.

**Search**: scan the node's keys to find where the target falls. Either it matches a key, or you descend into the corresponding child. Repeat until you find it or reach a leaf.

**Insert**: this implementation splits full nodes on the way down, so it never has to backtrack upward. If a node on the path is full, its middle key moves up into the parent and its remaining keys are divided into two nodes. If the root is full, it splits and a new root is created, which is the only way the tree grows taller. This is why all leaves are always at the same depth.

The demo uses `t = 3`, so each node holds up to 5 keys.

## Build and run

```bash
g++ -std=c++17 -O2 btree_storage_engine.cpp -o btree
./btree
```

## Sample output

```
in-order traversal (should be sorted): 1 3 5 6 7 8 10 12 15 17 19 20 22 25 30 33 45 50 
search(6) = FOUND
search(15) = FOUND
search(99) = NOT FOUND
search(45) = FOUND
```

## Complexity

| Operation | Time |
|-----------|------|
| Search | O(t log_t n) |
| Insert | O(t log_t n) |
| Traversal | O(n) |

For a fixed `t`, this is O(log n). A larger `t` makes the tree shallower.

## Limitations

- **In-memory only.** Nodes live on the heap, not in disk pages.
- No deletion.
- Duplicate keys are not checked and will be inserted again.
- Nodes are never freed, so the tree leaks memory when the program exits.
- Keys are plain `int` with no associated values.

## Future work

- Serialize nodes into fixed-size 4KB pages and add a page manager for reading and writing a file
- Implement deletion with merging and redistribution
- Store key-value pairs and support range queries
- Add a destructor and template the key type
