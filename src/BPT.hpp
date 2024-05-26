#include <iostream>
#include <cstdio>
#include <cstring>
#include <fstream>
#include "Pair.hpp"
#include "List.hpp"
#include "StorageSearch.hpp"

namespace BPlusTree 
{
template<typename T>
class vector 
{
public:
	size_t now_len, extended_len;
	T *vec_begin, *vec_end;
	class const_iterator;
	class iterator 
	{
	friend class vector<T>;
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;

	private:
		vector <T> *vec;
		pointer vec_ptr;
		iterator (vector <T> *vec, pointer vec_ptr): vec(vec), vec_ptr(vec_ptr){}
	public:
		iterator operator+(const int &n) const 
		{
			iterator temp = *this;
			temp.vec_ptr += n;
			return temp;
		}
		iterator operator-(const int &n) const 
		{
			iterator temp = *this;
			temp.vec_ptr -= n;
			return temp;
		}
		int operator-(const iterator &rhs) const 
		{
			return vec_ptr - rhs.vec_ptr;
		}
		iterator& operator+=(const int &n) 
		{
			vec_ptr += n;
			return *this;
		}
		iterator& operator-=(const int &n) 
		{
			vec_ptr -= n;
			return *this;
		}
		iterator operator++(int) 
		{
			iterator temp = *this;
			vec_ptr++;
			return temp;
		}
		iterator& operator++() 
		{
			vec_ptr++;
			return *this;
		}
		iterator operator--(int) 
		{
			iterator temp = *this;
			vec_ptr--;
			return temp;
		}
		iterator& operator--() 
		{
			vec_ptr--;
			return *this;
		}
		T& operator*() const
		{
			return *vec_ptr;
		}
		bool operator==(const iterator &rhs) const 
		{
			return vec_ptr == rhs.vec_ptr;
		}
		bool operator==(const const_iterator &rhs) const 
		{
			return vec_ptr == rhs.vec_ptr;
		}
		bool operator!=(const iterator &rhs) const 
		{
			return vec_ptr != rhs.vec_ptr;
		}
		bool operator!=(const const_iterator &rhs) const 
		{
			return vec_ptr != rhs.vec_ptr;
		}
	};
	class const_iterator 
	{

		friend class vector<T>;
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;
	
	private:
		const vector <T> *vec;
		pointer vec_ptr;
		const_iterator (const vector <T> *vec,pointer vec_ptr): vec(vec), vec_ptr(vec_ptr){}

	public:
	  const_iterator operator+(const int &n) const 
		{
			const_iterator temp = *this;
			temp.vec_ptr += n;
			return temp;
		}
		const_iterator operator-(const int &n) const 
		{
			const_iterator temp = *this;
			temp.vec_ptr -= n;
			return temp;
		}
		int operator-(const const_iterator &rhs) const 
		{
			return vec_ptr - rhs.vec_ptr;
		}
		const_iterator& operator+=(const int &n) 
		{
			vec_ptr += n;
			return *this;
		}
		const_iterator& operator-=(const int &n) 
		{
			vec_ptr -= n;
			return *this;
		}
		const_iterator operator++(int) 
		{
			const_iterator temp = *this;
			vec_ptr++;
			return temp;
		}
		const_iterator& operator++() 
		{
			vec_ptr++;
			return *this;
		}
		const_iterator operator--(int) 
		{
			const_iterator temp = *this;
			vec_ptr--;
			return temp;
		}
		const_iterator& operator--() 
		{
			vec_ptr--;
			return *this;
		}
		T& operator*() const
		{
			return *vec_ptr;
		}
		bool operator==(const iterator &rhs) const 
		{
			return vec_ptr == rhs.vec_ptr;
		}
		bool operator==(const const_iterator &rhs) const 
		{
			return vec_ptr == rhs.vec_ptr;
		}
		bool operator!=(const iterator &rhs) const 
		{
			return vec_ptr != rhs.vec_ptr;
		}
		bool operator!=(const const_iterator &rhs) const 
		{
			return vec_ptr != rhs.vec_ptr;
		}
	};
	
