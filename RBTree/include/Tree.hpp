#include <memory>
#include <functional>
#include <utility>

namespace RB 
{

enum Color {red = false, black = true};

template<typename KeyT, typename Compare = std::less<KeyT>>
class Tree
{
    //! Forward declaration
    struct Node;

    //! Tree usings
    using NodePtr = std::unique_ptr<Node>;

    //! Tree members
    Node* root_ = nullptr;
    std::vector<NodePtr> data_;
    Compare comp_;

    struct Node
    {
        //! Node members
        KeyT key;

        Node* right{};
        Node* left{};
        Node* parent{};

        Color color = red;

        //! Node constructors
        Node(const KeyT& key_)
        :   key(key_)
        {}

        Node(const KeyT& key_, Node* parent_, Color color_)
        :   key(key_),
            parent(parent_),
            color(color_)
        {}

        //! Node methods
        
        bool isLeftChild() const 
        {
            return parent && parent->left == this;
        }

        bool isRightChild() const 
        {
            return parent && parent->right == this;
        }
        
        Node* getGrandp() const
        {
            return parent ? parent : nullptr;   
        }

        Node* getUncle() const
        {
            Node* grandp = getGrandp();

            if (grandp == nullptr) return nullptr;

            return grandp->right == parent ? grandp->left : grandp->right;
        }
    };

    //! Private Tree methods 
    void rotateLeft(Node* node) 
    {
        Node* pivot = node->right;

        if (pivot == nullptr) return;

        pivot->parent = node->parent;

        if (node->parent != nullptr)
        {
            if (node->parent->left == node)
            {
                node->parent->left = pivot;
            }           
            else
            {
                node->parent->right = pivot;
            }
        }
        else
        {
            root_ = pivot;
        }

        node->right = pivot->left;

        if (pivot->left != nullptr)
        {
            pivot->left->parent = node;
        }

        node->parent = pivot;
        pivot->left = node;
    }

    void rotateRight(Node* node) 
    {
        Node* pivot = node->left;

        if (pivot == nullptr) return;

        pivot->parent = node->parent;

        if (node->parent != nullptr)
        {
            if (node->parent->left == node)
            {
                node->parent->left = pivot;
            }           
            else
            {
                node->parent->right = pivot;
            }
        }
        else
        {
            root_ = pivot;
        }

        node->left = pivot->right;

        if (pivot->right != nullptr)
        {
            pivot->right->parent = node;
        }

        node->parent = pivot;
        pivot->right = node;
    }

    void fixBlackUncle(Node* node)
    {
        Node* parent = node->parent;
        Node* grandp = node->getGrandp();

        if (parent->isRightChild() && node->isRightChild())
        {
            rotateRight(parent);
            node = node->left;
        }

        parent = node->parent;
        grandp = node->getGrandp();

        if (parent->isLeftChild())
        {
            rotateRight(grandp);
        }
        else
        {
            rotateLeft(grandp);
        }

        paintBlack(parent);
        paintRed(grandp);
    }

    void fixViolation(Node* node) 
    {
        while (node != root_ && node->parent && node->parent->color == red)
        {
            Node* parent = node->parent;
            Node* grandp = node->getGrandp();
            Node* uncle = node->getUncle();

            // Case 1: Uncle is red
            
            if (uncle && uncle->color == red)
            {
                paintBlack(parent);
                paintRed(uncle);
                paintRed(grandp);

                node = grandp;
            }
            else // Case 2: Uncle is black
            {
                fixBlackUncle(node);

                break;
            }
        }

        root_->color = black;
    }

    void insertNode(Node* node, const KeyT& key)
    {
        while (node)
        {
            if (comp_(node->key, key))
            {
                if (node->right == nullptr)
                {
                    auto insertNode = std::make_unique<Node>(key, node);

                    Node* insertNodeRaw = insertNode.get();

                    node->right = insertNodeRaw;

                    data_.push_back(std::move(insertNodeRaw));

                    fixViolation(insertNodeRaw);

                    return;
                }
            }
            else if (comp_(key, node->key))
            {
                if (node->left == nullptr)
                {
                    auto insertNode = std::make_unique<Node>(key, node);

                    Node* insertNodeRaw = insertNode.get();

                    node->left = insertNodeRaw;

                    data_.push_back(std::move(insertNodeRaw));

                    fixViolation(insertNodeRaw);

                    return;
                }
            }

            return;
        }
    }

    void paintBlack(Node* node) const
    {
        if (node != nullptr)
            node->color = black;
    }

    void paintRed(Node* node) const
    {
        if (node != nullptr)
            node->color = red;
    }

    //! Public Tree methods
    
public:
    void insert(const KeyT& key)
    {
        if (root_ == nullptr)
        {
            auto root = std::make_unique<Node>(key);

            root_ = root.get();

            data_.push_back(std::move(root));

            paintBlack(root_);
        }
        else
        {
            insertNode(root_, key);
        }
    }

    
};
    
} // namespace RB
