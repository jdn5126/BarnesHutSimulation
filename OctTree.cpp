/*
 * Copyright 2020 Bryson Banks, David Campbell, and Jeffrey Nelson.  All rights reserved.
 *
 * BarnesHutSimulation: OctTree.cpp
 */

#include <assert.h>
#include <iostream>
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

    // Insert each node into the tree
    for(Leaf *particle : particles) {
        insert(this->root, particle);
    }
}

// Destroy OctTree by freeing memory allocated for root
OctTree::~OctTree() {
    delete root;
}

void
OctTree::insert(Leaf *particle) {
    insert(this->root, particle);
}

// Insert new particle into tree rooted at root
void
OctTree::insert(Root *root, Leaf *particle) {
    // Determine which octet the particle belongs in
    int octet = findOctet(root->pos, particle->body.pos);
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
        insert(newRoot, (Leaf *)child);
        insert(newRoot, particle);
    } else {
        // Continue down tree
        insert((Root *)root->children[octet], particle);
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
        for (Node *node : root->children) {
            if (node != nullptr && node->isLeaf()) {
                leaf = (Leaf *)node;
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
OctTree::print() {
    std::cout << *(this->root) << std::endl;
    printRecurse(this->root);
}

void
OctTree::printRecurse(Root *root) {
    // Fixed iteration order to help with debugging
    for(int i=0; i < root->children.size(); i++) {
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
    this->children.resize(8);
    for(int i=0; i < 8; i++) {
        this->children[i] = nullptr;
    }
    this->numChildren = 0;
}

Root::~Root() {
    // Delete children
    this->children.clear();
}

std::ostream& operator<<(std::ostream& out, const Leaf& l) {
    out << "Leaf " << l.body.id << ": (" << std::get<X>(l.body.pos) << ", " <<
        std::get<Y>(l.body.pos) << ", " << std::get<Z>(l.body.pos) << ")";
    return out;
}

std::ostream& operator<<(std::ostream& out, const Root& r) {
    out << "Root(" << std::get<X>(r.pos) << ", " << std::get<Y>(r.pos) <<
        ", " << std::get<Z>(r.pos) << ")";
    return out;
}