	vector() 
	{
		vec_begin = std::allocator<T>().allocate(1);
		vec_end = vec_begin;
		extended_len = 1;
		now_len = 0;
	}
	vector(const vector &other) 
	{
		vec_begin = std::allocator<T>().allocate(other.extended_len);
		vec_end = vec_begin + other.now_len;
		extended_len = other.extended_len;
		now_len = other.now_len;
		for (size_t i = 0; i < now_len; i++)
		{
			std::construct_at(vec_begin + i, other.vec_begin[i]);
		}
	}
	~vector() 
	{
		if (vec_begin != nullptr)
		{
			for (size_t i = 0; i < now_len; i++)
			{
				std::destroy_at(vec_begin + i);
			}
			std::allocator<T>().deallocate(vec_begin, extended_len);
		}
	}
	vector &operator=(const vector &other) 
	{
		if (this == &other) return *this;
		if (vec_begin != nullptr)
		{
			for (size_t i = 0; i < now_len; i++)
			{
				std::destroy_at(vec_begin + i);
			}
			std::allocator<T>().deallocate(vec_begin, extended_len);
		}
		vec_begin = std::allocator<T>().allocate(other.extended_len);
		vec_end = vec_begin + other.now_len;
		extended_len = other.extended_len;
		now_len = other.now_len;
		for (size_t i = 0; i < now_len; i++)
		{
			std::construct_at(vec_begin + i, other.vec_begin[i]);
		}
		return *this;
	}
	T & at(const size_t &pos) 
	{
		return vec_begin[pos];
	}
	const T & at(const size_t &pos) const 
	{
		return vec_begin[pos];
	}
	T & operator[](const size_t &pos) 
	{
		return vec_begin[pos];
	}
	const T & operator[](const size_t &pos) const 
	{
		return vec_begin[pos];
	}
	const T & front() const 
	{
		return vec_begin[0];
	}
	const T & back() const 
	{
		return vec_end[-1];
	}
	iterator begin() 
	{
		return iterator(this, vec_begin);
	}
	const_iterator cbegin() const 
	{
		return const_iterator(this, vec_begin);
	}
	iterator end() 
	{
		return iterator(this, vec_end);
	}
	const_iterator cend() const 
	{
		return const_iterator(this, vec_end);
	}
	bool empty() const 
	{
		return now_len == 0;
	}
	size_t size() const 
	{
		return now_len;
	}
	void clear() 
	{
		if (vec_begin != nullptr)
		{
			for (size_t i = 0; i < now_len; i++)
			{
				std::destroy_at(vec_begin + i);
			}
			std::allocator<T>().deallocate(vec_begin, extended_len);
		}
		vec_begin = std::allocator<T>().allocate(1);
		vec_end = vec_begin;
		extended_len = 1;
		now_len = 0;
	}
	iterator insert(iterator pos, const T &value) 
	{
		if (extended_len == now_len)
		{
			size_t corrected_extended_len = extended_len * 2;
			T *corrected_vec_begin = std::allocator<T>().allocate(corrected_extended_len);
			pos.vec_ptr = corrected_vec_begin + (pos.vec_ptr - vec_begin);
			for (size_t i = 0; i < now_len; i++)
			{
				std::construct_at(corrected_vec_begin + i, std::move(vec_begin[i]));
				std::destroy_at(vec_begin + i);
			}
			std::allocator<T>().deallocate(vec_begin, extended_len);
			vec_begin = corrected_vec_begin;
			vec_end = vec_begin + now_len;
			extended_len = corrected_extended_len;
		}
		for (T *it = vec_end; it != pos.vec_ptr; it--)
		{
			std::construct_at(it, std::move(*(it - 1)));
			std::destroy_at(it - 1);
		}
		std::construct_at(pos.vec_ptr, value);
		now_len++;
		vec_end++;
		return pos;
	}
	iterator insert(const size_t &ind, const T &value) 
	{
		if (extended_len == now_len)
		{
			size_t corrected_extended_len = extended_len * 2;
			T *corrected_vec_begin = std::allocator<T>().allocate(corrected_extended_len);
			for (size_t i = 0; i < now_len; i++)
			{
				std::construct_at(corrected_vec_begin + i, std::move(vec_begin[i]));
				std::destroy_at(vec_begin + i);
			}
			std::allocator<T>().deallocate(vec_begin, extended_len);
			vec_begin = corrected_vec_begin;
			extended_len = corrected_extended_len;
			vec_end = vec_begin + now_len;
		}
		for (T *it = vec_end; it != vec_begin + ind; it--)
		{
			std::construct_at(it, std::move(*(it - 1)));
			std::destroy_at(it - 1);
		}
		std::construct_at(vec_begin + ind, value);
		now_len++;
		vec_end++;
		return iterator(this, vec_begin + ind);
	}
	iterator erase(iterator pos) 
	{
		for (T *it = pos.vec_ptr; it != vec_end - 1; it++)
		{
			std::construct_at(it, std::move(*(it + 1)));
			std::destroy_at(it + 1);
		}
		vec_end--;
		now_len--;
		if (now_len <= extended_len / 4 && now_len != 0)
		{
			T *corrected_vec_begin = std::allocator<T>().allocate(extended_len / 2);
			for (size_t i = 0; i < now_len; i++)
			{
				std::construct_at(corrected_vec_begin + i, std::move(vec_begin[i]));
				std::destroy_at(vec_begin + i);
			}
			std::allocator<T>().deallocate(vec_begin, extended_len);
			vec_begin = corrected_vec_begin;
			extended_len /= 2;
			vec_end = vec_begin + now_len;
		}
		return pos;
	}
	iterator erase(const size_t &ind) 
	{
		for (T* it = vec_begin + ind;  it != vec_end - 1; it++)
		{
			std::construct_at(it, std::move(*(it + 1)));
			std::destroy_at(it + 1);
		}
		vec_end--;
		now_len--;
		if (now_len <= extended_len / 4 && now_len != 0)
		{
			T *corrected_vec_begin = std::allocator<T>().allocate(extended_len / 2);
			for (size_t i = 0; i < now_len; i++)
			{
				std::construct_at(corrected_vec_begin + i, std::move(vec_begin[i]));
				std::destroy_at(vec_begin + i);
			}
			std::allocator<T>().deallocate(vec_begin, extended_len);
			vec_begin = corrected_vec_begin;
			extended_len /= 2;
			vec_end = vec_begin + now_len;
		}
		return iterator(this, vec_begin + ind);
	}
	void push_back(const T &value) 
	{
		if (extended_len == now_len)
		{
			size_t corrected_extended_len = extended_len * 2;
			T *corrected_vec_begin = std::allocator<T>().allocate(corrected_extended_len);
			for (size_t i = 0; i < now_len; i++)
			{
				std::construct_at(corrected_vec_begin + i, std::move(vec_begin[i]));
				std::destroy_at(vec_begin + i);
			}
			std::allocator<T>().deallocate(vec_begin, extended_len);
			vec_begin = corrected_vec_begin;
			extended_len = corrected_extended_len;
			vec_end = vec_begin + now_len;
		}
		std::construct_at(vec_end, value);
		vec_end++;
		now_len++;
	}
	void pop_back() 
	{
		std::destroy_at(vec_end - 1);
		vec_end--;
		now_len--;
		if (now_len <= extended_len / 4 && now_len != 0)
		{
			T *corrected_vec_begin = std::allocator<T>().allocate(extended_len / 2);
			for (size_t i = 0; i < now_len; i++)
			{
				std::construct_at(corrected_vec_begin + i, std::move(vec_begin[i]));
				std::destroy_at(vec_begin + i);
			}
			std::allocator<T>().deallocate(vec_begin, extended_len);
			vec_begin = corrected_vec_begin;
			extended_len /= 2;
			vec_end = vec_begin + now_len;
		}
	}
};

template<class KeyType, class ValueType, int M = 100, int L = 100>
class BPlusTree : public Storage<KeyType, ValueType> 
{
private:
    std::fstream treeNodeFile, leafFile;
    std::string treeNodeFileName, leafFileName;
    int LastTreeNode, LastLeafNode;
    int sizeData;  
    List<int> emptyTreeNode;
    List<int> emptyLeaf;
    struct TreeNode 
    {
        bool flag;
        int pos, data_num;
        int Child_Position[M];
        Pair<KeyType, ValueType> divide_name[M - 1];
    };
    struct Leaf 
    {
        int nxt, pos; 
        int data_num;
        Pair<KeyType, ValueType> value[L]; 
    };
    TreeNode root;

public:
    explicit BPlusTree(const std::string &name) 
    {
        treeNodeFileName = name + "_treefile", leafFileName = name + "_leafFile";
        treeNodeFile.open(treeNodeFileName);
        leafFile.open(leafFileName);
        if (!leafFile || !treeNodeFile) 
        { 
            initialize();
        } 
        else 
        {
            int rootPos;
            treeNodeFile.seekg(0), leafFile.seekg(0);
            treeNodeFile.read(reinterpret_cast<char *>(&rootPos), sizeof(int));
            treeNodeFile.read(reinterpret_cast<char *>(&LastTreeNode), sizeof(int));
            treeNodeFile.seekg(2 * sizeof(int) + rootPos * sizeof(TreeNode));
            treeNodeFile.read(reinterpret_cast<char *>(&root), sizeof(TreeNode));

            int TreeSize, LeafSize;
            treeNodeFile.seekg(2 * sizeof(int) + (LastTreeNode + 1) * sizeof(TreeNode));
            treeNodeFile.read(reinterpret_cast<char *>(&TreeSize), sizeof(int));
            for (int i = 0; i < TreeSize; i++) 
            {
                int data;
                treeNodeFile.read(reinterpret_cast<char *>(&data), sizeof(int));
                emptyTreeNode.pushBack(data);
            }
            leafFile.read(reinterpret_cast<char *>(&LastLeafNode), sizeof(int));
            leafFile.read(reinterpret_cast<char *>(&sizeData), sizeof(int));
            leafFile.seekg(2 * sizeof(int) + (LastLeafNode + 1) * sizeof(Leaf));
            leafFile.read(reinterpret_cast<char *>(&LeafSize), sizeof(int));
            for (int i = 0; i < LeafSize; i++) 
            {
                int data;
                leafFile.read(reinterpret_cast<char *>(&data), sizeof(int));
                emptyLeaf.pushBack(data);
            }
        }
    }

