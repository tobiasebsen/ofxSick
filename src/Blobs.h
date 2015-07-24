//
//  Blobs.h
//
//  Created by Tobias Ebsen on 22/07/15.
//
//

#pragma once

#include "ofMain.h"

#include "Scan.h"
#include "Plot.h"

namespace ofxSick {

    typedef struct {
        ofVec2f center;
        ofVec2f mapped;
        float mass;
    } Blob;


    class Blobs {
    public:
        
        void update(Plot & scan, float threshold);
        
        void filterByMass(float min, float max);
        void erase(int index);
        void groupByDistance(float maxDistance);
        
        size_t size() const;
        Blob & operator[](int index);
        const Blob & operator[](int index) const;
        Blobs & operator=(const Blobs & other);
        
        vector<int> & getLabels();

    protected:
        vector<int> labels;
        int numUniqueLabels;
        vector<Blob> blobs;
    };
}