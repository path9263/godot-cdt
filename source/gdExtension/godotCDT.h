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

protected:
    static void _bind_methods();

public:
    ConstrainedTriangulation();
    ~ConstrainedTriangulation();

    void insert_vertices(PackedVector2Array vertices);
    void insert_edges(PackedInt32Array edges);

    void erase_super_triangle();
    void erase_outer_triangles();
    void erase_outer_triangles_and_holes();

    PackedVector2Array get_all_vertices();

    PackedInt32Array get_all_triangles();



};

}

#endif