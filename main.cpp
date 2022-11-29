#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
using namespace std;

template<class T>
struct Node{
    T m_data;
    int l;
    int r;
    Node* m_left;
    Node* m_right;
    Node(T v, T a, T b):m_data(v),l(a),r(b),m_left(nullptr),m_right(nullptr){}

    friend std::ostream &operator<<(std::ostream &os, Node<T> const &n) {
        return os << n.m_data;
    }

    void dot(ostream &file){
        file<<"node_"<<m_data<<" [label = \"<l> | <m> "<< m_data <<" | <r>\"];\n";
        if(m_left){
            file<<"node_"<<m_data<<":l -> node_"<<m_left->m_data<<":m;\n";
        }
        if(m_right){
            file<<"node_"<<m_data<<":r -> node_"<<m_right->m_data<<":m;\n";
        }
    }
};


template<class T>
class SegmentTree{
private:
    Node<T>* m_root;
    vector<int> nums;

    //insertion
    Node<T>* buildTree(int l, int r){
        if(l==r)
            return new Node(nums[l],l,r);

        auto* node = new Node(0,l,r);
        int mid = (l+r)/2;
        node->m_left = buildTree(l,mid);
        node->m_right = buildTree(mid+1,r);
        node->m_data = node->m_left->m_data + node->m_right->m_data;
        return node;
    }
    //single insertion
    void insert(vector<int>& num, int val){
        num.push_back(val);
        m_root = buildTree(0,nums.size()-1);
    }

    //searchNode
    Node<T>* searchNode(int l, int r, Node<T>* node){
        if(node->l == l && node->r == r)
            return node;
        int mid = (node->l + node->r)/2;
        if(r<=mid)
            return searchNode(l,r,node->m_left);
        else if(l>mid)
            return searchNode(l,r,node->m_right);
        else{
            auto* node = new Node(0,l,r);
            node->m_left = searchNode(l,mid,node->m_left);
            node->m_right = searchNode(mid+1,r,node->m_right);
            node->m_data = node->m_left->m_data + node->m_right->m_data;
            return node;
        }
    }
    //delete node
    Node<T>* deleteNode(int l, int r, Node<T>* node){
        if(node->l == l && node->r == r){
            delete node;
            return nullptr;
        }
        int mid = (node->l + node->r)/2;
        if(r<=mid)
            node->m_left = deleteNode(l,r,node->m_left);
        else if(l>mid)
            node->m_right = deleteNode(l,r,node->m_right);
        else{
            node->m_left = deleteNode(l,mid,node->m_left);
            node->m_right = deleteNode(mid+1,r,node->m_right);
        }
        if(node->m_left == nullptr && node->m_right == nullptr){
            delete node;
            return nullptr;
        }
        if(node->m_left == nullptr){
            auto* temp = node->m_right;
            delete node;
            return temp;
        }
        if(node->m_right == nullptr){
            auto* temp = node->m_left;
            delete node;
            return temp;
        }
        node->m_data = node->m_left->m_data + node->m_right->m_data;
        return node;
    }
    //update node
    void updateTree(Node<T>* node, int pos, int val){
        if(node->l==node->r){
            node->m_data = val;
            return;
        }
        int mid = (node->l+node->r)/2;
        if(pos<=mid)
            updateTree(node->m_left,pos,val);
        else
            updateTree(node->m_right,pos,val);

        node->m_data = node->m_left->m_data + node->m_right->m_data;
    }
    //interval sum
    int queryTree(Node<T>* node, int l, int r){
        if(l==node->l && r==node->r)
            return node->m_data;
        int mid = (node->l+node->r)/2;
        if(r<=mid)
            return queryTree(node->m_left,l,r);
        else if(l>mid)
            return queryTree(node->m_right,l,r);
        else
            return queryTree(node->m_left,l,mid)+queryTree(node->m_right,mid+1,r);
    }


    //graphics
    void dot(ofstream &file, Node<T>* current){
        if (current==nullptr)
        {
            return;
        }else{
            current->dot(file);
            dot(file, current->m_left);
            dot(file, current->m_right);
        }
    }

public:
    //public function
    explicit SegmentTree(vector<int>& nums){
        this->nums = nums;
        m_root = buildTree(0,nums.size()-1);
    }
    void insert(int val){
        insert(nums, val);
    }
    void update(int i, int val){
        updateTree(m_root,i,val);
    }
    int sumRange(int i, int j){
        return queryTree(m_root,i,j);
    }
    void deleteNode(int l, int r){
        deleteNode(l, r, m_root);
    }
    void searchNode(int l, int r){
        Node<T>*node = searchNode(l, r, m_root);
     }
    void searchNode_viewData(int l, int r){
        Node<T>*node = searchNode(l, r, m_root);
        cout<<endl<<node->m_data;
    }
    
    void dot(const string& filename){
        ofstream mydot;
        mydot.open(filename, ios::out);

        mydot << "digraph g {\n";
        mydot << "node [shape=record, height=0.1];\n";
        dot(mydot, m_root);
        mydot << "}";
        mydot.close();
    }

};
int main() {
    vector<int> nums{2,3};
    auto T1 = SegmentTree<int>(nums);
    T1.insert(1);
    T1.insert(10);
    T1.insert(15);
    T1.insert(31);
    T1.update(2,4);
    T1.searchNode_viewData(4,4);
    T1.dot("tree.dot");

    return 0;
}