    ~BPlusTree() 
    {
        treeNodeFile.seekg(0), leafFile.seekg(0);
        treeNodeFile.write(reinterpret_cast<char *>(&root.pos), sizeof(int));
        treeNodeFile.write(reinterpret_cast<char *>(&LastTreeNode), sizeof(int));
        writeTreeNode(root);
        leafFile.write(reinterpret_cast<char *>(&LastLeafNode), sizeof(int));
        leafFile.write(reinterpret_cast<char *>(&sizeData), sizeof(int));
        treeNodeFile.seekg(2 * sizeof(int) + (LastTreeNode + 1) * sizeof(TreeNode));
        int emptyTreeNode_num = emptyTreeNode.length(), emptyLeaf_num = emptyLeaf.length();
        treeNodeFile.write(reinterpret_cast<char *>(&emptyTreeNode_num), sizeof(int));
        for (int i = 0; i < emptyTreeNode.length(); i++) 
        {
            int temp = emptyTreeNode.visit(i);
            treeNodeFile.write(reinterpret_cast<char *>(&temp), sizeof(int));
        }
        leafFile.seekg(2 * sizeof(int) + (LastLeafNode + 1) * sizeof(Leaf));
        leafFile.write(reinterpret_cast<char *>(&emptyLeaf_num), sizeof(int));
        for (int i = 0; i < emptyLeaf.length(); i++) 
        {
            int temp = emptyLeaf.visit(i);
            leafFile.write(reinterpret_cast<char *>(&temp), sizeof(int));
        }
        leafFile.close();
        treeNodeFile.close();
    }

