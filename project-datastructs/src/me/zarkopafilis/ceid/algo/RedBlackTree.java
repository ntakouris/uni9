package me.zarkopafilis.ceid.algo;


import static me.zarkopafilis.ceid.algo.SearchAlgo.NOT_FOUND;

/* As of http://software.ucv.ro/~mburicea/lab8ASD.pdf*/
public class RedBlackTree{

    protected static final boolean BLACK = false;
    protected static final boolean RED = true;

    protected Node root = null;

    public void insert(int n) {
        Node z = new Node(n);
        Node y = null;
        Node x = root;

        while (x != null) {
            y = x;
            if(z.val < x.val){
                x = x.left;
            }else{
                x = x.right;
            }
        }

        z.parent = y;
        if (y == null) {
            root = z;
        }else if(z.val < y.val){
            y.left = z;
        }else{
            y.right = z;
        }

        z.color = RED;
        insertFixup(z);
    }

    public int search(int x) {
        Node s = root;
        int hops = -1;
        while (s != null) {
            hops++;
            if(x < s.val){
                s = s.left;
            }else if(x > s.val){
                s = s.right;
            } else if (x == s.val) {
                return hops;
            }
        }
        return NOT_FOUND;
    }

    private void insertFixup(Node z) {
        while (z.parent != null && z.parent.color == RED) {
            if (z.parent == z.parent.parent.left) {
                Node y = z.parent.parent.right;
                if (y.color == RED) {
                    z.parent.color = BLACK;
                    y.color = BLACK;
                    z.parent.parent.color = RED;
                    z = z.parent.parent;
                } else if (z == z.parent.right) {
                    z = z.parent;
                    leftRotate(z);
                    z.parent.color = BLACK;
                    z.parent.parent.color = RED;
                    rightRotate(z);
                }else{
                    z = z.parent;
                    rightRotate(z);
                    z.parent.color = BLACK;
                    z.parent.parent.color = RED;
                    leftRotate(z);
                }
            }
        }

        root.color = BLACK;
    }

    private void leftRotate(Node x){
        Node y = x.right;
        x.right = y.left;
        y.left.parent = x;
        y.parent = x.parent;

        if (x.parent == null) {
            root = y;
        } else if (x == x.parent.left) {
            x.parent.right = y;
        }else{
            x.parent.left = y;
        }

        y.left = x;
        x.parent = y;
    }

    private void rightRotate(Node x){
        Node y = x.left;
        x.left = y.right;
        y.right.parent = x;
        y.parent = x.parent;
        if (x.parent == null) {
            root = y;
        } else if (x == x.parent.right) {
            x.parent.right = y;
        }else{
            x.parent.left = y;
        }
    }

    class Node{
        int val;
        Node parent, left, right;
        boolean color;

        public Node() {
        }

        public Node(int val) {
            this.val = val;
        }
    }
}
