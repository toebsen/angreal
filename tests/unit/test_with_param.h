//
// Created by toebsen on 29.11.2020.
//
#ifndef ANGREAL_TESTS_UNIT_TEST_WITH_PARAM_H_
#define ANGREAL_TESTS_UNIT_TEST_WITH_PARAM_H_

namespace angreal {
template<typename Fixture, typename Parameter>
class TestWithParam : public testing::WithParamInterface<Parameter>, public Fixture {
 protected:
  TestWithParam() : test_vector_{this->GetParam()} {}

  const Parameter &test_vector_;
};
}
#endif //ANGREAL_TESTS_UNIT_TEST_WITH_PARAM_H_
