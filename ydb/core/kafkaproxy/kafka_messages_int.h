#pragma once

#include <functional>
#include <memory>
#include <vector>

#include <util/generic/array_ref.h>
#include <util/generic/yexception.h>

#include <contrib/libs/cxxsupp/libcxx/include/type_traits>

#include "kafka.h"

namespace NKafka {
namespace NPrivate {

struct TKafkaBoolDesc {
    static constexpr bool Default = true;
    static constexpr bool Nullable = false;
    static constexpr bool FixedLength = true;
};

struct TKafkaInt8Desc {
    static constexpr bool Default = true;
    static constexpr bool Nullable = false;
    static constexpr bool FixedLength = true;
};

struct TKafkaInt16Desc {
    static constexpr bool Default = true;
    static constexpr bool Nullable = false;
    static constexpr bool FixedLength = true;
};

struct TKafkaUint16Desc {
    static constexpr bool Default = true;
    static constexpr bool Nullable = false;
    static constexpr bool FixedLength = true;
};

struct TKafkaInt32Desc {
    static constexpr bool Default = true;
    static constexpr bool Nullable = false;
    static constexpr bool FixedLength = true;
};

struct TKafkaUint32Desc {
    static constexpr bool Default = true;
    static constexpr bool Nullable = false;
    static constexpr bool FixedLength = true;
};

struct TKafkaInt64Desc {
    static constexpr bool Default = true;
    static constexpr bool Nullable = false;
    static constexpr bool FixedLength = true;
};

struct TKafkaUuidDesc {
    static constexpr bool Default = true;
    static constexpr bool Nullable = false;
    static constexpr bool FixedLength = true;
};

struct TKafkaFloat64Desc {
    static constexpr bool Default = true;
    static constexpr bool Nullable = false;
    static constexpr bool FixedLength = true;
};

struct TKafkaStringDesc {
    static constexpr bool Default = true;
    static constexpr bool Nullable = true;
    static constexpr bool FixedLength = false;

    static bool IsNull(const TKafkaString& value) { return !value; };
};

struct TKafkaStructDesc {
    static constexpr bool Default = false;
    static constexpr bool Nullable = false;
    static constexpr bool FixedLength = false;
};

struct TKafkaBytesDesc {
    static constexpr bool Default = false;
    static constexpr bool Nullable = true;
    static constexpr bool FixedLength = false;

    static bool IsNull(const TKafkaBytes& value) { return !value; };
};

struct TKafkaRecordsDesc {
    static constexpr bool Default = false;
    static constexpr bool Nullable = true;
    static constexpr bool FixedLength = false;

    static bool IsNull(const TKafkaRecords& value) { return !value; };
};

struct TKafkaArrayDesc {
    static constexpr bool Default = false;
    static constexpr bool Nullable = true;
    static constexpr bool FixedLength = false;

