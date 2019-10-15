// Copyright 2019 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
    
#include <gtest/gtest.h>
#include <dds/core/xtypes/xtypes.hpp>
#include <iostream>

#include <cmath>
#include <bitset>

using namespace std; 
using namespace dds::core::xtypes;

#define DEBUG_MACRO_OFF


#ifdef DEBUG_MACRO_ON 
    size_t debug_count = 0; 
    void debug(bool do_it=true) 
    { 
        if(do_it) 
        { 
            cout << "got up to: " << debug_count << endl; 
            ++debug_count; 
        } 
    }
#else
    void debug(bool do_it=true){}
#endif                

 /********************************
 *        DynamicType Tests        *
 ********************************/

TEST (StructType , verify_constructor_and_size) 
{ 
    StructType st("struct_name"); 
    EXPECT_EQ("struct_name", st.name());
    st.add_member(Member("int", primitive_type<int>()));
    EXPECT_EQ(4, st.memory_size());
}

TEST (StructType, build_one_of_each_primitive_type) 
{ 
    StructType st("struct_name"); 
    EXPECT_EQ("struct_name", st.name());
    EXPECT_EQ(TypeKind::STRUCTURE_TYPE, st.kind());
    size_t mem_size = 0;
    st.add_member(Member("bool", primitive_type<bool>()));
    mem_size+=sizeof(bool);
    EXPECT_EQ(mem_size, st.memory_size());
    st.add_member(Member("uint8_t", primitive_type<uint8_t>()));
    mem_size+=sizeof(uint8_t);
    EXPECT_EQ(mem_size, st.memory_size());
    st.add_member(Member("int16_t", primitive_type<int16_t>()));
    mem_size+=sizeof(int16_t);
    EXPECT_EQ(mem_size, st.memory_size());
    st.add_member(Member("uint16_t", primitive_type<uint16_t>()));
    mem_size+=sizeof(uint16_t);
    EXPECT_EQ(mem_size, st.memory_size());
    st.add_member(Member("int32_t", primitive_type<int32_t>()));
    mem_size+=sizeof(int32_t);
    EXPECT_EQ(mem_size, st.memory_size());
    st.add_member(Member("uint32_t", primitive_type<uint32_t>()));
    mem_size+=sizeof(uint32_t);
    EXPECT_EQ(mem_size, st.memory_size());
    st.add_member(Member("int64_t", primitive_type<int64_t>()));
    mem_size+=sizeof(int64_t);
    EXPECT_EQ(mem_size, st.memory_size());
    st.add_member(Member("uint64_t", primitive_type<uint64_t>()));
    mem_size+=sizeof(uint64_t);
    EXPECT_EQ(mem_size, st.memory_size());
    st.add_member(Member("float", primitive_type<float>()));
    mem_size+=sizeof(float);
    EXPECT_EQ(mem_size, st.memory_size());
    st.add_member(Member("double", primitive_type<double>()));
    mem_size+=sizeof(double);
    EXPECT_EQ(mem_size, st.memory_size());
    st.add_member(Member("long_double", primitive_type<long double>()));
    mem_size+=sizeof(long double);
    EXPECT_EQ(mem_size, st.memory_size());
}

TEST (StructType, cascade_add_member_and_copy)
{
    StructType st("struct_name");
    st.add_member( 
        Member("bool", primitive_type<bool>())).add_member(
        Member("uint8_t", primitive_type<uint8_t>())).add_member(
        Member("int16_t", primitive_type<int16_t>())).add_member(
        Member("uint16_t", primitive_type<uint16_t>())).add_member(
        Member("int32_t", primitive_type<int32_t>())).add_member(
        Member("uint32_t", primitive_type<uint32_t>())).add_member(
        Member("int64_t", primitive_type<int64_t>())).add_member(
        Member("uint64_t", primitive_type<uint64_t>())).add_member(
        Member("float", primitive_type<float>())).add_member(
        Member("double", primitive_type<double>())).add_member(
        Member("long_double", primitive_type<long double>())); 
    
    size_t mem_size = 0;
    mem_size+=sizeof(bool);
    mem_size+=sizeof(uint8_t);
    mem_size+=sizeof(int16_t);
    mem_size+=sizeof(uint16_t);
    mem_size+=sizeof(int32_t);
    mem_size+=sizeof(uint32_t);
    mem_size+=sizeof(int64_t);
    mem_size+=sizeof(uint64_t);
    mem_size+=sizeof(float);
    mem_size+=sizeof(double);
    mem_size+=sizeof(long double);
    
    StructType cp = st;
    EXPECT_EQ("struct_name", cp.name());
    EXPECT_EQ(mem_size, cp.memory_size());
}

TEST (StructType, self_assign)
{
    StructType st("struct_name");
    st.add_member(
        Member("long_double", primitive_type<long double>())).add_member(
        Member("uint64_t", primitive_type<uint64_t>())).add_member(
        Member("uint8_t", primitive_type<uint8_t>()));    
    
    StructType in("struct_name");
    in.add_member(
        Member("long_double", primitive_type<long double>())).add_member(
        Member("uint64_t", primitive_type<uint64_t>())).add_member(
        Member("uint8_t", primitive_type<uint32_t>()));    
    
    st.add_member(Member("in_member_name", in));
    st.add_member(Member("selfassign_member_name", st));
    EXPECT_EQ(TypeKind::STRUCTURE_TYPE, st.member("in_member_name").type().kind());
    EXPECT_EQ(TypeKind::STRUCTURE_TYPE, st.member("selfassign_member_name").type().kind());
    EXPECT_EQ(TypeKind::FLOAT_128_TYPE, 
        static_cast<const StructType&>(st.member("selfassign_member_name").type()).member("long_double").type().kind());
    EXPECT_EQ(TypeKind::UINT_64_TYPE, 
        static_cast<const StructType&>(st.member("selfassign_member_name").type()).member("uint64_t").type().kind());
    EXPECT_EQ(TypeKind::UINT_8_TYPE, 
        static_cast<const StructType&>(st.member("selfassign_member_name").type()).member("uint8_t").type().kind() );
    size_t mem_size_in = 0;
    mem_size_in+=sizeof(long double);
    mem_size_in+=sizeof(uint64_t);
    mem_size_in+=sizeof(uint32_t);
    EXPECT_EQ(mem_size_in, st.member("in_member_name").type().memory_size());
    
    mem_size_in+=sizeof(long double);
    mem_size_in+=sizeof(uint64_t);
    mem_size_in+=sizeof(uint8_t);
    EXPECT_EQ(mem_size_in, st.member("selfassign_member_name").type().memory_size());
}

