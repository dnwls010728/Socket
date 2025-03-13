#pragma once
#include "Serializer.h"
#include <functional>
#include <unordered_map>
#include <memory>


namespace Net {

#pragma pack(push, 1)
    struct PayloadHeader {
        uint32_t packet_id;
    };
#pragma pack(pop)

    // 직렬화/역직렬화 멤버 설정
#define SERIALIZABLE_FIELDS(...)                             \
    void Serialize(Serializer& serializer) const {           \
        Serializer::SerializeAll(serializer, __VA_ARGS__);   \
    }                                                        \
    void Deserialize(Serializer& serializer) {               \
        Serializer::DeserializeAll(serializer, __VA_ARGS__); \
    }

// 패킷 등록. packet ID는 사용자가 임의로 지정하며 중복되면 안됨. 100 이상부터 사용 가능
#define REGISTER_PACKET(PacketType, PacketID) \
    inline static PacketRegistrar<PacketType> registrar_##PacketType{PacketID};\
    uint16_t GetPacketID() const override { return PacketID; }

/* example:
struct TestPacket : public IPacket{
    int a;
    float b;
    std::string c;

    SERIALIZABLE_FIELDS(a, b, c);
    REGISTER_PACKET(TestPacket, 30);
};
*/

    struct IPacket
    {
        virtual void Serialize(Serializer& serializer) const = 0;
        virtual void Deserialize(Serializer& serializer) = 0;
        virtual uint16_t GetPacketID() const = 0;
    };
    using PacketFactoryFunc = std::function<std::unique_ptr<IPacket>()>;

    // 패킷 팩토리
    class PacketFactoryRegistry {
    public:
        static PacketFactoryRegistry& Instance() {
            static PacketFactoryRegistry instance;
            return instance;
        }

        void RegisterFactory(int packetID, PacketFactoryFunc func) {
            registry_[packetID] = func;
        }

        std::unique_ptr<IPacket> CreatePacket(uint16_t packetID) {
            auto it = registry_.find(packetID);
            if (it != registry_.end()) {
                return (it->second)();
            }
            return nullptr;
        }
    private:
        std::unordered_map<uint16_t, PacketFactoryFunc> registry_;
    };


    // 패킷 등록 도우미 템플릿. 패킷 클래스에서 PacketRegistrar를 선언함으로 써 패킷 등록
    template<typename T>
    class PacketRegistrar {
    public:
        PacketRegistrar(uint16_t packetID) {
            PacketFactoryRegistry::Instance().RegisterFactory(packetID, []() -> std::unique_ptr<IPacket> {
                return std::make_unique<T>();
                });
        }
    };
}//namespace Net    