#ifndef __trie_h__
#define __trie_h__

#include <string>
#include <map>
#include <stack>
#include <vector>
#include <utility>

template <class stringType>
class trie {
    friend class iterator;
    
public:
    trie() {
        tree.resize(2);
        tag.resize(2);
        nodeCount = 1;
        wordCount = 0;
    }
    
    
    class iterator {
        friend class trie;
        friend bool operator == (const iterator& a, const iterator& b) {
            return a.pointer == b.pointer && a.wordTag == b.wordTag;
        }

       
        friend bool operator != (const iterator& a, const iterator& b) {
            return a.pointer != b.pointer || a.wordTag != b.wordTag;
        }
        
        friend stringType operator * (const iterator& a) {
            return a.word;
        }
        
        friend void operator ++ (iterator& a, int _) {
            trie *T = a.pointer;
            if (a == T->end()) return;
            int nowID = a.route.top().second->second;
            
            do {
                if (T->tree[nowID].size()) {
                    a.route.push(make_pair(nowID, T->tree[nowID].begin()));
                    a.word += T->tree[nowID].begin()->first;
                    
                    nowID = T->tree[nowID].begin()->second;
                } else {
                    do {
                        typename std::map<typename stringType::value_type, int>::iterator ii = a.route.top().second;
                        nowID = a.route.top().first;
                        a.word.resize(a.word.size() - 1);
                        a.route.pop();
                        if (++ii != T->tree[nowID].end()) {
                            a.route.push(std::make_pair(nowID, ii));
                            a.word += ii->first;
                            nowID = ii->second;
                            break;
                        }
                    } while (a.route.size());
                }
            } while (a.route.size() && !T->tag[nowID]);
            if (!a.route.size()) {
                a.clear();
                a.pointer = T;
            } else
                a.wordTag = T->tag[nowID];
        }

    
    public:
        iterator() {
            clear();
        }
        int getTag() {
            return wordTag;
        }
        
    private:
        std::stack< std::pair< int, typename std::map< typename stringType::value_type, int >::iterator > > route;
        stringType word;
        trie* pointer;
        int wordTag;
        void clear() {
            pointer = NULL;
            wordTag = 0;
            while (!route.empty()) route.pop();
        }

    };
    
    friend void operator ++ (iterator&, int);
    
    
    iterator begin() {
        if (beginIterator.pointer == NULL) {
            beginIterator.clear();
            beginIterator.pointer = this;
            if (this->wordCount) {
                int i = root;
                while (!tag[i]) {
                    beginIterator.route.push(std::make_pair(i, tree[i].begin()));
                    beginIterator.word += tree[i].begin()->first;
                    i = tree[i].begin()->second;
                }
                beginIterator.wordTag = tag[i];
            }
        }
        return beginIterator;
    }
    
    iterator end() {
        if (endIterator.pointer == NULL) {
            endIterator.clear();
            endIterator.pointer = this;
        }
        return endIterator;
    }

    
    int insert(const stringType &word) {
        beginIterator.clear();
        endIterator.clear();
        if (word.size())
            return insert(root, word, 0);
        else
            return 0;
    }
    
    int find(const stringType &word) {
        return find(root, word, 0);
    }

    int countWord() {
        return wordCount;
    }
    
    int maxMatching(const stringType &phrase) {
        return maxMatching(root, phrase, 0);
    }
private:
    std::vector< std::map< typename stringType::value_type, int > > tree;
    std::vector< int > tag;
    int nodeCount;
    int wordCount;
    static const int root = 1;
    iterator beginIterator, endIterator;
    
    int insert(int id, const stringType &word, int depth) {
        if (depth == word.size()) {
            if (!tag[id]) tag[id] = ++wordCount;
            return tag[id];
        } else {
            if (tree[id].find(word[depth]) == tree[id].end()) {
                tree[id][ word[depth] ] = ++nodeCount;
                tree.push_back(std::map< typename stringType::value_type, int >());
                tag.push_back(0);
            }
            return insert(tree[id][ word[depth] ], word, depth + 1);
        }
    }
    
    int find(int id, const stringType &word, int depth) {
        if (depth == word.size())
            return tag[id];
        if (tree[id].find(word[depth]) != tree[id].end())
            return find(tree[id][ word[depth] ], word, depth + 1);
        return 0;
        
    }
    
    int maxMatching(int id, const stringType &phrase, int depth) {
        if (depth == phrase.size())
            return tag[id] ? depth : 0;
        if (tree[id].find(phrase[depth]) != tree[id].end())
            return std::max(tag[id] ? depth : 0,
                            maxMatching(tree[id][ phrase[depth] ], phrase, depth + 1));
        return tag[id] ? depth : 0;
    }
};


#endif
