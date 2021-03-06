#ifndef __BINARY_SERIALIZER_H__
#define __BINARY_SERIALIZER_H__

/*
==========================================================================================
Copyright (c) SICK AG - Division Auto Ident, 2008

==========================================================================================
*/

#include "Serializer.h"

/*======================================================================================*/
/*! \brief A serializer for COLA-B-protocol

    \ingroup common

    This class implements a serializer that is used by CColaBProtocol to serialize 
    values for device requests. To encode a request (needed for CProtocol::WriteVariable() 
    and CProtocol::InvokeMethod()) get the according serializer of the selected protocol 
    by calling CProtocol::GetSerializer(). The COLA-B-protocol will return an instance 
    of this class.

    The COLA-B-protocol serializes data with fixed lengths for each type. COLA-B 
    transmits in Big Endian byte order.

    \note 64-bit integers are not supported.

    \note All serializer functions return a boolean value to indicate a successful 
      serialization. If an error occured during writing the write pointer of the 
      CBinaryDataStream could have been advanced already. The remaining data cannot be 
      written in that case and further processing should be cancelled.
 */
/*======================================================================================*/
class CBinarySerializer : public CSerializer
{
public:
  CBinarySerializer(void);
    //!< No initialization necessary.

  using CSerializer::Serialize; // Un-hide base class implementation
  using CSerializer::GetSerializedSize; // Un-hide base class implementation

  virtual bool Serialize(CBinaryDataStream& rOutputStream, SOPAS_Int8 value);
    //!< Serialize a #SOPAS_Int8.
  virtual bool Serialize(CBinaryDataStream& rOutputStream, SOPAS_Int16 value);
    //!< Serialize a #SOPAS_Int16.
  virtual bool Serialize(CBinaryDataStream& rOutputStream, SOPAS_Int32 value);
    //!< Serialize a #SOPAS_Int32.

  virtual bool Serialize(CBinaryDataStream& rOutputStream, SOPAS_UInt8 value);
    //!< Serialize a #SOPAS_UInt8.
  virtual bool Serialize(CBinaryDataStream& rOutputStream, SOPAS_UInt16 value);
    //!< Serialize a #SOPAS_UInt16.
  virtual bool Serialize(CBinaryDataStream& rOutputStream, SOPAS_UInt32 value);
    //!< Serialize a #SOPAS_UInt32.

  virtual bool Serialize(CBinaryDataStream& rOutputStream, SOPAS_Float value);
    //!< Serialize a #SOPAS_Float.
  virtual bool Serialize(CBinaryDataStream& rOutputStream, SOPAS_Double value);
    //!< Serialize a #SOPAS_Double.

  virtual bool Serialize(CBinaryDataStream& rOutputStream, 
    const SOPAS_FixString &rValue);
    //!< Serialize a #SOPAS_FixString.

  virtual size_t GetSerializedSize(SOPAS_UInt8) const;
    //!< Get necessary stream size for given #SOPAS_UInt8.
  virtual size_t GetSerializedSize(SOPAS_UInt16) const;
    //!< Get necessary stream size for given #SOPAS_UInt16.
  virtual size_t GetSerializedSize(SOPAS_UInt32) const;
    //!< Get necessary stream size for given #SOPAS_UInt32.
  
  virtual size_t GetSerializedSize(SOPAS_Int8 value) const;
    //!< Get necessary stream size for given #SOPAS_Int8.
  virtual size_t GetSerializedSize(SOPAS_Int16 value) const;
    //!< Get necessary stream size for given #SOPAS_Int16.
  virtual size_t GetSerializedSize(SOPAS_Int32 value) const;
    //!< Get necessary stream size for given #SOPAS_Int32.

  virtual size_t GetSerializedSize(SOPAS_Float value) const;
    //!< Get necessary stream size for given #SOPAS_Float.
  virtual size_t GetSerializedSize(SOPAS_Double value) const;
    //!< Get necessary stream size for given #SOPAS_Double.

  virtual size_t GetSerializedSize(const SOPAS_FixString& rValue) const;
    //!< Get necessary stream size for given #SOPAS_FixString.

private:
  CBinarySerializer(const CBinarySerializer&);
    //!< Do not allow copying
  CBinarySerializer& operator=(const CBinarySerializer&);
    //!< Do not allow copying

  template<class T>    
    bool SerializeSimpleType(CBinaryDataStream& rOutputStream, T value);      
    //!< Helper funcion that actually serializes all simple types.

  template<class T>
    size_t GetSimpleSerializedSize(T value) const;
    //!< Helper funcion that actually calculates the serialized size of all simple types.
};

#endif