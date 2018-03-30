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
            if (curr.children.get(str.substring(i, i + 1)) != null) {
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
            if (curr.children.get(str.substring(i, i + 1)) == null) {
                return false;
            }
            curr = curr.children.get(str.substring(i, i + 1));
            i++;
        }

        return curr.data != null && curr.data.equals(str);
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
