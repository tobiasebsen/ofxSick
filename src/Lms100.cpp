//
//  Lms100.cpp
//
//  Created by Tobias Ebsen on 22/07/15.
//
//

#include "Lms100.h"

using namespace ofxSick;

Lms100::Lms100() : protocol(comm, framer), CLms100SopasInterface(protocol) {
    scanCount = 0;
    bIsFrameNew = false;
    CLms100SopasInterface::SetAsyncCallback(this);
}

bool Lms100::connect(string host, int port) {

    return comm.Connect(host, port);
}

bool Lms100::start() {
    
    return CLms100SopasInterface::SubscribeScanDataEvent(true);
}

void Lms100::stop() {
    
    CLms100SopasInterface::SubscribeScanDataEvent(false);
}

void Lms100::update() {

    bIsFrameNew = false;
    CLms100SopasInterface::PollAsyncAnswers(0);
}

bool Lms100::isFrameNew() {
    return bIsFrameNew;
}

Scan & Lms100::getScan() {
    return scan;
}

void Lms100::AsyncCallback(const SDecodedAnswer& rAnswer, CBinaryDataStream& rStream, CDeserializer& rDeserializer) {

    if(rAnswer.eAnswerType == SDecodedAnswer::EVENT_RESULT) {
        if(rAnswer.coName.compare("LMDscandata") == 0) {
            scan.Deserialize(rDeserializer, rStream);
            if (scan.StatusBlock.uiScanCount != scanCount) {
                bIsFrameNew = true;
                scanCount = scan.StatusBlock.uiScanCount;
            }
        }
    }
}