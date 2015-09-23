/*
 ==========================================================================================
 Copyright (c) SICK AG - Division Auto Ident, 2008
 
 ==========================================================================================
 */

#include "TcpCommunication.h"
#include "BinaryDataStream.h"


CTcpCommunication::CTcpCommunication(void)
{
    connected = false;
    client.Create();
}

/*======================================================================================*/
/*! \param targetAddress The IP-address of the device. Use IPAddress::Parse() to enter
 the address as string.
 \param targetPort The SOPAS port of the device. Usually 2111 or 2112.
 
 \return <em>true</em> if the connection could be established using the settings.
 <em>false</em> otherwise.
 */
/*======================================================================================*/
bool CTcpCommunication::Connect(std::string targetAddress, int targetPort)
{
    bool retVal = false;
    try
    {
        client.SetTimeoutAccept(1);
        client.SetTimeoutReceive(1);
        client.SetTimeoutSend(1);
        client.SetNonBlocking(false);
        connected = client.Connect((char*)targetAddress.c_str(), targetPort);
        client.SetNonBlocking(true);
        //stream = client->GetStream();
        retVal = connected;
    }
    catch (...)
    {
        // Ignore, just return false
    }
    return retVal;
}

/*======================================================================================*/
/*! Uses the default SOPAS port 2111 and given IP-address to connect.
 the address as string.
 
 \param targetAddress The IP-address of the device. Use IPAddress::Parse() to enter
 the address as string.
 
 \return <em>true</em> if the connection could be established using the settings.
 <em>false</em> otherwise.
 */
/*======================================================================================*/
bool CTcpCommunication::Connect(std::string targetAddress)
{
    // Using SOPAS default port 2111
    return Connect(targetAddress, 2111);
}

void CTcpCommunication::Disconnect(void)
{
    if (connected)
    {
        client.Close();
    }
}

/*======================================================================================*/
/*! The implementation does not block but checks if there are bytes to read. If
 bytes are available for reading at most #READ_BUFFER_SIZE bytes are read and the
 function will return afterwards. If no bytes are available for reading the function
 will return at once.
 
 See CCommunication::ReadBytes() for more information.
 
 \return A binary datastream containing all bytes read. <em>Null</em> if no data
 has been read.
 */
/*======================================================================================*/
CBinaryDataStream* CTcpCommunication::ReadBytes(void)
{
    CBinaryDataStream* pData = 0;
    
    try
    {
        ofBuffer buffer;
        buffer.allocate(READ_BUFFER_SIZE);
        char *mgdData = buffer.getBinaryBuffer();
        int readBytes = 0;
        
        if (connected)
        {
            // Check if stream contains data to prevent blocking
            readBytes = client.Receive(mgdData, buffer.size());
        }
        
        // Copy managed Byte-array to unmanaged char array
        if(readBytes > 0)
        {
            pData = new CBinaryDataStream(readBytes);
            for(int i = 0; i < readBytes; ++i)
            {
                (void)pData->WriteToStream(mgdData[i]);
            }
        }
    }
    catch(...)
    {
        // Ignore, just return null
    }
    
    return pData;
}

/*======================================================================================*/
/*! The storage size of the CBinaryDataStream is not important here. Only the bytes that
 were written to the stream are transmitted to the TCP/IP-connection here.
 
 See CCommunication::WriteBytes() for more information.
 
 \param rData The data to write.
 \return <em>true</em> if data was written successfully. <em>false</em> otherwise.
 
 */
/*======================================================================================*/
bool CTcpCommunication::WriteBytes(const CBinaryDataStream& rData)
{
    bool retVal = false;
    
    try
    {
        if (connected)
        {
            // Copy unmanaged data to managed data
            int length = rData.GetUsedLength();
            const char* pRawData = (const char*)rData.GetData();
            retVal = (client.SendAll(pRawData, length) == length);
        }
    }
    catch(...)
    {
        // Ignore, just return false
    }
    return retVal;
}
