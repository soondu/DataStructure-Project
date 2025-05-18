#include <stdio.h>
#include <stdlib.h>
#pragma warning (disable:4996)
#define MAX_SIZE 50
#define MAX_TERMS 101

typedef enum { head, entry } tagfield;
typedef struct matrix_node* matrix_pointer;
matrix_pointer hdnode[MAX_SIZE];

typedef struct entry_node {
    int row;
    int col;
    int value;
}EntryNode;

typedef struct matrix_node {
    matrix_pointer down;
    matrix_pointer right;
    tagfield tag;
    matrix_pointer next;
    EntryNode entry;

}MatrixNode;

matrix_pointer mread(FILE*); // file에서 matrix 자료를 읽어 리스트에 저장한다.
void mwrite(matrix_pointer); // 행렬을 적절한 포맷 (2차원 행렬형태)으로 출력한다.
void merase(matrix_pointer*); // 행렬의 모든 노드를 지운다.
matrix_pointer madd(matrix_pointer, matrix_pointer); // matrix addition

int main() {
    matrix_pointer a, b, d;
    //파일 오픈한다.

    FILE* fileA = fopen("A.txt", "r");
    if (fileA == NULL) {
        printf("Error opening file A.txt.\n");
        return 1;
    }
    FILE* fileB = fopen("B.txt", "r");
    if (fileB == NULL) {
        printf("Error opening file B.txt.\n");
        fclose(fileA);
        return 1;
    }

    a = mread(fileA);
    b = mread(fileB);
    mwrite(a);
    mwrite(b);
    d = madd(a, b);
    mwrite(d);
    merase(&d);
    merase(&a);
    merase(&b);
    mwrite(a);
    mwrite(b);
    mwrite(d);

    fclose(fileA);
    fclose(fileB);
}