    int Getsize() 
    { 
        return sizeData; 
    }

    void insert(const KeyType &key, const ValueType &value) 
    {
        if (insertDFS(Pair<KeyType, ValueType>(key, value), root)) 
        { 
            TreeNode newRoot; 
            TreeNode newNode; 
            newNode.pos = GetTreenode_pos();
            newNode.flag = root.flag;
            newNode.data_num = M / 2;
            int mid = M / 2;
            for (int i = 0; i < mid; i++) 
            {
                newNode.Child_Position[i] = root.Child_Position[mid + i];
            }
            for (int i = 0; i < mid - 1; i++) 
            {
                newNode.divide_name[i] = root.divide_name[mid + i];
            }
            root.data_num = mid;
            writeTreeNode(root);
            writeTreeNode(newNode);
            newRoot.data_num = 2;
            newRoot.pos = GetTreenode_pos();
            newRoot.flag = false;
            newRoot.Child_Position[0] = root.pos;
            newRoot.Child_Position[1] = newNode.pos;
            newRoot.divide_name[0] = root.divide_name[mid - 1];
            root = newRoot;
            writeTreeNode(root);
        }
    }

    List<ValueType> find(const KeyType &key) 
    {
        List<ValueType> ans;
        TreeNode p = root;
        Leaf leaf;
        while (!p.flag) 
        { 
            readTreeNode(p, p.Child_Position[search_TreeNode(key, p)]); 
        }
        readLeaf(leaf, p.Child_Position[search_TreeNode(key, p)]); 
        int now = search_leaf(key, leaf);
        while (now < leaf.data_num && leaf.value[now].first == key) 
        {
            ans.pushBack(leaf.value[now++].second);
        }
        while (leaf.nxt && now == leaf.data_num) 
        { 
            readLeaf(leaf, leaf.nxt);
            now = 0;
            while (now < leaf.data_num && leaf.value[now].first == key) 
            {
                ans.pushBack(leaf.value[now++].second);
            }
        }
        return ans;
    }

