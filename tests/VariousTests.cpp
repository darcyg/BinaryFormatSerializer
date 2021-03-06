// VariousTests.cpp - tests for BinaryFormatSerializer
//

#include "serializers/VectorSaveSerializer.h"
#include "serializers/MemorySerializer.h"

#include "formatters/endian_formatter.h"
#include "formatters/const_formatter.h"
#include "formatters/size_prefix_formatter.h"
#include "formatters/vector_formatter.h"
#include "formatters/external_value.h"

#include "gtest/gtest.h"

namespace {

using namespace binary_format;
    
TEST(ConstFormatterWorks, SavingAndLoading)
{
    {
        VectorSaveSerializer vectorWriter;
        save< const_formatter< little_endian<4> > >(vectorWriter, 0x12345678);
        const auto value = std::vector<uint8_t> { 0x78, 0x56, 0x34, 0x12 };
        EXPECT_EQ(vectorWriter.getData(), value);
    }

    {
        std::vector<uint8_t> data { 0x12, 0x34, 0x56, 0x78 };
        MemoryLoadSerializer vectorReader(data);
        EXPECT_NO_THROW( load< const_formatter< little_endian<4> > >(vectorReader, 0x78563412) );
    }

    {
        std::vector<uint8_t> data { 0x12, 0x34, 0x56, 0x78 };
        MemoryLoadSerializer vectorReader(data);
        ASSERT_THROW(load< const_formatter< little_endian<4> > >(vectorReader, 0), invalid_data);
    }
}

TEST(SizePrefixFormatterWorks, SavingAndLoading)
{
    {
        std::array<uint8_t, 8> data;
        MemorySaveSerializer vectorWriter(data);
        save< size_prefix_formatter< little_endian<4>, little_endian<4> > >(vectorWriter, 0x12345678);
        const auto value = std::vector<uint8_t> { 0x04, 0x00, 0x00, 0x00, 0x78, 0x56, 0x34, 0x12 };
        ASSERT_TRUE(( std::equal(value.begin(), value.end(), vectorWriter.getData()) ));
    }

    {
        uint8_t data[] = { 0x04, 0x00, 0x00, 0x00, 0x78, 0x56, 0x34, 0x12 };
        MemoryLoadSerializer arrayReader(data, sizeof(data));
        int value;
        load< size_prefix_formatter< little_endian<4>, little_endian<4> > >(arrayReader, value);
        EXPECT_EQ(value, 0x12345678);
    }

    {
        uint8_t data[] = { 0x12, 0x34, 0x56, 0x78 };
        MemoryLoadSerializer arrayReader(data, sizeof(data));
        int value;
        ASSERT_THROW((load< size_prefix_formatter< little_endian<2>, little_endian<2> > >(arrayReader, value)), invalid_data);
    }
}

TEST(ExternalValueWorks, SavingAndLoading)
{
    {
        VectorSaveSerializer vectorWriter;
        int size = 5;
        std::vector<uint8_t> vec { 0x00, 0x01, 0x02, 0x03, 0x04 };
        save(vectorWriter, vec, create_vector_formatter(create_external_value(size), little_endian<1>()));
        const auto value = std::vector<uint8_t> { 0x00, 0x01, 0x02, 0x03, 0x04 };
        EXPECT_EQ(vectorWriter.getData(), value);
    }

    {
        std::vector<uint8_t> data { 0x00, 0x01, 0x02, 0x03, 0x04 };
        MemoryLoadSerializer vectorReader(data);
        int size = 5;
        std::vector<uint8_t> vec;
        load(vectorReader, vec, create_vector_formatter(create_external_value(size), little_endian<1>()));
        EXPECT_EQ(vec, data);
    }

    {
        VectorSaveSerializer vectorWriter;
        int size = 10;
        std::vector<uint8_t> vec { 0x00, 0x01, 0x02, 0x03, 0x04 };
        EXPECT_THROW( (save(vectorWriter, vec, create_vector_formatter(create_external_value(size), little_endian<1>()))), serialization_exception);
    }
}

}  // namespace