TEST (StructType, type_verify_test)
{
    StructType st("struct_name");
    st.add_member(
        Member("bool", primitive_type<bool>())).add_member(
        Member("uint8_t", primitive_type<uint8_t>())).add_member(
        Member("int16_t",  primitive_type<int16_t>())).add_member(
        Member("uint16_t",  primitive_type<uint16_t>())).add_member(
        Member("int32_t",  primitive_type<int32_t>())).add_member(
        Member("uint32_t", primitive_type<uint32_t>())).add_member(
        Member("int64_t", primitive_type<int64_t>())).add_member(
        Member("uint64_t", primitive_type<uint64_t>())).add_member(
        Member("float",  primitive_type<float>())).add_member(
        Member("double", primitive_type<double>())).add_member(
        Member("long double", primitive_type<long double>())).add_member(
        Member("char", primitive_type<char>())).add_member(
        Member("char16_t", primitive_type<wchar_t>()));

     
    EXPECT_EQ(TypeKind::BOOLEAN_TYPE, st.member("bool").type().kind());
    EXPECT_EQ(TypeKind::UINT_8_TYPE, st.member("uint8_t").type().kind());
    EXPECT_EQ(TypeKind::INT_16_TYPE , st.member("int16_t").type().kind());
    EXPECT_EQ(TypeKind::UINT_16_TYPE, st.member("uint16_t").type().kind());
    EXPECT_EQ(TypeKind::INT_32_TYPE, st.member("int32_t").type().kind());
    EXPECT_EQ(TypeKind::UINT_32_TYPE, st.member("uint32_t").type().kind());
    EXPECT_EQ(TypeKind::INT_64_TYPE, st.member("int64_t").type().kind());
    EXPECT_EQ(TypeKind::UINT_64_TYPE, st.member("uint64_t").type().kind());
    EXPECT_EQ(TypeKind::FLOAT_32_TYPE, st.member("float").type().kind());
    EXPECT_EQ(TypeKind::FLOAT_64_TYPE, st.member("double").type().kind());
    EXPECT_EQ(TypeKind::FLOAT_128_TYPE, st.member("long double").type().kind());
    EXPECT_EQ(TypeKind::CHAR_8_TYPE, st.member("char").type().kind());
    EXPECT_EQ(TypeKind::CHAR_16_TYPE, st.member("char16_t").type().kind());
    EXPECT_EQ(TypeKind::STRUCTURE_TYPE, st.kind());

    EXPECT_NE(0, uint32_t(TypeKind::PRIMITIVE_TYPE) & uint32_t(st.member("bool").type().kind()));
    EXPECT_NE(0, uint32_t(TypeKind::PRIMITIVE_TYPE) & uint32_t(st.member("uint8_t").type().kind()));
    EXPECT_NE(0, uint32_t(TypeKind::PRIMITIVE_TYPE) & uint32_t(st.member("int16_t").type().kind()));
    EXPECT_NE(0, uint32_t(TypeKind::PRIMITIVE_TYPE) & uint32_t(st.member("uint16_t").type().kind()));
    EXPECT_NE(0, uint32_t(TypeKind::PRIMITIVE_TYPE) & uint32_t(st.member("int32_t").type().kind()));
    EXPECT_NE(0, uint32_t(TypeKind::PRIMITIVE_TYPE) & uint32_t(st.member("uint32_t").type().kind()));
    EXPECT_NE(0, uint32_t(TypeKind::PRIMITIVE_TYPE) & uint32_t(st.member("int64_t").type().kind()));
    EXPECT_NE(0, uint32_t(TypeKind::PRIMITIVE_TYPE) & uint32_t(st.member("uint64_t").type().kind()));
    EXPECT_NE(0, uint32_t(TypeKind::PRIMITIVE_TYPE) & uint32_t(st.member("float").type().kind()));
    EXPECT_NE(0, uint32_t(TypeKind::PRIMITIVE_TYPE) & uint32_t(st.member("double").type().kind()));
    EXPECT_NE(0, uint32_t(TypeKind::PRIMITIVE_TYPE) & uint32_t(st.member("long double").type().kind()));
    EXPECT_NE(0, uint32_t(TypeKind::PRIMITIVE_TYPE) & uint32_t(st.member("char").type().kind()));
    EXPECT_NE(0, uint32_t(TypeKind::PRIMITIVE_TYPE) & uint32_t(st.member("char16_t").type().kind()));
    EXPECT_NE(0, uint32_t(TypeKind::STRUCTURE_TYPE) & uint32_t(st.kind()));

}

TEST (StructMember, base_methods_check)
{
    StructType st("struct_name");
    st.add_member(
        Member("long_double", primitive_type<long double>())).add_member(
        Member("uint64_t", primitive_type<uint64_t>())).add_member(
        Member("uint8_t", primitive_type<uint8_t>()));    
    StructMember sm("struct_member_name", st);

    EXPECT_EQ("struct_member_name", sm.name());
    EXPECT_EQ("struct_name", sm.type().name());
    EXPECT_EQ(false, sm.has_id());
    sm.id(8);
    EXPECT_EQ(true, sm.has_id());
    EXPECT_EQ(8, sm.get_id());
    EXPECT_EQ(false, sm.is_key());
    sm.key(true);
    EXPECT_EQ(true, sm.is_key());
    EXPECT_EQ(false, sm.is_optional());
    sm.optional(true);
    EXPECT_EQ(true, sm.is_optional());
    EXPECT_EQ(false, sm.is_bitset());
    sm.bitset(true);
    EXPECT_EQ(true, sm.is_bitset());
    EXPECT_EQ(0, st.member("long_double").offset());
}

TEST (StructMember, cascade_builder_with_qualifiers)
{
    StructType st("struct_name");
    st.add_member(StructMember("struct_member_name", StructType("inner_struct_name").add_member(
        Member("long_double", primitive_type<long double>())).add_member(
        Member("uint64_t", primitive_type<uint64_t>())).add_member(
        Member("uint8_t", primitive_type<uint8_t>()
            ).key(true).bitset(true).id(9))
        ).id(11).optional(true));
    
    EXPECT_EQ(true, st.member("struct_member_name").is_optional());
    EXPECT_EQ(true, st.member("struct_member_name").has_id());
    EXPECT_EQ(11, st.member("struct_member_name").get_id());

    const StructType &in = static_cast<const StructType&>(st.member("struct_member_name").type());

    EXPECT_EQ(true, in.member("uint8_t").has_id());
    EXPECT_EQ(9, in.member("uint8_t").get_id());
    EXPECT_EQ(true, in.member("uint8_t").is_key());
}

