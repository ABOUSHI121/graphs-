#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define null NULL
#define MAX_STRING 250
#define  INF 99999
#define MAX_VERTICES 100
typedef char String[MAX_STRING];
typedef struct Node Node;
struct Node {
    char *city;
    int distance;
    Node *prev;
    Node *next[MAX_VERTICES];
    int num_next;
};
typedef struct {
    Node *data[MAX_VERTICES];
    int front;
    int rear;
} Queue;
int V = 0; //VARIABLE TO STORE THE NUMBER OF VERTICES
// avl tree functions in orded to use it as a SET
typedef struct AVLnode *pAvl;
typedef struct AVLnode {
    String cityName;//key
    int indx;//index of the course in the graph
    pAvl left;
    pAvl right;
    int height; //Balance information
} AVLnode;
int height(pAvl P) {
    if (P == NULL) {
        return -1;
    }
    else {
        return P->height;
    }
}
int max(int Lhs, int Rhs) {
    return Lhs > Rhs ? Lhs : Rhs;
}
pAvl singleRotateWithLeft(pAvl K2) {
    pAvl K1;
    K1 = K2->left;
    K2->left = K1->right;
    K1->right = K2;
    K2->height = max(height(K2->left), height(K2->right)) + 1;
    K1->height = max(height(K1->left), K2->height) + 1;
    return K1;  /* New root */
}
pAvl singleRotateWithRight(pAvl K1) {
    pAvl K2;
    K2 = K1->right;
    K1->right = K2->left;
    K2->left = K1;
    K1->height = max(height(K1->left), height(K1->right)) + 1;
    K2->height = max(height(K2->right), K1->height) + 1;
    return K2;  /* New root */
}

/* This function can be called only if K3 has a left */
/* child and K3's left child has a right child */
/* Do the left-right double rotation */
/* Update heights, then return new root */
Queue *create_queue() {
    Queue *q = (Queue *) malloc(sizeof(Queue));
    q->front = -1;
    q->rear = -1;
    return q;
}
pAvl doubleRotateWithLeft(pAvl K3) {
    /* Rotate between K1 and K2 */
    K3->left = singleRotateWithRight(K3->left);
    /* Rotate between K3 and K2 */
    return singleRotateWithLeft(K3);
}
/* END */
/* This function can be called only if K1 has a right */
/* child and K1's right child has a left child */
/* Do the right-left double rotation */
/* Update heights, then return new root */
pAvl doubleRotateWithRight(pAvl K1) {
    /* Rotate between K3 and K2 */
    K1->right = singleRotateWithLeft(K1->right);
    /* Rotate between K1 and K2 */
    return singleRotateWithRight(K1);
}
pAvl insert(AVLnode treeNode, pAvl T) {
    if (T == NULL) {
        /* Create and return a one-listNode tree */
        T = malloc(sizeof(AVLnode));
//        V++;
        if (T == NULL) {
            printf("Out of space!!!");
        }
        else {
            //HERE WE ASSIGN THE VALUES
            *T = treeNode;
            T->height = 0;
            T->left = T->right = NULL;
        }
    }
    else if (strcmp(treeNode.cityName, T->cityName) < 0) {
        T->left = insert(treeNode, T->left);
        if (height(T->left) - height(T->right) == 2) {
            if (strcmp(treeNode.cityName, T->left->cityName) < 0) {
                T = singleRotateWithLeft(T);
            }
            else {
                T = doubleRotateWithLeft(T);
            }
        }
    }
    else if (strcmp(treeNode.cityName, T->cityName) > 0) {

        T->right = insert(treeNode, T->right);
        if (height(T->right) - height(T->left) == 2) {
            if (strcmp(treeNode.cityName, T->right->cityName) > 0) {
                T = singleRotateWithRight(T);
            }
            else {
                T = doubleRotateWithRight(T);
            }
        }
    }
    else {

        T->height = max(height(T->left), height(T->right)) + 1;
        return T;
    }
    /* Else cityName is in the tree already; we'll do nothing */

    T->height = max(height(T->left), height(T->right)) + 1;
    return T;

}
int getIndex(String cityName, pAvl T) {
    if (T == NULL) {
        return -1;
    }
    if (strcmp(cityName, T->cityName) < 0) {
        return getIndex(cityName, T->left);
    }
    else if (strcmp(cityName, T->cityName) > 0) {
        return getIndex(cityName, T->right);
    }
    else {
        return T->indx;
    }
}
pAvl SET = null;
int cnt = 0;
bool is_empty(Queue *q) {
    return q->rear == -1;
}
void enqueue(Queue *q, Node *value) {
    if (q->rear == MAX_VERTICES- 1)
        printf("\nQueue is Full!!");
    else {
        if (q->front == -1)
            q->front = 0;
        q->rear++;
        q->data[q->rear] = value;
    }
}

