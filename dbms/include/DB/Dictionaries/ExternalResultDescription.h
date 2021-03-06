#pragma once

#include <ext/range.hpp>
#include <DB/Core/Block.h>


namespace DB
{

namespace ErrorCodes
{
	extern const int UNKNOWN_TYPE;
}

/** Общая часть реализации MySQLBlockInputStream, MongoDBBlockInputStream, ODBCBlockInputStream.
  */
struct ExternalResultDescription
{
	enum struct value_type_t
	{
		UInt8,
		UInt16,
		UInt32,
		UInt64,
		Int8,
		Int16,
		Int32,
		Int64,
		Float32,
		Float64,
		String,
		Date,
		DateTime
	};

	Block sample_block;
	std::vector<value_type_t> types;
	std::vector<std::string> names;
	ConstColumnPlainPtrs sample_columns;

	void init(const Block & sample_block_)
	{
		sample_block = sample_block_;

		const auto num_columns = sample_block.columns();
		types.reserve(num_columns);
		names.reserve(num_columns);
		sample_columns.reserve(num_columns);

		for (const auto idx : ext::range(0, num_columns))
		{
			const auto & column = sample_block.getByPosition(idx);
			const auto type = column.type.get();

			if (typeid_cast<const DataTypeUInt8 *>(type))
				types.push_back(value_type_t::UInt8);
			else if (typeid_cast<const DataTypeUInt16 *>(type))
				types.push_back(value_type_t::UInt16);
			else if (typeid_cast<const DataTypeUInt32 *>(type))
				types.push_back(value_type_t::UInt32);
			else if (typeid_cast<const DataTypeUInt64 *>(type))
				types.push_back(value_type_t::UInt64);
			else if (typeid_cast<const DataTypeInt8 *>(type))
				types.push_back(value_type_t::Int8);
			else if (typeid_cast<const DataTypeInt16 *>(type))
				types.push_back(value_type_t::Int16);
			else if (typeid_cast<const DataTypeInt32 *>(type))
				types.push_back(value_type_t::Int32);
			else if (typeid_cast<const DataTypeInt64 *>(type))
				types.push_back(value_type_t::Int64);
			else if (typeid_cast<const DataTypeFloat32 *>(type))
				types.push_back(value_type_t::Float32);
			else if (typeid_cast<const DataTypeFloat64 *>(type))
				types.push_back(value_type_t::Float64);
			else if (typeid_cast<const DataTypeString *>(type))
				types.push_back(value_type_t::String);
			else if (typeid_cast<const DataTypeDate *>(type))
				types.push_back(value_type_t::Date);
			else if (typeid_cast<const DataTypeDateTime *>(type))
				types.push_back(value_type_t::DateTime);
			else
				throw Exception{
					"Unsupported type " + type->getName(),
					ErrorCodes::UNKNOWN_TYPE};

			names.emplace_back(column.name);
			sample_columns.emplace_back(column.column.get());
		}
	}
};

}