TEST (DynamicData, primitive_types)
{
    StructType st("struct_name");
    st.add_member(
    Member("bool", primitive_type<bool>())).add_member(
    Member("uint8_t", primitive_type<uint8_t>())).add_member(
    Member("int16_t",  primitive_type<int16_t>())).add_member(
    Member("uint16_t",  primitive_type<uint16_t>())).add_member(
    Member("int32_t",  primitive_type<int32_t>())).add_member(
    Member("uint32_t", primitive_type<uint32_t>())).add_member(
    Member("int64_t", primitive_type<int64_t>())).add_member(
    Member("uint64_t", primitive_type<uint64_t>())).add_member(
    Member("float",  primitive_type<float>())).add_member(
    Member("double", primitive_type<double>())).add_member(
    Member("long double", primitive_type<long double>())).add_member(
    Member("char", primitive_type<char>())).add_member(
    Member("char16_t", primitive_type<wchar_t>()));

    EXPECT_EQ(TypeKind::BOOLEAN_TYPE, st.member("bool").type().kind());
    EXPECT_EQ(TypeKind::UINT_8_TYPE, st.member("uint8_t").type().kind());
    EXPECT_EQ(TypeKind::INT_16_TYPE , st.member("int16_t").type().kind());
    EXPECT_EQ(TypeKind::UINT_16_TYPE, st.member("uint16_t").type().kind());
    EXPECT_EQ(TypeKind::INT_32_TYPE, st.member("int32_t").type().kind());
    EXPECT_EQ(TypeKind::UINT_32_TYPE, st.member("uint32_t").type().kind());
    EXPECT_EQ(TypeKind::INT_64_TYPE, st.member("int64_t").type().kind());
    EXPECT_EQ(TypeKind::UINT_64_TYPE, st.member("uint64_t").type().kind());
    EXPECT_EQ(TypeKind::FLOAT_32_TYPE, st.member("float").type().kind());
    EXPECT_EQ(TypeKind::FLOAT_64_TYPE, st.member("double").type().kind());
    EXPECT_EQ(TypeKind::FLOAT_128_TYPE, st.member("long double").type().kind());
    EXPECT_EQ(TypeKind::CHAR_8_TYPE, st.member("char").type().kind());
    EXPECT_EQ(TypeKind::CHAR_16_TYPE, st.member("char16_t").type().kind());
    EXPECT_EQ(TypeKind::STRUCTURE_TYPE, st.kind());

    DynamicData d(st);
    
    d["bool"].value<bool>(true);
    d["uint8_t"].value<uint8_t>(250);
    d["int16_t"].value<int16_t>(-32760);
    d["uint16_t"].value<uint16_t>(65530); 
    d["int32_t"].value<int32_t>(-2147483640); 
    d["uint32_t"].value<uint32_t>(4294967290);
    d["int64_t"].value<int64_t>(-9223372036854775800);
    d["uint64_t"].value<uint64_t>(18446744073709551610ULL);
    d["float"].value<float>(3.1415927410125732421875f);   
    d["double"].value<double>(3.141592653589793115997963468544185161590576171875);
    d["long double"].value<long double>(3.1415926535897932385);
    d["char"].value<char>('f');
    d["char16_t"].value<wchar_t>(34590);
    
    EXPECT_EQ(true, d["bool"].value<bool>()); 
    EXPECT_EQ(250, d["uint8_t"].value<uint8_t>());
    EXPECT_EQ(-32760, d["int16_t"].value<int16_t>());
    EXPECT_EQ(65530,d["uint16_t"].value<uint16_t>()); 
    EXPECT_EQ(-2147483640, d["int32_t"].value<int32_t>()); 
    EXPECT_EQ(4294967290, d["uint32_t"].value<uint32_t>());
    EXPECT_EQ(-9223372036854775800, d["int64_t"].value<int64_t>());
    EXPECT_EQ(18446744073709551610ULL,d["uint64_t"].value<uint64_t>());
    EXPECT_EQ( float(3.1415927410125732421875) , d["float"].value<float>());   
    EXPECT_EQ( double(3.141592653589793115997963468544185161590576171875) , d["double"].value<double>());
    EXPECT_EQ( 3.1415926535897932385 , d["long double"].value<long double>());
    EXPECT_EQ('f', d["char"].value<char>());
    EXPECT_EQ(34590, d["char16_t"].value<wchar_t>());

}

TEST (DynamicData, long_random_sequence)
{
    SequenceType st(primitive_type<double>());

    DynamicData d(st);
    srand48(time(0));
    
    for(int i = 0; i < 65000; ++i)
    {
        double r = lrand48()/double(RAND_MAX);
        d.push(r);
        EXPECT_EQ(r, d[i].value<double>());
    }
}

TEST (DynamicData, sequence)
{
    StructType s("struct");
    s.add_member(Member("sequence_1", SequenceType(primitive_type<char>())));
    s.add_member(Member("sequence_2", SequenceType(primitive_type<char>())));
    {
        DynamicData d(s);
        d["sequence_1"].push('a');
        d["sequence_1"].push('b');
        d["sequence_1"].push('c');
        d["sequence_1"].push('d');
        d["sequence_1"].push('e');
        d["sequence_2"].push(d["sequence_1"]);
    }                
}

TEST (DynamicData, test_just_for_luis)
{
  
    StructType temp_type("Temp");
    temp_type.add_member(Member("number", primitive_type<uint32_t>()));
    temp_type.add_member(Member("string", StringType()));
    StructType fixed_type("Fixed");
    fixed_type.add_member(Member("inner", SequenceType(temp_type)));

    DynamicData fixed(fixed_type);
                
    try{
        for(int i = 0; i < 2; i++)
        {
            DynamicData temp(temp_type);
            temp["number"].value<uint32_t>(i);
            temp["string"].value(std::to_string(i));
            fixed["inner"].push(temp);
         }
    }
    catch(exception &e)
    {
         cout << "dove: " << e.what();
         exit(0xff); 
     }
}

DynamicData cdd(StructType &st)
{     
    StringType stri_t;
    SequenceType sequ_t(stri_t);
    StructType stru_t("just_a_struct");
    stru_t.add_member(
    Member("sequence", sequ_t));
                
    StructType sec_stru("another_struct");
    sec_stru.add_member(
        Member("int", primitive_type<uint32_t>()));
                
    st.add_member("struct", stru_t).add_member("other_struct", sec_stru);
    
    DynamicData dd(st);
    for(int i = 0; i < 1E1; ++i)
        dd["struct"]["sequence"].push<string>("checking");
    return dd;
}

TEST (DynamicData, cp_type)
{
    StructType st("st");
    DynamicData dd = cdd(st);
    cout << dd["struct"]["sequence"][7].string() << endl;
}


