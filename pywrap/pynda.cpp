#include <world.hpp>
#include <body.hpp>
#include <linalg.hpp>
#include <interactor.hpp>
#include <boost/python.hpp>

using namespace boost::python;

BOOST_PYTHON_MODULE (pynda)
{
    // for function overloading
    void (world::*step1) (void) = &world::step;
    void (world::*step2) (unsigned int) = &world::step;
    class_<world> ("world")
        .def (init<> ())
        .def (init<vec3d, interactor, double> ())
        .def ("save", &world::save)
        .def ("load", &world::load)
        .def ("add_sphere", &world::add_sphere)
        .def ("num_spheres", &world::num_spheres)
        .def ("get_sphere", &world::get_sphere)
        .def ("add_brick", &world::add_brick)
        .def ("num_bricks", &world::num_bricks)
        .def ("get_brick", &world::get_brick)
        .def ("step", step1)
        .def ("step", step2)
    ;

    class_<vec3d> ("vec3d")
        .def (init<> ())
        .def (init<double, double, double> ())
        .def ("__getitem__", &vec3d::get)
        .def ("__setitem__", &vec3d::set)
    ;

    class_<interactor> ("interactor")
        .def (init<> ())
        .def (init<double, double> ())
    ;

    class_<brick> ("brick")
        .def (init<> ())
        .def (init<vec3d, vec3d> ())
        .def_readwrite ("x", &brick::x)
        .def_readwrite ("L", &brick::L)
    ;

    // begin sphere scope
    { 
        scope sphere_scope = class_<sphere> ("sphere")
            .def (init<> ())
            .def (init<double, double, double, vec3d, vec3d, vec3d> ())
            .def_readwrite ("m", &sphere::m)
            .def_readwrite ("r", &sphere::r)
            .def_readwrite ("I", &sphere::I)
            .def_readwrite ("x0", &sphere::x0)
            .def_readwrite ("x", &sphere::x)
            .def_readwrite ("v", &sphere::v)
            .def_readwrite ("q", &sphere::q)
            .def_readwrite ("w", &sphere::w)
            .def_readwrite ("flag", &sphere::flag)
        ;

        enum_<sphere::state> ("state")
            .value ("moving", sphere::state::moving)
            .value ("fixed", sphere::state::fixed)
            .value ("kill", sphere::state::kill)
        ; 
    }
    // end sphere scope
}
