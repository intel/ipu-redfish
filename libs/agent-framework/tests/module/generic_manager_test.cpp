/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2024 Intel Corporation */

/*!
 * @section LICENSE
 *
 * @copyright
 * Copyright (c) 2015-2019 Intel Corporation
 *
 * @copyright
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * @copyright
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * @copyright
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @section GenericManager tests
 * */

#include "agent-framework/module/enum/common.hpp"
#include "agent-framework/module/managers/generic_manager.hpp"
#include "agent-framework/module/model/resource.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>

using namespace agent_framework;
using namespace agent_framework::module;
using namespace agent_framework::model;

class TestObject : public Resource {
public:
    TestObject(const std::string& parent_uuid,
               const std::string& uuid,
               const std::uint64_t id,
               const std::uint64_t touched,
               const std::string& data) : Resource{parent_uuid, enums::Component::None}, m_data{data} {
        set_uuid(uuid);
        set_id(id);
        touch(touched);
    }

    /*!
     * @brief Get component name
     * @return component name
     */
    static enums::Component get_component() {
        return enums::Component::None;
    }

    const std::string& get_data() const { return m_data; }
    void set_data(const std::string& data) { m_data = data; }

    friend auto operator<=>(const TestObject& lhs, const TestObject& rhs) = default;
private:
    std::string m_data{""};
};

namespace {
constexpr static const unsigned num{13};
const TestObject elems[num] = {
    {"0", "1", 1, 0, "T"},
    {"1", "1-1", 1, 0, "C1"},
    {"1", "1-2", 2, 0, "C2"},
    {"1", "1-3", 3, 0, "C3"},
    {"1", "1-4", 4, 0, "C4"},
    {"1-1", "1-1-1", 1, 0, "GC1"},
    {"1-1", "1-1-2", 2, 0, "GC2"},
    {"1-2", "1-2-1", 3, 0, "GC3"},
    {"1-2", "1-2-2", 4, 0, "GC4"},
    {"-0", "-1", 1, 0, "H1"},
    {"-1", "-2", 1, 0, "H2"},
    {"-2", "-3", 1, 0, "H3"},
    {"-3", "-4", 1, 0, "H4"}};
} // namespace

class GenericManagerTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        for (unsigned i = 0; i < ::num; ++i) {
            gm.add_entry(::elems[i]);
        }
    }

    virtual void TearDown() {
    }

    virtual ~GenericManagerTest();

    // checks if the manager elements are the same as ones inserted by SetUp() method
    // optional parameter is an id of the element not to be checked
    // additionally there is a flag to check for changes in size
    virtual bool is_default(int omit = -1, bool check_size = true) const {
        if (check_size == true && gm.get_entry_count() != ::num) {
            return false;
        }
        for (unsigned i = 0; i < ::num; ++i) {
            if (omit == static_cast<int>(i)) {
                continue;
            }
            if (::elems[i] != gm.get_entry(::elems[i].get_uuid())) {
                return false;
            }
        }
        return true;
    }

    GenericManager<TestObject> gm;
};

GenericManagerTest::~GenericManagerTest() {}

TEST_F(GenericManagerTest, EntriesAreCorrectlyAddedCountedAndCleared) {
    // check if data were correctly added using add_entry in SetUp
    EXPECT_EQ(gm.get_entry_count("X"), 0u);
    EXPECT_EQ(gm.get_entry_count("1"), 4u);
    EXPECT_EQ(gm.get_entry_count(), ::num);
    EXPECT_TRUE(is_default());
    // check if exception is thrown on duplicates and nothing is changed
    EXPECT_THROW(gm.add_entry(::elems[0]), ::agent_framework::exceptions::InvalidUuid);
    EXPECT_TRUE(is_default());
    // check if clearing works;
    gm.clear_entries();
    EXPECT_EQ(gm.get_entry_count(), 0u);
    EXPECT_EQ(gm.get_entry_count("1"), 0u);
}

TEST_F(GenericManagerTest, EntriesAreCorrectlyFound) {
    // test entry_exists
    for (unsigned i = 0; i < ::num; ++i) {
        EXPECT_NO_THROW(gm.get_entry(::elems[i].get_uuid()));
        EXPECT_TRUE(gm.entry_exists(::elems[i].get_uuid()));
    }
    EXPECT_FALSE(gm.entry_exists("BAD_UUID1"));
    // test get_entry
    EXPECT_THROW(gm.get_entry("BAD_UUID2"), ::agent_framework::exceptions::InvalidUuid);
    EXPECT_TRUE(is_default());
    for (unsigned i = 0; i < ::num; ++i) {
        EXPECT_EQ(gm.get_entry(elems[i].get_uuid()), elems[i]);
    }
    // test get_entry_reference
    EXPECT_THROW(gm.get_entry_reference("BAD_UUID3"), ::agent_framework::exceptions::InvalidUuid);
    EXPECT_TRUE(is_default());
    for (unsigned i = 0; i < ::num; ++i) {
        EXPECT_EQ(*gm.get_entry_reference(::elems[i].get_uuid()), ::elems[i]);
    }
}