    void remove(const KeyType &key, const ValueType &value) 
    {
        if (removeDFS(Pair<KeyType, ValueType>(key, value), root)) 
        {
            if (!root.flag && root.data_num == 1) 
            {
                TreeNode sonNode;
                readTreeNode(sonNode, root.Child_Position[0]);
                emptyTreeNode.pushBack(root.pos);
                root = sonNode;
            }
        }
    }
    void clear() {
        treeNodeFile.close();
        leafFile.close();
        emptyTreeNode.clear();
        emptyLeaf.clear();
        initialize();
    }

private:
    void initialize() 
    {
        treeNodeFile.open(treeNodeFileName, std::ios::out);
        leafFile.open(leafFileName, std::ios::out);
        root.flag = root.pos = root.Child_Position[0] = 1, sizeData = 0;
        root.data_num = 1;
        LastLeafNode = LastTreeNode = 1;
        Leaf initLeaf;
        initLeaf.nxt = initLeaf.data_num = 0;
        initLeaf.pos = 1;
        writeLeaf(initLeaf);
        treeNodeFile.close();
        leafFile.close();
        treeNodeFile.open(treeNodeFileName);
        leafFile.open(leafFileName);
    }
    bool insertDFS(const Pair<KeyType, ValueType> &val, TreeNode &currentNode) 
    {
        if (currentNode.flag) 
        {
            Leaf leaf;
            int node_pos = search_treenodevalue(val, currentNode);
            readLeaf(leaf, currentNode.Child_Position[node_pos]);
            int leafPos = search_leafvalue(val, leaf);
            leaf.data_num++, sizeData++;
            for (int i = leaf.data_num - 1; i > leafPos; i--) 
            {
                leaf.value[i] = leaf.value[i - 1];
            }
            leaf.value[leafPos] = val;
            if (leaf.data_num == L) 
            {
                Leaf newLeaf;
                newLeaf.pos = GetLeaf_pos();
                newLeaf.nxt = leaf.nxt;
                leaf.nxt = newLeaf.pos;
                int mid = L / 2;
                for (int i = 0; i < mid; i++) 
                {
                    newLeaf.value[i] = leaf.value[i + mid];
                }
                leaf.data_num = newLeaf.data_num = mid;
                writeLeaf(leaf);
                writeLeaf(newLeaf);
                for (int i = currentNode.data_num; i > node_pos + 1; i--) 
                {
                    currentNode.Child_Position[i] = currentNode.Child_Position[i - 1];
                }
                currentNode.Child_Position[node_pos + 1] = newLeaf.pos;
                for (int i = currentNode.data_num - 1; i > node_pos; i--) 
                {
                    currentNode.divide_name[i] = currentNode.divide_name[i - 1];
                }
                currentNode.divide_name[node_pos] = leaf.value[mid - 1];
                currentNode.data_num++;
                if (currentNode.data_num == M) 
                { 
                    return true;
                } 
                else writeTreeNode(currentNode);
                return false;
            }
            writeLeaf(leaf);
            return false;
        }
        TreeNode sonNode;
        int now = search_treenodevalue(val, currentNode);
        readTreeNode(sonNode, currentNode.Child_Position[now]);
        if (insertDFS(val, sonNode)) 
        {
            TreeNode newNode;
            newNode.pos = GetTreenode_pos(), newNode.flag = sonNode.flag;
            int mid = M / 2;
            for (int i = 0; i < mid; i++) 
            {
                newNode.Child_Position[i] = sonNode.Child_Position[mid + i];
            }
            for (int i = 0; i < mid - 1; i++) 
            {
                newNode.divide_name[i] = sonNode.divide_name[mid + i];
            }
            newNode.data_num = sonNode.data_num = mid;
            writeTreeNode(sonNode);
            writeTreeNode(newNode);
            for (int i = currentNode.data_num; i > now + 1; i--) 
            {
                currentNode.Child_Position[i] = currentNode.Child_Position[i - 1];
            }
            currentNode.Child_Position[now + 1] = newNode.pos;
            for (int i = currentNode.data_num - 1; i > now; i--) 
            {
                currentNode.divide_name[i] = currentNode.divide_name[i - 1];
            }
            currentNode.divide_name[now] = sonNode.divide_name[mid - 1];
            currentNode.data_num++;
            if (currentNode.data_num == M) 
            {
                return true;
            } 
            else writeTreeNode(currentNode);
            return false;
        } 
        else return false;
    }

