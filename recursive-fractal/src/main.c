#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "frac_doublelinklist.h"

typedef struct point {
    float x, y, z;
} Point;

typedef struct Triangle {
    Point *point1, *point2, *point3;
} Triangle;

typedef struct mesh {
    DoublyList triangle_array;
} Mesh;

Point* createPoint(float x, float y, float z) {
    Point* p = (Point*)malloc(sizeof(Point));
    p->x = x; p->y = y; p->z = z;
    return p;
}

Triangle* createTriangle(Point* p1, Point* p2, Point* p3) {
    Triangle* t = (Triangle*)malloc(sizeof(Triangle));
    t->point1 = p1; t->point2 = p2; t->point3 = p3;
    return t;
}

void generateFractal(Mesh* mesh, float cx, float cy, float cz, float size, int depth) {//implemented by sait_cglr
    Point* p1 = createPoint(cx+size/2,cy+size/2,cz+size/2);
    Point* p2 = createPoint(cx+size/2,cy+size/2,cz-size/2);
    Point* p3 = createPoint(cx+size/2,cy-size/2,cz+size/2);
    Point* p4 = createPoint(cx+size/2,cy-size/2,cz-size/2);
    Point* p5 = createPoint(cx-size/2,cy+size/2,cz+size/2);
    Point* p6 = createPoint(cx-size/2,cy+size/2,cz-size/2);
    Point* p7 = createPoint(cx-size/2,cy-size/2,cz+size/2);
    Point* p8 = createPoint(cx-size/2,cy-size/2,cz-size/2);
    addBack(&mesh->triangle_array,createTriangle(p1,p2,p6));
    addBack(&mesh->triangle_array,createTriangle(p1,p5,p6));
    addBack(&mesh->triangle_array,createTriangle(p1,p7,p5));
    addBack(&mesh->triangle_array,createTriangle(p1,p7,p3));
    addBack(&mesh->triangle_array,createTriangle(p1,p4,p3));
    addBack(&mesh->triangle_array,createTriangle(p1,p4,p2));
    addBack(&mesh->triangle_array,createTriangle(p8,p6,p5));
    addBack(&mesh->triangle_array,createTriangle(p8,p7,p5));
    addBack(&mesh->triangle_array,createTriangle(p8,p2,p6));
    addBack(&mesh->triangle_array,createTriangle(p8,p2,p4));
    addBack(&mesh->triangle_array,createTriangle(p8,p3,p4));
    addBack(&mesh->triangle_array,createTriangle(p8,p3,p7));
    if(depth == 0){
        return;
    }else{
        depth--;
        generateFractal(mesh,cx+0.75*size,cy,cz,size/2,depth);
        generateFractal(mesh,cx,cy+0.75*size,cz,size/2,depth);
        generateFractal(mesh,cx,cy,cz+0.75*size,size/2,depth);
        generateFractal(mesh,cx-0.75*size,cy,cz,size/2,depth);
        generateFractal(mesh,cx,cy-0.75*size,cz,size/2,depth);
        generateFractal(mesh,cx,cy,cz-0.75*size,size/2,depth);
        
    }
}

void save_stl(const char* filename, Mesh* mesh) {
    FILE* f = fopen(filename, "w");
    if (!f) {
        perror("Error opening STL file");
        return;
    }
    fprintf(f, "solid fractal_cube\n");

    Node* node = mesh->triangle_array.head;
    while (node) {
        Triangle* t = node->data;
        fprintf(f, "facet normal 0 0 0\n");
        fprintf(f, "  outer loop\n");
        fprintf(f, "    vertex %f %f %f\n", t->point1->x, t->point1->y, t->point1->z);
        fprintf(f, "    vertex %f %f %f\n", t->point2->x, t->point2->y, t->point2->z);
        fprintf(f, "    vertex %f %f %f\n", t->point3->x, t->point3->y, t->point3->z);
        fprintf(f, "  endloop\n");
        fprintf(f, "endfacet\n");
        node = node->next;
    }
    fprintf(f, "endsolid fractal_cube\n");
    fclose(f);
}

int main() {
    while (1) {
        int option;
        printf("------------------------------------\n");
        printf("1- Cube Pattern\n");
        printf("2- Quit\n");
        printf("------------------------------------\n");
        printf("Please enter an action: ");
        scanf("%d", &option);

        if (option == 1) {
            int iteration;
            printf("Enter Iteration count: ");
            scanf("%d", &iteration);

            Mesh mesh;
            initDoublyList(&mesh.triangle_array);
            generateFractal(&mesh, 0.0f, 0.0f, 0.0f, 1.0f, iteration);
            save_stl("cube_result.stl", &mesh);
            printf("STL file 'cube_result.stl' generated.\n");
            int sum = 0;
            for(int i = 0 ; i <= iteration ; i++){
                sum += pow(6,i);
            }
            int size = 36 * sum;
            Point** array = (Point**)malloc(sizeof(Point*)*size);
            Node* node = mesh.triangle_array.head;
            int pointIndex = 0;
            while (node) {
                Triangle* t = node->data;
                array[pointIndex++] = t->point1;
                array[pointIndex++] = t->point2;
                array[pointIndex++] = t->point3;
                Node* temp = node;
                node = node->next;
                free(t);
                free(temp); 
            }

            for (int i = 0; i < size; i++) {
                int is_duplicate = 0;
                for (int j = 0; j < i; j++) {
                    if (array[i] == array[j]) {
                        is_duplicate = 1;
                        break;
                    }
                }
                if (!is_duplicate && array[i] != NULL) {
                    free(array[i]);
                }
            }
            free(array); 


        } else if (option == 2) {
            break;
        }
    }
    return 0;
}
