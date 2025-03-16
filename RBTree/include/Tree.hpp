#include <memory>
#include <functional>

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
        
        Node* getGrandp(Node* node) const
        {
            return parent ? parent : nullptr;   
        }

        Node* getUncle(Node* node) const
        {
            Node* grandp = getGrandp(node);

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

    void fixViolation(Node* node) 
    {

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

                    // fixViolation

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

                    // fixViolation

                    return;
                }
            }

            return;
        }
    }

    void printBlack(Node* node) const
    {
        if (node != nullptr)
            node->color = black;
    }

    void printRed(Node* node) const
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

            printBlack(root_);
        }
        else
        {
            //insert private method
        }
    }

    
};
    
} // namespace RB
