struct Node {
    int data;
    Node *next;

    Node(int _data) : data(_data), next(nullptr) {}

    Node() : data(0), next(nullptr) {}
};

Node *mergeLists(Node *a, Node *b) {
    Node tmp;
    Node *head = &tmp;
    Node *c = head;

    while ((a != nullptr) && (b != nullptr)) {
        if (a->data < b->data) {
            c->next = a;
            c = a;
            a = a->next;
            continue;
        }
        c->next = b;
        c = b;
        b = b->next;
    }
    c->next = (a == nullptr) ? b : a;
    return head->next;
}

Node *mergeSort(Node *c) {
    if (c == nullptr || c->next == nullptr)
        return c;

    Node *a = c;
    Node *b = c->next;

    while ((b != nullptr) && (b->next != nullptr)) {
        c = c->next;
        b = b->next->next;
    }
    b = c->next;
    c->next = nullptr;
    return mergeLists(mergeSort(a), mergeSort(b));
}
