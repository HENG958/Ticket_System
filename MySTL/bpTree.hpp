#ifndef BPTREE_HPP
#define BPTREE_HPP
#include "MemoryRiver.hpp"
#include <vector>
#include <iostream>
#include <unordered_map>
using namespace std;
template<
   class Key,
   class Value,
   int M = 100, int L = 100,
   class Hash = std::hash<Key>,
   class Equal = std::equal_to<Key>
>
class Bptree {
private:
   int root, _size, normal_node_number;
   std::string prefix_name;
   class Bptree_leaf_node;
   class Bptree_normal_node;
   Bptree_normal_node root_node;
   MemoryRiver<Bptree_leaf_node, 0> leaf_node_manager;
   MemoryRiver<Bptree_normal_node, 3> normal_node_manager;
   int CacheLimit;
   std::unordered_map<Key, Value, Hash, Equal> Cache;
   void Cache_Limit() {
      CacheLimit = (1 << 18) / sizeof(std::pair<Key, Value>);
      CacheLimit = std::min(CacheLimit,20000);
   }
   bool CacheInsert(const Key &key_, Value &value_) {
      if (Cache.find(key_) != Cache.end())return false;
      if (Cache.size() >= CacheLimit)Cache.erase(Cache.begin());
      Cache.insert(std::make_pair(key_, value_));
      return true;
   }

   bool CacheRemove(const Key &key_) {
      if (Cache.find(key_) == Cache.end())return false;
      Cache.erase(Cache.find(key_));
      return true;
   }

   bool CacheFind(const Key &key_, Value &result) {
      if (Cache.find(key_) == Cache.end())return false;
      result = Cache.find(key_)->second;
      return true;
   }

   bool CacheModify(const Key &key_, const Value &rhs) {
      if (Cache.find(key_) == Cache.end())return false;
      Cache[key_] = rhs;
      return true;
   }
   void get_root() {
      normal_node_manager.get_info(root, 1);
      normal_node_manager.read(root_node, root);
   }
   void write_root() {
      normal_node_manager.write_info(root, 1);
      normal_node_manager.update(root_node, root);
   }
   void get_normal_node_number() {
      normal_node_manager.get_info(normal_node_number, 2);
   }
   void write_normal_node_number() {
      normal_node_manager.write_info(normal_node_number, 2);
   }
   void get_size() {
      normal_node_manager.get_info(_size, 3);
   }
   void write_size() {
      normal_node_manager.write_info(_size, 3);
   }
   void read_normal_node(Bptree_normal_node &node, int pos) {
      if (pos == root) node = root_node;
      else normal_node_manager.read(node, pos);
   }
   void modify_normal_node(Bptree_normal_node &node, int pos) {
      if (pos == root) root_node = node;
      else normal_node_manager.update(node, pos);
   }
   class Bptree_normal_node {
   public:
      friend Bptree;
   private:
      int size;
      bool is_lowest;
      int children[M + 1];
      Key key_min, key_list[M];

   public:
      Bptree_normal_node() {
         size = 0;
         is_lowest = 1;
         for (int i = 0; i <= M; ++i) children[i] = -1;
      }
      Bptree_normal_node(const Bptree_normal_node &obj) {
         size = obj.size;
         key_min = obj.key_min;
         is_lowest = obj.is_lowest;
         for (int i = 0; i <= M; ++i) {
            children[i] = obj.children[i];
         }
         for (int i = 0; i < M; ++i) {
            key_list[i] = obj.key_list[i];
         }
      }
      ~Bptree_normal_node() {}
   };

   class Bptree_leaf_node {
   public:
      friend Bptree;
   private:
      int size;
      int predecessor, succssor;
      Key key_list[L + 1];
      Value value_list[L + 1];