DynamicData ret_dyn_data(StructType &st, int i )
{
    switch(i){
        case 1:
            st.add_member(
                Member("int", primitive_type<int32_t>()));
            break;
        case 2:
            st.add_member(
                Member("array", ArrayType(primitive_type<uint32_t>(), 10)));
            break;
        default:
            break;
    }
    DynamicData dt(st);
    switch(i){
        case 1:
            dt["int"].value<int32_t>(32);
            break;
        case 2:
            for(size_t j = 0; j < 10; ++j)
                dt["array"][i].value<uint32_t>(32);
            break;
        default:
            break;
    }
    return dt;
}

TEST (DynamicData, cp)
{
    StructType ist("int_st");
    DynamicData idt = ret_dyn_data(ist, 1);

    StructType ast("arr_st");
    DynamicData adt = ret_dyn_data(ast, 2);
}


DynamicData create_dynamic_data(long double pi, StructType& the_struct, StructType& inner_struct, StructType& second_inner_struct)
{
//    StructType inner_struct("inner_struct");
//    StructType second_inner_struct("second_inner_struct");
    second_inner_struct.add_member(
        Member("second_inner_string", StringType())).add_member(
        Member("second_inner_uint32_t", primitive_type<uint32_t>())).add_member(
        Member("second_inner_array", ArrayType(primitive_type<uint8_t>(), 10)));
    StringType st;     
    inner_struct.add_member(
        Member("inner_string", st)).add_member(
        Member("inner_float", primitive_type<float>())).add_member(
        Member("inner_sequence_string", SequenceType(st))).add_member(
        Member("inner_sequence_struct", SequenceType(second_inner_struct)));
                
    the_struct.add_member( 
        Member("bool", primitive_type<bool>())).add_member(
        Member("uint8_t", primitive_type<uint8_t>())).add_member(
        Member("int16_t", primitive_type<int16_t>())).add_member(
        Member("uint16_t", primitive_type<uint16_t>())).add_member(
        Member("int32_t", primitive_type<int32_t>())).add_member(
        Member("uint32_t", primitive_type<uint32_t>())).add_member(
        Member("int64_t", primitive_type<int64_t>())).add_member(
        Member("uint64_t", primitive_type<uint64_t>())).add_member(
        Member("float", primitive_type<float>())).add_member(
        Member("double", primitive_type<double>())).add_member(
        Member("long_double", primitive_type<long double>())).add_member(
        Member("array", ArrayType(ArrayType(primitive_type<long double>(), 10), 10))).add_member(
        Member("sequence", SequenceType(inner_struct)));
                
    DynamicData the_data(the_struct);
    the_data["bool"].value(true);
    the_data["uint8_t"].value(uint8_t(230));
    the_data["int16_t"].value<>(int16_t(-784));
    the_data["uint16_t"].value<>(uint16_t(784));
    the_data["int32_t"].value<>(int32_t(-5469372));
    the_data["uint32_t"].value<>(uint32_t(45350234));
    the_data["int64_t"].value<>(int64_t(-1234523556));
    the_data["uint64_t"].value<>(uint64_t(1234523556));
    the_data["float"].value<>(float(3.1415926));
    the_data["double"].value<>(double(-3.14159264));
                
    the_data["long_double"].value<>(pi);

    for(int i = 0; i < 1E1; ++i) // creating "sequence"
    {
        DynamicData tmp_data(inner_struct);
        tmp_data["inner_string"].string("lay_down_and_cry");
        tmp_data["inner_float"].value(float(3.1415));
        for (int j = 0; j < 1E1; ++j) // creating "sequence.inner_sequence_string"
        {
//          StringType s;
//            DynamicData dst(s);
//            dst.string("another_prick_in_the_world");
            tmp_data["inner_sequence_string"].push<string>("another_prick_in_the_world");
        }
                                
        for (int j = 0; j < 1E1; ++j) // creating "sequence.inner_sequence_struct"
        {
            DynamicData tmp_inner_data(second_inner_struct);
            tmp_inner_data["second_inner_string"].string("paint_it_black");
            tmp_inner_data["second_inner_uint32_t"].value<>(uint32_t(38));
            for(int k = 0; k < 1E1; ++k) //creating "sequence.inner_sequence_struct.second_inner_array"
            {
                tmp_inner_data["second_inner_array"][k].value<uint8_t>(56);
            }
            tmp_data["inner_sequence_struct"].push(tmp_inner_data);
        }
        for(int j = 0; j < 1E1; ++j)
        {
            for(int k = 0; k < 1E1; ++k)
            {
                the_data["array"][j][k].value<long double>(3.1415);
            }
        }
        the_data["sequence"].push(tmp_data);
    }

    return the_data;
}

TEST (DynamicData, cascade_construction)
{
    long double pi = 3.14159265358979323846;
    StructType the_struct("the_struct");
    StructType inner_struct("inner_struct");
    StructType second_inner_struct("second_inner_struct");
    
    DynamicData the_data = create_dynamic_data(pi, the_struct, inner_struct, second_inner_struct);

    debug(false);
    EXPECT_EQ(45350234, the_data["uint32_t"].value<uint32_t>());
    debug(false);
    EXPECT_EQ(-5469372, the_data["int32_t"].value<int32_t>()); 
    debug(false);
    EXPECT_EQ(784, the_data["uint16_t"].value<uint16_t>());
    debug(false);
    EXPECT_EQ(-784, the_data["int16_t"].value<int16_t>()); 
    debug(false);
    EXPECT_EQ(true, the_data["bool"].value<bool>());
    debug(false);
    EXPECT_EQ(230, the_data["uint8_t"].value<uint8_t>());
    debug(false);
    EXPECT_EQ(-1234523556, the_data["int64_t"].value<int64_t>()); 
    debug(false);
    EXPECT_EQ(1234523556, the_data["uint64_t"].value<uint64_t>());
    debug(false);
    EXPECT_EQ(3.1415926f, the_data["float"].value<float>()); 
    debug(false);
    EXPECT_EQ(-3.14159264, the_data["double"].value<double>()); 
    debug(false);
    EXPECT_EQ(pi, the_data["long_double"].value<long double>());
    debug(false);

    srand48(time(0));
    
    for (int i = 0; i < 1E2; ++i)
    {
        size_t idx_4 = lrand48()%int(1E1);
        EXPECT_EQ("lay_down_and_cry", the_data["sequence"][idx_4]["inner_string"].string());
        EXPECT_EQ(3.1415f, the_data["sequence"][idx_4]["inner_float"].value<float>());
        size_t idx_3 = lrand48()%int(1E1);
        EXPECT_EQ("another_prick_in_the_world", the_data["sequence"][idx_4]["inner_sequence_string"][idx_3].string());
        size_t idx_2 = lrand48()%int(1E1);
        EXPECT_EQ("paint_it_black", the_data["sequence"][idx_4]["inner_sequence_struct"][idx_2]["second_inner_string"].string());
        EXPECT_EQ(38, the_data["sequence"][idx_4]["inner_sequence_struct"][idx_2]["second_inner_uint32_t"].value<uint32_t>());
                
        size_t arr_idx_3 = lrand48()%int(1E1);
        size_t arr_idx_2 = lrand48()%int(1E1);
        size_t arr_idx_1 = lrand48()%int(1E1);
                                
        long double check_over = 3.1415;
        EXPECT_EQ(check_over, the_data["array"][arr_idx_3][arr_idx_2].value<long double>());
        EXPECT_EQ(uint8_t(56), the_data["sequence"][idx_4]["inner_sequence_struct"][idx_2]["second_inner_array"][arr_idx_1].value<uint8_t>());
        debug(false);
    }    
    debug(); //100
}

