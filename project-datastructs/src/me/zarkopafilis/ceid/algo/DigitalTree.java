package me.zarkopafilis.ceid.algo;

import java.util.HashMap;

public class DigitalTree {

    TrieNode root;

    public DigitalTree() {
        root = new TrieNode(null, new HashMap<>());
    }

    public void insert(String str) {
        int i = 0;
        TrieNode curr = root;

        while (i < str.length()) {
            if (curr.children.containsKey(str.substring(i, i + 1))) {
                curr = curr.children.get(str.substring(i, i + 1));
                i++;
            } else {
                break;
            }
        }

        while (i < str.length()) {
            curr.children.put(str.substring(i, i + 1),
                    new TrieNode(null, new HashMap<>()));
            curr = curr.children.get(str.substring(i, i + 1));
            i++;
        }

        curr.data = str;
    }

    public boolean search(String str) {
        int i = 0;
        TrieNode curr = root;

        while (i < str.length()) {
            if (!curr.children.containsKey(str.substring(i, i + 1))) {
                return false;
            }
            curr = curr.children.get(str.substring(i, i + 1));
            i++;
        }

        return curr.data != null && curr.data.equals(str);
    }

    public void delete(String str) {
        int len = str.length();
        if (len == 0) {
            return;
        }

        deleteHelper(root, str, 0);
    }

    private boolean deleteHelper(TrieNode node, String str, int level) {
        if (node != null) {
            int len = str.length();

            if (level == len) {
                node.data = null;
                if (node.children != null) {
                    return node.children.isEmpty();
                }
            } else {
                String k = str.substring(level, level + 1);
                if (deleteHelper(node.children.get(k), str, level + 1)) {
                    node.children.remove(k);

                    return (node.data == null && node.children.isEmpty());
                }

            }
        }

        return false;
    }

    private class TrieNode {
        String data;
        HashMap<String, TrieNode> children;

        TrieNode(String data, HashMap<String, TrieNode> children) {
            this.data = data;
            this.children = children;
        }
    }
}
