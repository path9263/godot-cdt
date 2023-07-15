#ifndef godotCDT_H
#define godotCDT_H


#include <godot_cpp/classes/node.hpp>

namespace godot {

class ConstrainedTriangulation  : public Node {
    GDCLASS(ConstrainedTriangulation , Node)

private:
    double testVar;

protected:
    static void _bind_methods();

public:
    ConstrainedTriangulation();
    ~ConstrainedTriangulation();

    void _process(double delta);

    double getTestVar();
};

}

#endif