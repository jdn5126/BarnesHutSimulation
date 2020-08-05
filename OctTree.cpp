/*
 * Copyright 2020 Bryson Banks, David Campbell, and Jeffrey Nelson.  All rights reserved.
 *
 * BarnesHutSimulation: OctTree.cpp
 */

#include <assert.h>
#include <iostream>
#include <map>
#include <thread>
#include <vector>
#include "OctTree.h"

static vector_3d
average(const vector_3d &lowerBound, const vector_3d &upperBound) {
    double xPos = (std::get<X>(lowerBound) + std::get<X>(upperBound)) / 2;
    double yPos = (std::get<Y>(lowerBound) + std::get<Y>(upperBound)) / 2;
    double zPos = (std::get<Z>(lowerBound) + std::get<Z>(upperBound)) / 2;
    return (vector_3d)std::make_tuple(xPos, yPos, zPos);
}

static std::pair<vector_3d, vector_3d>
getBounds(Root *root, int octet) {
    vector_3d lowerBound;
    vector_3d upperBound;
    double x, y;

    // Return bounds for new root node within octet
    // TODO: Figure out Z coordinates...
    if( octet == 0 ) {
        lowerBound = root->pos;
        upperBound = root->upperBound;
    } else if( octet == 2 ) {
        x = std::get<X>(root->lowerBound);
        y = std::get<Y>(root->pos);
        lowerBound = std::make_tuple(x, y, 0);
        x = std::get<X>(root->pos);
        y = std::get<Y>(root->upperBound);
        upperBound = std::make_tuple(x, y, 0);
    } else if( octet == 4) {
        lowerBound = root->lowerBound;
        upperBound = root->pos;
    } else if( octet == 6) {
        x = std::get<X>(root->pos);
        y = std::get<Y>(root->lowerBound);
        lowerBound = std::make_tuple(x, y, 0);
        x = std::get<X>(root->upperBound);
        y = std::get<Y>(root->pos);
        upperBound = std::make_tuple(x, y, 0);
    }
    return std::pair<vector_3d, vector_3d>(lowerBound, upperBound);
}

int
OctTree::findOctet(const vector_3d &rootPos, const vector_3d &bodyPos) {
    double rootX = std::get<X>(rootPos);
    double rootY = std::get<Y>(rootPos);
    double rootZ = std::get<Z>(rootPos);
    double particleX = std::get<X>(bodyPos);
    double particleY = std::get<Y>(bodyPos);
    double particleZ = std::get<Z>(bodyPos);

    if( particleX >= rootX && particleY >= rootY && particleZ >= rootZ ) {
        return 0;
    } else if( particleX >= rootX && particleY >= rootY && particleZ < rootZ ) {
        return 1;
    } else if( particleX < rootX && particleY >= rootY && particleZ >= rootZ ) {
        return 2;
    } else if( particleX < rootX && particleY >= rootY && particleZ < rootZ ) {
        return 3;
    } else if( particleX < rootX && particleY < rootY && particleZ >= rootZ ) {
        return 4;
    } else if( particleX < rootX && particleY < rootY && particleZ < rootZ ) {
        return 5;
    } else if( particleX >= rootX && particleY < rootY && particleZ >= rootZ ) {
        return 6;
    } else if( particleX >= rootX && particleY < rootY && particleZ < rootZ ) {
        return 7;
    } else {
        assert(false && "cannot determine octet");
    }
}

// Construct OctTree given list of particles
OctTree::OctTree(std::vector<Leaf *> &particles, vector_3d lowerBound,
                 vector_3d upperBound) {
    // Construct root of the tree
    this->root = new Root(nullptr, lowerBound, upperBound);

    // Insert particles into tree in parallel
    insertParticles(particles);
}

// Destroy OctTree by freeing memory allocated for root
OctTree::~OctTree() {
    delete root;
}

void
OctTree::insert(Leaf *particle) {
    int octet = findOctet(this->root->pos, particle->body.pos);
    insertParticle(this->root, particle, octet);
}

// Insert new particle into tree rooted at root
void
OctTree::insertParticle(Root *root, Leaf *particle, const int octet) {
    Node *child = root->children[octet];

    // If octet is empty, insert leaf at octet
    if (child == nullptr) {
        root->children[octet] = particle;
        root->numChildren += 1;
        particle->octet = octet;
        particle->parent = root;
    } else if (child->isLeaf()) {
        // If child is a leaf, construct a new subtree and re-insert child
        // along with particle.
        std::pair<vector_3d, vector_3d> bounds = getBounds(root, octet);
        Root *newRoot = new Root(root, bounds.first, bounds.second);
        newRoot->octet = octet;
        root->children[octet] = newRoot;
        Leaf *leaf = (Leaf *)child;
        insertParticle(newRoot, leaf,
                findOctet(newRoot->pos, leaf->body.pos));
        insertParticle(newRoot, particle,
                findOctet(newRoot->pos, particle->body.pos));
    } else {
        // Continue down tree
        Root *newRoot = (Root *)child;
        insertParticle(newRoot, particle,
                findOctet(newRoot->pos, particle->body.pos));
    }
}

// Insert particles into OctTree in parallel
void
OctTree::insertParticles(std::vector<Leaf *> &particles) {
    std::map<int, std::thread> threadPool;
    for (Leaf *particle : particles) {
        // Determine octet for particle
        int octet = findOctet(this->root->pos, particle->body.pos);
        // If existing thread is already executing on octet, wait for it to complete.
        std::map<int, std::thread>::iterator it = threadPool.find(octet);
        if (it != threadPool.end()) {
            it->second.join();
        }
        // Launch thread to insert particle into octet.
        threadPool[octet] =
            std::thread(&OctTree::insertParticle, this, this->root, particle, octet);
    }
    // Join all in-flight threads
    std::map<int, std::thread>::iterator it;
    for (it = threadPool.begin(); it != threadPool.end(); ++it) {
        it->second.join();
    }
}