TEST (DynamicData, curious_interactions)
{
    StructType st("st");
    st.add_member(
    Member("seq", SequenceType(StringType())));

    DynamicData the_data(st);

    StringType str;
    DynamicData dstr(str);
    dstr.string("all_this_stuff");
                
    the_data["seq"].push(dstr);
    EXPECT_EQ("all_this_stuff", the_data["seq"][0].string());
//  EXPECT_EQ("all_this_stuff", the_data["seq"].string());<<=this call fails. It is kept here as a memorandum
}

TEST (DynamicType, testing_is_compatible_string_no_bound)
{
    StringType s;
    StringType r;
    
    EXPECT_EQ(TypeConsistency::EQUALS, r.is_compatible(s) );
    EXPECT_EQ(TypeConsistency::EQUALS, s.is_compatible(r) );
}

TEST (DynamicType, testing_is_compatible_string_same_bound)
{
    srand48(time(0));
    size_t b = lrand48()%1000;
    cout << "the_bound: " << b << endl;
    StringType s(b);
    StringType r(b);
    
    EXPECT_EQ(TypeConsistency::EQUALS, r.is_compatible(s) );
    EXPECT_EQ(TypeConsistency::EQUALS, s.is_compatible(r) );
}


TEST (DynamicType, testing_is_compatible_string_different_bound)
{
    StringType s(15);
    StringType r(30);
    
    EXPECT_NE(0, uint32_t(TypeConsistency::IGNORE_STRING_BOUNDS) & uint32_t(s.is_compatible(r)) );
    EXPECT_NE(0, uint32_t(TypeConsistency::IGNORE_STRING_BOUNDS) & uint32_t(r.is_compatible(s)) );
}


TEST (DynamicType, testing_is_compatible_structure_of_string)
{
    StringType st ;
    StructType r("check");
    r.add_member(Member("string", st));
    StructType s("other_check");
    s.add_member(Member("string", st));

    cout << "types: " << uint32_t(r.is_compatible(s)) << "  " << uint32_t(s.is_compatible(r)) << endl ; 
    EXPECT_EQ(TypeConsistency::EQUALS , r.is_compatible(s) );
    EXPECT_EQ(TypeConsistency::EQUALS , s.is_compatible(r) );
}

TEST (DynamicType, testing_is_compatible_structure_of_sequence_no_bound)
{
    SequenceType s(primitive_type<uint32_t>());
    StructType the_str("check");
    the_str.add_member(Member("int", s));
    StructType other_str("other_check");
    other_str.add_member(Member("int", s));
    EXPECT_EQ(TypeConsistency::EQUALS , the_str.is_compatible(other_str));
    EXPECT_EQ(TypeConsistency::EQUALS , other_str.is_compatible(the_str));
}

TEST (DynamicType, testing_is_compatible_structure_of_sequence_different_bound)
{
    SequenceType s(primitive_type<uint32_t>(),15);
    SequenceType r(primitive_type<uint32_t>(),19);
    StructType the_str("check");
    the_str.add_member(Member("int", s));
    StructType other_str("other_check");
    other_str.add_member(Member("int", r));
    EXPECT_EQ(TypeConsistency::IGNORE_SEQUENCE_BOUNDS, the_str.is_compatible(other_str));
    EXPECT_EQ(TypeConsistency::IGNORE_SEQUENCE_BOUNDS , other_str.is_compatible(the_str));
}

TEST (DynamicType, testing_is_compatible_structure_of_sequence_same_bound)
{
    SequenceType s(primitive_type<uint32_t>(),15);
    StructType the_str("check");
    the_str.add_member(Member("int", s));
    StructType other_str("other_check");
    other_str.add_member(Member("int", s));
    EXPECT_EQ(TypeConsistency::EQUALS , the_str.is_compatible(other_str));
    EXPECT_EQ(TypeConsistency::EQUALS , other_str.is_compatible(the_str));
}

TEST (DynamicType, testing_is_compatible_structure_of_primitive_type_int)
{
    StructType the_str("check");
    the_str.add_member(Member("int", primitive_type<uint32_t>()));
    StructType other_str("other_check");
    other_str.add_member(Member("int", primitive_type<uint32_t>()));
    EXPECT_EQ(TypeConsistency::EQUALS , the_str.is_compatible(other_str));
    EXPECT_EQ(TypeConsistency::EQUALS , other_str.is_compatible(the_str));
}

TEST (DynamicType, testing_is_compatible_structure_of_primitive_type_float)
{
    StructType the_str("check");
    the_str.add_member(Member("int", primitive_type<long double>()));
    StructType other_str("other_check");
    other_str.add_member(Member("int", primitive_type<long double>()));
    EXPECT_EQ(TypeConsistency::EQUALS , the_str.is_compatible(other_str));
    EXPECT_EQ(TypeConsistency::EQUALS , other_str.is_compatible(the_str));
}

TEST (DynamicType, testing_is_compatible_structure_of_primitive_type_char)
{
    StructType the_str("check");
    the_str.add_member(Member("int", primitive_type<wchar_t>()));
    StructType other_str("other_check");
    other_str.add_member(Member("int", primitive_type<wchar_t>()));
    StructType another_str("another_check");
    another_str.add_member(Member("int", primitive_type<char>()));
    
    EXPECT_EQ(TypeConsistency::EQUALS , the_str.is_compatible(other_str));
    EXPECT_EQ(TypeConsistency::IGNORE_TYPE_WIDTH, the_str.is_compatible(another_str));
    EXPECT_EQ(TypeConsistency::EQUALS , other_str.is_compatible(the_str));
    EXPECT_EQ(TypeConsistency::IGNORE_TYPE_WIDTH, other_str.is_compatible(another_str));
    EXPECT_EQ(TypeConsistency::IGNORE_TYPE_WIDTH, another_str.is_compatible(the_str));
    EXPECT_EQ(TypeConsistency::IGNORE_TYPE_WIDTH, another_str.is_compatible(other_str));
}

