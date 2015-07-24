//
//  Blobs.cpp
//
//  Created by Tobias Ebsen on 22/07/15.
//
//

#include "Blobs.h"

using namespace ofxSick;

void Blobs::update(ofxSick::Plot &plot, float threshold) {
    
    blobs.clear();
    labels.resize(plot.size());
    int label = -1;
    numUniqueLabels = 0;
    float resmag = plot.getResolution() * DEG_TO_RAD;
    
    double m00 = 0.;
    double m10 = 0.;
    double m01 = 0.;
    double mag = 0.;

    bool wasnear = false;

    for (int i=0; i<plot.size(); i++) {

        ofVec2f & a = plot[i];
        bool near = false;
        
        for (int j=i+1; j<MIN(plot.size(),i+2); j++) {

            ofVec2f & b = plot[j];
            if (i != j && a.distance(b) < threshold) {
                near = true;
            }
        }
        if (near) {
            if (!wasnear) label++;
            
            float length = a.length();
            
            if (length > 0.) {
                m00 += 1.;
                m10 += a.x;
                m01 += a.y;
                mag += length * resmag;
            }

            labels[i] = label;
        }
        else {
            if (wasnear && m00 > 0.) {
                Blob blob;
                blob.center.set(m10/m00, m01/m00);
                blob.mass = mag;
                blob.mapped.set(0.);
                blobs.push_back(blob);
                
                m00 = 0.;
                m10 = 0.;
                m01 = 0.;
                mag = 0.;
            }
            labels[i] = -1;
        }
        wasnear = near;
    }
    numUniqueLabels = label;
}

void Blobs::filterByMass(float min, float max) {

    for (auto it=blobs.begin(); it!=blobs.end(); ) {
        Blob & blob = *it;
        if (blob.mass < min || blob.mass > max) {
            it = blobs.erase(it);
        }
        else {
            ++it;
        }
    }
}

void Blobs::groupByDistance(float maxDistance) {

    for (int i=0; i<blobs.size(); i++) {
        Blob & a = blobs[i];
        for (int j=0; j<blobs.size(); ) {
            Blob & b = blobs[j];
            
            float dist = a.center.distance(b.center) - a.mass*0.5 - b.mass*0.5;
            
            if (dist > 0. && dist < maxDistance) {
                float pct = b.mass / (a.mass + b.mass);
                a.center.interpolate(b.center, pct);
                a.mass += b.mass;
                blobs.erase(blobs.begin()+j);
                i--;
            }
            else {
                j++;
            }
        }
    }
}

void Blobs::erase(int index) {
    blobs.erase(blobs.begin()+index);
}

size_t Blobs::size() const {
    return blobs.size();
}

Blob & Blobs::operator[](int index) {
    return blobs[index];
}

const Blob & Blobs::operator[](int index) const {
    return blobs[index];
}

Blobs & Blobs::operator=(const Blobs & other) {
    blobs.assign(other.blobs.begin(), other.blobs.end());
    return *this;
}

vector<int> & Blobs::getLabels() {
    return labels;
}