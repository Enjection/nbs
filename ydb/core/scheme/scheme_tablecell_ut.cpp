#include <ydb/core/scheme/scheme_tablecell.h>

#include <library/cpp/testing/unittest/registar.h>
#include <util/generic/vector.h>

using namespace NActors;

Y_UNIT_TEST_SUITE(Scheme) {

    using namespace NKikimr;

    namespace NTypeIds = NScheme::NTypeIds;
    using TTypeInfo = NScheme::TTypeInfo;

    Y_UNIT_TEST(EmptyOwnedCellVec) {
        TOwnedCellVec empty;
        UNIT_ASSERT_VALUES_EQUAL(empty.size(), 0u);
        TOwnedCellVec copy(empty);
        UNIT_ASSERT_VALUES_EQUAL(copy.size(), 0u);
        copy = empty;
        UNIT_ASSERT_VALUES_EQUAL(copy.size(), 0u);
        TOwnedCellVec moved(std::move(empty));
        UNIT_ASSERT_VALUES_EQUAL(moved.size(), 0u);
        moved = std::move(copy);
        UNIT_ASSERT_VALUES_EQUAL(moved.size(), 0u);
    }

    Y_UNIT_TEST(NonEmptyOwnedCellVec) {
        ui64 intVal = 42;
        char bigStrVal[] = "This is a large string value that shouldn't be inlined";

        TVector<TCell> cells;
        cells.emplace_back(TCell::Make(intVal));
        cells.emplace_back(bigStrVal, sizeof(bigStrVal));

        TOwnedCellVec initial = TOwnedCellVec::Make(cells);
        UNIT_ASSERT_VALUES_EQUAL(initial.size(), 2u);
        UNIT_ASSERT_VALUES_EQUAL(initial[0].AsValue<ui64>(), intVal);
        UNIT_ASSERT_VALUES_EQUAL(initial[1].AsBuf(), TStringBuf(bigStrVal, sizeof(bigStrVal)));

        TOwnedCellVec copied(initial);
        UNIT_ASSERT_VALUES_EQUAL(copied.size(), 2u);
        UNIT_ASSERT_VALUES_EQUAL(copied[0].AsValue<ui64>(), intVal);
        UNIT_ASSERT_VALUES_EQUAL(copied[1].AsBuf(), TStringBuf(bigStrVal, sizeof(bigStrVal)));

        TOwnedCellVec moved(std::move(initial));
        UNIT_ASSERT_VALUES_EQUAL(initial.size(), 0u);
        UNIT_ASSERT_VALUES_EQUAL(moved.size(), 2u);
        UNIT_ASSERT_VALUES_EQUAL(moved[0].AsValue<ui64>(), intVal);
        UNIT_ASSERT_VALUES_EQUAL(moved[1].AsBuf(), TStringBuf(bigStrVal, sizeof(bigStrVal)));

        initial = copied;
        UNIT_ASSERT_VALUES_EQUAL(initial.size(), 2u);
        UNIT_ASSERT_VALUES_EQUAL(initial[0].AsValue<ui64>(), intVal);
        UNIT_ASSERT_VALUES_EQUAL(initial[1].AsBuf(), TStringBuf(bigStrVal, sizeof(bigStrVal)));

        moved = std::move(initial);
        UNIT_ASSERT_VALUES_EQUAL(initial.size(), 0u);
        UNIT_ASSERT_VALUES_EQUAL(moved.size(), 2u);
        UNIT_ASSERT_VALUES_EQUAL(moved[0].AsValue<ui64>(), intVal);
        UNIT_ASSERT_VALUES_EQUAL(moved[1].AsBuf(), TStringBuf(bigStrVal, sizeof(bigStrVal)));
    }

    Y_UNIT_TEST(TSerializedCellVec) {
        ui64 intVal = 42;
        char smallStrVal[] = "str1";
        char bigStrVal[] =
                "> You have requested to link your commit to an existing review request 849684\n"
                "> This review request is not ready yet to be merged, see its merge requirements below";
        float floatVal = 0.42;
        double doubleVal = -0.0025;

        TVector<TCell> cells;
        TVector<TTypeInfo> types;

        cells.push_back(TCell((const char*)&doubleVal, sizeof(doubleVal)));
        types.push_back(TTypeInfo(NTypeIds::Double));
        cells.push_back(TCell(smallStrVal, sizeof(smallStrVal)));
        types.push_back(TTypeInfo(NTypeIds::String));
        cells.push_back(TCell());
        types.push_back(TTypeInfo(NTypeIds::Utf8));
        cells.push_back(TCell(smallStrVal, sizeof(smallStrVal)));
        types.push_back(TTypeInfo(NTypeIds::Utf8));
        cells.push_back(TCell((const char*)&floatVal, sizeof(floatVal)));
        types.push_back(TTypeInfo(NTypeIds::Float));
        cells.push_back(TCell());
        types.push_back(TTypeInfo(NTypeIds::Decimal));
        cells.push_back(TCell((const char*)&intVal, sizeof(ui64)));
        types.push_back(TTypeInfo(NTypeIds::Uint64));
        cells.push_back(TCell());
        types.push_back(TTypeInfo(NTypeIds::Decimal));
        cells.push_back(TCell());
        types.push_back(TTypeInfo(NTypeIds::Uint8));
        cells.push_back(TCell(bigStrVal, sizeof(bigStrVal)));
        types.push_back(TTypeInfo(NTypeIds::Utf8));
        cells.push_back(TCell());
        types.push_back(TTypeInfo(NTypeIds::Double));
        cells.push_back(TCell((const char*)&intVal, sizeof(i32)));
        types.push_back(TTypeInfo(NTypeIds::Int32));

        TSerializedCellVec vec(TSerializedCellVec::Serialize(cells));

        UNIT_ASSERT_VALUES_EQUAL(cells.size(), 12);
        UNIT_ASSERT_VALUES_EQUAL(types.size(), cells.size());
        UNIT_ASSERT_VALUES_EQUAL(vec.GetCells().size(), cells.size());

        UNIT_ASSERT_DOUBLES_EQUAL(cells[0].AsValue<double>(), doubleVal, 0.00001);
        UNIT_ASSERT_VALUES_EQUAL(cells[1].AsBuf().data(), smallStrVal);
        UNIT_ASSERT_VALUES_EQUAL(cells[3].AsBuf().data(), smallStrVal);
        UNIT_ASSERT_DOUBLES_EQUAL(cells[4].AsValue<float>(), floatVal, 0.00001);
        UNIT_ASSERT_VALUES_EQUAL(cells[6].AsValue<ui64>(), intVal);
        UNIT_ASSERT_VALUES_EQUAL(cells[9].AsBuf().data(), bigStrVal);
        UNIT_ASSERT_VALUES_EQUAL(cells[11].AsValue<i32>(), intVal);

        UNIT_ASSERT_VALUES_EQUAL(CompareTypedCellVectors(vec.GetCells().data(), cells.data(),
                                                         types.data(),
                                                         vec.GetCells().size(), cells.size()),
                                 0);

        TSerializedCellVec vecCopy(vec);

        UNIT_ASSERT_VALUES_EQUAL(CompareTypedCellVectors(vecCopy.GetCells().data(), cells.data(),
                                                         types.data(),
                                                         vecCopy.GetCells().size(), cells.size()),
                                 0);


        TSerializedCellVec vec2(std::move(vecCopy));

        UNIT_ASSERT_VALUES_EQUAL(CompareTypedCellVectors(vec2.GetCells().data(), cells.data(),
                                                         types.data(),
                                                         vec2.GetCells().size(), cells.size()),
                                 0);

        TSerializedCellVec vec3;
        UNIT_ASSERT(vec3.GetCells().empty());
        UNIT_ASSERT(vec3.GetBuffer().empty());

        TString buf = vec.GetBuffer();
        UNIT_ASSERT(buf.size() > cells.size()*2);
        vec3.Parse(buf);

        UNIT_ASSERT_VALUES_EQUAL(CompareTypedCellVectors(vec3.GetCells().data(), cells.data(),
                                                         types.data(),
                                                         vec3.GetCells().size(), cells.size()),
                                 0);

        const int ITERATIONS = 1000;//10000000;

        {
            TInstant start = TInstant::Now();
            for (int i = 0; i < ITERATIONS; ++i) {
                TSerializedCellVec::Serialize(cells);
            }
            TInstant finish = TInstant::Now();
            Cerr << "Serialize: " << finish - start << Endl;
        }

        {
            TString buf = vec.GetBuffer();
            TInstant start = TInstant::Now();
                for (int i = 0; i < ITERATIONS; ++i) {
                vec3.Parse(buf);
            }
            TInstant finish = TInstant::Now();
            Cerr << "Parse: " << finish - start << Endl;
        }

        {
            TInstant start = TInstant::Now();
            for (int i = 0; i < ITERATIONS; ++i) {
                vec3 = vec;
            }
            TInstant finish = TInstant::Now();
            Cerr << "Copy: " << finish - start << Endl;
        }

        {
            size_t unused = 0;
            TInstant start = TInstant::Now();
            for (int i = 0; i < ITERATIONS; ++i) {
                TSerializedCellVec vec3(std::move(vec));
                unused += vec3.GetCells().size();
                if (unused % 10000 != 0) {
                    vec = std::move(vec3);
                } else {
                    vec = vec2;
                }
            }
            TInstant finish = TInstant::Now();
            UNIT_ASSERT_VALUES_EQUAL(unused, ITERATIONS*cells.size());
            Cerr << "Move: " << finish - start << Endl;
        }
    }

    Y_UNIT_TEST(CellVecTryParse) {
        TSerializedCellVec vec;
        UNIT_ASSERT(!TSerializedCellVec::TryParse("\1", vec));
        UNIT_ASSERT(!TSerializedCellVec::TryParse("\1\1", vec));

        const TString buf = TSerializedCellVec::Serialize({TCell(), TCell()});
        UNIT_ASSERT_VALUES_EQUAL(buf.size(), sizeof(ui16) + 2 * 4);

        {
            for (size_t i = 0; i < buf.size(); ++i) {
                TString hacked = buf;
                hacked[1] = hacked[i] + 1;
                UNIT_ASSERT(!TSerializedCellVec::TryParse(hacked, vec));
            }
        }
    }

    /**
     * CompareOrder test for cell1 < cell2 < cell3 given a type id
     */
    void DoTestCompareOrder(const TCell& cell1, const TCell& cell2, const TCell& cell3, NScheme::TTypeId typeId) {
        TCell nullCell;

        NScheme::TTypeIdOrder typeIdDescending(typeId, NScheme::EOrder::Descending);

        NScheme::TTypeInfo type(typeId);
        NScheme::TTypeInfoOrder typeDescending(typeIdDescending);

        // NULL is always equal to itself, both ascending and descending
        UNIT_ASSERT_EQUAL(CompareTypedCells(nullCell, nullCell, type), 0);
        UNIT_ASSERT_EQUAL(CompareTypedCells(nullCell, nullCell, typeDescending), 0);

        // NULL is always the first value, both ascending and descending
        UNIT_ASSERT_LT(CompareTypedCells(nullCell, cell1, type), 0);
        UNIT_ASSERT_LT(CompareTypedCells(nullCell, cell2, type), 0);
        UNIT_ASSERT_LT(CompareTypedCells(nullCell, cell3, type), 0);
        UNIT_ASSERT_LT(CompareTypedCells(nullCell, cell1, typeDescending), 0);
        UNIT_ASSERT_LT(CompareTypedCells(nullCell, cell2, typeDescending), 0);
        UNIT_ASSERT_LT(CompareTypedCells(nullCell, cell3, typeDescending), 0);

        // Values should be equal to themselves, both ascending and descending
        UNIT_ASSERT_EQUAL(CompareTypedCells(cell1, cell1, type), 0);
        UNIT_ASSERT_EQUAL(CompareTypedCells(cell2, cell2, type), 0);
        UNIT_ASSERT_EQUAL(CompareTypedCells(cell3, cell3, type), 0);
        UNIT_ASSERT_EQUAL(CompareTypedCells(cell1, cell1, typeDescending), 0);
        UNIT_ASSERT_EQUAL(CompareTypedCells(cell2, cell2, typeDescending), 0);
        UNIT_ASSERT_EQUAL(CompareTypedCells(cell3, cell3, typeDescending), 0);

        // Check all ascending permutations
        UNIT_ASSERT_LT(CompareTypedCells(cell1, cell2, type), 0);
        UNIT_ASSERT_LT(CompareTypedCells(cell1, cell3, type), 0);
        UNIT_ASSERT_GT(CompareTypedCells(cell2, cell1, type), 0);
        UNIT_ASSERT_LT(CompareTypedCells(cell2, cell3, type), 0);
        UNIT_ASSERT_GT(CompareTypedCells(cell3, cell1, type), 0);
        UNIT_ASSERT_GT(CompareTypedCells(cell3, cell2, type), 0);

        // Check all descending permutations
        UNIT_ASSERT_GT(CompareTypedCells(cell1, cell2, typeDescending), 0);
        UNIT_ASSERT_GT(CompareTypedCells(cell1, cell3, typeDescending), 0);
        UNIT_ASSERT_LT(CompareTypedCells(cell2, cell1, typeDescending), 0);
        UNIT_ASSERT_GT(CompareTypedCells(cell2, cell3, typeDescending), 0);
        UNIT_ASSERT_LT(CompareTypedCells(cell3, cell1, typeDescending), 0);
        UNIT_ASSERT_LT(CompareTypedCells(cell3, cell2, typeDescending), 0);
    }

    Y_UNIT_TEST(CompareOrder) {
        ui64 intVal42 = 42;
        ui64 intVal51 = 51;
        ui64 intVal99 = 99;
        DoTestCompareOrder(
            TCell((const char*)&intVal42, sizeof(intVal42)),
            TCell((const char*)&intVal51, sizeof(intVal51)),
            TCell((const char*)&intVal99, sizeof(intVal99)),
            NScheme::NTypeIds::Uint64);

        char strVal1[] = "This is the first value";
        char strVal2[] = "This is the first value2";
        char strVal3[] = "This is the last value";
        DoTestCompareOrder(
            TCell((const char*)&strVal1, sizeof(strVal1) - 1),
            TCell((const char*)&strVal2, sizeof(strVal2) - 1),
            TCell((const char*)&strVal3, sizeof(strVal3) - 1),
            NScheme::NTypeIds::String);

        std::pair<ui64, i64> decVal1{ 41, 41 };
        std::pair<ui64, i64> decVal2{ 51, 41 };
        std::pair<ui64, i64> decVal3{ 41, 51 };
        DoTestCompareOrder(
            TCell((const char*)&decVal1, sizeof(decVal1)),
            TCell((const char*)&decVal2, sizeof(decVal2)),
            TCell((const char*)&decVal3, sizeof(decVal3)),
            NScheme::NTypeIds::Decimal);
    }

    Y_UNIT_TEST(YqlTypesMustBeDefined) {
        const char charArr[64] = { 0 };

        TArrayRef<const NScheme::TTypeId> yqlIds(NScheme::NTypeIds::YqlIds);
        for (NScheme::TTypeId typeId : yqlIds) {
            NScheme::TTypeInfo typeInfo(typeId);
            switch (typeId) {
            case NScheme::NTypeIds::Int8:
                GetValueHash(typeInfo, TCell(charArr, sizeof(i8)));
                CompareTypedCells(TCell(charArr, sizeof(i8)), TCell(charArr, sizeof(i8)), typeInfo);
                break;
            case NScheme::NTypeIds::Uint8:
                GetValueHash(typeInfo, TCell(charArr, sizeof(ui8)));
                CompareTypedCells(TCell(charArr, sizeof(ui8)), TCell(charArr, sizeof(ui8)), typeInfo);
                break;
            case NScheme::NTypeIds::Int16:
                GetValueHash(typeInfo, TCell(charArr, sizeof(i16)));
                CompareTypedCells(TCell(charArr, sizeof(i16)), TCell(charArr, sizeof(i16)), typeInfo);
                break;
            case NScheme::NTypeIds::Uint16:
                GetValueHash(typeInfo, TCell(charArr, sizeof(ui16)));
                CompareTypedCells(TCell(charArr, sizeof(ui16)), TCell(charArr, sizeof(ui16)), typeInfo);
                break;
            case NScheme::NTypeIds::Int32:
                GetValueHash(typeInfo, TCell(charArr, sizeof(i32)));
                CompareTypedCells(TCell(charArr, sizeof(i32)), TCell(charArr, sizeof(i32)), typeInfo);
                break;
            case NScheme::NTypeIds::Uint32:
                GetValueHash(typeInfo, TCell(charArr, sizeof(ui32)));
                CompareTypedCells(TCell(charArr, sizeof(ui32)), TCell(charArr, sizeof(ui32)), typeInfo);
                break;
            case NScheme::NTypeIds::Int64:
                GetValueHash(typeInfo, TCell(charArr, sizeof(i64)));
                CompareTypedCells(TCell(charArr, sizeof(i64)), TCell(charArr, sizeof(i64)), typeInfo);
                break;
            case NScheme::NTypeIds::Uint64:
                GetValueHash(typeInfo, TCell(charArr, sizeof(ui64)));
                CompareTypedCells(TCell(charArr, sizeof(ui64)), TCell(charArr, sizeof(ui64)), typeInfo);
                break;
            case NScheme::NTypeIds::Bool:
                GetValueHash(typeInfo, TCell(charArr, sizeof(ui8)));
                CompareTypedCells(TCell(charArr, sizeof(ui8)), TCell(charArr, sizeof(ui8)), typeInfo);
                break;
            case NScheme::NTypeIds::Double:
                GetValueHash(typeInfo, TCell(charArr, sizeof(double)));
                CompareTypedCells(TCell(charArr, sizeof(double)), TCell(charArr, sizeof(double)), typeInfo);
                break;
            case NScheme::NTypeIds::Float:
                GetValueHash(typeInfo, TCell(charArr, sizeof(float)));
                CompareTypedCells(TCell(charArr, sizeof(float)), TCell(charArr, sizeof(float)), typeInfo);
                break;
            case NScheme::NTypeIds::String:
            case NScheme::NTypeIds::Utf8:
            case NScheme::NTypeIds::Yson:
            case NScheme::NTypeIds::Json:
            case NScheme::NTypeIds::JsonDocument:
            case NScheme::NTypeIds::DyNumber:
                GetValueHash(typeInfo, TCell(charArr, 30));
                CompareTypedCells(TCell(charArr, 30), TCell(charArr, 30), typeInfo);
                break;
            case NScheme::NTypeIds::Decimal:
                GetValueHash(typeInfo, TCell(charArr, sizeof(ui64) * 2));
                CompareTypedCells(TCell(charArr, sizeof(ui64) * 2), TCell(charArr, sizeof(ui64) * 2), typeInfo);
                break;
            case NScheme::NTypeIds::Date:
                GetValueHash(typeInfo, TCell(charArr, sizeof(ui16)));
                CompareTypedCells(TCell(charArr, sizeof(ui16)), TCell(charArr, sizeof(ui16)), typeInfo);
                break;
            case NScheme::NTypeIds::Datetime:
                GetValueHash(typeInfo, TCell(charArr, sizeof(ui32)));
                CompareTypedCells(TCell(charArr, sizeof(ui32)), TCell(charArr, sizeof(ui32)), typeInfo);
                break;
            case NScheme::NTypeIds::Timestamp:
                GetValueHash(typeInfo, TCell(charArr, sizeof(ui64)));
                CompareTypedCells(TCell(charArr, sizeof(ui64)), TCell(charArr, sizeof(ui64)), typeInfo);
                break;
            case NScheme::NTypeIds::Interval:
                GetValueHash(typeInfo, TCell(charArr, sizeof(i64)));
                CompareTypedCells(TCell(charArr, sizeof(i64)), TCell(charArr, sizeof(i64)), typeInfo);
                break;
            default:
                UNIT_FAIL("undefined YQL type");
            }
        }
    }
}
