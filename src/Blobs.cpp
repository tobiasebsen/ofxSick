//
//  Blobs.cpp
//
//  Created by Tobias Ebsen on 22/07/15.
//
//

#include "Blobs.h"

using namespace ofxSick;

void Blobs::update(ofxSick::Plot &plot, float threshold) {
    
    vector<Blob>::clear();
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
                vector<Blob>::push_back(blob);
                
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

void Blobs::draw() {
    for (int i=0; i<this->size(); i++) {
        Blob & blob = this->at(i);
        ofEllipse(blob.center, blob.mass, blob.mass);
    }
}

void Blobs::filterByMass(float min, float max) {

    for (auto it=this->begin(); it!=this->end(); ) {
        Blob & blob = *it;
        if (blob.mass < min || blob.mass > max) {
            it = vector<Blob>::erase(it);
        }
        else {
            ++it;
        }
    }
}

void Blobs::groupByDistance(float maxDistance) {

    for (int i=0; i<this->size(); i++) {
        Blob & a = this->at(i);
        for (int j=0; j<this->size(); ) {
            Blob & b = this->at(j);
            
            float dist = a.center.distance(b.center);

            if (dist != 0.) {
                dist -= a.mass*0.5 + b.mass*0.5, 0.;
                if (dist < maxDistance) {
                    float pct = b.mass / (a.mass + b.mass);
                    a.center.interpolate(b.center, pct);
                    a.mass = a.mass + b.mass;
                    vector<Blob>::erase(this->begin()+j);
                    if (j < i)
                        i--;
                    j--;
                }
            }
            j++;
        }
    }
}

vector<int> & Blobs::getLabels() {
    return labels;
}