    bool removeDFS(const Pair<KeyType, ValueType> &val, TreeNode &currentNode) 
    {
        if (currentNode.flag) 
        {
            Leaf leaf;
            int node_pos = search_treenodevalue(val, currentNode);  
            readLeaf(leaf, currentNode.Child_Position[node_pos]);
            int leafPos = search_leafvalue(val, leaf);
            if (leafPos == leaf.data_num || !Check2(leaf.value[leafPos], val)) 
            {
                return false;
            }
            leaf.data_num--, sizeData--;
            for (int i = leafPos; i < leaf.data_num; i++) 
            {
                leaf.value[i] = leaf.value[i + 1];
            }
            if (leaf.data_num < L / 2) 
            {
                Leaf pre, nxt;
                if (node_pos  >= 1) 
                {
                    readLeaf(pre, currentNode.Child_Position[node_pos - 1]);
                    if (pre.data_num > L / 2) 
                    {
                        leaf.data_num++, pre.data_num--;
                        for (int i = leaf.data_num - 1; i > 0; i--) 
                        {
                            leaf.value[i] = leaf.value[i - 1];
                        }
                        leaf.value[0] = pre.value[pre.data_num];
                        currentNode.divide_name[node_pos - 1] = pre.value[pre.data_num - 1];
                        writeLeaf(leaf);
                        writeLeaf(pre);
                        writeTreeNode(currentNode);
                        return false;
                    }
                }
                if (node_pos < currentNode.data_num - 1) 
                {
                    readLeaf(nxt, currentNode.Child_Position[node_pos + 1]);
                    if (nxt.data_num > L / 2) {
                        leaf.data_num++, nxt.data_num--;
                        leaf.value[leaf.data_num - 1] = nxt.value[0];
                        currentNode.divide_name[node_pos] = nxt.value[0];
                        for (int i = 0; i < nxt.data_num; i++) 
                        {
                            nxt.value[i] = nxt.value[i + 1];
                        }
                        writeLeaf(leaf);
                        writeLeaf(nxt);
                        writeTreeNode(currentNode);
                        return false;
                    }
                }
                if (node_pos >= 1) 
                {
                    for (int i = 0; i < leaf.data_num; i++) 
                    {
                        pre.value[pre.data_num + i] = leaf.value[i];
                    }
                    pre.data_num += leaf.data_num;
                    pre.nxt = leaf.nxt;
                    writeLeaf(pre);
                    emptyLeaf.pushBack(leaf.pos);
                    currentNode.data_num--;
                    for (int i = node_pos; i < currentNode.data_num; i++) 
                    {
                        currentNode.Child_Position[i] = currentNode.Child_Position[i + 1];
                    }
                    for (int i = node_pos - 1; i < currentNode.data_num - 1; i++) 
                    {
                        currentNode.divide_name[i] = currentNode.divide_name[i + 1];
                    }
                    if (currentNode.data_num < M / 2) 
                    { 
                        return true;
                    }
                    writeTreeNode(currentNode);
                    return false;
                }
                if (node_pos  < currentNode.data_num - 1) 
                { 
                    for (int i = 0; i < nxt.data_num; i++) 
                    {
                        leaf.value[leaf.data_num + i] = nxt.value[i];
                    }
                    leaf.data_num += nxt.data_num;
                    leaf.nxt = nxt.nxt;
                    writeLeaf(leaf);
                    emptyLeaf.pushBack(nxt.pos);
                    currentNode.data_num--;
                    for (int i = node_pos + 1; i < currentNode.data_num; i++) 
                    {
                        currentNode.Child_Position[i] = currentNode.Child_Position[i + 1];
                    }
                    for (int i = node_pos; i < currentNode.data_num - 1; i++) 
                    {
                        currentNode.divide_name[i] = currentNode.divide_name[i + 1];
                    }
                    if (currentNode.data_num < M / 2) 
                        return true;
                    writeTreeNode(currentNode);
                    return false;
                }
                writeLeaf(leaf);
            } 
            else writeLeaf(leaf);
            return false;
        }
        TreeNode sonNode;
        int now = search_treenodevalue(val, currentNode);
        readTreeNode(sonNode, currentNode.Child_Position[now]);
        if (removeDFS(val, sonNode)) 
        { 
            TreeNode pre, nxt;
            if (now >= 1) 
            {  
                readTreeNode(pre, currentNode.Child_Position[now - 1]);
                if (pre.data_num > M / 2) 
                { 
                    sonNode.data_num++, pre.data_num--;
                    for (int i = sonNode.data_num - 1; i > 0; i--) 
                    {
                        sonNode.Child_Position[i] = sonNode.Child_Position[i - 1];
                    }
                    for (int i = sonNode.data_num - 2; i > 0; i--) 
                    {
                        sonNode.divide_name[i] = sonNode.divide_name[i - 1];
                    }
                    sonNode.Child_Position[0] = pre.Child_Position[pre.data_num];
                    sonNode.divide_name[0] = currentNode.divide_name[now - 1];
                    currentNode.divide_name[now - 1] = pre.divide_name[pre.data_num - 1];
                    writeTreeNode(sonNode);
                    writeTreeNode(pre);
                    writeTreeNode(currentNode);
                    return false;
                }
            }
            if (now < currentNode.data_num - 1) 
            { 
                readTreeNode(nxt, currentNode.Child_Position[now + 1]);
                if (nxt.data_num > M / 2) 
                { 
                    sonNode.data_num++, nxt.data_num--;
                    sonNode.Child_Position[sonNode.data_num - 1] = nxt.Child_Position[0];
                    sonNode.divide_name[sonNode.data_num - 2] = currentNode.divide_name[now];
                    currentNode.divide_name[now] = nxt.divide_name[0];
                    for (int i = 0; i < nxt.data_num; i++) 
                    {
                        nxt.Child_Position[i] = nxt.Child_Position[i + 1];
                    }
                    for (int i = 0; i < nxt.data_num - 1; i++) 
                    {
                        nxt.divide_name[i] = nxt.divide_name[i + 1];
                    }
                    writeTreeNode(sonNode);
                    writeTreeNode(nxt);
                    writeTreeNode(currentNode);
                    return false;
                }
            }
            if (now  >= 1) 
            { 
                for (int i = 0; i < sonNode.data_num; i++) 
                {
                    pre.Child_Position[pre.data_num + i] = sonNode.Child_Position[i];
                }
                pre.divide_name[pre.data_num - 1] = currentNode.divide_name[now - 1];
                for (int i = 0; i < sonNode.data_num - 1; i++) 
                {
                    pre.divide_name[pre.data_num + i] = sonNode.divide_name[i];
                }
                pre.data_num += sonNode.data_num;
                writeTreeNode(pre);
                emptyTreeNode.pushBack(sonNode.pos);
                currentNode.data_num--;
                for (int i = now; i < currentNode.data_num; i++) 
                {
                    currentNode.Child_Position[i] = currentNode.Child_Position[i + 1];
                }
                for (int i = now - 1; i < currentNode.data_num - 1; i++) 
                {
                    currentNode.divide_name[i] = currentNode.divide_name[i + 1];
                }
                if (currentNode.data_num < M / 2) 
                {
                    return true;
                }
                writeTreeNode(currentNode);
                return false;
            }
            if (now  < currentNode.data_num - 1) 
            { 
                for (int i = 0; i < nxt.data_num; i++) 
                {
                    sonNode.Child_Position[sonNode.data_num + i] = nxt.Child_Position[i];
                }
                sonNode.divide_name[sonNode.data_num - 1] = currentNode.divide_name[now];
                for (int i = 0; i < nxt.data_num - 1; i++) 
                {
                    sonNode.divide_name[sonNode.data_num + i] = nxt.divide_name[i];
                }
                sonNode.data_num += nxt.data_num;
                writeTreeNode(sonNode);
                emptyTreeNode.pushBack(nxt.pos);
                currentNode.data_num--;
                for (int i = now + 1; i < currentNode.data_num; i++) 
                {
                    currentNode.Child_Position[i] = currentNode.Child_Position[i + 1];
                }
                for (int i = now; i < currentNode.data_num - 1; i++) 
                {
                    currentNode.divide_name[i] = currentNode.divide_name[i + 1];
                }
                if (currentNode.data_num < M / 2) 
                {
                    return true;
                }
                writeTreeNode(currentNode);
                return false;
            }
        }
        return false;
    }