    template<typename T>
    static bool IsNull(const std::vector<T>& value) { return value.empty(); };
};



struct TWriteCollector {
    ui32 NumTaggedFields = 0;
};

struct TSizeCollector {
    ui32 Size = 0;
    ui32 NumTaggedFields = 0;
};


constexpr i32 SizeOfUnsignedVarint(i32 value) {
    int bytes = 1;
    while ((value & 0xffffff80) != 0L) {
        bytes += 1;
        value >>= 7;
    }
    return bytes;
}


template<TKafkaVersion min, TKafkaVersion max>
constexpr bool VersionAll() {
    return 0 == min && max == Max<TKafkaVersion>();
}

template<TKafkaVersion min, TKafkaVersion max>
constexpr bool VersionNone() {
    return 0 == min && max == -1;
}

template<TKafkaVersion min, TKafkaVersion max>
inline bool VersionCheck(const TKafkaVersion value) {
    if constexpr (VersionNone<min, max>()) {
        return false;
    } else if constexpr (VersionAll<min, max>()) {
        return true;
    } else if constexpr (max == Max<TKafkaVersion>()) {
        return min <= value;
    } else if constexpr (min == 0) {
        return value <= max;
    } else {
        return min <= value && value <= max;
    }
}

template<typename Meta>
class ReadFieldRule {
public:
    static bool Apply(TKafkaVersion version) {
        return VersionCheck<Meta::PresentVersionMin, Meta::PresentVersionMax>(version) 
            && !VersionCheck<Meta::TaggedVersionMin, Meta::TaggedVersionMax>(version);
    }
};

template<typename Meta>
class ReadTaggedFieldRule {
public:
    static bool Apply(TKafkaVersion /*version*/) {
        return true;
    }
};

template<typename Meta>
bool IsDefaultValue(const typename Meta::Type& value) {
    if constexpr (std::is_base_of_v<TMessage, typename Meta::Type>) {
        typename Meta::Type defValue;
        return defValue == value;
    } else if constexpr (Meta::TypeDesc::Default) {
        return Meta::Default == value;
    } else if constexpr (Meta::TypeDesc::Nullable) {
        return Meta::TypeDesc::IsNull(value);
    } else {
        return false;
    }
}



template<TKafkaVersion FlexibleMin,
         TKafkaVersion FlexibleMax>
inline void WriteStringSize(TKafkaWritable& writable, TKafkaVersion version, TKafkaInt32 value) {
    if (VersionCheck<FlexibleMin, FlexibleMax>(version)) {
        writable.writeUnsignedVarint(value + 1);
    } else {
        writable << (TKafkaInt16)value;
    }
}

template<TKafkaVersion FlexibleMin,
         TKafkaVersion FlexibleMax>
inline TKafkaInt32 ReadStringSize(TKafkaReadable& readable, TKafkaVersion version) {
    if (VersionCheck<FlexibleMin, FlexibleMax>(version)) {
        return readable.readUnsignedVarint() - 1;
    } else {
        TKafkaInt16 v;
        readable >> v;
        return v;
    }
}

template<TKafkaVersion FlexibleMin,
         TKafkaVersion FlexibleMax>
inline void WriteArraySize(TKafkaWritable& writable, TKafkaVersion version, TKafkaInt32 value) {
    if (VersionCheck<FlexibleMin, FlexibleMax>(version)) {
        writable.writeUnsignedVarint(value + 1);
    } else {
        writable << value;
    }
}

template<TKafkaVersion FlexibleMin,
         TKafkaVersion FlexibleMax>
inline TKafkaInt32 ReadArraySize(TKafkaReadable& readable, TKafkaVersion version) {
    if (VersionCheck<FlexibleMin, FlexibleMax>(version)) {
        return readable.readUnsignedVarint() - 1;
    } else {
        TKafkaInt32 v;
        readable >> v;
        return v;
    }
}


template<typename T>
void NormalizeNumber(T& value) {
#ifndef WORDS_BIGENDIAN
        char* b = (char*)&value;
        char* e = b + sizeof(T) - 1;
        while(b < e) {
            std::swap(*b, *e);
            ++b;
            --e;
        }
#endif
}

class ReadUnsignedVarintStrategy {
public:
    void Init() {
        Finished = 0;
        Shift = -1;
        Value = 0;
    }

    TReadDemand Next() {
        if (Finished) {
            return NoDemand;
        }
        if (Shift >= 0) {
            Finished = !(Buffer & 0x80);
            ui32 v = Buffer & 0x7F;
            Value |= v << Shift;
            Shift += 7;
        } else {
            Shift = 0;
        }

        if (Finished) {
            return NoDemand;
        } else {
            return TReadDemand(&Buffer, sizeof(char));
        }
    }

    TKafkaInt32 Value;

private:
    bool Finished;
    char Buffer;
    i8 Shift;
};



template<typename Meta, typename TOldSizeType>
class ReadSizeStrategy {
public:
    void Init() {
        WasRead = false;
        WasNormalized = false;
        OldValue = 0;
        UnsignedVarintStrategy.Init();
    }