matrix_pointer mread(FILE* file) {
    int NumRows, NumCols, NumEntries = 0, NumHeads, i;
    int row, col, value;
    matrix_pointer temp, last, node;

    fscanf(file, "%d %d", &NumRows, &NumCols);
    NumHeads = (NumCols > NumRows) ? NumCols : NumRows;
    printf("%d, %d\n", NumRows, NumCols);
    /* set up head node for the list of head nodes */
    node = (matrix_pointer)malloc(sizeof(MatrixNode));
    node->tag = entry;
    node->entry.row = NumRows;
    node->entry.col = NumCols;

    if (!NumHeads)
        node->right = node;
    else {
        // 헤더노드 초기화        
        for (i = 0; i < NumHeads; i++) {
            temp = (matrix_pointer)malloc(sizeof(MatrixNode));
            hdnode[i] = temp;
            hdnode[i]->tag = head;
            hdnode[i]->right = temp;
            hdnode[i]->next = temp;
        }

        last = hdnode[0];
        for (row = 0; row < NumRows; row++) {
            for (col = 0; col < NumCols; col++) {
                fscanf(file, "%d", &value);
                if (value != 0) {
                    NumEntries++;
                    temp = (matrix_pointer)malloc(sizeof(MatrixNode));
                    temp->tag = entry;
                    temp->entry.row = row;
                    temp->entry.col = col;
                    temp->entry.value = value;

                    last->right = temp;          // 행리스트에 연결
                    last = temp;
                    hdnode[col]->next->down = temp;  // 열리스트에 연결
                    hdnode[col]->next = temp;
                }
            }
        }
        node->entry.value = NumEntries;
        last->right = NULL;
        // close all column lists 
        for (i = 0; i < NumCols; i++)
            hdnode[i]->next->down = hdnode[i];

        // link all head nodes together
        for (i = 0; i < NumHeads - 1; i++)
            hdnode[i]->next = hdnode[i + 1];

        hdnode[NumHeads - 1]->next = node;
        node->right = hdnode[0];
    }
    printf("FINISH READING\n");
    return node;
}
matrix_pointer madd(matrix_pointer a, matrix_pointer b) {
    int NumRows, NumCols, NumEntries = 0, i;
    int row, col, value;
    matrix_pointer temp, last, node;

    int r, c, NumHeads, inda = 0, indb = 0;
    r = a->entry.row;
    c = a->entry.col;
    NumHeads = (c > r) ? c : r;

    /* set up head node for the list of head nodes */
    node = malloc(sizeof(MatrixNode));
    node->tag = entry;
    node->entry.row = r;
    node->entry.col = c;

    if (!NumHeads)
        node->right = node;
    else {
        // 헤더노드 초기화        
        for (i = 0; i < NumHeads; i++) {
            temp = malloc(sizeof(MatrixNode));
            hdnode[i] = temp;
            hdnode[i]->tag = head;
            hdnode[i]->right = temp;
            hdnode[i]->next = temp;
        }
        matrix_pointer acop, bcop;
        acop = a->right->right;
        bcop = b->right->right;
        last = hdnode[0];
        for (row = 0; row < r; row++) {
            for (col = 0; col < c; col++) {
                if (acop == NULL) {
                    inda = c + 3;
                }
                if (bcop == NULL) {
                    indb = c + 3;
                }
                else {
                    if (acop->entry.row == row && bcop->entry.row == row) {
                        inda = acop->entry.col;
                        indb = bcop->entry.col;
                    }
                    else {
                        if (acop->entry.row != row) inda = c + 2;
                        if (bcop->entry.row != row) indb = c + 2;
                    }
                }
                //printf("acop rc = (%d, %d), bcop rc = (%d,%d)\n", acop->entry.row, acop->entry.col, bcop->entry.row, bcop->entry.col);
                //printf("inda = %d, indb = %d\n", inda, indb);
                if (inda < indb) {
                    if (inda == col) {
                        value = acop->entry.value;
                        acop = acop->right;
                    }
                    else { value = 0; }
                }
                else if (inda > indb) {
                    if (indb == col) {
                        value = bcop->entry.value;
                        bcop = bcop->right;
                    }
                    else { value = 0; }
                }
                else {
                    if (indb == col) {
                        value = acop->entry.value + bcop->entry.value;
                        acop = acop->right; bcop = bcop->right;
                    }
                    else { value = 0; }
                }

                if (value != 0) {
                    NumEntries++;
                    temp = malloc(sizeof(MatrixNode));
                    temp->tag = entry;
                    temp->entry.row = row;
                    temp->entry.col = col;
                    temp->entry.value = value;

                    last->right = temp;          // 행리스트에 연결
                    last = temp;
                    hdnode[col]->next->down = temp;  // 열리스트에 연결
                    hdnode[col]->next = temp;
                }
            }
        }
        node->entry.value = NumEntries;
        // close all column lists 
        for (i = 0; i < c; i++)
            hdnode[i]->next->down = hdnode[i];

        // link all head nodes together
        for (i = 0; i < NumHeads - 1; i++)
            hdnode[i]->next = hdnode[i + 1];

        hdnode[NumHeads - 1]->next = node;
        node->right = hdnode[0];
    }
    return node;
}
void mwrite(matrix_pointer node)
{
    int r = node->entry.row;
    int c = node->entry.col;
    matrix_pointer head, temp;
    // matrix dimensions
    printf("\nrows = %d, cols = %d, terms = %d\n", node->entry.row,
        node->entry.col, node->entry.value);
    printf("< The Matrix >\n");

    //전체가 0인 행렬일 때
    if (node->right == NULL) {
        for (int i = 0; i < node->entry.row; i++) {
            for (int j = 0; j < node->entry.col; j++) {
                printf("0 ");
            }
            printf("\n");
        }
        return;
    }

    head = node->right;
    temp = head->right;
    for (int i = 0; i < r; i++) {
        //print out the entries in each row
        //temp = head->right;
        for (int j = 0; j < c; j++) {
            if (j == temp->entry.col) {
                printf("%d ", temp->entry.value);
                if (temp->right != NULL) {

                temp = temp->right;
                }
            }
            else printf("0 ");
            //printf("j = %d, temp->entry.col = %d\n", j, temp->entry.col);
        }
        printf("\n");
        //head = head->next;
    }
}


void merase(matrix_pointer* node)
{
    matrix_pointer head = (*node)->right;
    matrix_pointer x, y, next, newnode;
    // 새로운 노드 할당
    newnode = malloc(sizeof(MatrixNode));
    newnode->tag = entry;
    newnode->entry.row = (*node)->entry.row;
    newnode->entry.col = (*node)->entry.col;
    newnode->entry.value = 0;
    newnode->right = NULL;

    // free each cols
    for (int i = 0; i < (*node)->entry.row; i++) {
        y = head;
        while (y != head) {
            next = y->right;
            free(y);
            y = next;
        }
        head = head->next;
    }
    // free each rows
    y = head;
    while (y != *node) {
        x = y;
        y = y->next;
        free(x);
    }
    // change node
    free(*node);
    (*node) = newnode;
    printf("FINISH ERASING!\n");
}