    void writeTreeNode(TreeNode &node) 
    {
        treeNodeFile.seekg(node.pos * sizeof(TreeNode) + 2 * sizeof(int));
        treeNodeFile.write(reinterpret_cast<char *>(&node), sizeof(TreeNode));
    }

    void writeLeaf(Leaf &leaf) 
    {
        leafFile.seekg(leaf.pos * sizeof(Leaf) + 2 * sizeof(int));
        leafFile.write(reinterpret_cast<char *>(&leaf), sizeof(Leaf));
    }

    void readTreeNode(TreeNode &node, int pos) 
    {
        treeNodeFile.seekg(pos * sizeof(TreeNode) + 2 * sizeof(int));
        treeNodeFile.read(reinterpret_cast<char *>(&node), sizeof(TreeNode));
    }

    void readLeaf(Leaf &lef, int pos) 
    {
        leafFile.seekg(pos * sizeof(Leaf) + 2 * sizeof(int));
        leafFile.read(reinterpret_cast<char *>(&lef), sizeof(Leaf));
    }

    int search_leafvalue(const Pair<KeyType, ValueType> &val, const Leaf &lef) 
    {
        int l = -1;
        int r = lef.data_num - 1;
        while (l < r) 
        {
            int mid = (l + r + 1) / 2;
            if (!Check1(lef.value[mid], val)) r = mid - 1;
            else l = mid;
        }
        return l + 1;
    }

