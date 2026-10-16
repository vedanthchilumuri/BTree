#include <iostream>
#include <vector>

// classic in-memory B-Tree of minimum degree t (each node holds up to 2t-1 keys)
class BTreeNode {
public:
    std::vector<int> keys;
    std::vector<BTreeNode*> children;
    bool leaf;
    int t;

    BTreeNode(int t, bool leaf) : leaf(leaf), t(t) {}

    void traverse() {
        size_t i;
        for (i = 0; i < keys.size(); i++) {
            if (!leaf) children[i]->traverse();
            std::cout << keys[i] << " ";
        }
        if (!leaf) children[i]->traverse();
    }

    BTreeNode* search(int k) {
        size_t i = 0;
        while (i < keys.size() && k > keys[i]) i++;
        if (i < keys.size() && keys[i] == k) return this;
        if (leaf) return nullptr;
        return children[i]->search(k);
    }

    void splitChild(int i, BTreeNode* y) {
        BTreeNode* z = new BTreeNode(y->t, y->leaf);
        for (int j = 0; j < t - 1; j++)
            z->keys.push_back(y->keys[j + t]);
        if (!y->leaf) {
            for (int j = 0; j < t; j++)
                z->children.push_back(y->children[j + t]);
        }
        int midKey = y->keys[t - 1];
        y->keys.resize(t - 1);
        if (!y->leaf) y->children.resize(t);

        children.insert(children.begin() + i + 1, z);
        keys.insert(keys.begin() + i, midKey);
    }

    void insertNonFull(int k) {
        int i = (int)keys.size() - 1;
        if (leaf) {
            keys.push_back(0);
            while (i >= 0 && keys[i] > k) {
                keys[i + 1] = keys[i];
                i--;
            }
            keys[i + 1] = k;
        } else {
            while (i >= 0 && keys[i] > k) i--;
            i++;
            if ((int)children[i]->keys.size() == 2 * t - 1) {
                splitChild(i, children[i]);
                if (keys[i] < k) i++;
            }
            children[i]->insertNonFull(k);
        }
    }
};

class BTree {
public:
    BTree(int t) : root(nullptr), t(t) {}

    void insert(int k) {
        if (!root) {
            root = new BTreeNode(t, true);
            root->keys.push_back(k);
            return;
        }

        if ((int)root->keys.size() == 2 * t - 1) {
            BTreeNode* s = new BTreeNode(t, false);
            s->children.push_back(root);
            s->splitChild(0, root);
            int i = (s->keys[0] < k) ? 1 : 0;
            s->children[i]->insertNonFull(k);
            root = s;
        } else {
            root->insertNonFull(k);
        }
    }

    bool search(int k) {
        return root && root->search(k) != nullptr;
    }

    void traverse() {
        if (root) root->traverse();
        std::cout << "\n";
    }

private:
    BTreeNode* root;
    int t;
};

int main() {
    BTree tree(3); // min degree 3 -> each node holds up to 5 keys, like a small disk page

    int values[] = {10, 20, 5, 6, 12, 30, 7, 17, 3, 25, 15, 8, 1, 50, 45, 22, 19, 33};
    for (int v : values) tree.insert(v);

    std::cout << "in-order traversal (should be sorted): ";
    tree.traverse();

    int queries[] = {6, 15, 99, 45};
    for (int q : queries)
        std::cout << "search(" << q << ") = " << (tree.search(q) ? "FOUND" : "NOT FOUND") << "\n";

    return 0;
}