TEST (DynamicType, testing_is_compatible_structure_of_primitive_type_int32_t)
{
    StructType the_str("check");
    the_str.add_member(Member("int", primitive_type<uint32_t>()));
    StructType other_str("other_check");
    other_str.add_member(Member("int", primitive_type<uint32_t>()));
    StructType another_str("another_check");
    another_str.add_member(Member("int", primitive_type<int32_t>()));
    EXPECT_EQ(TypeConsistency::EQUALS , the_str.is_compatible(other_str));
    EXPECT_EQ(TypeConsistency::IGNORE_TYPE_SIGN, the_str.is_compatible(another_str));
    EXPECT_EQ(TypeConsistency::EQUALS , other_str.is_compatible(the_str));
    EXPECT_EQ(TypeConsistency::IGNORE_TYPE_SIGN, other_str.is_compatible(another_str));
    EXPECT_EQ(TypeConsistency::IGNORE_TYPE_SIGN, another_str.is_compatible(the_str));
    EXPECT_EQ(TypeConsistency::IGNORE_TYPE_SIGN, another_str.is_compatible(other_str));
}

TEST (DynamicType, testing_is_compatible_structure_of_primitive_type_mixed_int)
{
    StructType the_str("check");
    the_str.add_member(Member("int", primitive_type<uint16_t>()));
    StructType other_str("other_check");
    other_str.add_member(Member("int", primitive_type<uint32_t>()));
    StructType another_str("another_check");
    another_str.add_member(Member("int", primitive_type<int64_t>()));
    EXPECT_EQ(TypeConsistency::IGNORE_TYPE_WIDTH, the_str.is_compatible(other_str));
    EXPECT_EQ(TypeConsistency::IGNORE_TYPE_WIDTH |
              TypeConsistency::IGNORE_TYPE_SIGN, the_str.is_compatible(another_str));
    EXPECT_EQ(TypeConsistency::IGNORE_TYPE_WIDTH |
              TypeConsistency::IGNORE_TYPE_SIGN, other_str.is_compatible(another_str));
}

TEST (DynamicType, testing_is_compatible_structure_of_array_same_bound)
{
    StructType the_str("check");
    ArrayType the_array(primitive_type<uint32_t>(), 10);
    the_str.add_member(Member("arr", the_array));
    StructType other_str("other_check");
    other_str.add_member(Member("arr", the_array));
    EXPECT_EQ(TypeConsistency::EQUALS , the_str.is_compatible(other_str));
    EXPECT_EQ(TypeConsistency::EQUALS , other_str.is_compatible(the_str));
}

TEST (DynamicType, testing_is_compatible_structure_of_array_different_bound_and_type)
{
    StructType the_str("check");
    ArrayType the_array(primitive_type<uint32_t>(), 10);
    the_str.add_member(Member("arr", the_array));

    ArrayType other_array(primitive_type<uint32_t>(), 11);
    StructType other_str("other_check");
    other_str.add_member(Member("arr", other_array));

    ArrayType another_array(primitive_type<int32_t>(), 10);
    StructType another_str("other_check");
    another_str.add_member(Member("arr", another_array));

    EXPECT_EQ(TypeConsistency::IGNORE_ARRAY_BOUNDS , the_str.is_compatible(other_str));
    EXPECT_EQ(TypeConsistency::IGNORE_TYPE_SIGN, the_str.is_compatible(another_str));
    EXPECT_EQ(TypeConsistency::IGNORE_ARRAY_BOUNDS, other_str.is_compatible(the_str));
    EXPECT_EQ(TypeConsistency::IGNORE_ARRAY_BOUNDS | TypeConsistency::IGNORE_TYPE_SIGN, other_str.is_compatible(another_str));
    EXPECT_EQ(TypeConsistency::IGNORE_TYPE_SIGN, another_str.is_compatible(the_str));
    EXPECT_EQ(TypeConsistency::IGNORE_ARRAY_BOUNDS | TypeConsistency::IGNORE_TYPE_SIGN, another_str.is_compatible(other_str));
}

TEST (DynamicData, testing_equality_check_primitive_type)
{
    {
        DynamicData dd1(primitive_type<uint8_t>());
        DynamicData dd2(primitive_type<uint8_t>());
        dd1.value(uint8_t(15));
        dd2.value(uint8_t(15));
        EXPECT_EQ(dd1, dd2);
        dd2.value(uint8_t(16));
        EXPECT_NE(dd1, dd2);
    }

    {
        DynamicData dd1(primitive_type<uint16_t>());
        DynamicData dd2(primitive_type<uint16_t>());
        dd1.value(uint16_t(15));
        dd2.value(uint16_t(15));
        EXPECT_EQ(dd1, dd2);
        dd2.value(uint16_t(16));
        EXPECT_NE(dd1, dd2);
    }

    {
        DynamicData dd1(primitive_type<uint32_t>());
        DynamicData dd2(primitive_type<uint32_t>());
        dd1.value(uint32_t(15));
        dd2.value(uint32_t(15));
        EXPECT_EQ(dd1, dd2);
        dd2.value(uint32_t(16));
        EXPECT_NE(dd1, dd2);
    }

    {
        DynamicData dd1(primitive_type<int32_t>());
        DynamicData dd2(primitive_type<int32_t>());
        dd1.value(int32_t(15));
        dd2.value(int32_t(15));
        EXPECT_EQ(dd1, dd2);
        dd2.value(int32_t(16));
        EXPECT_NE(dd1, dd2);
    }

    {   
        DynamicData dd1(primitive_type<int16_t>());
        DynamicData dd2(primitive_type<int16_t>());
        dd1.value(int16_t(15));
        dd2.value(int16_t(15));
        EXPECT_EQ(dd1, dd2);
        dd2.value(int16_t(16));
        EXPECT_NE(dd1, dd2);
    }

    {
        DynamicData dd1(primitive_type<uint64_t>());
        DynamicData dd2(primitive_type<uint64_t>());
        dd1.value(uint64_t(15));
        dd2.value(uint64_t(15));
        EXPECT_EQ(dd1, dd2);
        dd2.value(uint64_t(16));
        EXPECT_NE(dd1, dd2);
    }

    {
        DynamicData dd1(primitive_type<int64_t>());
        DynamicData dd2(primitive_type<int64_t>());
        dd1.value(int64_t(15));
        dd2.value(int64_t(15));
        EXPECT_EQ(dd1, dd2);
        dd2.value(int64_t(16));
        EXPECT_NE(dd1, dd2);
    }

    {
        DynamicData dd1(primitive_type<float>());
        DynamicData dd2(primitive_type<float>());
        dd1.value(float(15.1));
        dd2.value(float(15.1));
        EXPECT_EQ(dd1, dd2);
        dd2.value(float(16.3));
        EXPECT_NE(dd1, dd2);
    }

    {
        DynamicData dd1(primitive_type<double>());
        DynamicData dd2(primitive_type<double>());
        dd1.value(double(15.1));
        dd2.value(double(15.1));
        EXPECT_EQ(dd1, dd2);
        dd2.value(double(16.3));
        EXPECT_NE(dd1, dd2);
    }

    {
        DynamicData dd1(primitive_type<long double>());
        DynamicData dd2(primitive_type<long double>());
        long double d1 = 15.1;
        dd1.value(d1);
        dd2.value(d1);
        EXPECT_EQ(dd1, dd2);
        d1 = 16.3;
        dd2.value(d1);
        EXPECT_NE(dd1, dd2);
    }

    {
        DynamicData dd1(primitive_type<char>());
        DynamicData dd2(primitive_type<char>());
        dd1.value('d');
        dd2.value('d');
        EXPECT_EQ(dd1, dd2);
        dd2.value('f');
        EXPECT_NE(dd1, dd2);
    }

    {
        DynamicData dd1(primitive_type<wchar_t>());
        DynamicData dd2(primitive_type<wchar_t>());
        dd1.value(char16_t(RAND_MAX));
        dd2.value(char16_t(RAND_MAX));
        EXPECT_EQ(dd1, dd2);
        dd2.value(char16_t(RAND_MAX - 1));
        EXPECT_NE(dd1, dd2);
    }
}

