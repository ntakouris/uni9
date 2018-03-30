package me.zarkopafilis.ceid.algo;

public class RedBlackTree {

    protected static final boolean BLACK = false;
    protected static final boolean RED = true;

    protected Node root;

    public void insert(Integer e) {

    }

    class Node{
        int val;
        Node parent, left, right;
        boolean color;
        boolean isEmpty = true;

        public Node(int val) {
            this.val = val;
            isEmpty = false;
        }
    }
}