    TReadDemand Next(TKafkaVersion version) {
        if (VersionCheck<Meta::FlexibleVersionMin, Meta::FlexibleVersionMax>(version)) {
            return UnsignedVarintStrategy.Next();
        } else {
            if (WasRead) {
                if (!WasNormalized) {
                    WasNormalized = true;
                    NormalizeNumber(OldValue);
                }
                return NoDemand;
            }
            WasRead = true;
            return TReadDemand((char*)&OldValue, sizeof(TOldSizeType));
        }
    }

    TKafkaInt32 Value(TKafkaVersion version) {
        if (VersionCheck<Meta::FlexibleVersionMin, Meta::FlexibleVersionMax>(version)) {
            return UnsignedVarintStrategy.Value - 1;
        } else {
            return OldValue;
        }
    }

private:
    bool WasRead;
    bool WasNormalized;
    TOldSizeType OldValue;

    ReadUnsignedVarintStrategy UnsignedVarintStrategy;
};



template<typename Meta,
         typename TValueType = typename Meta::Type,
         typename TTypeDesc = typename Meta::TypeDesc>
class TReadStrategy {
public:
    template<typename Rule>
    void Init(TValueType& /*value*/, TKafkaVersion /*version*/) {
        WasRead = false;
    }

    template<typename Rule>
    TReadDemand Next(TValueType& value, TKafkaVersion version) {
        if (!Rule::Apply(version)) {
            if constexpr (Meta::TypeDesc::Default) {
                value = Meta::Default;
            }
            return NoDemand;
        }
        if (WasRead) {
            NormalizeNumber(value);
            return NoDemand;
        }
        WasRead = true;
        return TReadDemand((char*)&value, sizeof(TValueType));
    }

private:
    bool WasRead;
};


template<typename Meta, typename TValueType>
class TReadStrategy<Meta, TValueType, TKafkaStructDesc> {
public:
    template<typename Rule>
    void Init(TValueType& value, TKafkaVersion version) {
        if (Rule::Apply(version)) {
            Context = value.CreateReadContext(version);
        }
    }

    template<typename Rule>
    TReadDemand Next(TValueType& /*value*/, TKafkaVersion version) {
        if (Rule::Apply(version)) {
            return Context.get()->Next();
        } else {
            return NoDemand;
        }
    }

private:
    std::unique_ptr<TReadContext> Context;
};


template<typename Meta, typename TValueType>
class TReadStrategy<Meta, TValueType, TKafkaUuidDesc> {
public:
    template<typename Rule>
    void Init(TKafkaUuid& /*value*/, TKafkaVersion /*version*/) {
        WasRead = false;
    }

    template<typename Rule>
    TReadDemand Next(TKafkaUuid& value, TKafkaVersion version) {
        if (!Rule::Apply(version)) {
            return NoDemand;
        }
        if (WasRead) {
            NormalizeNumber(Buffer[0]);
            NormalizeNumber(Buffer[1]);
            value = TKafkaUuid(Buffer[0], Buffer[1]);
            return NoDemand;
        }
        WasRead = true;
        return TReadDemand((char*)Buffer, sizeof(ui64) << 1);
    }

private:
    bool WasRead;
    ui64 Buffer[2];
};



template<typename Meta, typename TValueType>
class TReadStrategy<Meta, TValueType, TKafkaStringDesc> {
public:
    template<typename Rule>
    void Init(TKafkaString& /*value*/, TKafkaVersion /*version*/) {
        WasRead = false;
        Size.Init();
    }

