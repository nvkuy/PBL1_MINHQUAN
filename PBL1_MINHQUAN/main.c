#include <stdio.h>
#include <stdlib.h>

struct Node
{
    double data;
    struct Node* next;
};

struct Mat {
    struct Node** head;
    struct Mat* next;
};

void addToListAtEnd(struct Node** head_ref, double new_data)
{
    struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
    struct Node* last = *head_ref;

    new_node->data = new_data;
    new_node->next = NULL;

    if (*head_ref == NULL)
    {
        *head_ref = new_node;
        return;
    }

    while (last->next != NULL) last = last->next;

    last->next = new_node;
}

void addToListAtPos(struct Node** head, int pos, double new_data) {
    struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
    struct Node* prev = *head;
    while (pos > 1) {
        prev = prev->next;
        pos--;
    }
    new_node->data = new_data;
    new_node->next = prev->next;
    prev->next = new_node;
}

void printList(struct Node* node)
{
    while (node != NULL)
    {
        printf("%.2lf ", node->data);
        node = node->next;
    }
}

void addToMatAtEnd(struct Mat** head_ref, struct Node* head_line)
{
    struct Mat* new_line = (struct Mat*)malloc(sizeof(struct Mat));
    struct Mat* last = *head_ref;

    new_line->head = head_line;
    new_line->next = NULL;

    if (*head_ref == NULL)
    {
        *head_ref = new_line;
        return;
    }

    while (last->next != NULL) last = last->next;

    last->next = new_line;
    return;
}

void addColMatAtPos(struct Mat** head_ref, struct Node* head_col, int m) {
    struct Mat* line = *head_ref;
    while (head_col != NULL) {
        addToListAtPos(&(line->head), m, head_col->data);
        line = line->next;
        head_col = head_col->next;
    }
}

void printMat(struct Mat* mat)
{
    while (mat != NULL)
    {
        printList(mat->head);
        printf("\n");
        mat = mat->next;
    }
}

void saveList(struct Node* node, char *filename, char *mode)
{
    FILE* file = fopen (filename, mode);
    while (node != NULL)
    {
        fprintf(file, "%.2lf ", node->data);
        node = node->next;
    }
    fprintf(file, "\n");
    fclose(file);
}

void saveMat(struct Mat* mat, char *filename) {
    FILE* file = fopen (filename, "w");
    while (mat != NULL)
    {
        saveList(mat->head, filename, "ab");
        mat = mat->next;
    }
    fclose(file);
}

struct Mat *mat;

void newMat() {
    mat = NULL;
}

void readMat(int n, int m, char *filename) {
    double aij;
    FILE* file = fopen (filename, "r");
    for (int i = 0; i < n; i++) {
        struct Node* head_line = NULL;
        for (int j = 0; j < m; j++) {
            fscanf(file, "%lf", &aij);
            addToListAtEnd(&head_line, aij);
        }
        addToMatAtEnd(&mat, head_line);
    }
    fclose(file);
}

struct Node* solve(struct Mat** mat, struct Node* x, double eps, int n) {
    int t = 1;
    while (t != 0) {
        t = 0;
        struct Mat* line = *mat;
        struct Node* node = line->head;
        struct Node* yi = x;
        struct Node* headYi = yi;
        printf("----------------\n");
        printList(yi);
        printf("\n");
        for (int i = 0; i < n; i++) {
            struct Node* xi = x;
            double s = 0, aii, last, xit;
            for (int j = 0; j <= n; j++) {
                if (j == n) {
                    last = node->data;
                    continue;
                }
                if (i != j)
                    s += ((node->data) * (xi->data));
                else {
                    aii = node->data;
                    xit = xi->data;
                }
                node = node->next, xi = xi->next;
            }
            yi->data = (last - s) / aii;
            if (fabs(yi->data - xit) > eps)
                t = 1;
            yi = yi->next;
            line = line->next;
            if (line != NULL)
                node = line->head;
        }
        x = headYi;
    }
    return x;
}

int main()
{
    newMat();
    int n;
    double tmp;

    //task 1
    scanf("%d", &n);
    readMat(n, n - 1, "DAYSO.IN");
    printMat(mat);

    //task 2
    struct Node *col = NULL;
    for (int i = 0; i < n; i++) {
        scanf("%lf", &tmp);
        addToListAtEnd(&col, tmp);
    }
    addColMatAtPos(&mat, col, (n - 1) / 2);
    printMat(mat);
    saveMat(mat, "RESULT1.OUT");

    //task 3
    newMat();
    scanf("%d", &n);
    readMat(n, n, "RESULT1.OUT");
    col = NULL;
    for (int i = 0; i < n; i++) {
        scanf("%lf", &tmp);
        addToListAtEnd(&col, tmp);
    }
    addColMatAtPos(&mat, col, n);
    printMat(mat);
    struct Node* ans = NULL;
    for (int i = 0; i < n; i++)
        addToListAtEnd(&ans, 0.0);
    ans = solve(&mat, ans, 1e-4, n);
    saveList(ans, "RESULT2.OUT", "w");

}
