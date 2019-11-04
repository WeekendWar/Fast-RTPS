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
// limitations under the License.#include <iostream>
    
#include <gtest/gtest.h>
#include <dds/core/xtypes/xtypes.hpp>
#include <iostream>

#include <cmath>

using namespace std; 
using namespace dds::core::xtypes;

DynamicData create_dynamic_data(StructType& the_struct/*, StructType& inner_struct, StructType& second_inner_struct*/)
{
    StructType inner_struct("inner_struct") ;

    inner_struct.add_member(
        Member("inner_sequence_string", SequenceType(StringType())));

    the_struct.add_member(
        Member("sequence", SequenceType(inner_struct)));

    DynamicData the_data(the_struct) ;

    for(int i = 0 ; i < 1E4 ; ++i) // creating "sequence"
    {
        DynamicData tmp_data(inner_struct);
        for (int j = 0 ; j < 1E3 ; ++j) // creating "sequence.inner_sequence_string"
        {
            tmp_data["inner_sequence_string"].push<string>("another_prick_in_the_world") ;
        }
        the_data["sequence"].push(tmp_data);
    }

    return the_data ;
}

#if(1)
TEST (DynamicData, run_against_time)
{
    StructType t1("t1"); //, t2("t2"), t3("t3") ;
    StructType t4("t4"); //, t5("t5"), t6("t6") ;
    DynamicData d1 = create_dynamic_data(t1);//, t2, t3 ) ;
    DynamicData d2 = create_dynamic_data(t4);//, t5, t6 ) ;
    DynamicData d3 = d1 ;
    EXPECT_NE(d1, d2) ;

    EXPECT_EQ(d1, d3) ;
}
#endif

TEST (DynamicData, cascade_construction)
{
    StructType the_struct("the_struct") ;

    DynamicData the_data = create_dynamic_data(the_struct/*, inner_struct, second_inner_struct*/) ;

    srand48(time(0)) ;

    for (int i = 0 ; i < 1E2 ; ++i)
    {
        size_t idx_4 = lrand48()%int(1E4) ;
        size_t idx_3 = lrand48()%int(1E3) ;
        EXPECT_EQ("another_prick_in_the_world", the_data["sequence"][idx_4]["inner_sequence_string"][idx_3].string()) ;
    }    
}


int main() 
{
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