TEST_F(GenericManagerTest, EntriesAreCorrectlyChangedViaReference) {
    int index = 2;
    std::string test_data = "TEST_DATA";
    TestObject obj = ::elems[index];
    obj.set_data(test_data);
    // test get_entry_reference
    {
        auto ref = gm.get_entry_reference(::elems[index].get_uuid());
        EXPECT_TRUE(is_default());
        ref->set_data(test_data);
    }
    EXPECT_TRUE(is_default(index));
    EXPECT_EQ(*gm.get_entry_reference(::elems[index].get_uuid()), obj);
}

TEST_F(GenericManagerTest, EntriesAreCorrectlyRemoved) {
    // test removing bad UUIDs
    gm.remove_entry("WRONG_UUID");
    EXPECT_TRUE(is_default());
    // test removing specific element
    int index = 3;
    gm.remove_entry(::elems[index].get_uuid());
    EXPECT_TRUE(is_default(index, false));
    EXPECT_THROW(gm.get_entry(::elems[index].get_uuid()), ::agent_framework::exceptions::InvalidUuid);
}

TEST_F(GenericManagerTest, EntriesAreCorrectlyRemovedByParent) {
    // removing children of non existing uuid
    gm.remove_by_parent("WRONG_UUID");
    EXPECT_TRUE(is_default());
    // removing children of existing uuid
    gm.remove_by_parent(::elems[0].get_uuid());
    EXPECT_EQ(gm.get_entry_count(), ::num - 4);
    EXPECT_NO_THROW(gm.get_entry(::elems[0].get_uuid()));
    for (unsigned i = 1; i < 5u; ++i) {
        EXPECT_THROW(gm.get_entry(::elems[i].get_uuid()), ::agent_framework::exceptions::InvalidUuid);
    }
    for (unsigned i = 5; i < ::num; ++i) {
        EXPECT_NO_THROW(gm.get_entry(::elems[i].get_uuid()));
    }
}

TEST_F(GenericManagerTest, EntriesAreCorrectlyAddedOrUpdated) {
    TestObject entry1 = {"test", "X1", 9, 0, "X"};
    TestObject entry2 = {"test", "X1", 9, 0, "Y"};
    EXPECT_THROW(gm.get_entry(entry1.get_uuid()), ::agent_framework::exceptions::InvalidUuid);
    // check if adding works
    EXPECT_EQ(gm.add_or_update_entry(entry1), GenericManager<TestObject>::UpdateStatus::Added);
    EXPECT_EQ(gm.get_entry_count("test"), 1u);
    EXPECT_TRUE(is_default(-1, false));
    EXPECT_EQ(entry1, gm.get_entry(entry1.get_uuid()));
    // check if updating works
    EXPECT_EQ(gm.add_or_update_entry(entry2), GenericManager<TestObject>::UpdateStatus::Updated);
    EXPECT_EQ(gm.get_entry_count("test"), 1u);
    EXPECT_TRUE(is_default(-1, false));
    EXPECT_EQ(entry2, gm.get_entry(entry1.get_uuid()));
}

TEST_F(GenericManagerTest, IDsAreCorrectlyListed) {
    // check all children ids
    auto ids = gm.get_ids();
    EXPECT_EQ(ids.size(), ::num);
    for (unsigned int i = 0; i < ::num; ++i) {
        EXPECT_NE(std::find(ids.begin(), ids.end(), ::elems[i].get_id()), ids.end());
    }
    // check children for specific parents
    ids = gm.get_ids("1");
    EXPECT_EQ(ids.size(), 4u);
    for (unsigned int i = 1; i < 5u; ++i) {
        EXPECT_NE(std::find(ids.begin(), ids.end(), ::elems[i].get_id()), ids.end());
    }
    // check children for parent that does not exist
    ids = gm.get_ids("WRONG UUID");
    EXPECT_EQ(ids.size(), 0u);
    // check that nothing has changed in the manager
    EXPECT_TRUE(is_default());
}