    template<typename Rule>
    TReadDemand Next(TKafkaString& value, TKafkaVersion version) {
        if (WasRead || !Rule::Apply(version)) {
            return NoDemand;
        }
        auto demand = Size.Next(version);
        if (demand) {
            return demand;
        }
        WasRead = true;
        TKafkaInt32 length = Size.Value(version);

        if (length < 0) {
            if (VersionCheck<Meta::NullableVersionMin, Meta::NullableVersionMax>(version)) {
                value = std::nullopt;
                return NoDemand;
            } else {
                ythrow yexception() << "non-nullable field " << Meta::Name << " was serialized as null";
            }
        } else if (length > Max<i16>()){
            ythrow yexception() << "string field " << Meta::Name << " had invalid length " << length;
        }

        value = TString();
        value->ReserveAndResize(length);
        return TReadDemand((char*)value->data(), length);
    }

private:
    bool WasRead;

    ReadSizeStrategy<Meta, TKafkaInt16> Size;
};


template<typename Meta, typename Desc>
class TReadStrategy<Meta, TKafkaRecords, Desc> {
public:
    template<typename Rule>
    void Init(TKafkaRecords& /*value*/, TKafkaVersion /*version*/) {
        WasRead = false;
        Size.Init();
    }

    template<typename Rule>
    TReadDemand Next(TKafkaRecords& value, TKafkaVersion version) {
        if (WasRead || !Rule::Apply(version)) {
            return NoDemand;
        }
        auto demand = Size.Next(version);
        if (demand) {
            return demand;
        }
        WasRead = true;
        TKafkaInt32 length = Size.Value(version);

        if (length < 0) {
            if (VersionCheck<Meta::NullableVersionMin, Meta::NullableVersionMax>(version)) {
                value = std::nullopt;
                return NoDemand;
            } else {
                ythrow yexception() << "non-nullable field " << Meta::Name << " was serialized as null";
            }
        }

        value = TKafkaRawBytes();
        value->Resize(length);
        return TReadDemand(value->data(), length);
    }

private:
    bool WasRead;

    ReadSizeStrategy<Meta, TKafkaInt32> Size;
};


template<typename Meta, typename TValueType>
class TReadStrategy<Meta, TValueType, TKafkaArrayDesc> {
public:
    template<typename Rule>
    void Init(std::vector<typename Meta::ItemType>& /*value*/, TKafkaVersion /*version*/) {
        ItemStep = -1;
        Size.Init();
    }

    template<typename Rule>
    TReadDemand Next(std::vector<typename Meta::ItemType>& value, TKafkaVersion version) {
        if (!Rule::Apply(version)) {
            return NoDemand;
        }
        auto demand = Size.Next(version);
        if (demand) {
            return demand;
        }
        if (-1 == ItemStep) {
            TKafkaInt32 length = Size.Value(version);

            if (length < 0) {
                if (VersionCheck<Meta::NullableVersionMin, Meta::NullableVersionMax>(version)) {
                    value.resize(0);
                    return NoDemand;
                } else {
                    ythrow yexception() << "non-nullable field " << Meta::Name << " was serialized as null";
                }
            }

            value.resize(length);
            if (0 == length) {
                return NoDemand;
            }

            ItemStep = 0;
            ItemStrategy.template Init<Rule>(value[ItemStep], version);
        } else if (ItemStep == (i32)value.size()) {
            return NoDemand;
        }

        demand = ItemStrategy.template Next<Rule>(value[ItemStep], version);
        if (demand) {
            return demand;
        }

        ++ItemStep;
        if (ItemStep == (i32)value.size()) {
            return NoDemand;
        }
        ItemStrategy.template Init<Rule>(value[ItemStep], version);
        return ItemStrategy.template Next<Rule>(value[ItemStep], version);
    }

private:
    i32 ItemStep;

    ReadSizeStrategy<Meta, TKafkaInt32> Size;
    TReadStrategy<Meta, typename Meta::ItemType, typename Meta::ItemTypeDesc> ItemStrategy;
};





//
// Common
//
template<typename Meta,
         typename TValueType>
class TypeStrategy {
public:
    inline static void DoWrite(TKafkaWritable& writable, TKafkaVersion version, const TValueType& value) {
        if constexpr (std::is_base_of_v<TMessage, TValueType>) {
            value.Write(writable, version);
        } else {
            writable << value;
        }
    }

