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

/**
 * @file
 */

/*
 * OMG PSM class declaration
 */

#include <dds/sub/AnyDataReader.hpp>
#include <dds/topic/TopicDescription.hpp>

namespace dds {
namespace sub {

AnyDataReader::~AnyDataReader()
{
}

const Subscriber& AnyDataReader::subscriber() const
{
    subscriber_ = (Subscriber*) delegate()->get_subscriber();
    return *this->subscriber_;
}

const dds::topic::TopicDescription& AnyDataReader::topic_description() const
{
    //TODO Implement topic_description in fastdss DataReader
    dds::topic::TopicDescription topic;
//    return this->delegate()->topic_description();
    return topic;
}

void AnyDataReader::wait_for_historical_data(
        const dds::core::Duration& timeout)
{
    (void) timeout;
    //TODO Implement wait_for_historical_data in fastdds DataReader
//    delegate()->wait_for_historical_data(timeout);
}


dds::sub::qos::DataReaderQos AnyDataReader::qos() const
{
    qos::DataReaderQos qos;
    // TODO Use DataReaderQos instead of ReaderQos
//    return delegate()->get_qos();
    return qos;
}

void AnyDataReader::qos(
        const dds::sub::qos::DataReaderQos& qos)
{
    (void) qos;
    // TODO Use DataReaderQos instead of ReaderQos
//    delegate()->qos(qos);
}

AnyDataReader& AnyDataReader::operator <<(
        const dds::sub::qos::DataReaderQos& qos)
{
    this->qos(qos);
    return *this;
}

const AnyDataReader& AnyDataReader::operator >>(
        dds::sub::qos::DataReaderQos& qos) const
{
    qos = this->qos();
    return *this;
}


dds::core::status::LivelinessChangedStatus AnyDataReader::liveliness_changed_status()
{
    dds::core::status::LivelinessChangedStatus status;
    delegate()->get_liveliness_changed_status(status);
    return status;
}

dds::core::status::SampleRejectedStatus AnyDataReader::sample_rejected_status()
{
    dds::core::status::SampleRejectedStatus status;
    //TODO Implement get_sample_rejected_status in fastdds DataReader
    //delegate()->get_sample_rejected_status(status);
    return status;
}

dds::core::status::SampleLostStatus AnyDataReader::sample_lost_status()
{
    dds::core::status::SampleLostStatus status;
    //TODO Implement get_sample_lost_status in fastdds DataReader
    //delegate()->get_sample_lost_status(status);
    return status;
}

dds::core::status::RequestedDeadlineMissedStatus AnyDataReader::requested_deadline_missed_status()
{
    dds::core::status::RequestedDeadlineMissedStatus status;
    delegate()->get_requested_deadline_missed_status(status);
    return status;
}

dds::core::status::RequestedIncompatibleQosStatus AnyDataReader::requested_incompatible_qos_status()
{
    dds::core::status::RequestedIncompatibleQosStatus status;
    //TODO Implement get_requested_incompatible_qos_status in fastdds DataReader
    //delegate()->get_requested_incompatible_qos_status(status);
    return status;
}

dds::core::status::SubscriptionMatchedStatus AnyDataReader::subscription_matched_status()
{
    dds::core::status::SubscriptionMatchedStatus status;
    //TODO Implement subscription_matched_status in fastdds DataReader
//    return delegate()->subscription_matched_status(status);
    return status;
}

} //namespace sub
} //namespace dds

