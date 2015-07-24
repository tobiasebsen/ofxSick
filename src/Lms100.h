//
//  Lms100.h
//
//  Created by Tobias Ebsen on 22/07/15.
//
//

#pragma once
#include "ofMain.h"

#include "CommunicationHandler.h"
#include "TcpCommunication.h"
#include "SopasAsciiFramer.h"
#include "ColaAProtocol.h"
#include "Lms100SopasInterface.h"

#include "Scan.h"

namespace ofxSick {

    class Lms100 : public CLms100SopasInterface, protected CCommunicationHandler::IAsyncCallback {
    public:
        Lms100();
        
        bool connect(string host, int port = 2111);
        bool start();
        void stop();
        void update();
        bool isFrameNew();
        
        Scan & getScan();

    protected:
        void AsyncCallback(const SDecodedAnswer& rAnswer, CBinaryDataStream& rStream, CDeserializer& rDeserializer);

        CTcpCommunication comm;
        CSopasAsciiFramer framer;
        CColaAProtocol protocol;
        
        Scan scan;
        
        uint16_t scanCount;
        bool bIsFrameNew;
    };
}