    inline static void DoWriteTag(TKafkaWritable& writable, TKafkaVersion version, const TValueType& value) {
        if constexpr (std::is_base_of_v<TMessage, TValueType>) {
            value.Write(writable, version);
        } else {
            writable << value;
        }
    }

    inline static void DoRead(TKafkaReadable& readable, TKafkaVersion version, TValueType& value) {
        if constexpr (std::is_base_of_v<TMessage, TValueType>) {
            value.Read(readable, version);
        } else {
            readable >> value;
        }
    }

    inline static i64 DoSize(TKafkaVersion version, const TValueType& value) {
        if constexpr (std::is_base_of_v<TMessage, TValueType>) {
            return value.Size(version);
        } else {
            return sizeof(TValueType);
        }
    }
};



//
// TKafkaString
//
template<typename Meta>
class TypeStrategy<Meta, TKafkaString> {
public:
    inline static void DoWrite(TKafkaWritable& writable, TKafkaVersion version, const TKafkaString& value) {
        if (value) {
            const auto& v = *value;
            if (v.length() > Max<i16>()) {
                ythrow yexception() << "string field " << Meta::Name << " is too long to be serialized " << v.length();
            }
            WriteStringSize<Meta::FlexibleVersionMin, Meta::FlexibleVersionMax>(writable, version, v.length());
            writable << v;
        } else {
            if (VersionCheck<Meta::NullableVersionMin, Meta::NullableVersionMax>(version)) {
                WriteStringSize<Meta::FlexibleVersionMin, Meta::FlexibleVersionMax>(writable, version, -1);
            } else {
                ythrow yexception() << "non-nullable field " << Meta::Name << " serializing as null";
            }
        }
    }

    inline static void DoWriteTag(TKafkaWritable& writable, TKafkaVersion version, const TKafkaString& value) {
        const auto& v = *value;
        WriteStringSize<0, Max<TKafkaVersion>()>(writable, version, v.length());
        writable << v;
    }

    inline static void DoRead(TKafkaReadable& readable, TKafkaVersion version, TKafkaString& value) {
        TKafkaInt32 length = ReadStringSize<Meta::FlexibleVersionMin, Meta::FlexibleVersionMax>(readable, version);
        if (length < 0) {
            if (VersionCheck<Meta::NullableVersionMin, Meta::NullableVersionMax>(version)) {
                value = std::nullopt;
            } else {
                ythrow yexception() << "non-nullable field " << Meta::Name << " was serialized as null";
            }
        } else if (length > Max<i16>()){
            ythrow yexception() << "string field " << Meta::Name << " had invalid length " << length;
        } else {
            value = TString();
            value->ReserveAndResize(length);
            readable.read(const_cast<char*>(value->data()), length);
        }
    }

    inline static i64 DoSize(TKafkaVersion version, const TKafkaString& value) {
        if (value) {
            const auto& v = *value;
            if (v.length() > Max<i16>()) {
                ythrow yexception() << "string field " << Meta::Name << " is too long to be serialized " << v.length();
            }
            if (VersionCheck<Meta::FlexibleVersionMin, Meta::FlexibleVersionMax>(version)) {
                return v.length() + SizeOfUnsignedVarint(v.length() + sizeof(TKafkaInt8));
            } else {
                return v.length() + sizeof(TKafkaInt16);
            }
        } else {
            if (VersionCheck<Meta::FlexibleVersionMin, Meta::FlexibleVersionMax>(version)) {
                return sizeof(TKafkaInt8);
            } else {
                return sizeof(TKafkaInt16);
            }
        }
    }
};


//
// TKafkaBytes
//
template<typename Meta>
class TypeStrategy<Meta, TKafkaBytes> {
public:
    inline static void DoWrite(TKafkaWritable& writable, TKafkaVersion version, const TKafkaBytes& value) {
        if (value) {
            const auto& v = *value;
            WriteArraySize<Meta::FlexibleVersionMin, Meta::FlexibleVersionMax>(writable, version, v.size());
            writable << v;
        } else {
            if (VersionCheck<Meta::NullableVersionMin, Meta::NullableVersionMax>(version)) {
                WriteArraySize<Meta::FlexibleVersionMin, Meta::FlexibleVersionMax>(writable, version, -1);
            } else {
                ythrow yexception() << "non-nullable field " << Meta::Name << " serializing as null";
            }
        }
    }

