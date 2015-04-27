#ifndef WORLD_HPP
#define WORLD_HPP

#include <vector>
#include <stdexcept>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include "linalg.hpp"
#include "body.hpp"

// grid used to find particle neighbors
class grid
{
    unsigned int n_cells;
    // stores the occupying particle indexes in each cell
    std::vector<std::vector<unsigned int>> cells;
    // stores the neighbors of each particle
    std::vector<std::vector<unsigned int>> neighbors; 
    // which cells should be searched for each given center cell
    std::vector<std::vector<unsigned int>> search_cells; 
    // cell numbers of the spheres
    std::vector<unsigned int> sphere_cells; 
    // the length of each dimension, centered at 0
    std::vector<double> box;
    // number of cells in each dimension
    std::vector<unsigned int> c;
    private:
        // find the cell number of the given sphere
        unsigned int get_sphere_cell (const sphere &);
        // sets which cells to look in for overlaps, should be called in constructor
        void set_search_cells ();
        // set cell # for spheres and also mark which cells have spheres
        void set_sphere_cells (const std::vector<sphere> &);
    public:
        grid ();
        grid (std::vector<unsigned int>, std::vector<double>);
        ~grid ();
        // adding spheres without recomputing entire grid
        void add (const sphere &, const unsigned int);
        // removing spheres without recomputing entire grid
        void remove (const unsigned int);
        // update the grid info for a given index
        void update (const unsigned int);
        // get neighbors of a sphere
        std::vector<unsigned int> get_neighbors (const unsigned int); 
        // make grid and neighbor list from scratch
        void complete_refresh (const std::vector<sphere> &);
};

// TODO: oct-tree variant of grid

// Handles tracking spheres and finding neighbors etc
class world
{
    // should choose timestep according to dt <= R/(100*U), and grid L ~ 2R

    friend class boost::serialization::access;
    template <class Archive> void serialize (Archive & ar, unsigned int version)
    {
        ar & spheres;
        ar & bricks;
        ar & bi;
        ar & t & dt;
    }
    std::vector<sphere> spheres; // list of spheres
    std::vector<brick> bricks;
    body_interactor bi; // needs forces
    double t, dt;
    std::vector<unsigned int> c = { 6, 6, 6 };
    std::vector<double> box = { 1.0, 1.0, 1.0 };
    // time stepper?
    // if a timestep and grid size is set oorrectly, then the grid should not need
    // to be updated more than once every 100 steps, since that should be the 
    // minimum time for spheres to leave cells.
    const unsigned int grid_update_steps = 50;
    unsigned int grid_step_counter;
    grid g;
    private:
        void update_flags (); // update sphere flags i.e. based on box boundaries
        void clean (); // remove spheres flagged for deletion
    public:
        // TODO: timestep, gather+apply impulses, save/load state
        world ();
        world (const vec3d, const body_interactor, const double);
        void add_sphere (sphere);
        void add_brick (brick);
        unsigned int num_spheres ();
        unsigned int num_bricks ();
        sphere get_sphere (unsigned int);
        brick get_brick (unsigned int);
        void step ();
        void step (unsigned int);
};

#endif // WORLD_HPP