   public:
      Bptree_leaf_node() {
         size = 0;
         predecessor = succssor = -1;
      }
      Bptree_leaf_node(const Bptree_leaf_node &obj) {
         size = obj.size;
         predecessor = obj.predecessor;
         succssor = obj.succssor;
         for (int i = 0; i <= L; ++i) {
            key_list[i] = obj.key_list[i];
            value_list[i] = obj.value_list[i];
         }
      }
      ~Bptree_leaf_node() {}
   };
public:
   Bptree(const std::string &_prefix_name) : prefix_name(_prefix_name) {
      leaf_node_manager.initialise(prefix_name + "_leaf_node");
      normal_node_manager.initialise(prefix_name + "_normal_node");
      get_root();
      get_normal_node_number();
      get_size();
      if (normal_node_number == 0) { 
         root = normal_node_manager.write(root_node);
         ++normal_node_number;
      }
      Cache_Limit();

   }
   ~Bptree() {
      write_root();
      write_normal_node_number();
      write_size();
   }
   void clear() {
      root = normal_node_number = _size = 0;
      root_node = Bptree_normal_node();
      leaf_node_manager.clear();
      normal_node_manager.clear();
      Cache.clear();
   }
   int size() const {
      return _size;
   }
   bool find(const Key &key, Value &result) {
      if (CacheFind(key, result))return true;
      if (dfs_find(root, 0, key, result) != -1) {
         CacheInsert(key, result);
         return true;
      } else return false;
   }
   bool insert(const Key &key, const Value &value) {
      Bptree_normal_node virtual_root;
      virtual_root.size = 1;
      virtual_root.is_lowest = 0;
      virtual_root.children[0] = root;
      bool flag = dfs_insert(root, 0, 0, virtual_root, key, value);
      if (flag) ++_size;
      return flag;
   }
   bool insert(const std::pair<Key, Value> &data) {
      return insert(data.first, data.second);
   }
   bool erase(const Key &key) {
      Bptree_normal_node virtual_root;
      bool flag = dfs_erase(root, 0, 0, virtual_root, key);
      if (flag) --_size;
      if (flag)CacheRemove(key);

      return flag;
   }
   bool modify(const Key &key, const Value &value) {
      CacheModify(key,value);

      return dfs_modify(root, 0, key, value) != -1;
   }
   bool modify(const std::pair<Key, Value> &data) {
      return modify(data.first, data.second);
   }
   void range_search(const Key &key_l, const Key &key_r, std::vector<Value> &result) {
      result.clear();
      int index = lower_bound(key_l);
      bool flag = 0;
      while (index != -1) {
         Bptree_leaf_node leaf;
         leaf_node_manager.read(leaf, index);
         if (leaf.size == 0 || leaf.key_list[0] >= key_r) break;
         for (int i = 0; i < leaf.size; ++i) {
            if (flag || key_l <= leaf.key_list[i]) {
               flag = 1;
               if (leaf.key_list[i] < key_r) {
                  result.push_back(leaf.value_list[i]);
               } else {
                  break;
               }
            }
         }
         index = leaf.succssor;
      }
   }
   void range_search(const Key &key_l, const Key &key_r, std::vector<std::pair<Key, Value>> &result) {
      result.clear();
      int index = lower_bound(key_l);
      bool flag = 0;
      while (index != -1) {
         Bptree_leaf_node leaf;
         leaf_node_manager.read(leaf, index);
         if (leaf.size == 0 || leaf.key_list[0] >= key_r) break;
         for (int i = 0; i < leaf.size; ++i) {
            if (flag || key_l <= leaf.key_list[i]) {
               flag = 1;
               if (leaf.key_list[i] < key_r) {
                  result.push_back(std::make_pair(leaf.key_list[i], leaf.value_list[i]));
               } else {
                  break;
               }
            }
         }
         index = leaf.succssor;
      }
   }

private:
   void update(Bptree_normal_node &obj) {
      if (!obj.is_lowest) {
         Bptree_normal_node son;
         normal_node_manager.read(son, obj.children[0]);
         obj.key_min = son.key_min;
      } else {
         Bptree_leaf_node son;
         leaf_node_manager.read(son, obj.children[0]);
         obj.key_min = son.key_list[0];
      }
   }
   Bptree_normal_node merge(const Bptree_normal_node &node1, const Bptree_normal_node &node2) const {
      if (node1.size == 0) {
         return node2;
      } else if (node2.size == 0) {
         return node1;
      }
      Bptree_normal_node merge_node(node1);
      merge_node.size = node1.size + node2.size;
      for (int i = 0; i < node2.size; ++i) {
         merge_node.children[i + node1.size] = node2.children[i];
      }
      merge_node.key_list[node1.size - 1] = node2.key_min;
      for (int i = 0; i + 1 < node2.size; ++i) {
         merge_node.key_list[i + node1.size] = node2.key_list[i];
      }
      return merge_node;
   }
   Bptree_leaf_node merge(const Bptree_leaf_node &node1, const Bptree_leaf_node &node2) const {
      if (node1.size == 0) {
         return node2;
      } else if (node2.size == 0) {
         return node1;
      }
      Bptree_leaf_node merge_node(node1);
      merge_node.size = node1.size + node2.size;
      merge_node.predecessor = node1.predecessor;
      merge_node.succssor = node2.succssor;
      for (int i = 0; i < node2.size; ++i) {
         merge_node.key_list[i + node1.size] = node2.key_list[i];
         merge_node.value_list[i + node1.size] = node2.value_list[i];
      }
      return merge_node;
   }
   std::pair<Bptree_normal_node, Bptree_normal_node> split(const Bptree_normal_node &obj,
                                                           const int &index,
                                                           int &new_index) {
      Bptree_normal_node x, y;
      new_index = normal_node_manager.write(y);
      x.size = (obj.size + 1) / 2;
      y.size = obj.size / 2;
      x.is_lowest = y.is_lowest = obj.is_lowest;
      for (int i = 0; i < x.size; ++i) {
         x.children[i] = obj.children[i];
      }
      for (int i = 0; i < y.size; ++i) {
         y.children[i] = obj.children[x.size + i];
      }
      for (int i = 0; i + 1 < x.size; ++i) {
         x.key_list[i] = obj.key_list[i];
      }
      for (int i = 0; i + 1 < y.size; ++i) {
         y.key_list[i] = obj.key_list[x.size + i];
      }
      x.key_min = obj.key_min;
      update(y);
      return std::make_pair(x, y);
   }
   std::pair<Bptree_leaf_node, Bptree_leaf_node> split(const Bptree_leaf_node &obj, const int &index, int &new_index) {
      Bptree_leaf_node x, y;
      new_index = leaf_node_manager.write(y);
      x.size = (obj.size + 1) / 2;
      y.size = obj.size / 2;
      x.predecessor = obj.predecessor;
      x.succssor = new_index;
      y.predecessor = index;
      y.succssor = obj.succssor;
      for (int i = 0; i < x.size; ++i) {
         x.key_list[i] = obj.key_list[i];
         x.value_list[i] = obj.value_list[i];
      }
      for (int i = 0; i < y.size; ++i) {
         y.key_list[i] = obj.key_list[i + x.size];
         y.value_list[i] = obj.value_list[i + x.size];
      }
      return std::make_pair(x, y);
   }
   int dfs_find(int pos, bool is_leaf, const Key &key, Value &result) {
      if (is_leaf) {
         Bptree_leaf_node node;
         leaf_node_manager.read(node, pos);
         for (int i = 0; i < node.size && node.key_list[i] <= key; ++i) {
            if (node.key_list[i] == key) {
               result = node.value_list[i];
               return pos;
            }
         }
         return -1;
      } else {
         Bptree_normal_node node;
         read_normal_node(node, pos);
         if (node.size == 0) return -1; 
         for (int i = 0; i < node.size; ++i) {
            if (i + 1 == node.size || key < node.key_list[i]) {
               return dfs_find(node.children[i], node.is_lowest, key, result);
            }
         }
         return 0;
      }
   }
   int dfs_modify(int pos, bool is_leaf, const Key &key, const Value &value) {
      if (is_leaf) {
         Bptree_leaf_node node;
         leaf_node_manager.read(node, pos);
         for (int i = 0; i < node.size && node.key_list[i] <= key; ++i) {
            if (node.key_list[i] == key) {
               node.value_list[i] = value;
               leaf_node_manager.update(node, pos);
               return pos;
            }
         }
         return -1;
      } else {
         Bptree_normal_node node;
         read_normal_node(node, pos);
         if (node.size == 0) return -1;
         for (int i = 0; i < node.size; ++i) {
            if (i + 1 == node.size || key < node.key_list[i]) {
               return dfs_modify(node.children[i], node.is_lowest, key, value);
            }
         }
         return -1;
      }
   }
   bool dfs_insert(int pos, bool is_leaf, int rank, Bptree_normal_node &parent, const Key &key, const Value &value) {
      if (is_leaf) {
         Bptree_leaf_node self;
         leaf_node_manager.read(self, pos);
         for (int i = 0; i <= self.size; ++i) {
            if (i != self.size && self.key_list[i] < key) continue;
            if (i != self.size && self.key_list[i] == key) return 0;
            for (int j = self.size; j > i; --j) {
               self.key_list[j] = self.key_list[j - 1];
               self.value_list[j] = self.value_list[j - 1];
            }
            self.key_list[i] = key;
            self.value_list[i] = value;
            ++self.size;
            break;
         }
         if (rank == 0) {
            parent.key_min = self.key_list[0];
         }
         if (self.size <= L) {
            leaf_node_manager.update(self, pos);
         } else {
            int new_index;
            std::pair<Bptree_leaf_node, Bptree_leaf_node> tmp = split(self, pos, new_index);
            leaf_node_manager.update(tmp.first, pos);
            leaf_node_manager.update(tmp.second, new_index);
            for (int i = parent.size; i >= rank + 1; --i) {
               if (i != rank + 1) {
                  parent.children[i] = parent.children[i - 1];
               }
               if (i != parent.size) {
                  parent.key_list[i] = parent.key_list[i - 1];
               }
            }
            parent.children[rank + 1] = new_index;
            parent.key_list[rank] = tmp.second.key_list[0];
            ++parent.size;
         }
         return 1;
      } else {
         Bptree_normal_node self;
         read_normal_node(self, pos);
         if (pos == root) parent.key_min = self.key_min;
         if (self.size == 0) {
            Bptree_leaf_node tmp;
            tmp.size = 1;
            tmp.key_list[0] = key;
            tmp.value_list[0] = value;
            self.children[0] = leaf_node_manager.write(tmp);
            self.size = 1;
            self.key_min = key;
            modify_normal_node(self, pos);
            return 1;
         }
         for (int i = 0; i < self.size; ++i) {
            if (i + 1 == self.size || key < self.key_list[i]) {
               if (!dfs_insert(self.children[i], self.is_lowest, i, self, key, value)) return 0;
               break;
            }
         }
         if (rank == 0) {
            parent.key_min = self.key_min;
         }
         if (self.size <= M) {
            modify_normal_node(self, pos);
         } else {
            int new_index;
            std::pair<Bptree_normal_node, Bptree_normal_node> tmp = split(self, pos, new_index);
            modify_normal_node(tmp.first, pos);
            modify_normal_node(tmp.second, new_index);
            for (int i = parent.size; i >= rank + 1; --i) {
               if (i != rank + 1) {
                  parent.children[i] = parent.children[i - 1];
               }
               if (i != parent.size) {
                  parent.key_list[i] = parent.key_list[i - 1];
               }
            }
            parent.children[rank + 1] = new_index;
            parent.key_list[rank] = tmp.second.key_min;
            ++parent.size;
            if (pos == root) {
               normal_node_manager.update(root_node, root);
               root = normal_node_manager.write(parent);
               root_node = parent;
            }
         }
         return 1;
      }
   }
   bool dfs_erase(int pos, bool is_leaf, int rank, Bptree_normal_node &parent, const Key &key) {
      if (is_leaf) {
         Bptree_leaf_node self;
         leaf_node_manager.read(self, pos);
         bool flag = 0;
         for (int i = 0; i < self.size; ++i) {
            if (self.key_list[i] == key) {
               flag = 1;
               for (int j = i; j + 1 < self.size; ++j) {
                  self.key_list[j] = self.key_list[j + 1];
                  self.value_list[j] = self.value_list[j + 1];
               }
               --self.size;
               break;
            }
         }
         if (flag) {
            if (self.size < (L + 1) / 2) {
               if (self.predecessor == -1 && self.succssor == -1) {
                  if (self.size == 0) {
                     parent.size = 0;
                     parent.children[0] = -1;
                  }
                  leaf_node_manager.update(self, pos);
               } else {
                  Bptree_leaf_node brother;
                  if (rank + 1 != parent.size) { 
                     leaf_node_manager.read(brother, parent.children[rank + 1]);
                     IfSize_leaf(rank, self, brother, parent);
                  } else {
                     leaf_node_manager.read(brother, parent.children[rank - 1]);
                     IfSize_leaf(rank - 1, brother, self, parent);
                  }
               }
            } else {
               leaf_node_manager.update(self, pos);
            }
         }
         return flag;
      } else {
         Bptree_normal_node self;
         read_normal_node(self, pos);
         if (self.size == 0) return 0;
         bool found = 0;
         for (int i = 0; i < self.size; ++i) {
            if (i + 1 == self.size || key < self.key_list[i]) {
               found = dfs_erase(self.children[i], self.is_lowest, i, self, key);
               break;
            }
         }
         if (found) {
            if (pos == root && !self.is_lowest && self.size == 1) {
               root = self.children[0];
               normal_node_manager.Delete(pos);
               normal_node_manager.read(root_node, root);
               --normal_node_number;
            } else if (self.size >= (M + 1) / 2 || pos == root) {
               modify_normal_node(self, pos);
            } else {
               Bptree_normal_node brother;
               if (rank + 1 != parent.size) {
                  normal_node_manager.read(brother, parent.children[rank + 1]);
                  IfSize_normal(rank, self, brother, parent);
               } else {
                  normal_node_manager.read(brother, parent.children[rank - 1]);
                  IfSize_normal(rank - 1, brother, self, parent);
               }
            }
         }
         return found;
      }
   }
   bool IfSize_leaf(int rank,
                                    Bptree_leaf_node &node1,
                                    Bptree_leaf_node &node2,
                                    Bptree_normal_node &parent) {
      int id1 = parent.children[rank], id2 = parent.children[rank + 1];
      if (node1.size == (L + 1) / 2 || node2.size == (L + 1) / 2) {
         node1 = merge(node1, node2);
         leaf_node_manager.update(node1, id1);
         leaf_node_manager.Delete(id2);
         --parent.size;
         for (int i = rank + 1; i < parent.size; ++i) {
            parent.children[i] = parent.children[i + 1];
         }
         for (int i = rank; i + 1 < parent.size; ++i) {
            parent.key_list[i] = parent.key_list[i + 1];
         }
         if (rank == 0) parent.key_min = node1.key_list[0];
         return 1;
      } else if (node1.size < (L + 1) / 2) {
         node1.key_list[node1.size] = node2.key_list[0];
         node1.value_list[node1.size] = node2.value_list[0];
         ++node1.size;
         --node2.size;
         for (int i = 0; i < node2.size; ++i) {
            node2.key_list[i] = node2.key_list[i + 1];
            node2.value_list[i] = node2.value_list[i + 1];
         }
         parent.key_list[rank] = node2.key_list[0];
         if (rank == 0) parent.key_min = node1.key_list[0];
         leaf_node_manager.update(node1, id1);
         leaf_node_manager.update(node2, id2);
         return 0;
      } else {
         for (int i = node2.size; i > 0; --i) {
            node2.key_list[i] = node2.key_list[i - 1];
            node2.value_list[i] = node2.value_list[i - 1];
         }
         --node1.size;
         ++node2.size;
         node2.key_list[0] = node1.key_list[node1.size];
         node2.value_list[0] = node1.value_list[node1.size];
         parent.key_list[rank] = node2.key_list[0];
         if (rank == 0) parent.key_min = node1.key_list[0];
         leaf_node_manager.update(node1, id1);
         leaf_node_manager.update(node2, id2);
         return 0;
      }
   }
   bool IfSize_normal(int rank,
                                      Bptree_normal_node &node1,
                                      Bptree_normal_node &node2,
                                      Bptree_normal_node &parent) {
      int id1 = parent.children[rank], id2 = parent.children[rank + 1];
      if (node1.size == (M + 1) / 2 || node2.size == (M + 1) / 2) {
         node1 = merge(node1, node2);
         modify_normal_node(node1, id1);
         normal_node_manager.Delete(id2);
         --parent.size;
         for (int i = rank + 1; i < parent.size; ++i) {
            parent.children[i] = parent.children[i + 1];
         }
         for (int i = rank; i + 1 < parent.size; ++i) {
            parent.key_list[i] = parent.key_list[i + 1];
         }
         return 1;
      } else if (node1.size < (M + 1) / 2) {
         node1.children[node1.size] = node2.children[0];
         node1.key_list[node1.size - 1] = node2.key_min;
         node2.key_min = node2.key_list[0];
         ++node1.size;
         --node2.size;
         for (int i = 0; i < node2.size; ++i) {
            node2.children[i] = node2.children[i + 1];
            if (i + 1 != node2.size) {
               node2.key_list[i] = node2.key_list[i + 1];
            }
         }
         parent.key_list[rank] = node2.key_min;
         modify_normal_node(node1, id1);
         modify_normal_node(node2, id2);
         return 0;
      } else {
         for (int i = node2.size; i > 0; --i) {
            node2.children[i] = node2.children[i - 1];
            if (i != node2.size) {
               node2.key_list[i] = node2.key_list[i - 1];
            }
         }
         --node1.size;
         ++node2.size;
         node2.children[0] = node1.children[node1.size];
         node2.key_list[0] = node2.key_min;
         node2.key_min = node1.key_list[node1.size - 1];
         parent.key_list[rank] = node2.key_min;
         modify_normal_node(node1, id1);
         modify_normal_node(node2, id2);
         return 0;
      }
   }
   int lower_bound(const Key &key) {
      return dfs_lower_bound(root, key);
   }
   int dfs_lower_bound(int pos, const Key &key) {
      Bptree_normal_node self;
      read_normal_node(self, pos);
      if (self.size == 0) return -1;
      for (int i = self.size - 1; i >= 0; --i) {
         if (i == 0 || key >= self.key_list[i - 1]) {
            if (self.is_lowest) {
               return self.children[i];
            } else {
               return dfs_lower_bound(self.children[i], key);
            }
            break;
         }
      }
      return -1;
   }
};
#endif