    inline static void DoWriteTag(TKafkaWritable& writable, TKafkaVersion version, const TKafkaBytes& value) {
        const auto& v = *value;
        WriteArraySize<0, Max<TKafkaVersion>()>(writable, version, v.size());
        writable << v;
    }

    inline static void DoRead(TKafkaReadable& readable, TKafkaVersion version, TKafkaBytes& value) {
        TKafkaInt32 length = ReadArraySize<Meta::FlexibleVersionMin, Meta::FlexibleVersionMax>(readable, version);
        if (length < 0) {
            if (VersionCheck<Meta::NullableVersionMin, Meta::NullableVersionMax>(version)) {
                value = std::nullopt;
            } else {
                ythrow yexception() << "non-nullable field " << Meta::Name << " was serialized as null";
            }
        } else {
            value = TBuffer();
            value->Resize(length);
            readable.read(value->data(), length);
        }
    }

    inline static i64 DoSize(TKafkaVersion version, const TKafkaBytes& value) {
        if (value) {
            const auto& v = *value;
            if (VersionCheck<Meta::FlexibleVersionMin, Meta::FlexibleVersionMax>(version)) {
                return v.size() + SizeOfUnsignedVarint(v.size() + 1);
            } else {
                return v.size() + sizeof(TKafkaInt32);
            }
        } else {
            if (VersionCheck<Meta::FlexibleVersionMin, Meta::FlexibleVersionMax>(version)) {
                return 1;
            } else {
                return sizeof(TKafkaInt32);
            }
        }
    }
};


//
// KafkaArray
//
template<typename Meta,
         typename TValueType>
class TypeStrategy<Meta, std::vector<TValueType>> {
public:
    inline static void DoWrite(TKafkaWritable& writable, TKafkaVersion version, const std::vector<TValueType>& value) {
        WriteArraySize<Meta::FlexibleVersionMin, Meta::FlexibleVersionMax>(writable, version, value.size());

        for(const auto& v : value) {
            TypeStrategy<Meta, TValueType>::DoWrite(writable, version, v);
        }
    }

    inline static void DoWriteTag(TKafkaWritable& writable, TKafkaVersion version, const std::vector<TValueType>& value) {
        WriteArraySize<0, Max<TKafkaVersion>()>(writable, version, value.size());

        for(const auto& v : value) {
            TypeStrategy<Meta, TValueType>::DoWrite(writable, version, v);
        }
    }

    inline static void DoRead(TKafkaReadable& readable, TKafkaVersion version, std::vector<TValueType>& value) {
        TKafkaInt32 length = ReadArraySize<Meta::FlexibleVersionMin, Meta::FlexibleVersionMax>(readable, version);
        if (length < 0) {
            if (VersionCheck<Meta::NullableVersionMin, Meta::NullableVersionMax>(version)) {
                return;
            } else {
                ythrow yexception() << "non-nullable field " << Meta::Name << " was serialized as null";
            }
        }
        value.resize(length);

        for (int i = 0; i < length; ++i) {
            TypeStrategy<Meta, TValueType>::DoRead(readable, version, value[i]);
        }
    }