Node *dequeue(Queue *q) {
    Node *value;
    if (is_empty(q)) {
        printf("\nQueue is Empty!!");
        return NULL;
    } else {
        value = q->data[q->front];
        q->front++;
        if (q->front > q->rear) {
            q->front = q->rear = -1;
        }
    }
    return value;
}
Node *nodes[MAX_VERTICES];
int num_nodes = 0;
Node *create_node(char *city) {
    Node *node = (Node *) malloc(sizeof(Node));
    if (node == NULL) {
        printf("Unable to allocate memory for node\n");
        exit(1);
    }
    node->city = strdup(city);
    node->distance = INF;
    node->prev = NULL;
    node->num_next = 0;
    return node;
}
void load_cities() {
    FILE *file = fopen("C:\\Users\\hp\\CLionProjects\\untitled108\\i", "r");
    if (file == NULL) {
        printf("Could not open the file \n");
        exit(1);
    }
    char city1[MAX_STRING], city2[MAX_STRING];
    int distance;
    while (fscanf(file, "%s %s %dkm\n", city1, city2, &distance) == 3) {
        Node *node1 = NULL, *node2 = NULL;
        for (int i = 0; i < V; i++) {
            if (strcmp(nodes[i]->city, city1) == 0) node1 = nodes[i];
            if (strcmp(nodes[i]->city, city2) == 0) node2 = nodes[i];
        }
        if (node1 == NULL) {
            node1 = create_node(city1);
            nodes[num_nodes++] = node1;
        }
        if (node2 == NULL) {
            node2 = create_node(city2);
            nodes[num_nodes++] = node2;
        }
        node1->next[node1->num_next++] = node2;
        node2->next[node2->num_next++] = node1;
    }

    fclose(file);
}
void bfs(char *source, char *destination) {
    Queue *q = create_queue();
    Node *src = NULL, *dst = NULL;
    for (int i = 0; i < V; i++) {
        if (strcmp(nodes[i]->city, source) == 0) src = nodes[i];
        if (strcmp(nodes[i]->city, destination) == 0) dst = nodes[i];
        nodes[i]->distance = i == 0 ? 0 : INF;
    }
    enqueue(q, src);
    while (!is_empty(q)) {
        Node *node = dequeue(q);
        if (node == dst) break;
        for (int i = 0; i < node->num_next; i++) {
            Node *next = node->next[i];
            if (next->distance == INF) {
                next->distance = node->distance + 1;
                next->prev = node;
                enqueue(q, next);
            }
        }
    }
    Node *node = dst;
    while (node != NULL) {
        printf("%s <- ", node->city);
        node = node->prev;
    }
    printf("\n");
}
void displayRoute(int *parent, int *dist, String srcCity, String destCity, String cityArray[]) {
    // display the route from source to destination city using the parent array and distance array from dijkstra algorithm
    FILE *out = fopen("C:\\Users\\hp\\CLionProjects\\untitled108\\o", "a");// append to the file
    if (out == null) {
        printf("COULDN'T OPEN THE OUTPUT FILE\n");// if the file couldn't be opened print an error message and exit the program
    }

    int parentVertix = parent[getIndex(destCity,
                                       SET)]; // get the parent of the destination city from the parent array and set it to parentVertix variable (the parent of the destination city)
    printf("%s", destCity);
    fprintf(out, "%s", destCity);

    int d1;
    d1 = getIndex(destCity, SET);
    int total = 0;
    while (parentVertix !=-1) { // while the parent of the destination city is not -1 ( the destination city is not the source city)
        total += abs(dist[d1] - dist[parentVertix]);

        printf("  (%dKm)<-  %s ", abs(dist[d1] - dist[parentVertix]), cityArray[parentVertix]); //
        fprintf(out, "  (%dKm)<-  %s ", abs(dist[d1] - dist[parentVertix]), cityArray[parentVertix]);

        d1 = parentVertix;
        parentVertix = parent[parentVertix]; // get the parent of the parent city and set it to parentVertix variable (the parent of the parent city)

    }
    printf("\n");

    printf("total destination : %dKm \n", total);

    fprintf(out, "\t| (total destination : %dKm) \n", total);
    fclose(out);
}
int getVsize(pAvl t, String CITITIES) {

    int dummy = 0;
    if (t != NULL) {
        dummy = getVsize(t->left, CITITIES);
        t->indx = cnt;
        strcat(CITITIES, t->cityName);
        strcat(CITITIES, "\t");
        cnt++;
        V++;
        dummy = getVsize(t->right, CITITIES);
    }
    return V;
}
void menu() {
    printf("1. Load cities\n"
           "2. Enter source city:\n"
           "3. Enter destination city:\n"
           "4. Exit:\n");
}
void readV() {
    FILE *in = fopen("C:\\Users\\hp\\CLionProjects\\untitled108\\i", "r");
    if (in == NULL) {
        printf("Error opening file\n");
        exit(1);
    }
    String city1;
    String city2;
    String cost;
    while (fscanf(in, "%s%s%s", city1, city2, cost) != EOF) {
        pAvl tmpNode1 = malloc(sizeof(AVLnode));
        strcpy(tmpNode1->cityName, city1);

        pAvl tmpNode2 = malloc(sizeof(AVLnode));
        strcpy(tmpNode2->cityName, city2);

        SET = insert(*tmpNode1, SET);
        SET = insert(*tmpNode2, SET);
    }
    fclose(in);
}
void assignToGraph(int GraphDik[V][V],int GraphBFS[V][V]) {
    FILE *in = fopen("C:\\Users\\hp\\CLionProjects\\untitled108\\i", "r");
    String city1, city2;
    String strCost;
    char *num;
    while (fscanf(in, "%s%s%s", city1, city2, strCost) != EOF) {

        num = strtok(strCost, "[Kk][Mm]"); // capital K or k for kilometers and M or m for miters
        int cost = atoi(num);
        GraphDik[getIndex(city1, SET)][getIndex(city2, SET)] = cost;
        GraphDik[getIndex(city2, SET)][getIndex(city1, SET)] = cost;
        if (cost==0) {
            GraphBFS[getIndex(city1, SET)][getIndex(city2, SET)] = 0;
            GraphBFS[getIndex(city2, SET)][getIndex(city1, SET)] = 0;
        }

        else {
            GraphBFS[getIndex(city1, SET)][getIndex(city2, SET)] = 1;
            GraphBFS[getIndex(city2, SET)][getIndex(city1, SET)] = 1;
        }
    }
    fclose(in);
}
void dijkstra(int Graph[V][V], int src, String allCities, int dist[V], bool visited[V], int parent[V], String cityArray[]) {
    int i, j;
    int min;
    int next;
    for (i = 0; i <
                V; i++) { // initialize the distance array and visited array to infinity and false respectively for all vertices
        dist[i] = INF;
        visited[i] = false;
        parent[i] = -1;
    }
    dist[src] = 0; // source node is always 0 distance from itself
    parent[src] = -1; // parent of source is set to -1 since source is the parent of itself
    for (i = 0; i < V - 1; i++) { // run for V-1 times since we have already visited source node once in the beginning
        // iterates over Dist array to find the minimum distance vertex available so far
        min = INF;
        for (j = 0; j <
                    V; j++) { // run for all vertices to find the minimum distance vertex from the set of unvisited vertices and mark it as visited
            if (visited[j] == false && dist[j] <= min) {
                min = dist[j];
                next = j; // next is the vertex with minimum distance from the set of unvisited vertices
            }
        }
        visited[next] = true; // the minimum distance vertex is marked as visited
        for (j = 0; j < V; j++) {
            // update the distance array by comparing the distance of the vertex from the set of unvisited vertices with the distance of the vertex from the set of visited vertices
            // comparing edges from the next vertex to all of its edges
            //j represents the  index of the edges to 'next' in the Graph
            if (visited[j] == false && Graph[next][j] && dist[next] != INF && dist[next] + Graph[next][j] < dist[j]) {
                parent[j] = next;
                dist[j] = dist[next] + Graph[next][j];
            }
        } // update the distance of all the vertices adjacent to the minimum distance vertex from the set of unvisited vertices
    }

    int z = 0;
    char *tmp = null;

    tmp = strtok(allCities, "\t"); // split the string by tabs
    while (tmp != NULL) {

        printf("%s:", tmp);
        printf("\t%d Km", dist[z]);
        printf("\n");
        strcpy(cityArray[z], tmp);
        tmp = strtok(NULL, "\t");
        z++;
    }
    printf("\n");
    strcpy(allCities, ""); // clear the string
    for (int k = 0; k < V; ++k) {
        strcat(allCities,
               cityArray[k]); // concatenate the cities to the string again cuz it changes when we split it by tabs
        strcat(allCities, "\t"); // add a tab after each city
    }
}void displayRoute(int parent[], int dist[], String strSrc, String strDest,String cityArray[V]);//display the route from source to destination
int main() {
    readV();
    String allCities = "";
    int dummy = getVsize(SET, allCities);
    String cityArray[V];
    int dist[V];
    bool visited[V];
    int parent[V];
    int GraphDik[V][V]; //Graph[i][j] = x is the edge from i to j
    int GraphBFS[V][V];
    for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j) {
            GraphDik[i][j] = INF;
        }

    }
    int selection;
    String srcCity;
    String destCity;
    String source;
    String destination;
    int srcIndx;
    bool selected = false;
    bool selected2 = false;
    while (true) {
        menu();
        printf("CITIES TO TRAVERSE : \n");
        printf("%s\n", allCities);
        scanf("%d", &selection);
        if (selection == 1) {
            selected = true;
            assignToGraph(GraphDik,GraphBFS);
            for (int i = 0; i < V; ++i) {
                for (int j = 0; j < V; ++j) {
                    if (i == j) {
                        GraphDik[i][j] = 0; //no edge between same city
                        GraphBFS[i][j] = 0;
                    }
                }
            }

        }
        else if (selection == 2) {
            selected2 = true;
            if (!selected) {
                printf("Please load graph first\n");
                continue;
            }
            printf("PLEASE ENTER SOURCE CITY\n");
            scanf("%s", srcCity);
            strcpy(source,srcCity);
            srcIndx = getIndex(srcCity, SET); // gets the index of the city in the tree
            source[strcspn(source, "\n")] = 0;
            if (srcIndx == -1) {
                printf("\nSOURCE CITY NOT FOUND\n");
                continue;
            }
            printf("shortest paths from %s to : \n", srcCity);
            dijkstra(GraphDik, srcIndx, allCities, dist, visited, parent,cityArray);//dijkstra algorithm to find the shortest path from source to destination city
        }
        else if (selection == 3) {
            if (!selected2) {
                printf("Please choose source destination f first\n");
                continue;
            }
            if (!selected) {
                printf("Please load graph first\n");
                continue;
            }
            printf("PLEASE ENTER DESTINATION CITY\n");
            scanf("%s", destCity);
            strcpy(destination,destCity);
            int destIndx = getIndex(destCity, SET);
            if (destIndx == -1) {
                printf("\nDESTINATION CITY NOT FOUND\n");
                continue;
            }
            printf("BFS:\n");
            destCity[strcspn(destination, "\n")] = 0;
            bfs(source, destination);
            printf("Dijkstra:\n");
            displayRoute(parent, dist, srcCity, destCity, cityArray);
        }
        else if (selection ==4) { // note: we auto save to the file each time in 3rd selection number 4 just exits the program
            break;
        }
        else {
            printf("Invalid Selection\n");
            continue;
        }
    }
}