TEST_F(GenericManagerTest, UUIDsAreCorrectlyListed) {
    // check all children uuids
    auto keys = gm.get_keys();
    EXPECT_EQ(keys.size(), ::num);
    for (unsigned int i = 0; i < ::num; ++i) {
        EXPECT_NE(std::find(keys.begin(), keys.end(), ::elems[i].get_uuid()), keys.end());
    }
    // check children for specific parents
    keys = gm.get_keys("1");
    EXPECT_EQ(keys.size(), 4u);
    for (unsigned int i = 1; i < 5u; ++i) {
        EXPECT_NE(std::find(keys.begin(), keys.end(), ::elems[i].get_uuid()), keys.end());
    }
    // check children for parent that does not exist
    keys = gm.get_keys("WRONG UUID");
    EXPECT_EQ(keys.size(), 0u);
    // check that nothing has changed in the manager
    EXPECT_TRUE(is_default());
}

TEST_F(GenericManagerTest, UUIDsAreCorrectlyListedWithFilters) {
    GenericManager<TestObject>::Filter id_one =
        [](const TestObject& t) { return t.get_id() == 1u; };
    // check all children uuids
    auto keys = gm.get_keys(id_one);
    EXPECT_EQ(keys.size(), 7u);
    for (unsigned int i = 0; i < 7u; ++i) {
        if (id_one(::elems[i])) {
            EXPECT_NE(std::find(keys.begin(), keys.end(), ::elems[i].get_uuid()), keys.end());
        }
    }
    // check children for specific parents
    keys = gm.get_keys("1", id_one);
    ASSERT_EQ(keys.size(), 1u);
    EXPECT_EQ(gm.get_entry(keys[0]), ::elems[1]);
    // check children for parent that does not exist
    keys = gm.get_keys("WRONG UUID", id_one);
    EXPECT_EQ(keys.size(), 0u);
    // check that nothing has changed in the manager
    EXPECT_TRUE(is_default());
}

TEST_F(GenericManagerTest, IDsAreCorrectlyTranslatedIntoUUIDs) {
    // check if exception is thrown on wrong ID
    EXPECT_THROW(gm.rest_id_to_uuid(999), ::agent_framework::exceptions::NotFound);
    // check if IDs/parents pairs are correctly translated to UUIDs
    for (unsigned int i = 0; i < ::num; ++i) {
        ASSERT_NO_THROW(gm.rest_id_to_uuid(::elems[i].get_id(), ::elems[i].get_parent_uuid()));
        auto uuid = gm.rest_id_to_uuid(::elems[i].get_id(), ::elems[i].get_parent_uuid());
        EXPECT_TRUE(gm.get_entry(uuid) == ::elems[i]);
    }
    // check if this works only with IDs
    for (unsigned int i = 0; i < ::num; ++i) {
        ASSERT_NO_THROW(gm.rest_id_to_uuid(::elems[i].get_id()));
    }
    EXPECT_EQ(gm.get_entry(gm.rest_id_to_uuid(1)), ::elems[0]);
    EXPECT_EQ(gm.get_entry(gm.rest_id_to_uuid(2)), ::elems[2]);
    EXPECT_EQ(gm.get_entry(gm.rest_id_to_uuid(3)), ::elems[3]);
    EXPECT_EQ(gm.get_entry(gm.rest_id_to_uuid(4)), ::elems[4]);
    // check that nothing has changed in the manager
    EXPECT_TRUE(is_default());
}

TEST_F(GenericManagerTest, UUIDsAreCorrectlyTranslatedIntoIDs) {
    // check if exception is thrown on wrong ID
    EXPECT_THROW(gm.uuid_to_rest_id("WRONG"), ::agent_framework::exceptions::InvalidUuid);
    // check if IDs/parents pairs are correctly translated to UUIDs
    for (unsigned int i = 0; i < ::num; ++i) {
        ASSERT_NO_THROW(gm.uuid_to_rest_id(::elems[i].get_uuid()));
        EXPECT_EQ(gm.uuid_to_rest_id(::elems[i].get_uuid()), ::elems[i].get_id());
    }
    // check that nothing has changed in the manager
    EXPECT_TRUE(is_default());
}

TEST_F(GenericManagerTest, GetOnlyThrowsIfMore) {
    EXPECT_THROW(gm.get_only(), ::agent_framework::exceptions::NotFound);
    EXPECT_THROW(gm.get_only_reference(), ::agent_framework::exceptions::NotFound);
}

TEST(GenericManager, GetOnlyThrowsIfEmpty) {
    GenericManager<TestObject> gm;
    EXPECT_THROW(gm.get_only(), ::agent_framework::exceptions::NotFound);
    EXPECT_THROW(gm.get_only_reference(), ::agent_framework::exceptions::NotFound);
}

TEST(GenericManager, GetOnlySuccess) {
    GenericManager<TestObject> gm;
    const TestObject elem{"0", "1", 1, 0, "T"};
    gm.add_entry(elem);

    auto entry = gm.get_only();
    EXPECT_EQ(elem, entry);

    auto entry_reference = gm.get_only_reference();
    EXPECT_EQ(elem, *entry_reference);
}
