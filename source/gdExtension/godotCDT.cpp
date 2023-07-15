#include "godotCDT.h"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void ConstrainedTriangulation::_bind_methods() {
}

ConstrainedTriangulation::ConstrainedTriangulation() {
    // Initialize any variables here.
    testVar = 50.0;
}

ConstrainedTriangulation::~ConstrainedTriangulation() {
    // Add your cleanup here.
}

void ConstrainedTriangulation::_process(double delta) {
    //
}

double ConstrainedTriangulation::getTestVar(){
    testVar += 1;
    return testVar;
}