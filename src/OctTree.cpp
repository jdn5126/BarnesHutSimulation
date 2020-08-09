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

static std::pair<vector_3d, vector_3d> getBounds(Root *root, int octet) {
    vector_3d lowerBound;
    vector_3d upperBound;
    double x, y, z;

    // Return bounds for new root node within octet
    if (octet == 0) {
        x = std::get<X>(root->pos);
        y = std::get<Y>(root->pos);
        z = std::get<Z>(root->pos);
        lowerBound = std::make_tuple(x, y, z);
        x = std::get<X>(root->upperBound);
        y = std::get<Y>(root->upperBound);
        z = std::get<Z>(root->upperBound);
        upperBound = std::make_tuple(x, y, z);
    } else if (octet == 1) {
        x = std::get<X>(root->pos);
        y = std::get<Y>(root->pos);
        z = std::get<Z>(root->lowerBound);
        lowerBound = std::make_tuple(x, y, z);
        x = std::get<X>(root->upperBound);
        y = std::get<Y>(root->upperBound);
        z = std::get<Z>(root->pos);
        upperBound = std::make_tuple(x, y, z);
    } else if (octet == 2) {
        x = std::get<X>(root->lowerBound);
        y = std::get<Y>(root->pos);
        z = std::get<Z>(root->pos);
        lowerBound = std::make_tuple(x, y, z);
        x = std::get<X>(root->pos);
        y = std::get<Y>(root->upperBound);
        z = std::get<Z>(root->upperBound);
        upperBound = std::make_tuple(x, y, z);
    } else if (octet == 3) {
        x = std::get<X>(root->lowerBound);
        y = std::get<Y>(root->pos);
        z = std::get<Z>(root->lowerBound);
        lowerBound = std::make_tuple(x, y, z);
        x = std::get<X>(root->pos);
        y = std::get<Y>(root->upperBound);
        z = std::get<Z>(root->pos);
        upperBound = std::make_tuple(x, y, z);
    } else if (octet == 4) {
        x = std::get<X>(root->lowerBound);
        y = std::get<Y>(root->lowerBound);
        z = std::get<Z>(root->pos);
        lowerBound = std::make_tuple(x, y, z);
        x = std::get<X>(root->pos);
        y = std::get<Y>(root->pos);
        z = std::get<Z>(root->upperBound);
        upperBound = std::make_tuple(x, y, z);
    } else if (octet == 5) {
        x = std::get<X>(root->lowerBound);
        y = std::get<Y>(root->lowerBound);
        z = std::get<Z>(root->lowerBound);
        lowerBound = std::make_tuple(x, y, z);
        x = std::get<X>(root->pos);
        y = std::get<Y>(root->pos);
        z = std::get<Z>(root->pos);
        upperBound = std::make_tuple(x, y, z);
    } else if (octet == 6) {
        x = std::get<X>(root->pos);
        y = std::get<Y>(root->lowerBound);
        z = std::get<Z>(root->pos);
        lowerBound = std::make_tuple(x, y, z);
        x = std::get<X>(root->upperBound);
        y = std::get<Y>(root->pos);
        z = std::get<Z>(root->upperBound);
        upperBound = std::make_tuple(x, y, z);
    } else if (octet == 7) {
        x = std::get<X>(root->pos);
        y = std::get<Y>(root->lowerBound);
        z = std::get<Z>(root->lowerBound);
        lowerBound = std::make_tuple(x, y, z);
        x = std::get<X>(root->upperBound);
        y = std::get<Y>(root->pos);
        z = std::get<Z>(root->pos);
        upperBound = std::make_tuple(x, y, z);
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

/* Construct OctTree given list of particles */
OctTree::OctTree(std::vector<Leaf *> &particles, vector_3d lowerBound,
                 vector_3d upperBound) {
    // Construct root of the tree
    this->root = new Root(nullptr, lowerBound, upperBound);

    // Cache whether class functions should use multiple threads
    this->parallel = NULL == std::getenv("SEQ");

    // Insert particles into tree
    insertParticles(particles);
}

// Destroy OctTree by freeing memory allocated for root
OctTree::~OctTree() {
    delete root;
}

// Insert particle into tree
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
        // Determine octet for particle (0-7)
        int octet = findOctet(this->root->pos, particle->body.pos);
        // Insert particle into tree sequentially or using std::thread
        if (this->parallel) {
            // If existing thread is already executing on octet, wait for it to complete.
            std::map<int, std::thread>::iterator it = threadPool.find(octet);
            if (it != threadPool.end()) {
                it->second.join();
            }
            threadPool[octet] =
                std::thread(&OctTree::insertParticle, this, this->root, particle, octet);
        } else {
            insertParticle(this->root, particle, octet);
        }
    }
    if (this->parallel) {
        // Wait for all in-flight insertions to complete.
        std::map<int, std::thread>::iterator it;
        for (it = threadPool.begin(); it != threadPool.end(); ++it) {
            it->second.join();
        }
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
    #pragma omp parallel
    {
        #pragma omp single
        centerOfMass(this->root);
    }
}

void
OctTree::centerOfMass(Root *root) {
    // Spawn task for each Root node and wait for tasks to complete
    for (int i=0; i < 8; i++) {
        Node *child = root->children[i];
        if (child != nullptr && !child->isLeaf()) {
            #pragma omp task
            centerOfMass((Root *)child);
        }
    }
    #pragma omp taskwait

    // Set center of mass for node
    double x = 0.0, y = 0.0, z = 0.0;
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

vector_3d
OctTree::treeForce(Leaf *particle) {
    return partialTreeForce(particle, (Node *)this->root);
}

vector_3d
OctTree::partialTreeForce(Leaf *particle, Node *node) {
    if (particle == nullptr || node == nullptr) {
        return zero_vect();
    }
    if (node->isLeaf()) {
        // if node is a leaf, return force produced on particle by leaf's body
        Leaf *leaf = (Leaf *)node;
        return particle->body.force(leaf->body);
    } else {
        Root *root = (Root *)node;
        double dist = particle->rootDistance(root);
        if (root->numChildren == 1 || root->size / dist < THETA) {
            // root only has a single child or is far enough away
            return particle->rootForce(root, dist);
        } else {
            // root to close, need to follow all its children
            vector_3d f = zero_vect();
            for (int i = 0; i < OCT_REGIONS; ++i) {
                vector_3d temp = partialTreeForce(particle, root->children[i]);
                std::get<X>(f) += std::get<X>(temp);
                std::get<Y>(f) += std::get<Y>(temp);
                std::get<Z>(f) += std::get<Z>(temp);
            }
            return f;
        }
    }
}

bool
OctTree::checkParticleBounds(Leaf *particle) {
    Root *root = (Root *)particle->parent;
    if (std::get<X>(particle->body.pos) < std::get<X>(root->lowerBound) ||
        std::get<Y>(particle->body.pos) < std::get<Y>(root->lowerBound) ||
        std::get<Z>(particle->body.pos) < std::get<Z>(root->lowerBound) ||
        std::get<X>(particle->body.pos) > std::get<X>(root->upperBound) ||
        std::get<Y>(particle->body.pos) > std::get<Y>(root->upperBound) ||
        std::get<Z>(particle->body.pos) > std::get<Z>(root->upperBound))
    {
        std::cout << "OUT OF BOUNDS!" << std::endl;
        return true;
    }
    return false;
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
