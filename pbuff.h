#pragma once
#include <QByteArray>


template<class T> class ProtoBytes
{
    public:
        ProtoBytes(){}
        static T protoFromByteArray(const QByteArray& inBA)
        {
             T proto;
             proto.ParseFromArray(inBA.data(), inBA.size());
             return proto;
        }
        static QByteArray  protoToByteArray(const T& proto)
        {
             if (!proto.IsInitialized()) {
              return QByteArray();
             }
             QByteArray outBA;
             outBA.resize(proto.ByteSize());
             proto.SerializeToArray(outBA.data(), outBA.size());
             return outBA;
        }
};
