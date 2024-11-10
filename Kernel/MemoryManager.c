// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <MemoryManager.h>

uint64_t test_mm(uint64_t argc, char * argv[]);

void * start;
int block_size;

void * free_ptrs[BLOCK_COUNT];
int current;


#define EMPTY 0
#define NOT_LEAF 1
#define LEAF 2

// PARA SALTAR AL HIJO IZQUIERDO: (2 * i) + 1
// PARA SALTAR AL HIJO DERECHO: (2 * i) + 2
typedef struct buddy_node
{
    uint8_t index;
    int64_t size;
    int8_t state;
    int8_t assigned;
    uint8_t level;
    void * address;
    struct buddy_node * left;
    struct buddy_node * right;
    struct buddy_node * parent;
} buddy_node;
uint8_t sizeOfNode = sizeof(buddy_node);

// max number of buddie control nodes.
// buddy_node buddies[(1<<0) + (1<<1) + (1<<2) + (1<<3) + (1<<4) + (1<<5) +
// (1<<6) + (1<<7)] = {0}; buddy_node buddies[(1<<0) + (1<<1) + (1<<2) + (1<<3)]
// = {0};

buddy_node * buddies = START_AUX_STRUCT;
// 1 + 2 + 4 + 8 + 16 + 32 + 64 + 128 = 255
buddy_node * root;

buddy_node * createNode(uint8_t index, uint64_t size, uint8_t level, void * address, buddy_node * parent)
{
    buddies[index].index    = index;
    buddies[index].size     = size;
    buddies[index].state    = LEAF;
    buddies[index].level    = level;
    buddies[index].address  = address;
    buddies[index].left     = NULL;
    buddies[index].right    = NULL;
    buddies[index].parent   = parent;
    buddies[index].assigned = 0;
    return &buddies[index];
}

void createMemoryManager(int size)
{
    start          = (void *) START_FREE_MEM;
    root           = buddies;
    root->size     = TOTAL_MEM_SIZE;
    root->state    = LEAF;
    root->level    = 0;
    root->address  = start;
    root->left     = NULL;
    root->right    = NULL;
    root->index    = 0;
    root->assigned = 0;
    current        = 0;
}

buddy_node * allocRecursive(buddy_node * node, uint32_t size)
{

    //    if(node == NULL || node->assigned == 1){
    //        return NULL;
    //    }

    // me fijo si tiene que ir en el actual o tengo que partirme en 2
    if (node->size / 2 < size)
    {
        if (node->state == LEAF)
        {
            if (node->assigned == 0)
            {
                node->assigned = 1;
                return node;
            }
            else
            {
                return NULL;
            }
        }
        else
        {
            return NULL;
        }
    }
    // tengo que dividirme en 2. si no tengo hijos, los creo.
    if (node->state == LEAF)
    {
        node->left  = createNode(node->index * 2 + 1, node->size / 2, node->level + 1, node->address, node);
        node->right = createNode(node->index * 2 + 2, node->size / 2, node->level + 1, node->address + node->size / 2, node);
        node->state = NOT_LEAF;
    }
    // me fijo si el hijo izquierdo tiene espacio
    buddy_node * aux = allocRecursive(node->left, size);
    if (aux != NULL)
    {
        return aux;
    }
    // me fijo si el hijo derecho tiene espacio
    aux = allocRecursive(node->right, size);

    // si me devuelve null es xq no hay espacio. sino uso el derecho.
    return aux;
}

void * allocMemory(uint32_t size)
{
    if (size > TOTAL_MEM_SIZE)
    {
        return NULL;
    }
    if (size < MIN_BLOCK_SIZE)
    {
        size = MIN_BLOCK_SIZE;
    }
    void * aux = allocRecursive(root, size)->address;
    return aux;
}

// yo se que los cada nodo tiene el mismo address que su hijo izq y
// su hijo derecho tiene address + node->size / 2.
// entonces puedo comparar el puntero que me pasan para buscar en el arbol
// derecho o en el izquierdo
void freeRecursive(buddy_node * node, void * p)
{
    if (node->state == LEAF)
    {
        if (node->address == p && node->assigned == 1)
        {
            node->assigned = 0;
        }
        return;
    }
    if (p < node->address + node->size / 2)
    {
        if (node->left)
        {
            freeRecursive(node->left, p);
        }
    }
    else
    {
        if (node->right)
        {
            freeRecursive(node->right, p);
        }
    }
    if (node->left && node->right && node->left->state == LEAF && node->left->assigned == 0 && node->right->state == LEAF && node->right->assigned == 0)
    {
        node->state        = LEAF;
        node->left->state  = EMPTY;
        node->right->state = EMPTY;
        node->left         = NULL;
        node->right        = NULL;
    }
}

void my_free(void * p)
{
    if (p == NULL)
        return;

    freeRecursive(root, p);
}

/*
void createMemoryManager(int size)
{
    start      = (void *) START_FREE_MEM;
    block_size = size;
    for (int i = 0; i < BLOCK_COUNT; i++)
    {
        free_ptrs[i] = (char *) start + i * block_size;
    }
    current = 0;
}

void * allocMemory(uint32_t size)
{
    if (size > block_size)
    {
        return NULL;
    }
    if (current < BLOCK_COUNT)
    {
        return free_ptrs[current++];
    }
    return NULL;
}

void my_free(void * p)
{
    free_ptrs[--current] = p;
}
*/