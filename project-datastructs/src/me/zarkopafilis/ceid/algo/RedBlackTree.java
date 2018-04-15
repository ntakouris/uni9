package me.zarkopafilis.ceid.algo;


import static me.zarkopafilis.ceid.algo.SearchAlgo.NOT_FOUND;

public class RedBlackTree {

    protected static final boolean BLACK = false;
    protected static final boolean RED = true;

    protected Node root = null;

    public void insert(int n) {
        Node z = new Node(n);

        insertHelper(root, z);
        insertRepair(z);

        root = z;
        while (root.parent != null) {
            root = root.parent;
        }
    }

    private void insertHelper(Node root, Node n) {
        if (root != null && n.val < root.val) {
            if (root.left != null) {
                insertHelper(root.left, n);
                return;
            } else {
                root.left = n;
            }
        } else if (root != null) {
            if (root.right != null) {
                insertHelper(root.right, n);
                return;
            } else {
                root.right = n;
            }
        }

        n.parent = root;
        n.left = null;
        n.right = null;
        n.color = RED;
    }

    private void insertRepair(Node n) {
        if (n.parent == null) {
            n.color = BLACK;
        } else if (n.parent.color == BLACK) {
            return;
        } else if (uncle(n) != null && uncle(n).color == RED) {
            n.parent.color = BLACK;
            uncle(n).color = BLACK;
            grandparent(n).color = RED;
            insertRepair(grandparent(n)); // ?: n.parent
        } else {
            Node p = n.parent;
            Node g = grandparent(n);

            if (g.left != null && n == g.left.right) {
                leftRotate(p);
                n = n.left;
            } else if (g.right != null && n == g.right.left) {
                rightRotate(p);
                n = n.right;
            }
            /////////////////
            p = n.parent;
            g = grandparent(n);

            if (n == p.left) {
                rightRotate(g);
            }else{
                leftRotate(g);
            }

            p.color = BLACK;
            g.color = RED;
        }
    }

    private Node uncle(Node n) {
        Node p = n.parent;
        if (p == null) {
            return null;
        }

        Node g = grandparent(n);
        if (g == null) {
            return null;
        }

        return sibling(p);
    }

    private Node grandparent(Node n) {
        Node p = n.parent;
        if (n.parent == null) {
            return null;
        }

        return p.parent;
    }

    private Node sibling(Node n) {
        Node p = n.parent;
        if (p == null) {
            return null;
        }
        if (n == p.left) {
            return p.right;
        }

        return p.left;
    }

    private void leftRotate(Node x) {
        Node y = x.right;
        if (y == null) {
            return;
        }

        x.right = y.left;
        y.left = x;
        y.parent = x.parent;
        x.parent = y;
    }

    private void rightRotate(Node x) {
        Node y = x.left;
        if (y == null) {
            return;
        }

        x.left = y.right;
        y.right = x;
        y.parent = x.parent;
        x.parent = y;
    }

    public int search(int x) {
        Node s = root;
        int hops = -1;
        while (s != null) {
            hops++;
            if (x < s.val) {
                s = s.left;
            } else if (x > s.val) {
                s = s.right;
            } else if (x == s.val) {
                return hops;
            }
        }
        return NOT_FOUND;
    }

    class Node {
        int val;
        Node parent, left, right;
        boolean color;

        public Node() {
        }

        public Node(int val) {
            this.val = val;
        }

        @Override
        public String toString() {
            return "Node{" +
                    "val=" + val +
                    ", color=" + (color == BLACK ? "BLACK" : "RED") +
                    '}';
        }
    }
}