TEST (DynamicData, testing_equality_check_string)
{
    SequenceType s(primitive_type<uint64_t>());
    DynamicData d1(s);
    DynamicData d2(s);
    d1.push<uint64_t>(3456); 
    d2.push<uint64_t>(3456); 
    EXPECT_EQ(true , d1 == d2);
    d2[0].value<uint64_t>(3457);
    EXPECT_NE(d1, d2);
    d2[0].value<uint64_t>(3456);
    d2.push<uint64_t>(435);
    cout << "d2: " << d2[1].value<uint64_t>() << endl;
    EXPECT_NE(d1, d2);
}

TEST (DynamicData, test_equality_check_struct)
{
    StructType stru("the_struct");
    stru.add_member(Member("lld", primitive_type<long double>()));

    DynamicData d1(stru);
    d1["lld"].value<long double>(3.1415926);
    DynamicData d2(stru);
    d2["lld"].value<long double>(3.1415926);
    EXPECT_EQ(d1, d2);
    d2["lld"].value<long double>(3.1415925);
    EXPECT_NE(d1, d2);

    stru.add_member(Member("c", primitive_type<char>()));
    DynamicData d3(stru);
    d3["lld"].value<long double>(3.1415926);
    EXPECT_NE(d3, d1);
    d3["c"].value<char>(3.1415926); //just an :"out of the ordinady
    EXPECT_NE(d3, d1);

}

namespace{
    template<typename T>
    ostream& operator << (ostream& o, const vector<T>& v)
    {
        for(auto it = v.begin(); it != v.end(); ++it)
        {
            o << '(' << *it << ')';
        }
        return o;
    }
}

TEST (DynamicData, test_equality_complex_struct)
{
    ArrayType ar(primitive_type<uint32_t>(), 15);
    SequenceType se(primitive_type<uint32_t>(), 15);
    StringType str;
    SequenceType seq(str, 15);
    StructType st("the_type");
    st.add_member(Member("array", ar));
    st.add_member(Member("sequence", se));
    st.add_member(Member("seqstring", seq));
    st.add_member(Member("string", str));
    DynamicData d1(st);
    d1["string"].string("sono io che sono qui");

    for(int i=0; i < 15; ++i)
    {
        d1["array"][i].value<uint32_t>(42);
        d1["sequence"].push<uint32_t>(42);
        d1["seqstring"].push<string>("ci sono anche io");
    }
    vector<char> v = d1["string"].as_vector<char>();
    vector<string> vv = d1["seqstring"].as_vector<string>();
    vector<uint32_t> t = d1["array"].as_vector<uint32_t>();
    vector<uint32_t> s = d1["sequence"].as_vector<uint32_t>();
    
    cout << v << endl;
    cout << vv << endl;
    cout << t << endl;
    cout << s << endl;

}

TEST (DynamicType , wstring_and_wstring_struct)
{
    WStringType wst;
    StringType st;
    DynamicData d(wst) ;
    DynamicData dd(st) ;

    d.wstring(L"sadfsfdasdf");
    dd.string("sadfsfdasdf");
   
    EXPECT_EQ( TypeConsistency::NONE, wst.is_compatible(st) );
    EXPECT_EQ( TypeConsistency::NONE, st.is_compatible(wst) );

    StructType struc1("the_struct");
    struc1.add_member(Member("theString", wst )) ;

    
    StructType struc2("the_struct");
    struc2.add_member(Member("theString", st )) ;

    EXPECT_EQ( TypeConsistency::NONE, struc1.is_compatible(struc2) );
    EXPECT_EQ( TypeConsistency::NONE, struc2.is_compatible(struc1) );
}

TEST (QoS, sequence)
{
    SequenceType s1(primitive_type<uint16_t>(), 10) ;
    SequenceType s2(primitive_type<uint16_t>(), 20) ;

    DynamicData d(s1) ;

    for(int i = 0 ; i < 10 ; ++i)
    {
        d.push(uint16_t()) ;
    }
    d[9].value<uint16_t>(256);
    
    cout << "primo: " << endl ;
    for(int i = 0 ; i < 10 ; ++i)
    {
        cout << '(' << d[i].value<uint16_t>() <<')';
    }
    cout << endl ;
    DynamicData dd(d, s2) ;

    cout << "secondo: " << endl ;
    for(int i = 0 ; i < 10 ; ++i)
    {
        cout << '(' << dd[i].value<uint16_t>() << ')' ; 
    }
    cout << endl;
    EXPECT_EQ(10, dd.size()) ;
}

TEST (QoS, other_sequence)
{
    SequenceType s1(primitive_type<uint16_t>(), 20) ;
    SequenceType s2(primitive_type<uint16_t>(), 10) ;

    DynamicData d(s1) ;

    for(int i = 0 ; i < 20 ; ++i)
    {
        d.push(uint16_t()) ;
    }
    d[9].value<uint16_t>(256);
    
    cout << "primo: " << endl ;
    for(int i = 0 ; i < 20 ; ++i)
    {
        cout << '(' << d[i].value<uint16_t>() << ')'; 
    }
    cout << endl;
    DynamicData dd(d, s2) ;

    cout << "secondo: " << endl ;
    for(size_t i = 0 ; i < dd.size() ; ++i)
    {
        cout << '(' << dd[i].value<uint16_t>() << ')';
    }
    cout << endl; 
    EXPECT_EQ(10, dd.size()) ;
}

