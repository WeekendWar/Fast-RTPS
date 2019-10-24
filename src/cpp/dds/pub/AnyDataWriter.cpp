/*
 * Copyright 2019, Proyectos y Sistemas de Mantenimiento SL (eProsima).
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#ifndef EPROSIMA_DDS_PUB_DETAIL_TANYDATAWRITER_HPP_
#define EPROSIMA_DDS_PUB_DETAIL_TANYDATAWRITER_HPP_

/**
 * @file
 */

/*
 * OMG PSM class declaration
 */
#include <dds/pub/AnyDataWriter.hpp>
#include <dds/core/status/Status.hpp>

namespace dds {
namespace pub {

AnyDataWriter::~AnyDataWriter()
{
}

const dds::pub::Publisher& AnyDataWriter::publisher() const
{
    publisher_ = (Publisher*) delegate()->get_publisher();
    return *this->publisher_;
}

const dds::topic::TopicDescription& AnyDataWriter::topic_description() const
{
    dds::topic::TopicDescription topic;
    //TODO Implement topic_description in fastdds DataWriter
    //return delegate()->topic_description();
    return topic;
}

qos::DataWriterQos AnyDataWriter::qos() const
{
    qos::DataWriterQos qos;
    // TODO Use DataWriterQos instead of WriterQos
    //return delegate()->get_qos();
    return qos;
}

void AnyDataWriter::qos(
        const qos::DataWriterQos& qos)
{
    (void) qos;
    // TODO Use DataWriterQos instead of WriterQos
    //delegate()->set_qos(qos);
}

AnyDataWriter& AnyDataWriter::operator <<(
        const dds::pub::qos::DataWriterQos& qos)
{
    this->qos(qos);
    return *this;
}

const AnyDataWriter& AnyDataWriter::operator >>(
        qos::DataWriterQos& qos) const
{
    qos = this->qos();
    return *this;
}

void AnyDataWriter::wait_for_acknowledgments(
        const dds::core::Duration& timeout)
{
    eprosima::fastrtps::Duration_t max_wait(static_cast<int32_t>(timeout.sec()), timeout.nanosec());
    delegate()->wait_for_acknowledgments(max_wait);
}

const dds::core::status::LivelinessLostStatus AnyDataWriter::liveliness_lost_status()
{
    dds::core::status::LivelinessLostStatus status;
    delegate()->get_liveliness_lost_status(status);
    return status;
}

const dds::core::status::OfferedDeadlineMissedStatus AnyDataWriter::offered_deadline_missed_status()
{
    dds::core::status::OfferedDeadlineMissedStatus status;
    delegate()->get_offered_deadline_missed_status(status);
    return status;
}

const dds::core::status::OfferedIncompatibleQosStatus AnyDataWriter::offered_incompatible_qos_status()
{
    dds::core::status::OfferedIncompatibleQosStatus status;
    //TODO Implement get_offered_incompatible_qos_status in fastdds DataWriter
//    delegate()->get_offered_incompatible_qos_status(status);
    return status;
}

const dds::core::status::PublicationMatchedStatus AnyDataWriter::publication_matched_status()
{
    dds::core::status::PublicationMatchedStatus status;
    //TODO Implement publication_matched_status in fastdds DataWriter
//   delegate()->get_publication_matched_status(status);
    return status;
}

void AnyDataWriter::assert_liveliness()
{
    delegate()->assert_liveliness();
}

} //namespace pub
} //namespace dds


#endif //EPROSIMA_DDS_PUB_DETAIL_TANYDATAWRITER_HPP_