    inline static i64 DoSize(TKafkaVersion version, const std::vector<TValueType>& value) {
        TKafkaInt32 size = 0;
        if constexpr (Meta::TypeDesc::FixedLength) {
            size = value.size() * sizeof(TValueType);
        } else {
            for(const auto& v : value) {
                size += TypeStrategy<Meta, TValueType>::DoSize(version, v);
            }
        }
        if (VersionCheck<Meta::FlexibleVersionMin, Meta::FlexibleVersionMax>(version)) {
            return size + SizeOfUnsignedVarint(value.size() + 1);
        } else {
            return size + sizeof(TKafkaInt32);
        }
    }
};



//
// Main fields function
//
template<typename Meta>
inline void Write(TWriteCollector& collector, TKafkaWritable& writable, TKafkaInt16 version, const typename Meta::Type& value) {
    if (VersionCheck<Meta::TaggedVersionMin, Meta::TaggedVersionMax>(version)) {
        if (!IsDefaultValue<Meta>(value)) {
            ++collector.NumTaggedFields;
        }
    } else if (VersionCheck<Meta::PresentVersionMin, Meta::PresentVersionMax>(version)) {
        TypeStrategy<Meta, typename Meta::Type>::DoWrite(writable, version, value);
    }
}

template<typename Meta>
inline void Read(TKafkaReadable& readable, TKafkaInt16 version, typename Meta::Type& value) {
    if (!VersionNone<Meta::TaggedVersionMin, Meta::TaggedVersionMax>() 
        && VersionCheck<Meta::TaggedVersionMin, Meta::TaggedVersionMax>(version)) {
        return;
    } else {
        if (VersionCheck<Meta::PresentVersionMin, Meta::PresentVersionMax>(version)) {
            try {
                TypeStrategy<Meta, typename Meta::Type>::DoRead(readable, version, value);
            } catch (const yexception& e) {
                ythrow yexception() << "error on read field " << Meta::Name << ": " << e.what();
            }
        } else if constexpr (Meta::TypeDesc::Default) {
            value = Meta::Default;
        }
    }
}

template<typename Meta>
inline void Size(TSizeCollector& collector, TKafkaInt16 version, const typename Meta::Type& value) {
    if constexpr (!VersionNone<Meta::TaggedVersionMin, Meta::TaggedVersionMax>()) {
        if (VersionCheck<Meta::TaggedVersionMin, Meta::TaggedVersionMax>(version)) {
            if (!IsDefaultValue<Meta>(value)) {
                ++collector.NumTaggedFields;

                i64 size = TypeStrategy<Meta, typename Meta::Type>::DoSize(version, value);
                collector.Size += size + SizeOfUnsignedVarint(Meta::Tag) + SizeOfUnsignedVarint(size); 
            }
        } else if (VersionCheck<Meta::PresentVersionMin, Meta::PresentVersionMax>(version)) {
            collector.Size += TypeStrategy<Meta, typename Meta::Type>::DoSize(version, value);
        }
    } else {
        if (VersionCheck<Meta::PresentVersionMin, Meta::PresentVersionMax>(version)) {
            collector.Size += TypeStrategy<Meta, typename Meta::Type>::DoSize(version, value);
        }
    }
}

template<typename Meta>
inline void WriteTag(TKafkaWritable& writable, TKafkaInt16 version, const typename Meta::Type& value) {
    if constexpr (!VersionNone<Meta::TaggedVersionMin, Meta::TaggedVersionMax>()) {
        if (VersionCheck<Meta::TaggedVersionMin, Meta::TaggedVersionMax>(version)) {
            if (!IsDefaultValue<Meta>(value)) {
                writable.writeUnsignedVarint(Meta::Tag);
                writable.writeUnsignedVarint(TypeStrategy<Meta, typename Meta::Type>::DoSize(version, value));
                TypeStrategy<Meta, typename Meta::Type>::DoWriteTag(writable, version, value);
            }
        }
    }
}

template<typename Meta>
inline void ReadTag(TKafkaReadable& readable, TKafkaInt16 version, typename Meta::Type& value) {
    if constexpr (!VersionNone<Meta::TaggedVersionMin, Meta::TaggedVersionMax>()) {
        TypeStrategy<Meta, typename Meta::Type>::DoRead(readable, version, value);
    }
}


} // NPrivate

} // namespace NKafka