TEST (QoS, string)
{
    StringType s(20) ;
    StringType t(10);
    
    EXPECT_EQ(s.is_compatible(t) , t.is_compatible(s)) ;
    EXPECT_EQ(TypeConsistency::IGNORE_STRING_BOUNDS, t.is_compatible(s)) ;
    
    DynamicData d(s);
    d.string("12345678901234567890");
    
    DynamicData e(d,t) ;
    EXPECT_EQ(10, e.size()); //still pending investigation
}

TEST (QoS, other_string)
{
    StringType s(10) ;
    StringType t(20);
    
    EXPECT_EQ(s.is_compatible(t) , t.is_compatible(s)) ;
    EXPECT_EQ(TypeConsistency::IGNORE_STRING_BOUNDS, t.is_compatible(s)) ;
    
    DynamicData d(s);
    d.string("1234567890");
    
    DynamicData e(d,t) ;
    EXPECT_EQ(10, e.size()); 
}

TEST (QoS, wstring)
{
    WStringType s(20) ;
    WStringType t(10);
    
    EXPECT_EQ(s.is_compatible(t) , t.is_compatible(s)) ;
    EXPECT_EQ(TypeConsistency::IGNORE_STRING_BOUNDS, t.is_compatible(s)) ;
    
    DynamicData d(s);
    d.wstring(L"12345678901234567890");
    
    DynamicData e(d,t) ;
    EXPECT_EQ(10, e.size()); //still pending investigation
}

TEST (QoS, other_wstring)
{
    WStringType s(10) ;
    WStringType t(20);
    
    EXPECT_EQ(s.is_compatible(t) , t.is_compatible(s)) ;
    EXPECT_EQ(TypeConsistency::IGNORE_STRING_BOUNDS, t.is_compatible(s)) ;
    
    DynamicData d(s);
    d.wstring(L"1234567890");
    
    DynamicData e(d,t) ;
    EXPECT_EQ(10, e.size()); 
}

TEST (QoS, array)
{
    ArrayType a(primitive_type<uint16_t>(), 10);
    ArrayType b(primitive_type<uint16_t>(), 20);
    
    DynamicData d(a);
    d[8].value(10) ;
    for(size_t i = 0 ; i < 10 ; ++i)
    {
        cout << '(' << d[i].value<uint16_t>() << ')';
    }
    cout << '\n';
    DynamicData e(d,b);
    for(size_t i = 0 ; i < e.size() ; ++i)
    {
        cout << '(' << e[i].value<uint16_t>() << ')'; 
    }
    cout << '\n';
    EXPECT_EQ(TypeConsistency::IGNORE_ARRAY_BOUNDS, b.is_compatible(a)) ;
}

TEST (QoS, other_array)
{
    ArrayType a(primitive_type<uint16_t>(), 20);
    ArrayType b(primitive_type<uint16_t>(), 10);
    
    DynamicData d(a);
    d[8].value(10) ;
    d[13].value(10) ;
    for(size_t i = 0 ; i < d.size() ; ++i)
    {
        cout << '(' << d[i].value<uint16_t>() << ')';
    }
    cout << '\n';
    DynamicData e(d,b);
    for(size_t i = 0 ; i < e.size() ; ++i)
    {
        cout << '(' << e[i].value<uint16_t>() << ')'; 
    }
    cout << '\n';
    EXPECT_EQ(TypeConsistency::IGNORE_ARRAY_BOUNDS, b.is_compatible(a)) ;
}

TEST (QoS, Array_qos)
{

    ArrayType a_arr(primitive_type<uint16_t>(), 10);
    ArrayType b_arr(primitive_type<int32_t>(), 11);

    EXPECT_EQ(TypeConsistency::IGNORE_ARRAY_BOUNDS |
              TypeConsistency::IGNORE_TYPE_WIDTH |
              TypeConsistency::IGNORE_TYPE_SIGN , a_arr.is_compatible(b_arr));
    
    cout << (bitset<16>(uint16_t(a_arr.is_compatible(b_arr)))) << endl ;
}

TEST (QoS, mixed_types)
{
    StructType a("composition");
    StructType b("composition");
    StringType a_string(10);
    WStringType b_wstring(10);
    SequenceType a_seq(primitive_type<uint32_t>(), 10);
    SequenceType b_seq(primitive_type<int32_t>(), 11);
    ArrayType a_arr(primitive_type<uint16_t>(), 10);
    ArrayType b_arr(primitive_type<int32_t>(), 11);


    
    EXPECT_EQ(TypeConsistency::IGNORE_TYPE_WIDTH, a_string.is_compatible(b_wstring));

    EXPECT_EQ(TypeConsistency::IGNORE_SEQUENCE_BOUNDS |
              TypeConsistency::IGNORE_TYPE_SIGN, a_seq.is_compatible(b_seq));

    EXPECT_EQ(TypeConsistency::IGNORE_ARRAY_BOUNDS |
              TypeConsistency::IGNORE_TYPE_WIDTH |
              TypeConsistency::IGNORE_TYPE_SIGN , a_arr.is_compatible(b_arr));
    
    a.add_member(
            Member("a_string", a_string)).add_member(
            Member("a_seq", a_seq)).add_member(
            Member("a_arr", a_arr)).add_member(
            Member("a_primitive", primitive_type<wchar_t>()));

    b.add_member(
            Member("b_wstring", b_wstring)).add_member(
            Member("b_seq", b_seq)).add_member(
            Member("b_arr", b_arr));

    EXPECT_EQ(TypeConsistency::IGNORE_MEMBER_NAMES|
              TypeConsistency::IGNORE_TYPE_SIGN |
              TypeConsistency::IGNORE_TYPE_WIDTH |
              TypeConsistency::IGNORE_STRING_BOUNDS |
              TypeConsistency::IGNORE_SEQUENCE_BOUNDS |
              TypeConsistency::IGNORE_ARRAY_BOUNDS |
              TypeConsistency::IGNORE_MEMBERS , a.is_compatible(b));
    

}

TEST (QoS, ignore_member)
{
    StructType a("composition");
    StructType b("composition");
    StringType string(10);
    SequenceType seq(primitive_type<uint32_t>(), 10);
    ArrayType arr(primitive_type<uint16_t>(), 10);
    a.add_member(
            Member("string", string)).add_member(
            Member("seq", seq)).add_member(
            Member("arr", arr)).add_member(
            Member("primitive", primitive_type<wchar_t>()));
    
    b.add_member(
            Member("string", string)).add_member(
            Member("seq", seq)).add_member(
            Member("arr", arr));

    EXPECT_EQ(TypeConsistency::IGNORE_MEMBERS , a.is_compatible(b));
    
}
int main() 
{
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
