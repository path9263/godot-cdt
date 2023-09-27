#ifndef godotCDT_H
#define godotCDT_H


#include <godot_cpp/classes/resource.hpp>

#include "../CDT/CDT/include/CDT.h"
using Triangulation = CDT::Triangulation<float>;


namespace godot {

class ConstrainedTriangulation  : public Resource {
    GDCLASS(ConstrainedTriangulation , Resource)

private:
    Triangulation triangulation = Triangulation(CDT::VertexInsertionOrder::Auto, CDT::IntersectingConstraintEdges::Resolve, 0.1);
    int vert_count = 0;
    CDT::VerticesTriangles vertTris; // triangles of each vertex, must be calculated manually with generate_vertex_triangles

protected:
    static void _bind_methods();

public:
    ConstrainedTriangulation();
    ~ConstrainedTriangulation();

    // enum testEnum {AAA, BBB};  // note to self: how to do Enums: https://godotforums.org/d/22886-is-there-a-way-to-expose-c-enums-into-gdscript-or-more-precisely-how-is-it-done-in-the-engine/4  this does work! 

    /* Call once before anything else, calling again will erase triangle data */
    void init(bool optomizeInsertOrder, bool resolveConstrainedEdges, float minDistToConstraintEdge);

    void insert_vertices(PackedVector2Array vertices);
    void insert_edges(PackedInt32Array edges);
    void insert_conforming_edges(PackedInt32Array edges);

    void erase_super_triangle();
    void erase_outer_triangles();
    void erase_outer_triangles_and_holes();

    int get_vertex_count();

    PackedVector2Array get_all_vertices();
    PackedInt32Array get_all_triangles();
    /* Halfedge will be -1 if there is no halfedge */
    PackedInt32Array get_all_halfedges();

    Vector2 get_vertex(int vertIndex);
    Vector3i get_triangle(int triIndex);
    /* Returns only the 3 tris that share edges with the given tri */
    Vector3i get_triangle_neighbors(int triIndex);
    /* This is going to be pretty slow, it is not smart at all: */
    int get_triangle_at_point(Vector2 point);
    /* Generate vertex to triangle relations */
    void generate_vertex_triangles();
    /* returns all the trianges that contain the given vertex, must call generate_vertex_triangles first! */
    PackedInt32Array get_vertex_triangles(int vertIndex);

    void flip_edge(int firstTri, int secondTri);
    void remove_triangles(PackedInt32Array triangles);

};

}

#endif