    int search_treenodevalue(const Pair<KeyType, ValueType> &val, const TreeNode &node) 
    {
        int l = -1;
        int r = node.data_num - 2;
        while (l < r) 
        {
            int mid = (l + r + 1) / 2;
            if (!Check1(node.divide_name[mid], val)) r = mid - 1;
            else l = mid;
        }
        return l + 1;
    }

    int search_leaf(const KeyType &key, const Leaf &lef) 
    {
        int l = -1;
        int r = lef.data_num - 1;
        while (l < r) 
        {
            int mid = (l + r + 1) / 2;
            if (lef.value[mid].first >= key) r = mid - 1;
            else l = mid;
        }
        return l + 1;
    }

    int search_TreeNode(const KeyType &key, const TreeNode &node) 
    {
        int l = -1;
        int r = node.data_num - 2;
        while (l < r) 
        {
            int mid = (l + r + 1) / 2;
            if (node.divide_name[mid].first >= key) r = mid - 1;
            else l = mid;
        }
        return l + 1;
    }

    int GetTreenode_pos() 
    {
        if (emptyTreeNode.empty()) 
        { 
            return ++LastTreeNode;
        } 
        else 
        { 
            int newIndex = emptyTreeNode.back();
            emptyTreeNode.popBack();
            return newIndex;
        }
    }

    int GetLeaf_pos() 
    {
        if (emptyLeaf.empty()) 
        {
            return ++LastLeafNode;
        } 
        else 
        {
            int newIndex = emptyLeaf.back();
            emptyLeaf.popBack();
            return newIndex;
        }
    }
};

}
struct String 
{
    char index[65];

    String(const String &other) 
    {
        for (int i = 0; i < 65; i++)index[i] = other.index[i];
    }

    String() = default;

    friend bool operator>(const String &lhs, const String &rhs) 
    {
        return std::string(lhs.index) > std::string(rhs.index);
    }

    friend bool operator>=(const String &lhs, const String &rhs) 
    {
        return std::string(lhs.index) >= std::string(rhs.index);
    }

    friend bool operator<(const String &lhs, const String &rhs) 
    {
        return std::string(lhs.index) < std::string(rhs.index);
    }

    friend bool operator<=(const String &lhs, const String &rhs) 
    {
        return std::string(lhs.index) <= std::string(rhs.index);
    }

    friend bool operator==(const String &lhs, const String &rhs) 
    {
        return std::string(lhs.index) == std::string(rhs.index);
    }

    friend bool operator!=(const String &lhs, const String &rhs) 
    {
        return std::string(lhs.index) != std::string(rhs.index);
    }

    friend std::ostream &operator<<(std::ostream &os, const String &obj) 
    {
        os << obj.index;
        return os;
    }
};
//thanks for the idea ,annotation and the LIST and PAIR ans STRING code from Vexoben
//thanks for the testcase from baihua
//thanks for the help from skywong
int main() 
{
    BPlusTree::BPlusTree<String, int> bpTree("1");
    String key;
    int value;
    int cnt;
    char ch[10];
    std::cin >> cnt;
    for (int i = 1; i <= cnt; i++) 
    {
        std::cin >> ch;
        if (ch[0] == 'i') 
        {
            std::cin >> key.index >> value;
            bpTree.insert(key, value);
        } 
        else if (ch[0] == 'f') 
        {
            std::cin >> key.index;
            BPlusTree::List<int> ans = bpTree.find(key);
            if (!ans.empty()) 
            {
                for (int i = 0; i < ans.length(); i++) 
                {
                    std::cout << ans.visit(i) << ' ';
                }
                std::cout << std::endl;
            } 
            else puts("null");
        } 
        else if (ch[0] == 'd') 
        {
            std::cin >> key.index >> value;
            bpTree.remove(key, value);
        }
    }
    return 0;
}