void
OctTree::remove(Leaf *particle) {
    // Invalidate parent pointer
    Root *parent = (Root *)particle->parent;
    particle->parent = nullptr;
    parent->children[particle->octet] = nullptr;
    parent->numChildren -= 1;

    // Replace parent with Leaf if necessary
    maybeReplaceRoot(parent);
}

void
OctTree::maybeReplaceRoot(Root *root) {
    if (root->numChildren == 0 && root != this->root) {
        // If root has zero children, and it is not the root of the OctTree,
        // remove root.
        Root *parent = (Root *)root->parent;
        parent->children[root->octet] = nullptr;
        parent->numChildren -= 1;
        delete root;
        // Recurse on parent
        maybeReplaceRoot(parent);
    } else if (root->numChildren == 1) {
        // If root has only one child, and that child is a Leaf, replace root
        Leaf *leaf = nullptr;
        for (int i=0; i < 8; ++i) {
            Node *node = root->children[i];
            if (node != nullptr && node->isLeaf()) {
                leaf = (Leaf *)node;
                // Clear root pointer to leaf so that it does not try to delete it
                // in Root destructor.
                root->children[i] = nullptr;
                break;
            }
        }
        if (leaf != nullptr) {
            // Replace root with leaf
            Root *parent = (Root *)root->parent;
            int octet = root->octet;
            parent->children[octet] = leaf;
            leaf->parent = parent;
            leaf->octet = octet;
            delete root;
            // Recurse on parent
            maybeReplaceRoot(parent);
        }
    }
}

void
OctTree::setCenterOfMass() {
    // Calculate center of mass for each Root node through recursion
    centerOfMass(this->root);
}

void
OctTree::centerOfMass(Root *root) {
    std::vector<std::thread> threadPool;
    // Spawn thread for each Root node
    for (int i=0; i < 8; i++) {
        Node *child = root->children[i];
        if (child != nullptr && !child->isLeaf()) {
            Root *newRoot = (Root *)child;
            threadPool.push_back(std::thread(&OctTree::centerOfMass, this, newRoot));
        }
    }
    // Wait for all threads to join
    for (int i=0; i < threadPool.size(); i++) {
        threadPool[i].join();
    }
    // Set center of mass for node
    double x, y, z;
    for (int i=0; i < 8; i++) {
        Node *child = root->children[i];
        if (child == nullptr) {
            continue;
        } else if (child->isLeaf()) {
            Leaf *leaf = (Leaf *)child;
            x += (leaf->body.mass * std::get<X>(leaf->body.pos));
            y += (leaf->body.mass * std::get<Y>(leaf->body.pos));
            z += (leaf->body.mass * std::get<Z>(leaf->body.pos));
            root->mass += leaf->body.mass;
        } else {
            Root *rootChild = (Root *)child;
            x += (rootChild->mass * std::get<X>(rootChild->centerOfMass));
            y += (rootChild->mass * std::get<Y>(rootChild->centerOfMass));
            z += (rootChild->mass * std::get<Z>(rootChild->centerOfMass));
            root->mass += rootChild->mass;
        }
    }
    // Update center of mass
    root->centerOfMass = std::make_tuple(x / root->mass, y / root->mass, z / root->mass);
}

void
OctTree::print() {
    std::cout << *(this->root) << std::endl;
    printRecurse(this->root);
}

void
OctTree::printRecurse(Root *root) {
    // Fixed iteration order to help with debugging
    for(int i=0; i < 8; i++) {
        Node *child = root->children[i];
        if( child == nullptr ) {
            continue;
        }
        if( child->isLeaf() ) {
            Leaf *leaf = (Leaf *)child;
            std::cout << *leaf << std::endl;
        } else {
            Root *newRoot = (Root *)child;
            std::cout << *newRoot << std::endl;
            printRecurse(newRoot);
        }
    }
}

// Default constructor for Node
Node::Node(Node *parent) {
    this->parent = parent;
}

// Leaf Constructor
Leaf::Leaf(Node *parent, Body &&body) : Node(parent) {
    this->body = body;
}

// Root Constructor
Root::Root(Node *parent, vector_3d lowerBound, vector_3d upperBound ) : Node(parent) {
    this->lowerBound = lowerBound;
    this->upperBound = upperBound;
    this->pos = average(lowerBound, upperBound);
    this->children = new Node *[8];
    for (int i=0; i < 8; ++i) {
        this->children[i] = nullptr;
    }
    this->numChildren = 0;
}

Root::~Root() {
    // Delete children
    for (int i=0; i < 8; ++i) {
        delete this->children[i];
    }
}

std::ostream& operator<<(std::ostream& out, const Leaf& l) {
    out << "Leaf " << l.body.id << ": (" << std::get<X>(l.body.pos) << ", " <<
        std::get<Y>(l.body.pos) << ", " << std::get<Z>(l.body.pos) << ")";
    return out;
}

std::ostream& operator<<(std::ostream& out, const Root& r) {
    out << "Root(" << std::get<X>(r.pos) << ", " << std::get<Y>(r.pos) << ", " <<
        std::get<Z>(r.pos) << ") mass: " << r.mass << " center of mass: (" <<
        std::get<X>(r.centerOfMass) << ", " << std::get<Y>(r.centerOfMass) << ", " <<
        std::get<Z>(r.centerOfMass) << ")";
    return out;
}
